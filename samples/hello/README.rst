.. zephyr:code-sample:: korlinx_hello
   :name: Hello console

   Print to the console with printk and the logging subsystem.

Overview
********

A minimal bring-up sample for the Korlinx NX boards. It prints a banner with
the board target, then once per second prints a ``printk`` tick and emits a
``LOG_INF`` tick. Both go to the board console.

On ``nx40/nrf52840`` the console is USB CDC ACM, which the board starts at
boot. Log messages are held for 4 seconds after boot so the host has time to
open the port.

Requirements
************

One of the following boards:

* ``nx15/nrf54l15/cpuapp``: console on UART20 (TX P1.13, RX P1.12), 115200 baud
* ``nx40/nrf52840``: console on USB CDC ACM

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/hello -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. Open a terminal on the console port
to see the ``printk`` and log output.
