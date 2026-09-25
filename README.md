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


## AetherOS Revolution 7+ — Pass 2
- Added **Aether Marauder / Security Lab** as a safety-gated RF/network analysis workspace.
- Passive RF mode supports authorized signal telemetry concepts such as samples, devices, RSSI/channel/waterfall-oriented analysis through an external SDR gateway.
- Authorized Network mode is limited to metadata/diagnostic workflows for systems the operator owns or is authorized to test.
- Lab Simulation mode provides non-transmitting, non-credential, attack-concept simulations for learning and defensive testing.
- Gateway Hardening mode focuses on protocol framing, CRC, authentication state, diagnostics, and resilience.
- Transmit/jamming, credential capture, covert interception, unauthorized access, and destructive operations are deliberately locked out of the Rev7 on-device security interface. External radio hardware is still required for RF functions.
- The interface displays the authorization/safety state before security-lab operation.


## AetherOS Revolution 7+ — Pass 3
- Integrated Mission Control health surface spanning boot, SD/config, project storage, local compute modules, network, gateway, recovery, and the security boundary.
- Added persistent runtime heartbeat/health scoring without pretending unavailable external hardware is online.
- Completed Marauder controls: authorization acknowledgement, passive-RF mode selection, and non-transmitting lab-simulation mode selection from the DSi controls.
- Completed action routing for DSP, RF, network, AI, and project workflows while preserving existing quantum, studio, settings, diagnostics, HIL, gateway, and recovery flows.
- System health now exposes a single readiness score and subsystem matrix.
- Pass 3 does not add unsafe jamming, credential theft, covert interception, unauthorized access, or destructive functions.


## Rev8
Visual/theme cleanup removes the Hobbit fallback entirely. Rev8 keeps configurable Aether, Night, Ocean, Quantum, and Forest themes with the dual-screen control surface, telemetry, module panels, settings, gateway diagnostics, and safety-locked security lab integrated into the DSi UI.
