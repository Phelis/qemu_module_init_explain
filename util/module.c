#include <stdio.h>
#include "../include/qemu/module.h"
#include "../include/qemu/queue.h"
// 初始化裝置 function pointer
void (*init)(void);

typedef struct ModuleEntry
{
	void (*init)(void);
	QTAILQ_ENTRY(ModuleEntry) node;
	module_init_type type;
} ModuleEntry;

// 創立第一個 head
typedef QTAILQ_HEAD(, ModuleEntry) ModuleTypeList;

static ModuleTypeList init_type_list[MODULE_INIT_MAX];

static ModuleTypeList dso_init_list;

static void init_lists(void)
{
	// 判斷是否已經初始化
	static int inited;
	int i;
	
	if (inited) {
		return;
	}
	
	for (i = 0; i < MODULE_INIT_MAX; i++) {
		QTAILQ_INIT(&init_type_list[i]);
	}
	
	QTAILQ_INIT(&dso_init_list);
	
	inited = 1;
}

static ModuleTypeList *find_type(module_init_type type)
{
	init_lists();
	
	return &init_type_list[type];
}


// 裝置模組註冊的
void register_module_init(void (*fn)(void), module_init_type type)
{
	printf("register_module_init\n");

	ModuleEntry *e;
	ModuleTypeList *l;
	
	e = g_malloc0(sizeof(*e));
	e->init = fn;
	e->type = type;
	
	l = find_type(type);
	
	QTAILQ_INSERT_TAIL(l, e, node);

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

