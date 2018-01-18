
make:
	llvm-gcc ./include/hw/pci/pci.h ./include/hw/pci/pci_bus.h ./include/qom/object.h ./include/qemu/module.h ./include/qemu/queue.h ./qom/object.c ./hw/pci/pci.c ./util/module.c ./hw/net/vmxnet3.c main.c `pkg-config --cflags --libs glib-2.0`



clean:
	$(shell find . -name "*gch" | xargs rm 2>&1)
	$(shell rm ./a.out 2>&1)
	