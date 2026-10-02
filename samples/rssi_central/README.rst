.. zephyr:code-sample:: korlinx_rssi_central
   :name: RSSI and power control (central)
   :relevant-api: bluetooth

   Connect to a peripheral and log the connection RSSI and peer TX power while
   LE Power Control adjusts the link.

Overview
********

The central half of a two-board demonstration of Bluetooth LE Power Control.
The board scans for a device named ``power_control_peripheral``, connects to
it, and enables TX power reporting. Every 500 ms it logs the peer's TX power
and the connection RSSI::

   Tx: <dBm> dBm, RSSI: <dBm> dBm

Power Control is configured through the SoftDevice Controller vendor-specific
command ``sdc_hci_cmd_vs_set_power_control_request_params()`` with these values,
defined at the top of ``src/main.c``:

.. list-table::
   :header-rows: 1

   * - Parameter
     - Value
   * - RSSI lower / upper limit
     - -60 dBm / -40 dBm
   * - RSSI lower / upper target
     - -55 dBm / -45 dBm
   * - Wait period
     - 1000 ms

Move the boards closer together or further apart to watch the peer's TX power
follow the RSSI.

Requirements
************

* Two boards from the following list. They can be different models:

  * ``nx15/nrf54l15/cpuapp``
  * ``nx40/nrf52840``

* One board running this sample and one running ``rssi_peripheral``.

This sample uses the nRF Connect SDK SoftDevice Controller and Bluetooth scan
library, so it does not build with the Zephyr Bluetooth LE controller.

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/rssi_central -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. Power the board running
``rssi_peripheral``, then watch the log output on the console or over RTT.
