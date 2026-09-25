# AetherOS Rev:F

A Nintendo DSi-native experimental desktop, quantum-simulation, audio/DSP, science, AI, networking and external-gateway platform.

## Phase 5
- Real framed gateway protocol over TCP/Wi-Fi with CRC32 validation.
- Reference external gateway server in `tools/aether_gateway.py`.
- Automated gateway protocol hardware-in-loop foundation test.
- On-device protocol self-test and HIL health score.
- Expanded 128-point FFT DSP engine.
- Expanded lightweight AI energy classification.
- Aether Studio step sequencer/performance engine.
- External gateway architecture for 5G, satellite, Bluetooth, SDR, remote QPU and heavy AI.

### Hardware boundary
The original DSi does not contain native 5G, satellite, Bluetooth, SDR, or a physical QPU. Rev:F exposes those as external gateway interfaces; the DSi remains the controller/client and local quantum simulator. Actual external hardware is required for those links.

## Build
GitHub Actions builds `AETHEROS.nds` with devkitPro/devkitARM. The SD card workspace is documented in the source and releases.

## Safe RF policy
RF/Marauder features are receive-only/authorized telemetry and analysis. No jamming, credential capture, or unauthorized access tooling is included.

## Project
https://github.com/way4out/AetherOS-Rev-F


### Rev:F6
F6 usability/UI pass: dual-screen dashboard, clearer module navigation, touch actions, and action panels while preserving Phase 5 gateway, quantum, DSP, AI, lab, RF, studio, and diagnostics capabilities.


## AetherOS Revolution 7+ — Pass 1
- Persistent user Settings surface with theme, layout, density, animation, audio, clock, telemetry, quote, location, quantum, voice, and visual controls.
- MySpace-inspired personal-space home layout with direct touchscreen module access while retaining D-pad/button navigation.
- Theme engine with location-aware automatic selection and a Hobbit fallback when no location is configured.
- Live DSi RTC timestamp shown throughout the UI; location coordinates are user-configurable because original DSi hardware has no native GPS.
- Settings persist to `REVF/CORE/SETTINGS.CFG` when the SD workspace is writable.
