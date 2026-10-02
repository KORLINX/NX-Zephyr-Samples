# Copyright (c) 2026 Korlinx
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_BOARD_NX40_UF2_1200BPS_TOUCH)
  zephyr_library()
  zephyr_library_sources(uf2_touch.c)
endif()
