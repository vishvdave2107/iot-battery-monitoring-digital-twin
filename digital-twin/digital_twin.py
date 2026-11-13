#!/usr/bin/env python3
"""Minimal MQTT digital-twin baseline for a nominal resistive load."""
import argparse
import json
import math
import sys

import paho.mqtt.client as mqtt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1", help="MQTT broker host/IP")
    parser.add_argument("--port", type=int, default=1883)
    parser.add_argument("--topic", default="battery/demo/telemetry")
    parser.add_argument("--resistance", type=float, default=50.0,
                        help="Nominal load resistance in ohms")
    args = parser.parse_args()

    if args.resistance <= 0:
        parser.error("--resistance must be greater than zero")

    def on_connect(client, userdata, flags, reason_code, properties=None):
        if reason_code == 0:
            print(f"Connected; subscribing to {args.topic}")
            client.subscribe(args.topic)
        else:
            print(f"MQTT connection failed: {reason_code}", file=sys.stderr)

    def on_message(client, userdata, message):
        try:
            data = json.loads(message.payload.decode("utf-8"))
            voltage = float(data["voltage"])
            measured_current = float(data["current"])
            measured_power = float(data.get("power", voltage * measured_current))
            predicted_current = voltage / args.resistance
            predicted_power = voltage * voltage / args.resistance
            result = {
                "voltage_V": round(voltage, 3),
                "measured_current_A": round(measured_current, 4),
                "predicted_current_A": round(predicted_current, 4),
                "current_abs_error_A": round(abs(abs(measured_current) - predicted_current), 4),
                "measured_power_W": round(measured_power, 3),
                "predicted_power_W": round(predicted_power, 3),
                "power_error_W": round(measured_power - predicted_power, 3),
                "model": "ideal_resistive_load",
                "resistance_ohm": args.resistance,
            }
            print(json.dumps(result))
        except (ValueError, KeyError, TypeError, json.JSONDecodeError) as exc:
            print(f"Skipped invalid telemetry: {exc}", file=sys.stderr)

    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message
    try:
        client.connect(args.host, args.port, keepalive=60)
        client.loop_forever()
    except KeyboardInterrupt:
        print("\nStopping digital twin.")
    finally:
        client.disconnect()


if __name__ == "__main__":
    main()
