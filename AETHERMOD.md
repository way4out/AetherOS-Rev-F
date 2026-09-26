# AetherMod 7.0 Apex — Nintendo DSi / DS Release

## Pass 3 / 5 — RF + TinySA Operator Layer

Pass 3 expands the Marauder/RF and TinySA interfaces while preserving the local-first, bounded-storage design.

### Marauder / RF
- Four operator modes: Survey, Channel View, Packet Metadata, RSSI History.
- Band/channel navigation and signal telemetry display.
- Warning index for signal/noise/authorization conditions.
- Authorized push-test queue with an 8-item runtime cap.
- Bounded session event log with a 32-event runtime counter.
- Explicit safety gate: no jamming, deauthentication, credential capture, or unauthorized access.
- Stock DSi RF limitations remain explicit; physical RF requires compatible external hardware.

### TinySA
- Hybrid visual + text interface.
- Spectrum, Waterfall, Text Stats, and Marker views.
- Span, RBW, attenuation, marker, sweep, trace-hold and input-source controls.
- SIM mode works without hardware; EXTERNAL mode is a gateway state, not a claim of native DSi USB/serial TinySA support.

### 4 GB SD strategy
- The 4 GB card is treated as the available storage budget.
- The NDS application remains compact.
- Runtime logs are bounded rather than allowed to grow indefinitely.
- Large Codex/AI corpora should remain optional SD data and must fit within the user's remaining card space.
- No module is designed to reserve the entire card or silently consume it.

### Remaining passes
1. Deeper Calculator/DAW/DSP interaction.
2. Telemetry + AI Home + Network Gateway expansion.
3. AI Safety + Family Safety + System/Service + General Settings + Quantum Core + final touch/navigation hardening.
