#include <stdlib.h>
#include <stdio.h>

typedef enum {
	MODULE_INIT_BLOCK,		// block device
	MODULE_INIT_OPTS,		// options
	MODULE_INIT_QAPI,		// qemu api
	MODULE_INIT_QOM,		// qemu object module
	MODULE_INIT_TRACE,		// trace
	MODULE_INIT_MAX			// max limit
} module_init_type;

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

// 由於採用 constructor attribute ，所以在系統呼叫 main 之前，會先自動執行這個 function
// 定義在 include/qemu/module.c
#define module_init(function, type) \
static void __attribute__((constructor)) do_qemu_init_ ## function(void)	\
{																			\
	printf("註冊 do_qemu_init_vmxnet3_register_types\n");						\
	register_module_init(function, type);									\
}

// 定義 macro type init 換成 module_init 因為在 qemu 內存在六種類型的 type，如上述。
#define type_init(function) module_init(function, MODULE_INIT_QOM)

// 主要的運行程式碼
int main(){
	
	printf("主程式 main()\n");
	
	module_call_init(MODULE_INIT_QOM);

    return 0;
}

// 註冊 vmxnet3 網路卡，經由 macro 去替換成 C function
type_init(vmxnet3_register_types);



