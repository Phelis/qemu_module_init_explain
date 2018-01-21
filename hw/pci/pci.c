#include "../../include/hw/pci/pci.h"
#include "../../include/hw/pci/pci_bus.h"
#include <stdio.h>

// type info 必須要加的兩個 headers, 1) object.h, 2) module.h
#include "../../include/qom/object.h"
#include "../../include/qemu/module.h"

// 不應該放在這邊?
#include "../../include/hw/qdev-core.h"

#define DEBUG_PCI
#ifdef DEBUG_PCI
#define PCI_DPRINTF(format, ...)       printf(format, ## __VA_ARGS__)
#else
#define PCI_DPRINTF(format, ...)       do { } while (0)
#endif


// qdev-core
#define TYPE_DEVICE "device"

static void pci_bus_class_init(ObjectClass *klass, void *data)
{
    printf("pci_bus_class_init\n");

}


static const TypeInfo pci_bus_info = {
    .name = TYPE_PCI_BUS,
    .parent = TYPE_BUS,
    .instance_size = sizeof(PCIBus),      // 定義在 include/qemu/typedefs.h
    .class_size = sizeof(PCIBusClass),
    .class_init = pci_bus_class_init,
};

static const TypeInfo pcie_bus_info = {
    .name = TYPE_PCIE_BUS,
    .parent = TYPE_PCI_BUS,
};

static void pci_device_class_init(ObjectClass *klass, void *data)
{
    printf("pci_device_class_init\n");
	
	
}

static void pci_device_class_base_init(ObjectClass *klass, void *data)
{
    printf("pci_device_class_base_init\n");
}


static const TypeInfo pcie_interface_info = {
    .name          = INTERFACE_PCIE_DEVICE,     // 定義在 pci.h 內
    .parent        = TYPE_INTERFACE,            // 定義在 object.h 內
};

static const TypeInfo conventional_pci_interface_info = {
    .name          = INTERFACE_CONVENTIONAL_PCI_DEVICE,     // 定義在 pci.h 內
    .parent        = TYPE_INTERFACE,                        // 定義在 object.h 內
};


static const TypeInfo pci_device_type_info = {
	.name = TYPE_PCI_DEVICE,
	.parent = TYPE_DEVICE,
	.instance_size = sizeof(PCIDevice),
	.abstract = true,
	.class_size = sizeof(PCIDeviceClass),
	.class_init = pci_device_class_init,
	.class_base_init = pci_device_class_base_init,
};


static void pci_register_types(void)
{
    PCI_DPRINTF("\n");
    PCI_DPRINTF("\033[33mpci_register_types called...(pci.c)\033[0m\n");

    type_register_static(&pci_bus_info);
    type_register_static(&pcie_bus_info);
    type_register_static(&conventional_pci_interface_info);
    type_register_static(&pcie_interface_info);
    type_register_static(&pci_device_type_info);
}


type_init(pci_register_types)
