# IoT-Based Battery Monitoring System with Digital Twin

An ESP8266-based demonstration that measures battery voltage, current and temperature, publishes telemetry over MQTT, and documents a simple physics-based digital-twin comparison.

## Project status and scope

- **Firmware:** reads an INA219 current/voltage sensor and DS18B20 temperature probe; publishes JSON telemetry to MQTT.
- **Node-RED dashboard:** the supplied flow is a **simulated dashboard**. It generates sample readings and is not wired to the firmware's MQTT topic yet.
- **Digital twin:** this repository includes a lightweight Python MQTT subscriber that estimates resistive-load current and power using the configured load resistance. It is an educational baseline, not an electrochemical battery model.
- **Results:** no measured experimental results are claimed. Add your own readings, photos and test observations before submission.

## Hardware

- Witty Cloud ESP8266 board (confirm exact carrier and pin labels)
- INA219 voltage/current sensor
- DS18B20 temperature probe with 4.7 kΩ data pull-up
- 12 V, 1.2–1.3 Ah SLA battery
- 50 Ω, 10 W power resistor as a supervised demonstration load
- Inline fuse, jumper wires and optional LEDs/buzzer

### Pin mapping used by firmware

| Function | ESP8266 pin | GPIO |
|---|---:|---:|
| INA219 SDA | D2 | GPIO4 |
| INA219 SCL | D1 | GPIO5 |
| DS18B20 data | D5 | GPIO14 |
| Red LED | D6 | GPIO12 |
| Green LED | D7 | GPIO13 |
| Optional buzzer | D0 | GPIO16 |

Check the pinout of your exact Witty Cloud carrier before wiring.

## Electrical safety

- Put the fuse close to the battery positive terminal.
- The battery/load current path must use appropriately rated wire and connections, not a solderless breadboard.
- At 12 V, a 50 Ω load draws about 0.24 A and dissipates about 2.9 W. Keep the resistor on a non-flammable surface and supervise it.
- Never connect the 12 V battery directly to an ESP8266 GPIO or 3.3 V rail.
- Confirm the INA219 breakout's supply and I2C pull-ups are compatible with 3.3 V logic.
- Verify sensor polarity and current direction. INA219 current sign depends on wiring direction.
- Keep the buzzer disabled until its module voltage, polarity and startup behaviour are verified.

## Firmware setup

1. Install the ESP8266 board package in Arduino IDE.
2. Install libraries: **Adafruit INA219**, **OneWire**, **DallasTemperature**, and **PubSubClient**.
3. Open `firmware/battery_monitor.ino`.
4. Replace the Wi-Fi and MQTT placeholders with your own local settings. Never commit real credentials.
5. Set `MQTT_HOST` to the LAN IPv4 address of the computer running Mosquitto, not `localhost`.
6. Select the correct ESP8266 board and port, then upload.
7. Start Mosquitto and monitor serial output at 115200 baud.

The firmware publishes to `battery/demo/telemetry` every 2 seconds. Example payload:

```json
{"voltage":12.35,"current":0.241,"power":2.98,"temperature":27.4,"soc":75,"alarm":0}
```

The example values above are illustrative only.

## MQTT broker

For a local demonstration, run Mosquitto on the same LAN as the ESP8266 and allow the device to reach TCP port 1883. Do not expose an unauthenticated broker to the public internet. Configure authentication and network restrictions for any shared or deployed setup.

## Node-RED dashboard

Import `node-red/ev_battery_dashboard_flow.json` into Node-RED with the Dashboard 2.0 nodes installed. The provided flow is intentionally marked **simulated** and generates random demonstration values. It does not subscribe to `battery/demo/telemetry`. Do not describe it as live hardware data until you add and test an MQTT-in node and map the incoming JSON fields.

## Digital-twin baseline

See [digital-twin/README.md](digital-twin/README.md). For a nominal resistive load:

- (I_{pred}=V/R)
- (P_{pred}=V^2/R)

Compare predictions with measured INA219 values. Model mismatch can arise from load tolerance, wiring losses, sensor calibration and measurement noise. This baseline does not estimate battery health, remaining useful life or accurate state of charge.

## Repository layout

- `firmware/` — ESP8266 Arduino firmware
- `node-red/` — simulated Node-RED dashboard export
- `digital-twin/` — Python MQTT comparison script and model notes
- `docs/` — wiring, test-record template and limitations

## Before presenting or submitting

- Replace placeholders locally; keep credentials out of Git.
- Verify the exact board pinout and sensor voltage compatibility.
- Record actual voltage/current/temperature readings with date, load and instrument details.
- Add your own circuit photographs and dashboard screenshots.
- Clearly distinguish simulated dashboard values from hardware measurements.
- Review [docs/testing-record-template.md](docs/testing-record-template.md) and fill it with real observations.

## License

No open-source license has been selected for this repository yet. Until one is added, standard copyright applies.
