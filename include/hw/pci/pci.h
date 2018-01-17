#define TYPE_PCI_DEVICE "pci-device"

#define INTERFACE_PCIE_DEVICE "pci-express-device"

/* Implemented by devices that can be plugged on Conventional PCI buses */
#define INTERFACE_CONVENTIONAL_PCI_DEVICE "conventional-pci-device"

#define TYPE_PCI_BUS "PCI"

#define TYPE_PCIE_BUS "PCIE"


// 被定義在 include/qemu/typedefs.h 內，目前搬過來用而已
typedef struct PCIDevice PCIDevice;


typedef struct PCIDeviceClass {

    
} PCIDeviceClass;


struct PCIDevice {
    
    
};
