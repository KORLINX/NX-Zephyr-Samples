/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <hal/nrf_power.h>

#define DFU_MAGIC_UF2_RESET 0x57

#define TOUCH_POLL_MS 100

static const struct device *const cdc_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

static void uf2_touch_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	if (!device_is_ready(cdc_dev)) {
		return;
	}

	while (true) {
		uint32_t baudrate = 0U;
		uint32_t dtr = 1U;

		(void)uart_line_ctrl_get(cdc_dev, UART_LINE_CTRL_BAUD_RATE, &baudrate);
		(void)uart_line_ctrl_get(cdc_dev, UART_LINE_CTRL_DTR, &dtr);

		if (baudrate == 1200U && dtr == 0U) {
			nrf_power_gpregret_set(NRF_POWER, 0, DFU_MAGIC_UF2_RESET);
			NVIC_SystemReset();
		}

		k_msleep(TOUCH_POLL_MS);
	}
}

K_THREAD_DEFINE(uf2_touch, 512, uf2_touch_thread, NULL, NULL, NULL,
		K_LOWEST_APPLICATION_THREAD_PRIO, 0, 0);
