#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ===== Wi-Fi Credentials =====
const char* WIFI_SSID = "Nokia";           // Remplacez par votre SSID Wi-Fi
const char* WIFI_PASSWORD = "20231422"; // Remplacez par votre mot de passe Wi-Fi

// ===== Firebase Configuration =====
#define API_KEY "AIzaSyBY6ncN66Aq3N8kjx3_xZXIhZkJs4fww9k"               // Remplacez par votre clé API Firebase
#define DATABASE_URL "https://votre-projet.firebaseio.com/" // URL Realtime Database Firebase

// Firebase objects
FirebaseData fbdo;      // Objet pour les opérations Firebase
FirebaseAuth auth;      // Authentification Firebase
FirebaseConfig config;  // Configuration Firebase

unsigned long sendDataPrevMillis = 0; // Chronomètre pour l'envoi des données
bool signupOK = false;                // État de la connexion Firebase

void connectToWiFi() {
  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void connectToFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Initialisation de Firebase
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Connexion Firebase réussie !");
    signupOK = true;
  } else {
    Serial.printf("Erreur Firebase : %s\n", config.signer.signupError.message.c_str());
  }

  // Assignation de la fonction callback pour les tokens
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  connectToFirebase();
}

void loop() {
  if (Firebase.ready() && signupOK &&
      (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Exemple : Écriture dans Firebase
    float temperature = 23.5; // Exemple de donnée (remplacez par un capteur réel)
    if (Firebase.RTDB.setFloat(&fbdo, "sensors/temperature", temperature)) {
      Serial.println("Donnée envoyée à Firebase avec succès !");
      Serial.println("Chemin : " + fbdo.dataPath());
    } else {
      Serial.printf("Erreur d'envoi : %s\n", fbdo.errorReason().c_str());
    }

    // Exemple : Lecture depuis Firebase
    if (Firebase.RTDB.getFloat(&fbdo, "sensors/temperature")) {
      Serial.println("Donnée reçue de Firebase : " + String(fbdo.floatData()));
    } else {
      Serial.printf("Erreur de lecture : %s\n", fbdo.errorReason().c_str());
    }
  }
}
