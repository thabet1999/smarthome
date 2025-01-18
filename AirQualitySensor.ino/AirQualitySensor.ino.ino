#define AIRQUALITYPIN 34

void setup() {
  Serial.begin(115200);
  Serial.println("Capteur de qualité de l'air prêt !");
}

void loop() {
  int airQualityValue = analogRead(AIRQUALITYPIN);
  Serial.print("Qualité de l'air (valeur brute) : ");
  Serial.println(airQualityValue);
  delay(2000);
}
