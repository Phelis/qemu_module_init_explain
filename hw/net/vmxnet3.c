
#include <stdio.h>

#include "../../include/qom/object.h"
#include "../../include/qemu/module.h"

#define TYPE_VMXNET3 "vmxnet3"

// 暫時放在這邊, vmxnet3 採用 pci
#define TYPE_PCI_DEVICE "pci-device"

// instance
typedef struct {
	
} VMXNET3State;


static void vmxnet3_instance_init(Object *obj)
{
	printf("vmxnet3_instance_init\n");

    // 初始化這個物件，會把相關的資料寫入
//	VMXNET3State *s = VMXNET3(obj);
//	device_add_bootindex_property(obj, &s->conf.bootindex,
//								  "bootindex", "/ethernet-phy@0",
//								  DEVICE(obj), NULL);
}

static void vmxnet3_class_init(ObjectClass *class, void *data)
{
	printf("vmxnet3_class_init\n");
}


// static
static const TypeInfo vmxnet3_info = {
    .name          = TYPE_VMXNET3,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(VMXNET3State), // 將被用於 object_new_with_type 去建立物件空間，主要是用 qdev_device_add
    .class_init    = vmxnet3_class_init,
    .instance_init = vmxnet3_instance_init,
};

static void vmxnet3_register_types(void)
{
    printf("vmxnet3_register_types called...\n");
	
	// 註冊 type 種類 vmxnet3_info 到系統內，在此時會把 vmxnet3_info 轉成 object 的方式去儲存。
    // 之後要操作上面定義的 instance_init, class_init 會藉由 type_table_lookup 和 type_table_add
    // 被定義在 object.c 內去操作。
    type_register_static(&vmxnet3_info);
}


// 註冊 vmxnet3 網路卡，系統會用 register module init 去寫入
type_init(vmxnet3_register_types);
