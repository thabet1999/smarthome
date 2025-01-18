client.setServer(mqttServer, mqttPort);
while (!client.connected()) {
    Serial.println("Connexion au serveur MQTT...");
    if (client.connect("ESP32Client")) {
        Serial.println("Connecté au serveur MQTT");
    } else {
        delay(2000);
    }
}
