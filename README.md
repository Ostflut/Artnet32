# ESP32 ArtNet DMX Node

A two-universe ArtNet to DMX512 converter running on an ESP32.
It creates a WiFi hotspot, receives ArtNet UDP packets (e.g. from QLC+),
and outputs DMX512 signals on two independent physical universes via MAX485.

---

## Features

- 2 independent DMX512 output universes
- WiFi Access Point mode (no router needed)
- Compatible with QLC+, Photon, and any ArtNet software
- Cheap DIY build (~5–10€ in parts)

---

## Required Software Versions

| Component | Name | Version |
|---|---|---|
| Arduino Board Core | esp32 by Espressif Systems | **2.0.14** |
| Library | esp_dmx by Mitch Weisbrod | **3.1.0** |
| Library | ArtnetWifi by rstephan | latest |

> ⚠️ These versions are specifically tested together.
> Other combinations may cause compile errors or runtime crashes.

### Installing the Board Core
`Arduino IDE → Tools → Boards Manager → search "esp32" → install version 2.0.14`

### Installing Libraries
`Arduino IDE → Tools → Manage Libraries → search and install each library`

---

## Hardware

| Component | Quantity |
|---|---|
| ESP32 DevKit v1 (ESP32-WROOM-32) | 1 |
| MAX485 module | 2 |
| XLR Female connector (3-pin) | 2 |
| 120Ω resistor | 2 |
| Jumper wires | several |

> 💡 For cleaner 3.3V compatibility, use **MAX3485** or **SN65HVD485** instead of MAX485.

---

## Wiring

### Power (both MAX485 modules)

| ESP32 | MAX485 |
|---|---|
| VIN (5V) | VCC |
| GND | GND |

### Universe 1 (MAX485 #1)

| ESP32 | MAX485 #1 |
|---|---|
| GPIO 17 | DI |
| GPIO 27 | DE + RE (bridge DE and RE on the module, one wire to GPIO 27) |
| — | RO (leave unconnected) |

| MAX485 #1 | XLR Female #1 |
|---|---|
| A | Pin 3 (Data +) |
| B | Pin 2 (Data –) |
| GND | Pin 1 (Shield/Ground) |

Solder a **120Ω resistor** between XLR Pin 2 and Pin 3, directly at the connector.

### Universe 2 (MAX485 #2)

| ESP32 | MAX485 #2 |
|---|---|
| GPIO 13 | DI |
| GPIO 26 | DE + RE (bridge DE and RE on the module, one wire to GPIO 26) |
| — | RO (leave unconnected) |

| MAX485 #2 | XLR Female #2 |
|---|---|
| A | Pin 3 (Data +) |
| B | Pin 2 (Data –) |
| GND | Pin 1 (Shield/Ground) |

Solder a **120Ω resistor** between XLR Pin 2 and Pin 3, directly at the connector.

### Unused Pins

| ESP32 Pin | Purpose |
|---|---|
| GPIO 35 | Dummy RX universe 1 — leave unconnected |
| GPIO 34 | Dummy RX universe 2 — leave unconnected |

---

## WiFi Configuration

After flashing, the ESP32 creates a WiFi hotspot:

| Setting | Value |
|---|---|
| SSID | `ESP32-DMX-Node` |
| Password | `dmxdmxdmx` |
| IP Address | `192.168.4.1` |

Connect your computer or tablet to this network before opening QLC+.

---

## QLC+ Setup

1. Connect to the `ESP32-DMX-Node` WiFi network
2. Open QLC+ → **Inputs/Outputs**
3. Select **Universe 1** → Output: **Art-Net**
4. Click **Configure**:
   - Interface: your PC IP (e.g. `192.168.4.x`)
   - IP Address: `192.168.4.255` (broadcast)
   - ArtNet Universe: `0`
   - Transmission Mode: **Full**
5. Enable the output (✓ checkbox)
6. Repeat for Universe 2 with ArtNet Universe `1`
7. Switch to **Operate mode (▶)** before using Simple Desk

> ⚠️ QLC+ only sends data in Operate mode. Transmission Mode must be set to **Full**,
> otherwise only changed values are sent.

---

## Partition Scheme

In Arduino IDE under `Tools → Partition Scheme`:
**No OTA (2MB APP / 2MB SPIFFS)**

---

## License

MIT
