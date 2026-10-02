.. zephyr:board:: nx40

Overview
********

The Korlinx NX40 is a Bluetooth LE module based on the Nordic Semiconductor
nRF52840 SoC. This board definition (``nx40/nrf52840``) supports the on-board
peripherals described below and uses USB CDC ACM as the console.

Hardware
********

* Nordic Semiconductor nRF52840 SoC (Arm Cortex-M4F)
* Sensirion SHTC3 temperature and humidity sensor (I2C)
* ST LSM6DSL 6-axis IMU (I2C)
* MSM261D3526 PDM microphone
* Puya P25Q32SU 32 Mbit NOR flash (QSPI)
* USB device
* Four LEDs and one push button

Supported Features
==================

.. zephyr:board-supported-hw::

Connections and IOs
===================

LEDs and button
---------------

.. list-table::
   :header-rows: 1

   * - Function
     - Devicetree alias
     - Pin
   * - Red LED (active low)
     - ``led0``
     - P0.11
   * - Blue LED (active low)
     - ``led1``
     - P0.08
   * - Green LED (active low)
     - ``led2``
     - P0.19
   * - Run LED (active low)
     - ``led3``
     - P0.14
   * - Button 1 (active low, pull-up)
     - ``sw0``
     - P0.07

Peripherals
-----------

.. list-table::
   :header-rows: 1

   * - Peripheral
     - Instance
     - Pins
     - Notes
   * - Console
     - ``cdc_acm_uart0`` on ``usbd``
     - USB D+/D-
     - ``zephyr,console``, ``zephyr,shell-uart``
   * - SHTC3
     - ``i2c0`` at 0x70
     - SDA P0.06, SCL P0.23
     - Devicetree alias ``dht0``
   * - LSM6DSL
     - ``i2c0`` at 0x6a
     - INT1 P1.00
     - P0.17 is held high by a GPIO hog to select I2C mode
   * - P25Q32SU
     - ``qspi``
     - SCK P1.07, IO0 P1.10, IO1 P1.14, IO2 P1.13, IO3 P1.12, CSN P1.11
     - ``nordic,qspi-nor``, 24 MHz, quad I/O
   * - MSM261D3526
     - ``pdm0`` (node label ``dmic_dev``)
     - CLK P1.02, DIN P1.03
     -
   * - ADC
     - ``adc`` channel 7
     - AIN7
     - Node label ``vbat_channel``

The console uses the legacy USB device stack
(:kconfig:option:`CONFIG_USB_DEVICE_STACK`). Applications must call
``usb_enable()`` before the console is available.

Programming and Debugging
*************************

.. zephyr:board-supported-runners::

The board uses ``nrfutil`` as the default runner. ``nrfjprog``, J-Link, pyOCD
and OpenOCD are also supported.

Flashing
========

Connect a debug probe to the module's SWD pins. Then build and flash an
application from your nRF Connect SDK workspace, for example the ``hello``
sample in this repository (``<repo>`` is its path):

.. code-block:: console

   west build -b nx40/nrf52840 <repo>/samples/hello -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

Debugging
=========

.. code-block:: console

   west debug

Log output is available on the USB CDC ACM console and over SEGGER RTT.

References
**********

* nRF52840: https://www.nordicsemi.com/Products/nRF52840
