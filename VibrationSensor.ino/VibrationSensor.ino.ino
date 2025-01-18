#define VIBRATIONPIN 27

void setup() {
  Serial.begin(115200);
  pinMode(VIBRATIONPIN, INPUT);
  Serial.println("Capteur de vibration prêt !");
}

void loop() {
  int vibration = digitalRead(VIBRATIONPIN);
  
  if (vibration == HIGH) {
    Serial.println("Vibration détectée !");
  } else {
    Serial.println("Aucune vibration.");
  }
  
  delay(1000);
}
