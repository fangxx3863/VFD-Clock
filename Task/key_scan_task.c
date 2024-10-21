#include "key_scan_task.h"
#include "cmsis_os.h"

#include "key.h"

// 按键扫描任务
void key_scan_task() {
    while (1) {
        KEY_Proc();
        osDelay(25);
    }
}
