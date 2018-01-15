

// 對於一個 type 最多可以有 32 個介面(interface) 可以繼承和實作
#define MAX_INTERFACES 32

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

static TypeImpl *type_new(const TypeInfo *info) {
    TypeImpl *ti = g_malloc0(sizeof(*ti));
    
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

TypeImpl *type_register(const TypeInfo *info)
{
    return type_register_internal(info);
}


TypeImpl *type_register_static(const TypeInfo *info)
{
    return type_register(info);
}
