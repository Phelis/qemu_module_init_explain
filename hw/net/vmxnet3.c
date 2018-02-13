/*
 * 目前看來在每一個 TypeInfo 檔案內，只會幫忙填寫需要的資料
 */

#include "../../include/hw/pci/pci.h"
#include "../../include/hw/pci/pci_bus.h"

#include <stdio.h>

// type info 必須要加的兩個 headers, 1) object.h, 2) module.h
#include "../../include/qom/object.h"
#include "../../include/qemu/module.h"
#include "../../include/hw/qdev-core.h"

#define TYPE_VMXNET3 "vmxnet3"

#define VMXNET3_DEVICE_CLASS(klass) \
	OBJECT_CLASS_CHECK(VMXNET3Class, (klass), TYPE_VMXNET3)


typedef struct VMXNET3Class {
    PCIDeviceClass parent_class;
    DeviceRealize parent_dc_realize;
} VMXNET3Class;


// VMXNET3State 為系統建構起來後，對於 vmxnet3 所需要的 instance 結構。自己的空間和父類別的空間都會在這裡填滿。
typedef struct {
    // 定義在 pci.h，由於 vmxnet3_info 的 parent 為 TYPE_PCI_DEVICE 所以會需要一個 PCIDevice 在 vmxnet3tate 內。
    PCIDevice parent_obj;
    // ..
} VMXNET3State;

#define VMXNET3(obj) OBJECT_CHECK(VMXNET3State, (obj), TYPE_VMXNET3)

/*
 * 寫入 properties 到 vmxnet3 網路卡內
 */
static void vmxnet3_instance_init(Object *obj)
{
	printf("vmxnet3_instance_init\n");

    // 初始化這個物件，會把相關的資料寫入, VMXNET3 做的是 type_check
//    VMXNET3State *s = VMXNET3(obj);
    
    
//	device_add_bootindex_property(obj, &s->conf.bootindex,
//								  "bootindex", "/ethernet-phy@0",
//								  DEVICE(obj), NULL);
}


static void vmxnet3_class_init(ObjectClass *class, void *data)
{
	printf("vmxnet3_class_init %p\n", class);

	DeviceClass *dc = DEVICE_CLASS(class);
	PCIDeviceClass *c = PCI_DEVICE_CLASS(class);
	VMXNET3Class *vc = VMXNET3_DEVICE_CLASS(class);		// 空間夠
	printf("vmxnet3_class_init %p\n", c);

}


// static const 代表 .name 的值不可以做修改，否則會被編譯器判斷 const-qualified type 錯誤
static const TypeInfo vmxnet3_info = {
    .name          = TYPE_VMXNET3,
    .parent        = TYPE_PCI_DEVICE,
    .class_size    = sizeof(VMXNET3Class),
    .instance_size = sizeof(VMXNET3State), // 將被用於 object_new_with_type 去建立物件空間，主要是用 qdev_device_add
    .class_init    = vmxnet3_class_init,
    .instance_init = vmxnet3_instance_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_PCIE_DEVICE },
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void vmxnet3_register_types(void)
{
    printf("\n");
    printf("\033[33mvmxnet3_register_types called...(vmxnet3.c)\033[0m\n");
	
    // 註冊 type 種類 vmxnet3_info 到系統內，在此時會把 vmxnet3_info 轉成 object 的方式去儲存。
    // 之後要操作上面定義的 instance_init, class_init 會藉由 type_table_lookup 和 type_table_add
    // 被定義在 object.c 內去操作。
    type_register_static(&vmxnet3_info);
}


// 註冊 vmxnet3 網路卡，系統會用 register module init 去寫入
type_init(vmxnet3_register_types);
