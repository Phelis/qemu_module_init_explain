#include <stdlib.h>
#include <stdio.h>

#include "./include/qemu/module.h"
#include "./include/qom/object.h"

//// 初始化裝置 function pointer
//void (*init)(void);

// 裝置 vmxnet3 的初始化


//// universal 的 module initialization function
//void module_call_init(module_init_type type)
//{
//	printf("啟動 module_call_init\n");
//
//	// 初始化 vmxnet3
//	init();
//}


//// 裝置模組註冊的
//void register_module_init(void (*fn)(void), char type)
//{
//	printf("註冊 register_module_init\n");
//
//	// 更動一下流程方便說明，這裡我們直接做
//	init = fn;
//}

static const TypeInfo vmxnet3_info = {
//	.name          = TYPE_VMXNET3,
//	.parent        = TYPE_PCI_DEVICE,
	//    .instance_size = sizeof(VMXNET3State),
//	.class_init    = vmxnet3_class_init,
//	.instance_init = vmxnet3_instance_init,
};

static void vmxnet3_register_types(void)
{
	printf("vmxnet3_register_types\n");
	
	// 註冊 typeinfo 名稱 vmxnet3_info 到系統內
	type_register_static(&vmxnet3_info);
}

// 主要的運行程式碼
int main(){
	
	printf("主程式 main()\n");
	
	module_call_init(MODULE_INIT_QOM);

    return 0;
}




