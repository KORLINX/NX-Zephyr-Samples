.. zephyr:code-sample:: korlinx_dm
   :name: Distance measurement

   Measure the distance between two boards with the nRF Distance Measurement
   library.

Overview
********

This sample is the nRF Connect SDK ``nrf_dm`` sample adapted to the Korlinx
NX40. Each board both advertises and scans as ``nRF_DM``. When a board finds a
peer that supports distance measurement, the two synchronize and run ranging
using the Multi-Carrier Phase Difference (MCPD) method by default. The Distance
Measurement library (:kconfig:option:`CONFIG_DM_MODULE`) handles the ranging.

Each result is printed to the console with the peer address, the quality and
the distance estimates. The sample also exposes the Distance Measurement
service (DDFS), so a connected central can read the results and switch the
ranging mode between MCPD and Round-Trip Timing (RTT).

User interface
==============

.. list-table::
   :header-rows: 1

   * - Indicator
     - Function
   * - Blue LED (``led1``)
     - Blinks once per second while the sample runs
   * - Green LED (``led2``)
     - On while a Bluetooth LE connection is open
   * - Red LED (P0.11, PWM)
     - Brightness follows the measured distance: brighter when closer

The overlay ``boards/nx40_nrf52840.overlay`` also routes two ranging debug
signals to P0.27 (``dm-ranging``) and P0.26 (``dm-add-request``) for a logic
analyzer.

Requirements
************

* Two ``nx40/nrf52840`` boards.

The ``nrf_dm`` library is not prebuilt for nRF54L, so ``nx15`` is not supported.

Building and Running
********************

This sample uses sysbuild. From your nRF Connect SDK workspace, build it and
flash it to both boards:

.. code-block:: console

   west build -b nx40/nrf52840 <repo>/samples/dm -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. Power both boards and watch the
``Measurement result`` blocks on the console or over RTT.

References
**********

* nRF Connect SDK: nRF Distance Measurement sample and Distance Measurement
  library documentation, https://docs.nordicsemi.com/
