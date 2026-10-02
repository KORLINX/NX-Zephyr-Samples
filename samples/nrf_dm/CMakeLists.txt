#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

# Make the repo-root custom boards (nx15, nx40) discoverable.
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nrf_dm)

target_sources(app PRIVATE
  src/main.c
  src/pwm_led.c
  src/peer.c
  src/service.c
  )

zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
