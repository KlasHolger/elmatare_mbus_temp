# ESPHome M-Bus Reader Component - Kamstrup Omnipower

ESPHome external component for reading electricity meter data from Kamstrup Omnipower meters via M-Bus/NVE-HAN interface.

## Features

- **10 OBIS Codes**: Active Power, Current (3-phase), Voltage (3-phase), Reactive Power, Energy
- **Home Assistant Integration**: Full device class support
- **External Component**: Modern ESPHome architecture
- **Real-time Monitoring**: Updates every 10 seconds

## Installation

Add to your ESPHome YAML:

```yaml
external_components:
  - source: github://KlasHolger/elmatare_mbus_temp@main
    components: [mbus_reader]

uart:
  id: uart_bus
  rx_pin: GPIO16
  baud_rate: 2400

sensor:
  - platform: mbus_reader
    uart_id: uart_bus
    wattage_sensor:
      name: "Power Usage"
    # ... more sensors
```

## Supported OBIS Codes

| OBIS Code | Description | Unit |
|-----------|-------------|------|
| 1.1.1.7.0.255 | Active Power | kW |
| 1.1.31.7.0.255 | Current L1 | A |
| 1.1.51.7.0.255 | Current L2 | A |
| 1.1.71.7.0.255 | Current L3 | A |
| 1.1.32.7.0.255 | Voltage L1 | V |
| 1.1.52.7.0.255 | Voltage L2 | V |
| 1.1.72.7.0.255 | Voltage L3 | V |
| 1.1.1.8.0.255 | Total Energy | kWh |
| 1.1.4.7.0.255 | Reactive Power | VAr |
| 1.1.4.8.0.255 | Reactive Energy | kVArh |

## Hardware

- ESP32 with UART
- Kamstrup Omnipower meter with NVE-HAN port
- Voltage divider (5V to 3.3V)

## License

MIT
