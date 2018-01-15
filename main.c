#include <stdlib.h>
#include <stdio.h>

#include "./include/qemu/module.h"
#include "./include/qom/object.h"

// 主要的運行程式碼
int main(){
	
	printf("主程式 main()\n");
	
	module_call_init(MODULE_INIT_QOM);

    return 0;
}




