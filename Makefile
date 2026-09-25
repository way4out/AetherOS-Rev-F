.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment.")
endif
include $(DEVKITARM)/ds_rules

TARGET := AETHEROS
BUILD := build
SOURCES := source
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

CORE_SRC := $(SOURCES)/main.cpp $(SOURCES)/core/aether_core.cpp $(SOURCES)/hardware/hardware_profile.cpp $(SOURCES)/benchmark/benchmark.cpp $(SOURCES)/ui/aether_ui.cpp $(SOURCES)/quantum/quantum_core.cpp

$(BUILD)/main.o: $(SOURCES)/main.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/core_aether_core.o: $(SOURCES)/core/aether_core.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/hardware_profile.o: $(SOURCES)/hardware/hardware_profile.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/benchmark.o: $(SOURCES)/benchmark/benchmark.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/aether_ui.o: $(SOURCES)/ui/aether_ui.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD)/quantum_core.o: $(SOURCES)/quantum/quantum_core.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

OBJECTS := $(BUILD)/main.o $(BUILD)/core_aether_core.o $(BUILD)/hardware_profile.o $(BUILD)/benchmark.o $(BUILD)/aether_ui.o $(BUILD)/quantum_core.o

$(TARGET).elf: $(OBJECTS)
	@$(CXX) $(LIBPATHS) $(LDFLAGS) $^ $(LIBS) -o $@

$(TARGET).nds: $(TARGET).elf
	@echo "Packaging $@"

-include $(BUILD)/*.d

clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).map
