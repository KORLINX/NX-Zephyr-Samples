# NX Zephyr Samples

This repository contains board definitions and sample applications for the
Korlinx NX Bluetooth LE modules, for use with the [nRF Connect SDK][ncs]. It is
a [Zephyr module][modules] that provides:

- [Custom boards][board_porting] for the Korlinx NX modules:
  - [`nx15/nrf54l15/cpuapp`](boards/korlinx/nx15/doc/index.rst): NX15, nRF54L15
  - [`nx40/nrf52840`](boards/korlinx/nx40/doc/index.rst): NX40, nRF52840
- Sample applications for the on-board peripherals and Bluetooth LE:
  - [`hello`](samples/hello/README.rst): console and SEGGER RTT output
  - [`uart`](samples/uart/README.rst): console UART heartbeat and echo
  - [`shtc3`](samples/shtc3/README.rst): SHTC3 temperature and humidity sensor
  - [`imu_lsm6d`](samples/imu_lsm6d/README.rst): LSM6DSL accelerometer and gyroscope
  - [`micro_msm261d35`](samples/micro_msm261d35/README.rst): PDM microphone (NX40 only)
  - [`qspi_flash`](samples/qspi_flash/README.rst): external NOR flash test
  - [`gatt`](samples/gatt/README.rst): custom GATT service with notifications
  - [`rssi_central`](samples/rssi_central/README.rst) and
    [`rssi_peripheral`](samples/rssi_peripheral/README.rst): connection RSSI
    and LE Power Control
  - [`dm`](samples/dm/README.rst): distance measurement (NX40 only)
  - [`dtm`](samples/dtm/README.rst): Bluetooth LE Direct Test Mode

The samples are tested with nRF Connect SDK v3.3.1.

[ncs]: https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/index.html
[modules]: https://docs.zephyrproject.org/latest/develop/modules.html
[board_porting]: https://docs.zephyrproject.org/latest/hardware/porting/board_porting.html

## Getting started

Before getting started, make sure you have a working nRF Connect SDK
development environment. Follow the official
[installation guide](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/installation/install_ncs.html).

### Initialization

Clone this repository next to your nRF Connect SDK workspace, or anywhere else:

```shell
git clone https://github.com/KORLINX/NX-Zephyr-Samples
```

The commands below refer to the path of the clone as `<repo>`.

### Building and running

Run `west build` from your nRF Connect SDK workspace. Pass the repository as an
extra Zephyr module so the build system, including sysbuild, can find the
Korlinx boards:

```shell
west build -b $BOARD <repo>/samples/hello -- -DZEPHYR_EXTRA_MODULES=<repo>
```

where `$BOARD` is `nx15/nrf54l15/cpuapp` or `nx40/nrf52840`.

Once you have built the application, run the following command to flash it:

```shell
west flash
```

See each sample's `README.rst` for the supported boards and expected behavior.

### Testing

To build every sample for both boards with Twister, run:

```shell
west twister -T <repo>/samples --build-only \
    -p nx15/nrf54l15/cpuapp -p nx40/nrf52840 \
    --board-root <repo>/boards -x=ZEPHYR_EXTRA_MODULES=<repo> \
    --disable-warnings-as-errors
```

`--disable-warnings-as-errors` is needed because the NX40 samples use the
legacy USB device stack, which is deprecated and triggers compiler warnings.

