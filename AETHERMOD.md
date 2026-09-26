# AetherMod 7.0 Apex — Nintendo DSi / DS Release

## Pass 2 / 5 — Corpus + Interactive Data Foundation

Pass 1 established the recovery foundation and three-second START-button BASE RESET. Pass 2 expands the offline data layer used by the Codex and Animal AI systems while preserving DSi-safe, local-first behavior.

### YHWH Biblio Codex
- Added `data/AetherMod/codex_manifest.txt` with the complete indexed book list, YHWH/יהוה/LORD/ADONAI name-layer metadata, and a structured-corpus contract.
- Offline corpus remains SD-first.
- Recommended structured format: `BOOK|CHAPTER|VERSE|TEXT`.
- Full text can be supplied through public-domain or properly licensed UTF-8 corpus files.
- The application does not represent a particular translation as divinely authored.

### Animal AI
- Added `data/AetherMod/animal_profiles.txt`.
- Expanded the profile vocabulary to pitch, energy, rhythm, spectrum, onsets, duration and spectral centroid.
- Added per-species feature emphasis for the onboard 15-profile set.
- Classification remains probabilistic/educational rather than literal animal-language translation.
- Optional external AI remains explicitly labeled as an external gateway.

### Release architecture
- DSi/DS local-first operation remains the baseline.
- External RF, TinySA, 5G, satellite and QPU capabilities require compatible external hardware/gateways; the software does not fabricate stock hardware capabilities.
- Existing three-second START recovery path remains the system recovery mechanism.

### Remaining passes
1. Codex + Animal deeper runtime integration.
2. Marauder/RF + TinySA expansion.
3. Calculator + DAW + DSP/FFT expansion.
4. Telemetry + AI Home + Network Gateway expansion.
5. AI Safety + Family Safety + System/Service + General Settings + Quantum Core + final UI hardening.
