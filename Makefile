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

OBJECTS := $(BUILD)/main.o $(BUILD)/aether_core.o $(BUILD)/hardware_profile.o $(BUILD)/benchmark.o $(BUILD)/aether_ui.o $(BUILD)/quantum_core.o $(BUILD)/engine_modules.o $(BUILD)/aether_audio.o $(BUILD)/aether_dsp.o $(BUILD)/aether_lab.o $(BUILD)/aether_ai.o $(BUILD)/network_fabric.o $(BUILD)/radio_gateway.o $(BUILD)/remote_compute.o $(BUILD)/gateway_protocol.o $(BUILD)/gateway_session.o $(BUILD)/gateway_security.o $(BUILD)/wifi_transport.o $(BUILD)/gateway_client.o $(BUILD)/gateway_manager.o $(BUILD)/system_graph.o $(BUILD)/recovery.o $(BUILD)/governor.o $(BUILD)/diagnostics.o $(BUILD)/hil.o $(BUILD)/aether_studio.o $(BUILD)/aether_settings.o $(BUILD)/aether_theme.o

.PHONY: all clean
all: $(TARGET).nds
$(BUILD):
	@mkdir -p $@
define COMPILE
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
endef
$(BUILD)/main.o: source/main.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_core.o: source/core/aether_core.cpp | $(BUILD); $(COMPILE)
$(BUILD)/hardware_profile.o: source/hardware/hardware_profile.cpp | $(BUILD); $(COMPILE)
$(BUILD)/benchmark.o: source/benchmark/benchmark.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_ui.o: source/ui/aether_ui.cpp | $(BUILD); $(COMPILE)
$(BUILD)/quantum_core.o: source/quantum/quantum_core.cpp | $(BUILD); $(COMPILE)
$(BUILD)/engine_modules.o: source/engine/engine_modules.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_audio.o: source/audio/aether_audio.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_dsp.o: source/dsp/aether_dsp.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_lab.o: source/lab/aether_lab.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_ai.o: source/ai/aether_ai.cpp | $(BUILD); $(COMPILE)
$(BUILD)/network_fabric.o: source/network/network_fabric.cpp | $(BUILD); $(COMPILE)
$(BUILD)/radio_gateway.o: source/radio/radio_gateway.cpp | $(BUILD); $(COMPILE)
$(BUILD)/remote_compute.o: source/compute/remote_compute.cpp | $(BUILD); $(COMPILE)
$(BUILD)/gateway_protocol.o: source/network/gateway_protocol.cpp | $(BUILD); $(COMPILE)
$(BUILD)/gateway_session.o: source/network/gateway_session.cpp | $(BUILD); $(COMPILE)
$(BUILD)/gateway_security.o: source/network/gateway_security.cpp | $(BUILD); $(COMPILE)
$(BUILD)/wifi_transport.o: source/network/wifi_transport.cpp | $(BUILD); $(COMPILE)
$(BUILD)/gateway_client.o: source/network/gateway_client.cpp | $(BUILD); $(COMPILE)
$(BUILD)/gateway_manager.o: source/network/gateway_manager.cpp | $(BUILD); $(COMPILE)
$(BUILD)/system_graph.o: source/core/system_graph.cpp | $(BUILD); $(COMPILE)
$(BUILD)/recovery.o: source/core/recovery.cpp | $(BUILD); $(COMPILE)
$(BUILD)/governor.o: source/core/governor.cpp | $(BUILD); $(COMPILE)
$(BUILD)/diagnostics.o: source/core/diagnostics.cpp | $(BUILD); $(COMPILE)
$(BUILD)/hil.o: source/core/hil.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_studio.o: source/studio/aether_studio.cpp | $(BUILD); $(COMPILE)
$(TARGET).elf: $(OBJECTS)
	@$(CXX) $(LIBPATHS) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
$(TARGET).nds: $(TARGET).elf
	@ndstool -c $@ -9 $(TARGET).elf
-include $(BUILD)/*.d
clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).map

$(BUILD)/aether_settings.o: source/settings/aether_settings.cpp | $(BUILD); $(COMPILE)
$(BUILD)/aether_theme.o: source/theme/aether_theme.cpp | $(BUILD); $(COMPILE)
