# Architecture, wiring and safety notes

## Data path

Battery/load → INA219 voltage/current measurements → ESP8266 firmware → Wi-Fi → MQTT broker → (optional integration) Node-RED dashboard and Python digital-twin subscriber.

The supplied Node-RED export currently generates simulated values and is not connected to MQTT. The Python twin subscribes to real telemetry when run against the broker.

## Pin map

| Signal | ESP8266 label | GPIO |
|---|---|---:|
| INA219 SDA | D2 | 4 |
| INA219 SCL | D1 | 5 |
| DS18B20 data | D5 | 14 |
| Red LED | D6 | 12 |
| Green LED | D7 | 13 |
| Optional buzzer | D0 | 16 |

Use a 4.7 kΩ pull-up from DS18B20 data to 3.3 V. Confirm exact carrier pinout and INA219 breakout logic-level compatibility before connecting.

## Power path concept

Battery positive → fuse close to battery → INA219 VIN+ → INA219 VIN− → load resistor → battery negative.

Use suitably rated wiring and secure connections for the battery/load path. Do not route load current through a solderless breadboard. Follow the INA219 breakout's own terminal markings and verify the measured current sign at low-risk conditions before extended testing.

## Load estimate

For a nominal 50 Ω resistor at 12 V:

- (I=V/R=12/50=0.24\,A)
- (P=V^2/R=144/50=2.88\,W)

A 10 W resistor has nominal power headroom, but it can become hot. Mount it on a non-flammable surface, keep it clear of wires/plastic, and supervise the test.

## Firmware alarm defaults

The sketch uses demonstration thresholds: voltage below 11.80 V, absolute current above 0.70 A, temperature above 45 °C, or missing DS18B20. These are not universal battery protection limits. Verify appropriate thresholds using the battery and component datasheets and your test conditions.

## Important caveats

- The INA219 bus-voltage reading is taken at the module's bus measurement point; interpret it according to the actual wiring.
- Current sign depends on sensor orientation.
- Voltage-only SOC is approximate and changes with load, temperature, rest time and battery condition.
- Do not expose an unauthenticated MQTT broker to the public internet.
- Never publish Wi-Fi passwords, broker credentials, tokens or private keys.
