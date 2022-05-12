/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

void core1_entry() {
	static int i=1000;
	multicore_fifo_push_blocking(i++);
	printf("Hello, core1: %d!\n",multicore_fifo_pop_blocking());
}

int main() {
	int i=2000;
	stdio_init_all();
	sleep_ms(3000);
	printf("Hello, multicore world!\n");
	while (true) {
		sleep_ms(1000);
		multicore_reset_core1();
		multicore_launch_core1(core1_entry);
		multicore_fifo_push_blocking(i++); // This function must be called after calling multicore_launch_core1()
		sleep_ms(1000);
		printf("Hello, core0: %d!\n",multicore_fifo_pop_blocking());
	}
	return 0;
}
