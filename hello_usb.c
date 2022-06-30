/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

void core1_entry() {
	int i,t;
	t=time_us_32();
	for(i=0;i<0xffffff;i++) asm volatile("nop");
	multicore_fifo_push_blocking(time_us_32()-t);
	while (true) {
		sleep_ms(1000);
	}
}

int main() {
	int i,t;
	stdio_init_all();
	sleep_ms(3000);
	printf("Hello, multicore world!\n");
	t=time_us_32();
	for(i=0;i<0xffffff;i++) asm volatile("nop");
	printf("Hello, core0 only: %d!\n",time_us_32()-t);
	multicore_launch_core1(core1_entry);
	t=time_us_32();
	for(i=0;i<0xffffff;i++) asm volatile("nop");
	printf("Hello, core0: %d!\n",time_us_32()-t);
	printf("Hello, core1: %d!\n",multicore_fifo_pop_blocking());	
	while (true) {
		sleep_ms(1000);
	}
	return 0;
}
