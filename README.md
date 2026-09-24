# Kamstrup Omnipower M-Bus Reader with ESPHome External Component

This repository provides an ESPHome external component for reading electricity-meter data from Kamstrup Omnipower meters through an M-Bus/NVE-HAN interface.

## Installation

Add the following to your ESPHome YAML. The reader is a top-level component; it is not configured as `sensor: - platform: mbus_reader`.

```yaml
external_components:
  - source: github://KlasHolger/elmatare_mbus_temp@main
    components: [mbus_reader]

uart:
  id: uart_bus
  rx_pin: GPIO16
  baud_rate: 2400
  parity: NONE
  stop_bits: 1
  data_bits: 8

mbus_reader:
  uart_id: uart_bus
  wattage_sensor:
    name: "Power Usage"
  reactive_power_sensor:
    name: "Reactive Power"
  amperage_l1_sensor:
    name: "Current L1"
  amperage_l2_sensor:
    name: "Current L2"
  amperage_l3_sensor:
    name: "Current L3"
  voltage_l1_sensor:
    name: "Voltage L1"
  voltage_l2_sensor:
    name: "Voltage L2"
  voltage_l3_sensor:
    name: "Voltage L3"
  energy_sensor:
    name: "Total Energy"
  reactive_energy_sensor:
    name: "Reactive Energy"
```

## Supported OBIS codes

| OBIS code | Description | Unit |
|---|---|---|
| `1.1.1.7.0.255` | Active power | kW |
| `1.1.31.7.0.255` | Current L1 | A |
| `1.1.51.7.0.255` | Current L2 | A |
| `1.1.71.7.0.255` | Current L3 | A |
| `1.1.32.7.0.255` | Voltage L1 | V |
| `1.1.52.7.0.255` | Voltage L2 | V |
| `1.1.72.7.0.255` | Voltage L3 | V |
| `1.1.1.8.0.255` | Total energy | kWh |
| `1.1.4.7.0.255` | Reactive power | VAr |
| `1.1.4.8.0.255` | Reactive energy | kVArh |

## Hardware

- ESP32 with UART
- Kamstrup Omnipower meter with NVE-HAN port
- M-Bus Slave

## License

MIT
