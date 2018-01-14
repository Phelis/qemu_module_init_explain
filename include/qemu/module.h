// 如果有別的地方也定義相同的名稱，系統就不要使用這個 module.h ，定義的名稱為 qemu_module.h
#ifndef QEMU_MODULE_H
#define QEMU_MODULE_H

// 由於採用 constructor attribute ，所以在系統呼叫 main 之前，會先自動執行這個 function
// 定義在 include/qemu/module.c
#define module_init(function, type) \
static void __attribute__((constructor)) do_qemu_init_ ## function(void)		\
{																				\
	register_module_init(function, type);										\
}

typedef enum {
	MODULE_INIT_BLOCK,		// block device
	MODULE_INIT_OPTS,		// options
	MODULE_INIT_QAPI,		// qemu api
	MODULE_INIT_QOM,		// qemu object module
	MODULE_INIT_TRACE,		// trace
	MODULE_INIT_MAX			// max limit
} module_init_type;

// 定義 macro type init 換成 module_init 因為在 qemu 內存在六種類型的 type，如上述。
#define type_init(function) module_init(function, MODULE_INIT_QOM)


// 會在 main 之前使用，被定義在 do_qemu_init_ ## function 裡面
void register_module_init(void (*fn)(void), module_init_type type);

// 會在系統執行 main 的時候呼叫 initialization
void module_call_init(module_init_type type);

#endif
