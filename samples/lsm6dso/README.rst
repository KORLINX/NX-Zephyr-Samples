.. zephyr:code-sample:: korlinx_lsm6dso
   :name: LSM6DSL IMU
   :relevant-api: sensor_interface

   Read accelerometer and gyroscope data from the on-board LSM6DSL sensor.

Overview
********

This sample is the Zephyr ``samples/sensor/lsm6dso`` sample adapted to the
LSM6DSL fitted on the Korlinx NX boards. It sets the accelerometer and
gyroscope output data rates to 12.5 Hz. With
:kconfig:option:`CONFIG_LSM6DSL_TRIGGER` enabled (the default in ``prj.conf``),
it prints a sample each time the data-ready interrupt fires::

   accel x:<x> ms/2 y:<y> ms/2 z:<z> ms/2
   gyro x:<x> rad/s y:<y> rad/s z:<z> rad/s
   trig_cnt:<n>

Requirements
************

One of the following boards:

* ``nx15/nrf54l15/cpuapp``: LSM6DSL on ``i2c21``, address 0x6a, INT1 on P1.07
* ``nx40/nrf52840``: LSM6DSL on ``i2c0``, address 0x6a, INT1 on P1.00

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/lsm6dso -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository.

References
**********

* LSM6DSL: https://www.st.com/en/mems-and-sensors/lsm6dsl.html
