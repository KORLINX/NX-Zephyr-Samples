.. zephyr:code-sample:: korlinx_shtc3
   :name: SHTC3 blocking read
   :relevant-api: sensor_interface

   Read temperature and humidity from the on-board SHTC3 and print them on the console.

Overview
********

This sample reads the on-board Sensirion SHTC3 (devicetree alias ``dht0``)
once per second with :c:func:`sensor_sample_fetch` and
:c:func:`sensor_channel_get`. The console and the log go to the board
console, which is USB CDC ACM on ``nx40/nrf52840`` and UART20 on
``nx15/nrf54l15/cpuapp``::

   <inf> shtc3_sample: Reading shtc3@70 every 1000 ms
   #<n> temp: <T> °C, humidity: <RH> %RH

If the sensor doesn't answer, the sample logs
``Reading shtc3@70 failed: <errno>`` and tries again on the next period.

The sample uses the blocking fetch/get sensor API. For the asynchronous
(RTIO) API on the same sensor, see the ``dht_polling`` sample.

Requirements
************

One of the following boards:

* ``nx15/nrf54l15/cpuapp``: SHTC3 on ``i2c21`` (SDA P1.11, SCL P1.15), address 0x70
* ``nx40/nrf52840``: SHTC3 on ``i2c0`` (SDA P0.06, SCL P0.23), address 0x70

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/shtc3 -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. On ``nx40/nrf52840``, don't add
``--erase``: the application runs behind the UF2 bootloader, and a full erase
removes it.

Viewing the output
==================

On ``nx40/nrf52840``, open the USB CDC ACM port (``/dev/ttyACM<n>`` on Linux,
listed as ``/dev/serial/by-id/usb-KORLINX_NX40_nRF52840_*``) in any serial
terminal. The baud rate doesn't matter, except 1200 bps, which resets the
board into the UF2 bootloader. Log output starts 4 s after boot, so the host
has time to open the port.

On ``nx15/nrf54l15/cpuapp``, open the UART20 port at 115200 baud.

To also get the output over SEGGER RTT through a J-Link, build with the
``rtt-console`` snippet (``west build ... -S rtt-console``) and run
``west rtt -r jlink``.

References
**********

* SHTC3: https://sensirion.com/products/catalog/SHTC3
