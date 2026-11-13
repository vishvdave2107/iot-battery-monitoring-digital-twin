# Project report

**Title:** IoT-Based Battery Monitoring System with Digital Twin for Real-Time Parameter Monitoring and Predictive Analysis

> Submission note: Complete the cover-page identity fields and replace all result placeholders with measurements from the actual prototype. Do not present illustrative values as experimental results.

## Abstract

This project describes an ESP8266-based battery monitoring prototype that measures voltage, current, power and temperature. An INA219 sensor provides voltage/current readings and a DS18B20 probe measures temperature. The ESP8266 publishes telemetry through Wi-Fi and MQTT. A simple software digital twin estimates expected current and power for a known resistive load and compares those estimates with measured values.

The prototype is an educational monitoring and modelling system, not a certified battery-management system. Voltage-only state-of-charge estimates are approximate. The resistive-load model does not model battery electrochemistry or predict remaining useful life.

## Aim and objectives

**Aim:** Design an IoT-based battery monitoring prototype with a software digital twin for real-time parameter monitoring and basic model-based predictive analysis.

Objectives:
- Measure voltage and current with INA219.
- Measure temperature with DS18B20.
- Calculate power from voltage and current.
- Use ESP8266 Wi-Fi and MQTT to publish telemetry.
- Display values through a dashboard when connected to the telemetry stream.
- Compare measured values with a resistive-load model.
- Document safe testing and limitations.

## Hardware and software

Hardware: ESP8266 Witty Cloud board (verify exact carrier), INA219, DS18B20, 12 V 1.2–1.3 Ah SLA battery, nominal 50 Ω 10 W load resistor, inline 1 A fuse, LEDs and resistors, 4.7 kΩ DS18B20 pull-up. The optional buzzer is disabled by default in the committed firmware.

Software: Arduino IDE, ESP8266 board package, Adafruit INA219, OneWire, DallasTemperature, PubSubClient, Mosquitto, Node-RED, Python and Paho MQTT.

## System architecture

Physical battery and sensors → ESP8266 acquisition and threshold checks → Wi-Fi/MQTT broker → dashboard and digital-twin subscriber.

Firmware topic: `battery/demo/telemetry`. Sample interval: 2 seconds.

The provided Node-RED flow generates simulated sample readings. It is not yet wired to subscribe to the ESP8266 MQTT topic; label its output as simulated until that integration is completed and tested.

## Digital twin

For a nominal resistive load (R) and measured voltage (V):

[
I_{pred}=V/R
]

[
P_{pred}=V^2/R
]

Compare these predictions with INA219 measurements. Differences can arise from resistor tolerance and heating, wiring/contact losses, sensor calibration and measurement noise. This model is not a battery electrochemical model and does not predict state of health or remaining useful life.

## Testing and results

Enter actual readings in [the testing record](testing-record-template.md). Record date/time, battery condition, load resistance, instrument readings, sensor readings, temperature, MQTT receipt, model output and observed alarm behaviour. Do not claim accuracy, reliability or successful alert response without evidence from tests.

## Safety and limitations

- Fit the fuse close to battery positive.
- Use suitably rated wire and secure terminals for battery/load current; do not route load current through a solderless breadboard.
- At 12 V, a 50 Ω resistor dissipates about 2.88 W and becomes hot. Mount it on a non-flammable surface and supervise testing.
- Never connect 12 V directly to ESP8266 pins or its 3.3 V rail.
- Confirm INA219 breakout I²C pull-ups are compatible with 3.3 V.
- Confirm exact carrier pinout and DS18B20 probe wiring.
- Keep MQTT on a trusted network; do not expose an unauthenticated broker to the public internet.
- Thresholds in firmware are demonstration settings and must be checked against the battery and component specifications.
- Voltage-only SOC is approximate, especially under load.

## Conclusion

The prototype demonstrates sensor acquisition, wireless telemetry and a simple model-based comparison for a resistive load. Its validity depends on safe assembly, correct sensor configuration and transparent testing. Any performance claims must be supported by completed measurements. Future work can include calibrated sensing, longer-term logging, improved SOC estimation, a battery-specific model and tested MQTT dashboard integration.
