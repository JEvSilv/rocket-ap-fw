###################################################
# Build Flags for the Release Configuration
###################################################

# Set the optimization level
# For CPU+AP scenario
RISCV_ASFLAGS += -Os
RISCV_CFLAGS += -Os
RISCV_CXXFLAGS += -Os

# For CPU scenario
#RISCV_ASFLAGS += -O2
#RISCV_CFLAGS += -O2
#RISCV_CXXFLAGS += -O2

# Enable debug
RISCV_ASFLAGS += -g
RISCV_CFLAGS += -g
RISCV_CXXFLAGS += -g
