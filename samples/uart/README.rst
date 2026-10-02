.. zephyr:code-sample:: korlinx_uart
   :name: UART smoke test
   :relevant-api: uart_interface

   Send a heartbeat on the console UART and echo back every received byte.

Overview
********

This sample checks both directions of the board's console UART. Once per
second it prints a ``uart alive #N`` heartbeat, and it echoes back every byte
it receives, using the interrupt-driven UART API for reception.

Requirements
************

One of the following boards:

* ``nx15/nrf54l15/cpuapp``: console on UART20 (TX P1.13, RX P1.12)
* ``nx40/nrf52840``: console on USB CDC ACM

A serial terminal set to 115200 baud, 8N1.

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/uart -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. Open the console port in a serial
terminal. The heartbeat appears once per second, and every character you type
is echoed back.
