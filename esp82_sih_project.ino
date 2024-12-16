#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "fatigueanalysis"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


char auth[] = "";
char ssid[] = "TEJASWI";
char pass[] = ""; 

DHT dht(D3, DHT11);  // D4 DHT11
BlynkTimer timer;

#define sweat A0

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  dht.begin();
  
  timer.setInterval(100L, sweatSensor);
  timer.setInterval(100L, DHT11sensor);
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

void sweatSensor() {
  int value = analogRead(sweat);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  
  Blynk.virtualWrite(V3, value);
  if (value < 30) {
    Blynk.virtualWrite(V4, HIGH);
  } else if (value > 60) {
    Blynk.virtualWrite(V4, LOW);
  }

}

void loop() {
  Blynk.run(); 
  timer.run();
}
