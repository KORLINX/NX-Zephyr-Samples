.. zephyr:code-sample:: korlinx_hello
   :name: Hello console and RTT

   Print to the console and send log messages to both the console and SEGGER RTT.

Overview
********

A minimal bring-up sample for the Korlinx NX boards. It prints a banner with
the board target, then once per second prints a ``printk`` tick and emits a
``LOG_INF`` tick. Log messages go to both the UART console backend and the
SEGGER RTT backend, so you can check both output paths at once.

On ``nx40/nrf52840`` the console is USB CDC ACM. The sample enables USB and
waits 2 seconds before its first print so the host can open the port.

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
to see the ``printk`` and log output, or connect an RTT viewer to see the log
output only.
