.. zephyr:code-sample:: korlinx_qspi_flash
   :name: P25Q32SU external flash
   :relevant-api: flash_interface

   Erase, write and verify the on-board P25Q32SU NOR flash and measure
   throughput.

Overview
********

This sample is based on the Zephyr ``samples/drivers/spi_flash`` sample. It
exercises the whole 32 Mbit (4 MB) P25Q32SU with the :ref:`flash API
<flash_api>` in three phases:

#. Erase every sector.
#. Read every page back and check that it is ``0xFF``.
#. Write a test pattern page by page, then read it back and verify it.

The sample prints the number of errors and the throughput for each phase.

.. warning::

   This sample erases the entire external flash.

Requirements
************

One of the following boards:

* ``nx15/nrf54l15/cpuapp``: ``jedec,spi-nor`` on ``spi00`` (SCK P2.09,
  MOSI P2.10, MISO P2.04, CS P0.02)
* ``nx40/nrf52840``: ``nordic,qspi-nor`` on ``qspi`` (SCK P1.07,
  IO0-IO3 P1.10/P1.14/P1.13/P1.12, CSN P1.11)

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample as follows,
changing ``nx15/nrf54l15/cpuapp`` for your board:

.. code-block:: console

   west build -b nx15/nrf54l15/cpuapp <repo>/samples/qspi_flash -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository.

References
**********

* P25Q32SU datasheet: Puya Semiconductor, https://www.puyasemi.com/
