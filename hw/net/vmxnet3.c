#include <stdio.h>
#include "../../include/qemu/module.h"
#include "../../include/qom/object.h"


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
//    type_register_static();
}


// 註冊 vmxnet3 網路卡，經由 macro 去替換成 C function
type_init(vmxnet3_register_types);
