/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(hello, LOG_LEVEL_INF);

int main(void)
{
	printk("Korlinx NX hello sample on %s\n", CONFIG_BOARD_TARGET);
	LOG_INF("logging is up");

	uint32_t count = 0U;

	while (1) {
		printk("printk tick #%u\n", count);
		LOG_INF("LOG tick #%u", count);
		count++;
		k_sleep(K_MSEC(1000));
	}

	return 0;
}
