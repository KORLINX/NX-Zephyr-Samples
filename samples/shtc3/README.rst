.. zephyr:code-sample:: korlinx_shtc3
   :name: SHTC3 temperature and humidity
   :relevant-api: sensor_interface

   Read temperature and humidity from the on-board SHTC3 sensor.

Overview
********

This sample is the Zephyr ``samples/sensor/dht_polling`` sample adapted to the
Korlinx NX boards. It reads every sensor with a ``dhtN`` devicetree alias
(``N`` from 0 to 9) through the asynchronous sensor API. Once per second it
prints a line in this format::

   <device>: temp is <T> °C humidity is <RH> %RH

The NX boards define ``dht0 = &shtc3``.

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

``<repo>`` is the path to this repository.

References
**********

* SHTC3: https://sensirion.com/products/catalog/SHTC3
