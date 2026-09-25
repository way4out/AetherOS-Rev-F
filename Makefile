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
LIBS := -ldswifi9 -lfat -lnds9
LIBPATHS := -L$(LIBNDS)/lib

OBJECTS := $(BUILD)/main.o $(BUILD)/aether_core.o $(BUILD)/hardware_profile.o $(BUILD)/benchmark.o $(BUILD)/aether_ui.o $(BUILD)/quantum_core.o $(BUILD)/engine_modules.o $(BUILD)/aether_audio.o $(BUILD)/aether_dsp.o $(BUILD)/aether_lab.o $(BUILD)/aether_ai.o $(BUILD)/network_fabric.o $(BUILD)/radio_gateway.o $(BUILD)/remote_compute.o $(BUILD)/gateway_protocol.o $(BUILD)/gateway_session.o $(BUILD)/gateway_security.o $(BUILD)/wifi_transport.o $(BUILD)/gateway_client.o $(BUILD)/gateway_manager.o $(BUILD)/system_graph.o $(BUILD)/recovery.o $(BUILD)/governor.o $(BUILD)/diagnostics.o

.PHONY: all clean
all: $(TARGET).nds
$(BUILD):; @mkdir -p $@
$(BUILD)/%.o: source/%.cpp | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
$(TARGET).elf: $(OBJECTS)
	@$(CXX) $(LIBPATHS) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
$(TARGET).nds: $(TARGET).elf
	@ndstool -c $@ -9 $(TARGET).elf
-include $(BUILD)/*.d
clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).map
