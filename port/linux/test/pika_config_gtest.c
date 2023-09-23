#include "pika_config_gtest.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataMemory.h"
#include "PikaVM.h"

char log_buff[LOG_BUFF_MAX][LOG_SIZE] = {0};
uint32_t log_index = 0;

/* save printf content to log_buff */
void pika_platform_printf(char* fmt, ...) {
    for (int i = LOG_BUFF_MAX - 2; i >= 0; i--) {
        memcpy(log_buff[i + 1], log_buff[i], LOG_SIZE);
    }
    va_list args;
    va_start(args, fmt);
    pika_platform_vsnprintf(log_buff[0], LOG_SIZE - 1, fmt, args);
    va_end(args);
    va_start(args, fmt);
    pika_vprintf(fmt, args);
    va_end(args);
}

// static volatile uint64_t tick_ms = 0;
// int64_t pika_platform_get_tick(void) {
//     tick_ms += 50;
//     return tick_ms;
// }

/* quick_malloc is always open */
uint8_t __is_quick_malloc(void) {
    // return 1;
    return 0;
}

void __gtest_hook_default(void) {
    return;
}
typedef void (*hook_func)(void);

volatile int g_hook_cnt = 0;
volatile hook_func g_hook_func = __gtest_hook_default;
void __pks_hook_instruct(void) {
    g_hook_cnt++;
    g_hook_func();
}

volatile pika_bool g_always_repl_mode = pika_true;
PIKA_WEAK void pika_hook_unused_stack_arg(PikaVMFrame* vm, Arg* arg) {
    if (vm->in_repl || g_always_repl_mode) {
        arg_print(arg, pika_true, "\r\n");
    }
}
