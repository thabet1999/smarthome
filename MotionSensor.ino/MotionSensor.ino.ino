#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Configuration
const char* ssid = "Votre_SSID";
const char* password = "Votre_Mot_De_Passe";

// MQTT Configuration
const char* mqttServer = "192.168.1.100";
const int mqttPort = 1883;
const char* motionTopic = "home/motion";

WiFiClient espClient;
PubSubClient client(espClient);

#define PIRPIN 15

void setup() {
  Serial.begin(115200);
  pinMode(PIRPIN, INPUT);

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
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Reconnexion MQTT...");
      if (client.connect("ESP32Client")) {
        Serial.println("Reconnecté à MQTT");
      } else {
        delay(2000);
      }
    }
  }

  bool motionDetected = digitalRead(PIRPIN);
  if (motionDetected) {
    client.publish(motionTopic, "Mouvement détecté");
    Serial.println("Mouvement détecté");
  }

  delay(5000);
}
