.. zephyr:code-sample:: korlinx_gatt
   :name: Custom GATT notify
   :relevant-api: bt_gatt bluetooth

   Expose a custom 128-bit GATT service that sends a counter as notifications.

Overview
********

The board advertises as a connectable Bluetooth LE peripheral and exposes one
vendor-specific GATT service with one characteristic:

.. list-table::
   :header-rows: 1

   * - Attribute
     - UUID
     - Properties
   * - Service
     - ``4b4f524c-0001-494e-585f-475054000001``
     -
   * - Counter
     - ``4b4f524c-0002-494e-585f-475054000001``
     - Read, Notify (1 byte)

A one-byte counter increments every :kconfig:option:`CONFIG_GATT_NOTIFY_INTERVAL_MS`
milliseconds (default 3000), whether or not a client is subscribed. When a
client enables notifications, each new value is notified. After a disconnect
the board starts advertising again.

The advertised device name is set per board in ``boards/<board>.conf``:
``Korlinx_NX15_GATT`` or ``Korlinx_NX40_GATT``. The service UUID is included in
the scan response, so scanners can filter on it.

Requirements
************

* One of the following boards:

  * ``nx15/nrf54l15/cpuapp``
  * ``nx40/nrf52840``

* A Bluetooth LE central, for example a phone running nRF Connect for Mobile.

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/gatt -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. To change the notification interval,
add ``-DCONFIG_GATT_NOTIFY_INTERVAL_MS=<ms>`` after ``--``.

To test the sample:

#. Scan for the board's device name in nRF Connect for Mobile and connect.
#. Find the service ``4b4f524c-0001-...`` and enable notifications on the
   counter characteristic.
#. The counter value updates at the configured interval. The board logs each
   notification on the console and over RTT.
