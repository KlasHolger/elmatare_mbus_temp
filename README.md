# ESPHome M-Bus Reader Component - Kamstrup Omnipower

An ESPHome external component for reading electricity meter data from Kamstrup Omnipower meters via M-Bus/NVE-HAN interface.

## Features

- **10 OBIS Code Support**: Reads all essential 3-phase power consumption metrics
- **Real-time Monitoring**: Active Power, Current, Voltage, Reactive Power, Energy
- **Home Assistant Integration**: Native device classes and state classes
- **External Component**: Modern ESPHome architecture (no custom components required)
- **Proper Filtering**: Built-in conversions for all sensor units

## Supported OBIS Codes

| OBIS Code     | Description                  | Unit   | Range        |
|---------------|------------------------------|--------|--------------|
| 1.1.1.7.0.255 | Active Power (Output)        | kW     | 0-100        |
| 1.1.31.7.0.255| Current Phase L1             | A      | 0-63.75      |
| 1.1.51.7.0.255| Current Phase L2             | A      | 0-63.75      |
| 1.1.71.7.0.255| Current Phase L3             | A      | 0-63.75      |
| 1.1.32.7.0.255| Voltage Phase L1             | V      | 0-65535      |
| 1.1.52.7.0.255| Voltage Phase L2             | V      | 0-65535      |
| 1.1.72.7.0.255| Voltage Phase L3             | V      | 0-65535      |
| 1.1.1.8.0.255 | Active Energy Total          | kWh    | Incremental  |
| 1.1.4.7.0.255 | Reactive Power               | VAr    | 0-100000     |
| 1.1.4.8.0.255 | Reactive Energy              | kVArh  | Incremental  |

## Installation

### 1. Extract Component Files

```
your-esphome-config/
├── components/
│   └── mbus_reader/
│       ├── __init__.py
│       ├── sensor.py
│       ├── mbus_reader.h
│       ├── mbus_reader.cpp
│       └── manifest.yaml
└── your-config.yaml
```

### 2. Hardware Setup

**ESP32 Pin Configuration (elmatare_mbus_temp.yaml)**:
- GPIO 16: M-Bus UART RX (from Kamstrup NVE-HAN interface)
- GPIO 18: One-Wire Dallas Temperature Sensor (optional)

**Kamstrup Connection**:
- Pin 2: GND
- Pin 3: +5V
- Pin 4: Data Out (connect to GPIO 16 with voltage divider or RS485 converter)

### 3. Configuration

Replace secrets in your YAML:
```yaml
api_encryption_key: "your-api-encryption-key"
ota_password: "your-ota-password"
wifi_ssid: "your-ssid"
wifi_password: "your-wifi-password"
fallback_password: "fallback-ap-password"
```

### 4. Flash to Device

```bash
esphome run elmatare_mbus_temp.yaml
```

## YAML Configuration Example

```yaml
external_components:
  - source: github://KlasHolgerFile/esphome-mbus-reader
    components: [mbus_reader]
    refresh: 0h

uart:
  id: uart_bus
  rx_pin: GPIO16
  baud_rate: 2400

sensor:
  - platform: mbus_reader
    uart_id: uart_bus
    wattage_sensor:
      name: "Power Usage"
    amperage_l1_sensor:
      name: "Current L1"
    # ... more sensors
```

## Data Type Parsing

The component automatically handles two M-Bus data types:

- **Type 6**: 4-byte unsigned integer (used for power, energy)
- **Type 18**: 2-byte unsigned integer (used for voltage, current)

Raw values are automatically converted using configured multipliers:
- Wattage: × 0.001 (W → kW)
- Amperage: × 0.01 (cA → A)
- Voltage: × 1.0 (V)
- Energy: × 0.001 (Wh → kWh)
- Reactive Energy: × 0.01 (cVArh → kVArh)

## Debug Logging

Enable debug logging for troubleshooting:

```yaml
logger:
  level: DEBUG
  components:
    mbus_reader: DEBUG
```

This will show:
- OBIS codes detected
- Raw values received
- Parsed sensor values
- Unknown OBIS codes

## Troubleshooting

**No data received**:
- Check UART connection (GPIO 16 must be RX pin)
- Verify baud rate (2400 bps for Kamstrup)
- Check voltage levels (3.3V ESP32 vs 5V interface)

**Incorrect values**:
- Verify OBIS code mapping
- Check multiplier factors
- Review raw debug logs

**Connection drops**:
- Verify power supply stability
- Check cable shielding
- Reduce baud rate if needed

## License

MIT License - See LICENSE file

## Credits

- Original code: Klas Jansson (2022-2026)
- Based on Home Assistant M-Bus integration
- Refactored for ESPHome external component architecture