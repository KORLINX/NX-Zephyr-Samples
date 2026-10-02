#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

# Make the repo-root custom boards (nx15, nx40) discoverable.
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(dtm)

target_include_directories(app PRIVATE ./src)

target_sources(app PRIVATE
  src/transport/dtm_uart_twowire.c
  src/dtm.c
  src/dtm_hw.c
  src/main.c
)
