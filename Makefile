.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment.")
endif
include $(DEVKITARM)/ds_rules

TARGET := AETHEROS
BUILD := build
SOURCE := source/main.cpp
ARCH := -march=armv5te -mtune=arm946e-s -mthumb
CFLAGS := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH) -DARM9
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
INCLUDES := -I$(CURDIR)/source -I$(CURDIR)/build -I$(LIBNDS)/include
LDFLAGS := -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(TARGET).map
LIBS := -lnds9
LIBPATHS := -L$(LIBNDS)/lib

.PHONY: all clean
all: $(TARGET).nds

$(BUILD):
	@mkdir -p $@

$(BUILD)/main.o: $(SOURCE) | $(BUILD)
	@echo compiling main.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(TARGET).elf: $(BUILD)/main.o
	@echo linking $(TARGET).elf
	@$(CXX) $(LIBPATHS) $(LDFLAGS) $^ $(LIBS) -o $@

$(TARGET).nds: $(TARGET).elf
	@echo packaging $(TARGET).nds

-include $(BUILD)/*.d
