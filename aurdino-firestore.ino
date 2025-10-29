#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ135_PIN 34

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("ESP32 Sensor Data Logger Started");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int mq135_raw = analogRead(MQ135_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Create a JSON-like string for the Python script
  Serial.print("{");
  Serial.print("\"temperature\": "); Serial.print(temperature, 2); Serial.print(", ");
  Serial.print("\"humidity\": "); Serial.print(humidity, 2); Serial.print(", ");
  Serial.print("\"mq135\": "); Serial.print(mq135_raw);
  Serial.println("}");

  delay(5000); // send data every 5 seconds
}

