#include "../qdev-core.h"


#define TYPE_PCI_DEVICE "pci-device"

#define PCI_DEVICE_CLASS(klass) \
	OBJECT_CLASS_CHECK(PCIDeviceClass, (klass), TYPE_PCI_DEVICE)

#define INTERFACE_PCIE_DEVICE "pci-express-device"

/* Implemented by devices that can be plugged on Conventional PCI buses */
#define INTERFACE_CONVENTIONAL_PCI_DEVICE "conventional-pci-device"

#define TYPE_PCI_BUS "PCI"

#define TYPE_PCIE_BUS "PCIE"

// 被定義在 include/qemu/typedefs.h 內，目前搬過來用而已
typedef struct PCIDevice PCIDevice;


typedef struct PCIDeviceClass {
	DeviceClass parent_class;

	int is_bridge;
	
	/* pcie stuff */
	int is_express;   /* is this device pci express? */
	
	/* rom bar */
	const char *romfile;

    
} PCIDeviceClass;


struct PCIDevice {
    
    
};
