/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(shtc3_sample, LOG_LEVEL_INF);

#define SAMPLE_PERIOD_MS 1000

/* The NX boards alias the on-board SHTC3 as dht0. */
static const struct device *const shtc3 = DEVICE_DT_GET(DT_ALIAS(dht0));

static int read_shtc3(struct sensor_value *temp, struct sensor_value *hum)
{
	int rc;

	/* The driver wakes the sensor, measures, then puts it back to sleep. */
	rc = sensor_sample_fetch(shtc3);
	if (rc != 0) {
		return rc;
	}

	rc = sensor_channel_get(shtc3, SENSOR_CHAN_AMBIENT_TEMP, temp);
	if (rc != 0) {
		return rc;
	}

	return sensor_channel_get(shtc3, SENSOR_CHAN_HUMIDITY, hum);
}

int main(void)
{
	uint32_t count = 0U;

	if (!device_is_ready(shtc3)) {
		LOG_ERR("%s is not ready, check the I2C wiring and power", shtc3->name);
		return 0;
	}

	LOG_INF("Reading %s every %d ms", shtc3->name, SAMPLE_PERIOD_MS);

	while (true) {
		struct sensor_value temp;
		struct sensor_value hum;
		int rc;

		rc = read_shtc3(&temp, &hum);
		if (rc != 0) {
			LOG_ERR("Reading %s failed: %d", shtc3->name, rc);
		} else {
			printk("#%u temp: %.2f °C, humidity: %.2f %%RH\n", ++count,
			       sensor_value_to_double(&temp), sensor_value_to_double(&hum));
		}

		k_msleep(SAMPLE_PERIOD_MS);
	}

	return 0;
}
