# Copyright (C) 2020 SiFive Inc
# SPDX-License-Identifier: Apache-2.0

RISCV_ARCH = rv32imac
RISCV_ABI = ilp32
RISCV_CMODEL = medlow
RISCV_SERIES = sifive-3-series

TARGET_TAGS = board openocd
TARGET_DHRY_ITERS = 20000000
TARGET_CORE_ITERS = 5000
TARGET_FREERTOS_WAIT_MS = 1000