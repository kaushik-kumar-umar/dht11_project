#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// ---------- USER CONFIG ----------
const char* WIFI_SSID     = "XYZ";
const char* WIFI_PASSWORD = "********";

// Get this from thingspeak.com -> your channel -> API Keys -> Write API Key
const char* TS_API_KEY = "YOUR_THINGSPEAK_API_KEY";
const char* TS_SERVER  = "http://api.thingspeak.com/update";

#define DHTPIN  4        // GPIO pin the DHT11 data line is connected to
#define DHTTYPE DHT11

const unsigned long SEND_INTERVAL_MS = 20000; 

DHT dht(DHTPIN, DHTTYPE);
unsigned long lastSendTime = 0;

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

void sendToThingSpeak(float temperature, float humidity) {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  HTTPClient http;
  String url = String(TS_SERVER) + "?api_key=" + TS_API_KEY +
               "&field1=" + String(temperature, 1) +
               "&field2=" + String(humidity, 1);

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.println("ThingSpeak update sent. HTTP code: " + String(httpCode));
  } else {
    Serial.println("ThingSpeak update failed: " + http.errorToString(httpCode));
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWiFi();
}

void loop() {
  if (millis() - lastSendTime >= SEND_INTERVAL_MS) {
    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature(); // Celsius

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT11 sensor!");
    } else {
      Serial.printf("Temp: %.1f C  Humidity: %.1f %%\n", temperature, humidity);
      sendToThingSpeak(temperature, humidity);
    }

    lastSendTime = millis();
  }
}
