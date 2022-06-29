/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "./core1.h"

static int g_num;

void core1_callback(void){
	g_num+=1;
	request_core1_callback_at(core1_callback,time_us_32()+1000000);
}

int main() {
    stdio_init_all();
    sleep_ms(3000);
    start_core1();
    printf("Core1 started. g_num: %d\n",g_num);
    sleep_ms(500);
    request_core1_callback(core1_callback);
    printf("core1_callback requested. g_num: %d\n",g_num);
    while (true) {
        printf("Hello, world! g_num: %d\n",g_num);
        sleep_ms(1000);
    }
    return 0;
}
