.. zephyr:code-sample:: korlinx_micro_msm261d35
   :name: MSM261D3526 PDM microphone
   :relevant-api: audio_dmic_interface

   Capture PCM data from the on-board PDM microphone with the DMIC API.

Overview
********

This sample is the Zephyr ``samples/drivers/audio/dmic`` sample adapted to the
Korlinx NX40. It runs two PDM transfers through the :ref:`Audio DMIC API
<audio_dmic_api>`, one with one channel and one with two channels, and logs
the size of each received buffer. It does not process the audio data.

The microphone is selected with the ``dmic_dev`` devicetree node label.

Requirements
************

* ``nx40/nrf52840``: MSM261D3526 on ``pdm0`` (CLK P1.02, DIN P1.03).

The nx15 has no PDM microphone, so it is not supported.

Building and Running
********************

From your nRF Connect SDK workspace, build and flash the sample:

.. code-block:: console

   west build -b nx40/nrf52840 <repo>/samples/micro_msm261d35 -- -DZEPHYR_EXTRA_MODULES=<repo>
   west flash

``<repo>`` is the path to this repository. The log starts with ``DMIC sample``
and ends with ``Exiting`` once both transfers are done.
