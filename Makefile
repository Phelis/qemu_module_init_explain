## Compiler ##
CC=llvm-gcc

## Header Files ##
DEPS =./include/hw/pci/pci.h
DEPS+=./include/hw/pci/pci_bus.h
DEPS+=./include/qom/object.h
DEPS+=./include/qemu/module.h
DEPS+=./include/qemu/queue.h
DEPS+=./include/qemu/typedefs.h


## Object Files ##
OBJ=./qom/object.c
OBJ+=./hw/pci/pci.c
OBJ+=./util/module.c
OBJ+=./hw/net/vmxnet3.c
OBJ+=./hw/core/qdev.c
OBJ+=main.c


make:
	$(CC) $(DEPS) $(OBJ) `pkg-config --cflags --libs glib-2.0`


clean:
	$(shell find . -name "*gch" | xargs rm 2>&1)
	$(shell rm ./a.out 2>&1)
	