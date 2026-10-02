.. zephyr:board:: nx15

Overview
********

The Korlinx NX15 is a Bluetooth LE module based on the Nordic Semiconductor
nRF54L15 SoC. This board definition supports the application core
(``nx15/nrf54l15/cpuapp``) and the on-board peripherals described below.

Hardware
********

* Nordic Semiconductor nRF54L15 SoC (Arm Cortex-M33 application core)
* Sensirion SHTC3 temperature and humidity sensor (I2C)
* ST LSM6DSL 6-axis IMU (I2C)
* Puya P25Q32SU 32 Mbit NOR flash (SPI)
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
   * - Red LED (active high)
     - ``led0``
     - P1.09
   * - Blue LED (active high)
     - ``led1``
     - P0.04
   * - Green LED (active high)
     - ``led2``
     - P2.06
   * - Run LED (active high)
     - ``led3``
     - P2.01
   * - Button 1 (active low, pull-up)
     - ``sw0``
     - P1.10

Peripherals
-----------

.. list-table::
   :header-rows: 1

   * - Peripheral
     - Instance
     - Pins
     - Notes
   * - Console UART
     - ``uart20``
     - TX P1.13, RX P1.12
     - ``zephyr,console``, 115200 baud
   * - SHTC3
     - ``i2c21`` at 0x70
     - SDA P1.11, SCL P1.15
     - Devicetree alias ``dht0``
   * - LSM6DSL
     - ``i2c21`` at 0x6a
     - INT1 P1.07
     - P2.05 is held high by a GPIO hog to select I2C mode
   * - P25Q32SU
     - ``spi00``
     - SCK P2.09, MOSI P2.10, MISO P2.04, CS P0.02, WP P0.00, HOLD P0.03
     - ``jedec,spi-nor``, 8 MHz
   * - ADC
     - ``adc`` channel 0
     - AIN7
     - Node label ``vbat_channel``

Programming and Debugging
*************************

.. zephyr:board-supported-runners::

The board uses ``nrfutil`` as the default runner. J-Link is also supported.

Flashing
========

Connect a SEGGER J-Link compatible debug probe to the module's SWD pins. Then
build and flash an application from your nRF Connect SDK workspace, for example
the ``hello`` sample in this repository (``<repo>`` is its path):

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/hello -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

Debugging
=========

.. code-block:: console

   west debug

Log output is available on the console UART and over SEGGER RTT.

References
**********

* nRF54L15: https://www.nordicsemi.com/Products/nRF54L15
