/*
 * Copyright (c) 2026 Korlinx
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef GATT_SVC_H_
#define GATT_SVC_H_

#include <stdint.h>

/*
 * Custom Korlinx notify service — vendor 128-bit UUIDs.
 * Service : 4b4f524c-0001-494e-585f-475054000001
 * Counter : 4b4f524c-0002-494e-585f-475054000001  (Read + Notify)
 */

/**
 * Update the counter characteristic value and, if a client has enabled
 * notifications, notify it.
 *
 * The value is always stored (so a GATT read returns the latest value),
 * regardless of whether anyone is subscribed.
 *
 * @param value  Byte to publish.
 *
 * @retval 0          Notified successfully.
 * @retval -ENOTCONN  No client has enabled notifications (value still stored).
 * @retval <0         bt_gatt_notify() error.
 */
int counter_notify(uint8_t value);

#endif /* GATT_SVC_H_ */
