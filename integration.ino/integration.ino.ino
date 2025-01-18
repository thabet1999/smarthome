#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ===== Configuration WiFi =====
const char* ssid = "Votre_SSID";
const char* password = "Votre_Mot_De_Passe";

// ===== Configuration MQTT =====
const char* mqttServer = "192.168.1.100";
const int mqttPort = 1883;
const char* motionTopic = "home/motion";
const char* humidityTopic = "home/humidity";
const char* airQualityTopic = "home/airquality";
const char* vibrationTopic = "home/vibration";

WiFiClient espClient;
PubSubClient client(espClient);

// ===== Configuration des capteurs =====
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define PIRPIN 15
#define VIBRATIONPIN 27
#define AIRQUALITYPIN 34 // Pin analogique pour MQ-135

void connectToWiFi() {
  Serial.print("Connexion au WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connexion au broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connecté au broker MQTT !");
    } else {
      Serial.print("Échec, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Initialisation des capteurs
  pinMode(PIRPIN, INPUT);
  pinMode(VIBRATIONPIN, INPUT);
  dht.begin();

  // Connexion WiFi et MQTT
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  // Reconnexion au broker MQTT si nécessaire
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // ===== Lecture des capteurs =====

  // Mouvement (PIR)
  bool motionDetected = digitalRead(PIRPIN);
  if (motionDetected) {
    client.publish(motionTopic, "Mouvement détecté");
    Serial.println("Mouvement détecté");
  }

  // Vibration
  int vibration = digitalRead(VIBRATIONPIN);
  if (vibration == HIGH) {
    client.publish(vibrationTopic, "Vibration détectée");
    Serial.println("Vibration détectée");
  }

  // Température et humidité (DHT22)
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (!isnan(humidity) && !isnan(temperature)) {
    String humidityPayload = "{\"humidity\": " + String(humidity) + ", \"temperature\": " + String(temperature) + "}";
    client.publish(humidityTopic, humidityPayload.c_str());
    Serial.println("Humidité et température publiées : " + humidityPayload);
  }

  // Qualité de l'air (MQ-135)
  int airQualityValue = analogRead(AIRQUALITYPIN);
  if (airQualityValue >= 0) {
    String airQualityPayload = "{\"airQuality\": " + String(airQualityValue) + "}";
    client.publish(airQualityTopic, airQualityPayload.c_str());
    Serial.println("Qualité de l'air publiée : " + airQualityPayload);
  }

  delay(5000); // Attente avant la prochaine lecture
}
