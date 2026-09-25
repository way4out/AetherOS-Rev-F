TARGET := AETHEROS
BUILD := build
SOURCES := source
DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM
CXX := $(DEVKITARM)/bin/arm-none-eabi-g++
CXXFLAGS := -g -Wall -O2 -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -std=gnu++17 -I$(DEVKITPRO)/libnds/include
LDFLAGS := -mthumb -mthumb-interwork -specs=ds_arm9.specs -Wl,--gc-sections -L$(DEVKITPRO)/libnds/lib
LIBS := -lnds9
CXXFILES := $(shell find $(SOURCES) -name '*.cpp')
OFILES := $(patsubst %.cpp,$(BUILD)/%.o,$(CXXFILES))
.PHONY: all clean
all: $(TARGET).nds
$(TARGET).elf: $(OFILES)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $(OFILES) $(LIBS) -o $@
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $<
$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
-include $(OFILES:.o=.d)
clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds
