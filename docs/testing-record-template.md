# Testing record template

> Fill this document with your own observed results. Do not enter estimated or invented readings as experimental data.

## Test setup

- Date/time:
- Battery make/model and rated capacity:
- Battery resting voltage before test:
- Load type and measured resistance:
- INA219 module/calibration:
- Temperature probe location:
- MQTT broker host (do not include credentials):
- Firmware version/commit:

## Measurement log

| Trial | Time | Voltage (V) | Current (A) | Power (W) | Temperature (°C) | Firmware alarm | Notes |
|---|---|---:|---:|---:|---:|---|---|
| 1 | | | | | | | |
| 2 | | | | | | | |
| 3 | | | | | | | |
| 4 | | | | | | | |
| 5 | | | | | | | |

## Digital-twin comparison

| Trial | Measured current (A) | Predicted current (A) | Current absolute error (A) | Measured power (W) | Predicted power (W) | Notes |
|---|---:|---:|---:|---:|---:|---|
| 1 | | | | | | |
| 2 | | | | | | |
| 3 | | | | | | |

## Verification checklist

- [ ] Fuse installed close to battery positive terminal
- [ ] INA219 polarity and current sign checked
- [ ] ESP8266 powered from a suitable regulated supply
- [ ] No 12 V connected to ESP8266 pins
- [ ] MQTT telemetry received on the configured topic
- [ ] Temperature sensor disconnect behaviour checked safely
- [ ] Dashboard labelled as simulated or connected to MQTT and tested
- [ ] Photos/screenshots captured from the actual build
- [ ] Thresholds checked against component/battery datasheets

## Observations and issues

Write only what was actually observed.

## Conclusion

Summarize what worked, what did not, and what remains unverified.
