#include "../include/qom/object.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> // memcpy

#include <stdio.h>

// type info 必須要加的兩個 headers, 1) object.h, 2) module.h
#include "../include/qom/object.h"
#include "../include/qemu/module.h"
#include "../include/hw/qdev-core.h"


// 對於一個 type 最多可以有 32 個介面(interface) 可以繼承和實作
#define MAX_INTERFACES 32

typedef struct InterfaceImpl InterfaceImpl;
typedef struct TypeImpl TypeImpl;

typedef struct InterfaceClass InterfaceClass;

struct InterfaceImpl
{
	const char *typename;
};

// 用於實作 typeinfo 使用
struct TypeImpl
{
    const char *name;
    
    size_t class_size;
    
    size_t instance_size;
    
    void (*class_init)(ObjectClass *klass, void *data);
    void (*class_base_init)(ObjectClass *klass, void *data);
    void (*class_finalize)(ObjectClass *klass, void *data);
    
    void *class_data;
    
    void (*instance_init)(Object *obj);
    void (*instance_post_init)(Object *obj);
    void (*instance_finalize)(Object *obj);
    
    bool abstract;
    
    const char *parent;
    TypeImpl *parent_type;
    
    ObjectClass *class;
    
    int num_interfaces;
    InterfaceImpl interfaces[MAX_INTERFACES];
};

static GHashTable *type_table_get(void)
{
	//  type_table 可以看成 global 變數
	static GHashTable *type_table;
	
	if (type_table == NULL) {
		type_table = g_hash_table_new(g_str_hash, g_str_equal);
	}
	
	return type_table;
}

static Type type_interface;


static void type_table_add(TypeImpl *ti)
{
	g_hash_table_insert(type_table_get(), (void *)ti->name, ti);
}

static TypeImpl *type_table_lookup(const char *name)
{
	return g_hash_table_lookup(type_table_get(), name);
}


static TypeImpl *type_new(const TypeInfo *info) {
    TypeImpl *ti = g_malloc0(sizeof(*ti));
	int i;
	
	/**
	 * the associated value, or NULL if the key is not found.
	 * GLIB-2.0 參考文件：https://developer.gnome.org/glib/stable/glib-Hash-Tables.html#g-hash-table-lookup
	 */
	if (type_table_lookup(info->name) != NULL) {
		printf("Registering `%s' which already exists\n", info->name);
		abort();
	}

	printf("Registering `%s'\n", info->name);

    ti->name = g_strdup(info->name);
	ti->parent = g_strdup(info->parent);

    // instance size, 關係到整個 object 創立空間。
    ti->instance_size = info->instance_size;

    
    ti->class_init = info->class_init;
    // ...
    
    ti->instance_init = info->instance_init;
    // ...

    for (i = 0; info->interfaces && info->interfaces[i].type; i++) {
        ti->interfaces[i].typename = g_strdup(info->interfaces[i].type);
        
        printf("interfaces typename[%s]\n", ti->interfaces[i].typename);

    }
    ti->num_interfaces = i;

    return ti;
}

static void type_initialize(TypeImpl *ti);

static TypeImpl *type_get_by_name(const char *name)
{
	if (name == NULL) {
		return NULL;
	}
	
	return type_table_lookup(name);
}

static TypeImpl *type_register_internal(const TypeInfo *info)
{
    TypeImpl *ti;
    // 會把 typeinfo 的資料
    ti = type_new(info);

    type_table_add(ti);

	// 用於測試 type_initialize
	if (strcmp(ti->name,"device") == 0 ) {
		printf("\n");
		type_initialize(type_get_by_name("vmxnet3"));
	}
	
	return ti;
}



TypeImpl *type_register_static(const TypeInfo *info)
{
    return type_register(info);
}

TypeImpl *type_register(const TypeInfo *info)
{
	return type_register_internal(info);
}



static TypeImpl *type_get_parent(TypeImpl *type)
{
	if (!type->parent_type && type->parent) {
		// 利用字串取得父母的 TypeImpl
		type->parent_type = type_get_by_name(type->parent);
		g_assert(type->parent_type != NULL);
	}
	
	return type->parent_type;
}


static bool type_has_parent(TypeImpl *type)
{
	return (type->parent != NULL);
}


static size_t type_class_get_size(TypeImpl *ti)
{
	if (ti->class_size) {
		return ti->class_size;
	}
	
	if (type_has_parent(ti)) {
		return type_class_get_size(type_get_parent(ti));
	}
	
	return sizeof(ObjectClass);
}


// 
static void object_initialize_with_type(void *data, size_t size, TypeImpl *type)
{
    
}

// memory region, size of memory region, type_memory_region
void object_initialize(void *data, size_t size, const char *typename)
{
    TypeImpl *type = type_get_by_name(typename);
    
    object_initialize_with_type(data, size, type);
}



static bool type_is_ancestor(TypeImpl *type, TypeImpl *target_type)
{
	assert(target_type);
	
	/* Check if target_type is a direct ancestor of type */
	while (type) {
		if (type == target_type) {
			return true;
		}
		
		type = type_get_parent(type);
	}
	
	return false;
}


static void type_initialize_interface(TypeImpl *ti, TypeImpl *interface_type,
									  TypeImpl *parent_type)
{

	InterfaceClass *new_iface;
	TypeInfo info = { };
	TypeImpl *iface_impl;
	
	info.parent = parent_type->name;
	info.name = g_strdup_printf("%s::%s", ti->name, interface_type->name);
	info.abstract = true;
	// 因為 interface 會產生新的 type，在此註冊 vmxnet3::pci-express-device，vmxnet3::conventional-pci-device
	iface_impl = type_new(&info);
	iface_impl->parent_type = parent_type;
	type_initialize(iface_impl);
	g_free((char *)info.name);
	
	new_iface = (InterfaceClass *)iface_impl->class;
	new_iface->concrete_class = ti->class;
	new_iface->interface_type = interface_type;
	
	ti->class->interfaces = g_slist_append(ti->class->interfaces,
										   iface_impl->class);
}

static size_t type_object_get_size(TypeImpl *ti)
{
	// 如果本身有 instance size
	if (ti->instance_size) {
		return ti->instance_size;
	}
	
	if (type_has_parent(ti)) {
		// 取得父親的 object size
		return type_object_get_size(type_get_parent(ti));
	}
	
	return 0;
}

static void object_property_free(gpointer data)
{
//	ObjectProperty *prop = data;
//
//	g_free(prop->name);
//	g_free(prop->type);
//	g_free(prop->description);
//	g_free(prop);
}

static void type_initialize(TypeImpl *ti)
{
	TypeImpl *parent;
	
	if (ti->class) {
		return;
	}


	ti->class_size = type_class_get_size(ti);
	ti->instance_size = type_object_get_size(ti);
	/* Any type with zero instance_size is implicitly abstract.
	 * This means interface types are all abstract.
	 */
	if (ti->instance_size == 0) {
		ti->abstract = true;
	}
	
	ti->class = g_malloc0(ti->class_size);
	
	parent = type_get_parent(ti);
	

	if (parent) {
		type_initialize(parent);
		GSList *e;
		int i;
		
		g_assert_cmpint(parent->class_size, <=, ti->class_size);
		memcpy(ti->class, parent->class, parent->class_size);
		ti->class->interfaces = NULL;
		ti->class->properties = g_hash_table_new_full(
													  g_str_hash, g_str_equal, g_free, object_property_free);

		for (e = parent->class->interfaces; e; e = e->next) {
			InterfaceClass *iface = e->data;
			ObjectClass *klass = OBJECT_CLASS(iface);
			
			type_initialize_interface(ti, iface->interface_type, klass->type);
		}

		for (i = 0; i < ti->num_interfaces; i++) {
			TypeImpl *t = type_get_by_name(ti->interfaces[i].typename);
			for (e = ti->class->interfaces; e; e = e->next) {

				TypeImpl *target_type = OBJECT_CLASS(e->data)->type;

				if (type_is_ancestor(target_type, t)) {
					break;
				}
			}


			if (e) {
				continue;
			}
			type_initialize_interface(ti, t, t);
		}
		
		for (e = ti->class->interfaces; e; e = e->next) {
			printf("%s 已經註冊 interface TypeImpl (%s) \n", ti->name, OBJECT_CLASS(e->data)->type->name);
		}

	} else {
		ti->class->properties = g_hash_table_new_full(
													  g_str_hash, g_str_equal, g_free, object_property_free);
	}
	
	ti->class->type = ti;
	
	while (parent) {
		if (parent->class_base_init) {
			parent->class_base_init(ti->class, ti->class_data);
		}
		parent = type_get_parent(parent);
	}
	
	if (ti->class_init) {
		ti->class_init(ti->class, ti->class_data);
	}
}


ObjectClass *object_class_dynamic_cast(ObjectClass *class,
									   const char *typename)
{
	ObjectClass *ret = NULL;
	TypeImpl *target_type;
	TypeImpl *type;
	
	if (!class) {
		return NULL;
	}
	
	/* A simple fast path that can trigger a lot for leaf classes.  */
	type = class->type;
	if (type->name == typename) {
		return class;
	}
	
	target_type = type_get_by_name(typename);
	if (!target_type) {
		/* target class type unknown, so fail the cast */
		return NULL;
	}
	
	if (type->class->interfaces &&
		type_is_ancestor(target_type, type_interface)) {
		int found = 0;
		GSList *i;
		
		for (i = class->interfaces; i; i = i->next) {
			ObjectClass *target_class = i->data;
			
			if (type_is_ancestor(target_class->type, target_type)) {
				ret = target_class;
				found++;
			}
		}
		
		/* The match was ambiguous, don't allow a cast */
		if (found > 1) {
			ret = NULL;
		}
	} else if (type_is_ancestor(type, target_type)) {
		ret = class;
	}
	
	return ret;
}


ObjectClass *object_class_dynamic_cast_assert(ObjectClass *class,
											  const char *typename,
											  const char *file, int line,
											  const char *func)
{
	ObjectClass *ret;

	
	if (!class || !class->interfaces) {
		return class;
	}
	
	ret = object_class_dynamic_cast(class, typename);
	if (!ret && class) {
		fprintf(stderr, "%s:%d:%s: Object %p is not an instance of type %s\n",
				file, line, func, class, typename);
		abort();
	}
	
out:
	return ret;

}


static void object_instance_init(Object *obj)
{
	printf("object_instance_init\n");

//	object_property_add_str(obj, "type", qdev_get_type, NULL, NULL);
}


static void register_types(void)
{
	static TypeInfo interface_info = {
		.name = TYPE_INTERFACE,
		.class_size = sizeof(InterfaceClass),
		.abstract = true,
	};
	
	static TypeInfo object_info = {
		.name = TYPE_OBJECT,
		.instance_size = sizeof(Object),
		.instance_init = object_instance_init,
		.abstract = true,
	};

	printf("\n");
	printf("\033[33mregister_types interface_info and object_info called..(object.c)\033[0m\n");
	
	type_interface = type_register_internal(&interface_info);
	type_register_internal(&object_info);
}

type_init(register_types)
