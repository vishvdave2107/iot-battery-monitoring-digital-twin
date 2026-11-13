#include <Wire.h>
#include <Adafruit_INA219.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <math.h>

// Fill these in locally. Do not commit real Wi-Fi credentials.
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_HOST = "YOUR_MQTT_BROKER_LAN_IP";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "battery/demo/telemetry";

// Witty Cloud / ESP8266 pin mapping; verify your exact carrier.
const uint8_t TEMP_PIN = D5;       // GPIO14
const uint8_t RED_LED = D6;        // GPIO12
const uint8_t GREEN_LED = D7;      // GPIO13
const uint8_t BUZZER_PIN = D0;     // GPIO16

// Keep disabled until buzzer voltage and active polarity are verified.
const bool USE_BUZZER = false;
const bool BUZZER_ACTIVE_LOW = false;

const float LOW_VOLTAGE_V = 11.80;  // Demonstration threshold; verify battery datasheet
const float HIGH_CURRENT_A = 0.70;  // Demonstration threshold; verify load/sensor limits
const float HIGH_TEMP_C = 45.0;     // Demonstration threshold; verify probe placement
const unsigned long SAMPLE_INTERVAL_MS = 2000;

Adafruit_INA219 ina219;
OneWire oneWire(TEMP_PIN);
DallasTemperature tempSensor(&oneWire);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
unsigned long lastSample = 0;

int estimateSOC(float voltage) {
  // Indicative voltage-only lookup; not a calibrated SOC measurement.
  if (voltage <= 11.60) return 0;
  if (voltage <= 11.80) return 10;
  if (voltage <= 12.00) return 25;
  if (voltage <= 12.20) return 50;
  if (voltage <= 12.40) return 75;
  if (voltage <= 12.60) return 90;
  return 100;
}

void setOutputs(bool alarm) {
  digitalWrite(RED_LED, alarm ? HIGH : LOW);
  digitalWrite(GREEN_LED, alarm ? LOW : HIGH);
  if (USE_BUZZER) {
    digitalWrite(BUZZER_PIN,
      BUZZER_ACTIVE_LOW ? (alarm ? LOW : HIGH) : (alarm ? HIGH : LOW));
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
  Serial.print("ESP8266 IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    String clientId = "BatteryMonitor-";
    clientId += String(ESP.getChipId(), HEX);
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("connected.");
    } else {
      Serial.print("MQTT state = ");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void publishReadings() {
  float voltage = ina219.getBusVoltage_V();
  float currentA = ina219.getCurrent_mA() / 1000.0;
  float powerW = voltage * currentA;

  tempSensor.requestTemperatures();
  float tempC = tempSensor.getTempCByIndex(0);
  bool tempValid = (tempC != DEVICE_DISCONNECTED_C);

  bool lowVoltage = voltage < LOW_VOLTAGE_V;
  bool highCurrent = fabs(currentA) > HIGH_CURRENT_A;
  bool highTemp = tempValid && tempC > HIGH_TEMP_C;
  bool sensorError = !tempValid;
  bool alarm = lowVoltage || highCurrent || highTemp || sensorError;

  setOutputs(alarm);
  if (!tempValid) tempC = -127.0;
  int soc = estimateSOC(voltage);

  char payload[240];
  snprintf(payload, sizeof(payload),
    "{\"voltage\":%.2f,\"current\":%.3f,\"power\":%.2f,"
    "\"temperature\":%.1f,\"soc\":%d,\"alarm\":%d}",
    voltage, currentA, powerW, tempC, soc, alarm ? 1 : 0);

  Serial.println("\n----- BATTERY MONITOR -----");
  Serial.printf("Voltage: %.2f V\n", voltage);
  Serial.printf("Current: %.3f A\n", currentA);
  Serial.printf("Power: %.2f W\n", powerW);
  if (tempValid) Serial.printf("Temperature: %.1f C\n", tempC);
  else Serial.println("Temperature: SENSOR ERROR");
  Serial.printf("Indicative SOC: %d%%\n", soc);
  Serial.printf("Alarm: %s\n", alarm ? "YES" : "NO");

  if (mqtt.connected()) {
    bool sent = mqtt.publish(MQTT_TOPIC, payload);
    Serial.println(sent ? "MQTT publish OK" : "MQTT publish FAILED");
    Serial.println(payload);
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, BUZZER_ACTIVE_LOW ? HIGH : LOW);
  setOutputs(false);

  // INA219 SDA = D2/GPIO4; SCL = D1/GPIO5
  Wire.begin(D2, D1);
  tempSensor.begin();

  if (!ina219.begin()) {
    Serial.println("ERROR: INA219 not detected. Check VCC, GND, SDA and SCL.");
    while (true) {
      digitalWrite(RED_LED, !digitalRead(RED_LED));
      delay(300);
    }
  }
  ina219.setCalibration_32V_2A();

  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setBufferSize(256);
  Serial.println("Battery monitor ready.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (millis() - lastSample >= SAMPLE_INTERVAL_MS) {
    lastSample = millis();
    publishReadings();
  }
}
