/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/logging/log.h>
#include "gatt_svc.h"

LOG_MODULE_REGISTER(gatt, LOG_LEVEL_INF);

#define DEVICE_NAME     CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* Advertised so nRF Connect can filter for this device by service UUID. */
#define BT_UUID_KORLINX_SVC_VAL \
	BT_UUID_128_ENCODE(0x4b4f524c, 0x0001, 0x494e, 0x585f, 0x475054000001)

/* Connectable advertising with a fast interval. */
static const struct bt_le_adv_param *adv_param =
	BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONN,
			BT_GAP_ADV_FAST_INT_MIN_2,
			BT_GAP_ADV_FAST_INT_MAX_2,
			NULL);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_KORLINX_SVC_VAL),
};

static void notify_work_handler(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(notify_work, notify_work_handler);

static void advertising_start(void)
{
	int err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return;
	}
	LOG_INF("Advertising as \"%s\"", DEVICE_NAME);
}

/* Fires every CONFIG_GATT_NOTIFY_INTERVAL_MS. The counter advances regardless
 * of whether a client is subscribed; counter_notify() only hits the radio when
 * it is.
 */
static void notify_work_handler(struct k_work *work)
{
	static uint8_t counter;
	int err = counter_notify(counter);

	if (err == 0) {
		LOG_INF("Notified counter = %u", counter);
	} else if (err == -ENOTCONN) {
		LOG_DBG("No subscriber; counter = %u (not sent)", counter);
	} else {
		LOG_WRN("Notify failed (err %d); counter = %u", err, counter);
	}

	counter++;
	k_work_reschedule(&notify_work, K_MSEC(CONFIG_GATT_NOTIFY_INTERVAL_MS));
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection failed (err 0x%02x)", err);
		return;
	}
	LOG_INF("Connected");
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason 0x%02x)", reason);
}

static void recycled(void)
{
	/* Connection object freed after a disconnect — advertise again. */
	advertising_start();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
	.recycled = recycled,
};

int main(void)
{
	int err;

	LOG_INF("Starting Korlinx custom GATT notify sample on %s", CONFIG_BOARD_TARGET);

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return 0;
	}
	LOG_INF("Bluetooth initialized");

	advertising_start();

	/* Kick off the notify loop. */
	k_work_schedule(&notify_work, K_MSEC(CONFIG_GATT_NOTIFY_INTERVAL_MS));

	return 0;
}
