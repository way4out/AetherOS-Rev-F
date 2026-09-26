# AetherMod 5.0 Genesis for Nintendo DSi — Pass 1 / 8

AetherMod is a local-first dual-screen DSi homebrew cockpit with modular quantum simulation,
codex/data, animal-analysis, RF telemetry, TinySA gateway, calculator, DAW, DSP, telemetry,
projects, network gateway, AI safety, family safety, system and release diagnostics.

## Pass 1 fixes
- Canonical selection state: one persisted PIN controls the marker, displayed module number,
  module name and launch target.
- Home touchscreen navigation now exposes all 16 modules across two 8-module pages.
- Visible release branding is 5.0 Genesis; stale Q2/4.5 labels are removed.
- Release metadata identifies the 8-pass development line.
- CI packaging now publishes both the bootable NDS and the complete SD bundle.

## Hardware boundary
The stock DSi provides dual displays, touch, microphone, cameras and SD storage. External RF
analyzers, SDRs, QPU hardware, satellite/5G/Bluetooth devices and other specialized hardware
remain gateway hardware; the DSi does not natively contain those capabilities.

## RF safety
Marauder/RF functionality is limited to receive/analyze/telemetry concepts and external
authorized gateways. No jamming, deauthentication, credential theft, or unauthorized-access
tooling is included.

## Installation
Copy the packaged `apps/AetherMod/AetherMod.nds` and `data/AetherMod/` directory to the
DSi SD card and launch the NDS with a compatible homebrew loader.
