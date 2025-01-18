#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi Configuration
const char* ssid = "Votre_SSID";
const char* password = "Votre_Mot_De_Passe";

// MQTT Configuration
const char* mqttServer = "192.168.1.100";
const int mqttPort = 1883;
const char* humidityTopic = "home/humidity";

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion WiFi...");
  }
  Serial.println("WiFi connecté");

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connexion MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT connecté");
    } else {
      delay(2000);
    }
  }
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    String payload = "{\\\"humidity\\\": " + String(humidity) + ", \\\"temperature\\\": " + String(temperature) + "}";
    client.publish(humidityTopic, payload.c_str());
    Serial.println("Humidité et température envoyées : " + payload);
  }

  delay(5000);
}
