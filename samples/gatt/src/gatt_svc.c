/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/logging/log.h>

#include "gatt_svc.h"

LOG_MODULE_DECLARE(gatt, LOG_LEVEL_INF);

/* Vendor 128-bit UUIDs (see gatt_svc.h). */
#define BT_UUID_KORLINX_SVC_VAL \
	BT_UUID_128_ENCODE(0x4b4f524c, 0x0001, 0x494e, 0x585f, 0x475054000001)
#define BT_UUID_KORLINX_COUNTER_VAL \
	BT_UUID_128_ENCODE(0x4b4f524c, 0x0002, 0x494e, 0x585f, 0x475054000001)

static const struct bt_uuid_128 korlinx_svc_uuid =
	BT_UUID_INIT_128(BT_UUID_KORLINX_SVC_VAL);
static const struct bt_uuid_128 korlinx_counter_uuid =
	BT_UUID_INIT_128(BT_UUID_KORLINX_COUNTER_VAL);

/* Latest counter value — kept so a plain GATT read returns something useful. */
static uint8_t counter_value;
/* Set when a client writes the CCC to enable notifications. */
static bool notify_enabled;

static void counter_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_enabled = (value == BT_GATT_CCC_NOTIFY);
	LOG_INF("Notifications %s", notify_enabled ? "enabled" : "disabled");
}

static ssize_t read_counter(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			    void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
				 &counter_value, sizeof(counter_value));
}

/* attrs[0] service, attrs[1] characteristic decl, attrs[2] value, attrs[3] CCC. */
BT_GATT_SERVICE_DEFINE(korlinx_svc,
	BT_GATT_PRIMARY_SERVICE(&korlinx_svc_uuid),
	BT_GATT_CHARACTERISTIC(&korlinx_counter_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_counter, NULL, &counter_value),
	BT_GATT_CCC(counter_ccc_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

int counter_notify(uint8_t value)
{
	counter_value = value;

	if (!notify_enabled) {
		return -ENOTCONN;
	}

	/* Passing the characteristic declaration attr notifies its value. */
	return bt_gatt_notify(NULL, &korlinx_svc.attrs[1],
			      &counter_value, sizeof(counter_value));
}
