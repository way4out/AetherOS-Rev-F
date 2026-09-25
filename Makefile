.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment.")
endif
include $(DEVKITARM)/ds_rules

TARGET := AETHEROS
BUILD := build
ARCH := -march=armv5te -mtune=arm946e-s -mthumb
CFLAGS := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH) -DARM9
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
INCLUDES := -I$(CURDIR)/source -I$(CURDIR)/include -I$(CURDIR)/build -I$(LIBNDS)/include
LDFLAGS := -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(TARGET).map
LIBS := -lfat -lnds9
LIBPATHS := -L$(LIBNDS)/lib

OBJECTS := $(BUILD)/main.o $(BUILD)/aether_core.o $(BUILD)/hardware_profile.o $(BUILD)/benchmark.o $(BUILD)/aether_ui.o $(BUILD)/quantum_core.o $(BUILD)/engine_modules.o $(BUILD)/aether_audio.o $(BUILD)/network_fabric.o $(BUILD)/radio_gateway.o $(BUILD)/remote_compute.o

.PHONY: all clean
all: $(TARGET).nds

$(BUILD):
	@mkdir -p $@

$(BUILD)/main.o: source/main.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/aether_core.o: source/core/aether_core.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/hardware_profile.o: source/hardware/hardware_profile.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/benchmark.o: source/benchmark/benchmark.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/aether_ui.o: source/ui/aether_ui.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/quantum_core.o: source/quantum/quantum_core.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/engine_modules.o: source/engine/engine_modules.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/aether_audio.o: source/audio/aether_audio.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/network_fabric.o: source/network/network_fabric.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/radio_gateway.o: source/radio/radio_gateway.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/remote_compute.o: source/compute/remote_compute.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(TARGET).elf: $(OBJECTS)
	@$(CXX) $(LIBPATHS) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

$(TARGET).nds: $(TARGET).elf
	@ndstool -c $@ -9 $(TARGET).elf

-include $(BUILD)/*.d

clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).map
