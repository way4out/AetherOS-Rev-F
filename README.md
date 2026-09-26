# AetherMod 5.0 Genesis

Nintendo DSi-native local-first cockpit and homebrew platform.

## Pass 2 / 8
- Runtime save-integrity, SD-write, selection-sync and recovery diagnostics.
- Telemetry exposes runtime health, last-save frame and error counters.
- V4 save compatibility is preserved.

## Pass 1 / 8
- Canonical home selection/PIN state for marker, number, label and launch target.
- Full 16-module touchscreen access through two 8-module home pages.
- 5.0 Genesis release identity and stale 4.5/Q2 UI metadata removed.
- Persisted selection remains compatible with the existing V4 save format.
- CI builds a DSi-enhanced NDS and packages the complete SD workspace.

## Modules
Quantum Core, YHWH Biblio Codex, Animal AI, Marauder/RF, TinySA Lab, Quantum Calculator,
Aether DAW/Studio, DSP/FFT, Telemetry, Projects, Network Gateway, AI Safety,
Family Safety, System and About.

## Hardware boundary
The original DSi does not contain native 5G, satellite, Bluetooth, SDR or a physical QPU.
Those are represented as external gateway interfaces. Local quantum functions are deterministic
software simulations.

## RF safety
RF/Marauder functions are receive/analyze/telemetry-oriented and require appropriate external
hardware for real RF capture. No jamming, deauthentication, credential theft or unauthorized
access tooling is included.

## Build
GitHub Actions uses devkitPro/devkitARM to build `AetherMod.nds` and a complete SD bundle.

Repository: https://github.com/way4out/AetherOS-Rev-F
