/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>

static const struct device *const uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

K_MSGQ_DEFINE(rx_msgq, sizeof(uint8_t), 64, 1);

static void uart_rx_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	ARG_UNUSED(user_data);

	if (!uart_irq_update(dev)) {
		return;
	}

	while (uart_irq_rx_ready(dev) && uart_fifo_read(dev, &c, 1) == 1) {
		(void)k_msgq_put(&rx_msgq, &c, K_NO_WAIT);
	}
}

int main(void)
{
	int err;

	if (!device_is_ready(uart_dev)) {
		printk("UART device %s not ready\n", uart_dev->name);
		return 0;
	}

	err = uart_irq_callback_user_data_set(uart_dev, uart_rx_cb, NULL);
	if (err) {
		printk("UART interrupt-driven API not supported (err %d)\n", err);
		return 0;
	}
	uart_irq_rx_enable(uart_dev);

	printk("=== Korlinx NX UART test on %s ===\n", CONFIG_BOARD_TARGET);
	printk("console UART: %s  |  115200 8N1\n", uart_dev->name);

	uint32_t count = 0U;
	int64_t next_beat = k_uptime_get();

	while (1) {
		uint8_t c;

		while (k_msgq_get(&rx_msgq, &c, K_NO_WAIT) == 0) {
			uart_poll_out(uart_dev, c);
		}

		if (k_uptime_get() >= next_beat) {
			printk("uart alive #%u\n", count++);
			next_beat += 1000;
		}

		k_sleep(K_MSEC(10));
	}

	return 0;
}
