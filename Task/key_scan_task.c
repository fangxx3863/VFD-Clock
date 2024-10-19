#include "key_scan_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "key.h"

void key_scan_task() {
    while (1) {
        KEY_Proc();
        osDelay(25);
    }
}
