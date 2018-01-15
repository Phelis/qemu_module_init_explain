#include <stdio.h>
#include "../include/qemu/module.h"

// 初始化裝置 function pointer
void (*init)(void);

// 裝置模組註冊的
void register_module_init(void (*fn)(void), module_init_type type)
{
	printf("register_module_init\n");
	
	// 更動一下流程方便說明，這裡我們直接做
	init = fn;
}

// universal 的 module initialization function
void module_call_init(module_init_type type)
{
	printf("module_call_init\n");
	
	// 初始化 vmxnet3
	init();
}

