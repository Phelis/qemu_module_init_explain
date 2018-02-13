#include "../../include/hw/pci/pci.h"
#include "../../include/hw/pci/pci_bus.h"
#include <stdio.h>

// type info 必須要加的兩個 headers, 1) object.h, 2) module.h
#include "../../include/qom/object.h"
#include "../../include/qemu/module.h"
#include "../../include/hw/qdev-core.h"

/* Unlink device from bus and free the structure.  */
static void device_finalize(Object *obj)
{
	printf("device_finalize\n");

}

static void device_initfn(Object *obj)
{
	printf("device_initfn\n");

}
static void device_post_init(Object *obj)
{
	printf("device_post_init\n");

}

static void device_class_base_init(ObjectClass *class, void *data)
{
	printf("device_class_base_init\n");

	DeviceClass *klass = DEVICE_CLASS(class);
	
	/* We explicitly look up properties in the superclasses,
	 * so do not propagate them to the subclasses.
	 */
//	klass->props = NULL;
}

static void device_class_init(ObjectClass *class, void *data)
{
	printf("device_class_init\n");
	
	// 將原本的 object class 轉換成 device class
	DeviceClass *dc = DEVICE_CLASS(class);
	
//	class->unparent = device_unparent;
//	dc->realize = device_realize;
//	dc->unrealize = device_unrealize;
//
//	/* by default all devices were considered as hotpluggable,
//	 * so with intent to check it in generic qdev_unplug() /
//	 * device_set_realized() functions make every device
//	 * hotpluggable. Devices that shouldn't be hotpluggable,
//	 * should override it in their class_init()
//	 */
	dc->hotpluggable = true;
	dc->user_creatable = true;
}

// 執行起來時的 device instance state 儲存的位置
typedef struct DeviceState DeviceState;

static const TypeInfo device_type_info = {
	.name = TYPE_DEVICE,
	.parent = TYPE_OBJECT,
	.instance_size = sizeof(DeviceState),
	.instance_init = device_initfn,
	.instance_post_init = device_post_init,
	.instance_finalize = device_finalize,
	.class_base_init = device_class_base_init,
	.class_init = device_class_init,
	.abstract = true,
	.class_size = sizeof(DeviceClass),
};

static void qdev_register_types(void)
{
	printf("\n");
	printf("\033[33mqdev_register_types called...(qdev.c)\033[0m\n");

	type_register_static(&device_type_info);
}

type_init(qdev_register_types)
