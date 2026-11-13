# Digital-twin baseline

This folder contains a deliberately simple, explainable reference model for the project's nominal resistive demonstration load.

## Equations

For measured load voltage (V) and nominal resistance (R):

- Predicted current: (I_{pred}=V/R)
- Predicted power: (P_{pred}=V^2/R)

The script subscribes to the ESP8266 telemetry topic, calculates these estimates, and prints the measured-versus-predicted comparison as JSON.

## Run

1. Install Python 3.
2. Install dependencies: `pip install -r requirements.txt`.
3. Ensure Mosquitto is running and reachable from this computer.
4. Run:

```bash
python digital_twin.py --host 127.0.0.1 --port 1883 --topic battery/demo/telemetry --resistance 50
```

If the broker is on another machine, replace `127.0.0.1` with its LAN IP.

## Interpretation and limitations

- Current comparison uses absolute measured current because INA219 sign depends on sensor/load orientation.
- This is a resistive-load electrical reference model, not a detailed SLA electrochemical model.
- Resistance tolerance, resistor heating, wiring/contact losses, INA219 calibration and sample timing create differences.
- Voltage-only SOC in the firmware is indicative and should not be presented as precision SOC.
- The model does not predict battery capacity fade, state of health, remaining useful life or future failure.
