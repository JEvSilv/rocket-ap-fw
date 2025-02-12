###################################################
# Build Flags for the Release Configuration
###################################################

# Set the optimization level
RISCV_ASFLAGS += -Os
RISCV_CFLAGS += -Os
RISCV_CXXFLAGS += -Os


# Enable debug
RISCV_ASFLAGS += -g
RISCV_CFLAGS += -g
RISCV_CXXFLAGS += -g