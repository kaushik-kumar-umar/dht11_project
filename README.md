# DHT11 IoT Weather Station

Real-time weather monitoring system using an ESP32 and a DHT11 sensor. Live temperature and humidity readings are pushed over Wi-Fi to a cloud dashboard (ThingSpeak) every 20 seconds for remote visualization.

# Hardware
ESP32 dev board
DHT11 temperature & humidity sensor
Jumper wires, breadboard

# How it works
ESP32 connects to Wi-Fi.
Every 20 seconds, it reads temperature and humidity from the DHT11.
Readings are sent via an HTTP GET request to a ThingSpeak channel.
The ThingSpeak dashboard shows live temperature and humidity graphs.
