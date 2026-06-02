#define BLYNK_TEMPLATE_ID "TMPL6FFc3biAM"
#define BLYNK_TEMPLATE_NAME "ESP32 Smart Home System"
#define BLYNK_AUTH_TOKEN "Bm_wAKf8yxatNa42j6qbAChzI43fYbUk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_APDS9960.h>
#include <Adafruit_BME280.h>

// WiFi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "AC2900-24GHz";
char pass[] = "L3IFHN002019";

// Sensors
Adafruit_APDS9960 apds;
Adafruit_BME280 bme;

// LED config
const int ledPin = 2;
const int pwmFreq = 5000;
const int pwmResolution = 8;

// LED state
bool ledOn = false;

// Temperature update
unsigned long lastTempUpdate = 0;
const unsigned long tempUpdateInterval = 3000;

// Blynk Virtual Pin handler (toggle LED)
BLYNK_WRITE(V0) {
  int pinValue = param.asInt();
  ledOn = pinValue;
  Serial.print("Blynk Toggle: ");
  Serial.println(ledOn ? "ON" : "OFF");

  if (!ledOn) {
    ledcWrite(ledPin, 255); // Turn OFF (active-low)
  }
  // If ON, brightness will be updated in loop() based on temperature
}

void setup() {
  Serial.begin(9600);

  // Start Blynk
  Blynk.begin(auth, ssid, pass);

  // Init PWM for LED
  ledcAttach(ledPin, pwmFreq, pwmResolution);
  ledcWrite(ledPin, 255); // Start OFF

  // Init gesture sensor
  if (!apds.begin()) {
    Serial.println("Failed to find Hibiscus Sense APDS9960 chip");
  } else {
    apds.enableProximity(true);
    apds.enableGesture(true);
  }

  // Init BME280
  if (!bme.begin()) {
    Serial.println("Failed to find Hibiscus Sense BME280 chip");
  }
}

void loop() {
  Blynk.run();

  // Handle gesture input
  uint8_t gesture = apds.readGesture();

  if (gesture == APDS9960_UP) {
    Serial.println("Gesture: UP (LED ON)");
    ledOn = true;
    Blynk.virtualWrite(V0, 1); // Sync with Blynk UI
  }

  if (gesture == APDS9960_DOWN) {
    Serial.println("Gesture: DOWN (LED OFF)");
    ledOn = false;
    ledcWrite(ledPin, 255); // OFF
    Blynk.virtualWrite(V0, 0); // Sync with Blynk UI
  }

  if (gesture == APDS9960_LEFT) Serial.println("Gesture: LEFT");
  if (gesture == APDS9960_RIGHT) Serial.println("Gesture: RIGHT");

  // Update temperature and brightness every 3 seconds
  if (millis() - lastTempUpdate >= tempUpdateInterval) {
    float tempC = bme.readTemperature();
    Serial.print("Ambient Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");

    Blynk.virtualWrite(V1, tempC);

    if (ledOn) {
      int pwmValue;
      if (tempC <= 25) {
        pwmValue = 243;
      } else if (tempC >= 30) {
        pwmValue = 0;
      } else {
        pwmValue = map(tempC * 100, 2500, 3000, 243, 0); // Smooth scale
      }

      Serial.print("PWM Brightness (LED ON): ");
      Serial.println(pwmValue);
      ledcWrite(ledPin, pwmValue);
    }

    lastTempUpdate = millis();
  }
}
