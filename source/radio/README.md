# Rev:F Hardware Gateway Contract
The DSi cannot natively provide 5G, satellite, Bluetooth, or SDR. Rev:F therefore exposes a hardware-abstraction contract for external gateways.

Required future gateway classes:
- USB/serial or network 5G modem/router
- satellite terminal/gateway
- Bluetooth serial/audio HID bridge
- SDR receiver such as a supported USB SDR/TinySA-class analyzer
- remote compute gateway for AI/QPU

The OS must never claim a gateway is physically present until discovery/handshake succeeds. Receive/analysis features only; no jamming or unauthorized interception.