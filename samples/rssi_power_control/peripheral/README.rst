.. zephyr:code-sample:: korlinx_rssi_power_control_peripheral
   :name: RSSI and power control (peripheral)
   :relevant-api: bluetooth

   Advertise as the peripheral side of the LE Power Control demonstration.

Overview
********

The peripheral half of a two-board demonstration of Bluetooth LE Power Control.
The board advertises as ``power_control_peripheral`` and accepts a connection
from the :zephyr:code-sample:`korlinx_rssi_power_control_central` sample. After a disconnect
it advertises again.

The central does the RSSI measurement and power adjustment. This sample only
needs LE Power Control (:kconfig:option:`CONFIG_BT_TRANSMIT_POWER_CONTROL`)
enabled.

Requirements
************

* Two boards from the following list. They can be different models:

  * ``nx15/nrf54l15/cpuapp``
  * ``nx40/nrf52840``

* One board running this sample and one running ``rssi_power_control/central``.

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/rssi_power_control/peripheral -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. Then flash ``rssi_power_control/central`` to the
second board. See that sample for the expected output.

References
**********

* Bluetooth Core Specification, LE Power Control feature
