/*
 * QEMU Module Infrastructure
 */

#include <stdio.h>
#include "../include/qemu/module.h"
#include "../include/qemu/queue.h"


typedef struct ModuleEntry
{
	void (*init)(void);
	QTAILQ_ENTRY(ModuleEntry) node;
	module_init_type type;
} ModuleEntry;

// 建立一個 head structure 有 tqh_first 和 tqh_last 沒有 qualifier, type 為 ModuleEntry
typedef QTAILQ_HEAD(, ModuleEntry) ModuleTypeList;

// 根據定義在檔案 module.c 在 enum 內，自動被 assign 一個 number
static ModuleTypeList init_type_list[MODULE_INIT_MAX];

static ModuleTypeList dso_init_list;

static void init_lists(void)
{
	// 判斷是否已經初始化，加入 static 代表的意義不同，變成 global
	static int inited;
	int i;
	
    // 不管呼叫多少次，在此只會執行一次，因為 inited 是被寫入 heap memory 而非 stack
    // 採用 user process 的 4G 去看 inited 位址， why?
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
}

// universal 的 module initialization function
void module_call_init(module_init_type type)
{
	printf("module_call_init\n");
	
	// 初始化 vmxnet3
    ModuleTypeList *l;
    ModuleEntry *e;
    
    l = find_type(type);
    
    QTAILQ_FOREACH(e, l, node) {
        e->init();
    }
}

