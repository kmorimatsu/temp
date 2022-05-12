/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

void core1_entry() {
	printf("Hello, core1!\n");
	// multicore_reset_core1(); // This function doesn't work by core1
}

int main() {
	stdio_init_all();
	sleep_ms(3000);
	printf("Hello, multicore world!\n");
	multicore_reset_core1();
	multicore_launch_core1(core1_entry);
	sleep_ms(1000);
	printf("Hello, core0!\n");
	sleep_ms(1000);
	multicore_reset_core1();// This line is needed to start core1 again. Use this function by core0
	multicore_launch_core1(core1_entry);
	while (true) {
		sleep_ms(1000);
		printf("Hello, world!\n");
	}
	return 0;
}
