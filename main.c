#include <stdlib.h>
#include <stdio.h>

#include "./include/qemu/module.h"


// 初始化裝置 function pointer
void (*init)(void);

// 裝置 vmxnet3 的初始化
static void vmxnet3_register_types(void)
{
	printf("啟動 vmxnet3_register_types\n");
}

// universal 的 module initialization function
void module_call_init(module_init_type type)
{
	printf("啟動 module_call_init\n");
	
	// 初始化 vmxnet3
	init();
}


// 裝置模組註冊的
void register_module_init(void (*fn)(void), char type)
{
	printf("註冊 register_module_init\n");
	
	// 更動一下流程方便說明，這裡我們直接做
	init = fn;
}



// 主要的運行程式碼
int main(){
	
	printf("主程式 main()\n");
	
	module_call_init(MODULE_INIT_QOM);

    return 0;
}

// 註冊 vmxnet3 網路卡，經由 macro 去替換成 C function
type_init(vmxnet3_register_types);



