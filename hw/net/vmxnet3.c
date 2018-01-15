
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

//	VMXNET3State *s = VMXNET3(obj);
//	device_add_bootindex_property(obj, &s->conf.bootindex,
//								  "bootindex", "/ethernet-phy@0",
//								  DEVICE(obj), NULL);
}

static void vmxnet3_class_init(ObjectClass *class, void *data)
{
	printf("vmxnet3_class_init\n");
}


static const TypeInfo vmxnet3_info = {
    .name          = TYPE_VMXNET3,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(VMXNET3State),
    .class_init    = vmxnet3_class_init,
    .instance_init = vmxnet3_instance_init,
};

static void vmxnet3_register_types(void)
{
    printf("vmxnet3_register_types\n");
	
	// 註冊 typeinfo 名稱 vmxnet3_info 到系統內
    type_register_static(&vmxnet3_info);
}


// 註冊 vmxnet3 網路卡，經由 macro 去替換成 C function
type_init(vmxnet3_register_types);
