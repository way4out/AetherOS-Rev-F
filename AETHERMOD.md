# AetherMod 5.0 Genesis for Nintendo DSi — Pass 4 / 8

AetherMod is a local-first dual-screen DSi homebrew cockpit with modular quantum simulation,
codex/data, animal-analysis, RF telemetry, TinySA gateway, calculator, DAW, DSP, telemetry,
projects, network gateway, AI safety, family safety, system and release diagnostics.

## Pass 4 / 8
- Runtime diagnostics now validate save integrity, SD write access, selection synchronization and recovery state.
- Telemetry exposes save/storage/selection diagnostics and last-save frame.
- Home diagnostics surface recovery state without changing the V4 save format.
- Input events and runtime error counters are tracked locally.

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


### Pass 3 hardening
- Canonical home PIN remains the single source of selection truth.
- Home touch navigation now exposes both eight-module pages.
- Quantum and Telemetry hardware-key shortcuts target their correct modules.
- Runtime diagnostics reset per diagnostic run and expose the latest diagnostic frame.
- DSP telemetry exposes the active scale; System branding is 5.0 Genesis.


## Pass 4 / 8
- Persistence dirty-state tracking and periodic autosave were added.
- Navigation now uses a centralized home-return path that restores the canonical selection.
- Home exposes dirty state and last module context.
- Telemetry exposes boot count and persistence state.


## Pass 8 / 8

- Added a capability-health layer for runtime status reporting.
- Added explicit guarded/armed gateway state for external hardware paths.
- Safe Mode now clears gateway state alongside wireless/external controls.
- RF and TinySA actions explicitly transition the gateway state when armed.
- Health polling avoids repeated FAT initialization.


## Pass 8 / 8
- Added centralized DSi input-event servicing after key scanning.
- Added input-event, key-repeat, burst, and frame-budget diagnostics.
- Updated About and release identity to Pass 6.
- Preserved existing V4 save compatibility and guarded gateway behavior.


## Pass 8 / 8
- Added runtime state validation and deterministic recovery guards.
- Added module-boundary guard checks.
- Added recovery, validation, and module-guard telemetry counters.
- Added forward declaration coverage for recovery dependencies.


## Pass 8 / 8
- Final release source identity set to Pass 8 / 8.
- Release configuration and workflow artifact identity finalized.
- This pass targets the bootable `.nds` release artifact plus SD bundle from CI.


## AetherMod 6.0 Expansion — Pass 1 / 3
- Replaced public Projects surface with AI Home / Aether Bot.
- Added General Settings surface with working persistent controls.
- Expanded YHWH Biblio Codex navigation/name-layer architecture.
- Expanded Animal AI into a DSi-native signal-analysis game workflow.
- Expanded Marauder/RF into authorized survey/analysis with bounded push queue.
- Added TinySA visual spectrum/marker/current-stat presentation.
- Expanded Calculator, Quantum, DAW, DSP/FFT, Telemetry, Network/Browser gateway and System surfaces.
- Stock DSi limitations remain explicitly represented as external gateways where required.
