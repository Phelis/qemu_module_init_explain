#include "../include/qom/object.h"
#include <stdio.h>
// 對於一個 type 最多可以有 32 個介面(interface) 可以繼承和實作
#define MAX_INTERFACES 32

typedef struct InterfaceImpl InterfaceImpl;
typedef struct TypeImpl TypeImpl;


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
	 * 參考：https://developer.gnome.org/glib/stable/glib-Hash-Tables.html#g-hash-table-lookup
	 */
	if (type_table_lookup(info->name) != NULL) {
		printf("Registering `%s' which already exists\n", info->name);
	}

	ti->name = g_strdup(info->name);
	ti->parent = g_strdup(info->parent);

    // ...
    
    return ti;
}

static TypeImpl *type_register_internal(const TypeInfo *info)
{
    TypeImpl *ti;
    // 會把 typeinfo 的資料
    ti = type_new(info);

    type_table_add(ti);
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


