.. zephyr:code-sample:: korlinx_dtm
   :name: Direct Test Mode

   Run Bluetooth LE Direct Test Mode (DTM) for RF testing over a two-wire UART.

Overview
********

This sample is the nRF Connect SDK Direct Test Mode sample adapted to the
Korlinx NX boards. It implements the DTM two-wire UART interface defined in the
Bluetooth Core Specification (Vol 6, Part F). A tester sends DTM commands over
the UART to start transmitter and receiver tests, for example during RF
qualification or production testing.

The console is disabled. The DTM transport uses the UART selected by the
``ncs,dtm-uart`` chosen node at 19200 baud:

.. list-table::
   :header-rows: 1

   * - Board
     - DTM UART
   * - ``nx15/nrf54l15/cpuapp``
     - UART20 (TX P1.13, RX P1.12)
   * - ``nx40/nrf52840``
     - USB CDC ACM

The ``boards/*.overlay`` files set these per board. Log output, if any, goes to
SEGGER RTT.

Requirements
************

* One of the following boards:

  * ``nx15/nrf54l15/cpuapp``
  * ``nx40/nrf52840``

* A DTM tester: a Bluetooth LE tester, or a second nRF device with the
  Direct Test Mode app in nRF Connect for Desktop.

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/dtm -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. Connect the tester to the DTM UART
at 19200 baud, 8N1, and send DTM commands.

References
**********

* Bluetooth Core Specification, Vol 6, Part F: Direct Test Mode
* nRF Connect SDK: Direct Test Mode sample documentation,
  https://docs.nordicsemi.com/
