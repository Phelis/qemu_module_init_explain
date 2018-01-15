#ifndef QEMU_OBJECT_H
#define QEMU_OBJECT_H


// object 採用 queue.h 內的機制去做收尋
#include "../qemu/queue.h"

struct TypeImpl;
typedef struct TypeImpl *Type;

typedef struct TypeInfo TypeInfo;

#define OBJECT_CLASS_CAST_CACHE 4

struct ObjectClass
{
    /*< private >*/
    Type type;
    GSList *interfaces;
    
    const char *object_cast_cache[OBJECT_CLASS_CAST_CACHE];
    const char *class_cast_cache[OBJECT_CLASS_CAST_CACHE];
    
    ObjectUnparent *unparent;
    
    GHashTable *properties;
};

struct Object
{
    /*< private >*/
    ObjectClass *class;
    ObjectFree *free;
    GHashTable *properties;
    uint32_t ref;
    Object *parent;
};

struct TypeInfo {
    // 裝置名稱
    const char *name;
    // 繼承的父親 type
    const char *parent;
    
    // 基本上只需要 instance 的 function pointers 即可
    size_t instance_size;
    void (*instance_init)(Object *obj);
    void (*instance_post_init)(Object *obj);
    void (*instance_finalize)(Object *obj);
    
    bool abstract;
    size_t class_size;
    
    // 如果需要 class 也就是每一個 type 自己的個別設定，才需要用到 class
    void (*class_init)(ObjectClass *klass, void *data);
    void (*class_base_init)(ObjectClass *klass, void *data);
    void (*class_finalize)(ObjectClass *klass, void *data);
    void *class_data;
    
    InterfaceInfo *interfaces;
};

