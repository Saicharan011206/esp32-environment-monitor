#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// WiFi credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Firebase credentials
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define FIREBASE_PROJECT_ID "YOUR_PROJECT_ID"
#define USER_EMAIL "device@yourdomain.com"
#define USER_PASSWORD "device_password"

// Sensor pins
#define DHTPIN 4          // DHT22 data pin (GPIO 4)
#define DHTTYPE DHT22
#define MQ135PIN 34       // MQ135 analog pin (GPIO 34)

DHT dht(DHTPIN, DHTTYPE);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Device configuration
String deviceId = "DEVICE_001"; // Unique device ID
unsigned long sendDataPrevMillis = 0;
const long sendDataInterval = 60000; // Send data every 60 seconds

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Configure Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > sendDataInterval || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    
    // Read sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int airQuality = analogRead(MQ135PIN);
    
    // Calculate CO2 (MQ135 approximation - calibrate for accuracy)
    float co2Level = map(airQuality, 0, 4095, 400, 2000);
    
    // Check if readings are valid
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
    // Print readings
    Serial.printf("Temperature: %.2f°C\n", temperature);
    Serial.printf("Humidity: %.2f%%\n", humidity);
    Serial.printf("Air Quality: %d\n", airQuality);
    Serial.printf("CO2 Level: %.2f ppm\n", co2Level);
    
    // Prepare JSON data
    FirebaseJson json;
    json.set("temperature", temperature);
    json.set("humidity", humidity);
    json.set("airQuality", airQuality);
    json.set("co2Level", co2Level);
    json.set("timestamp/.sv", "timestamp");
    
    // Path: sensorData/{deviceId}/readings/{timestamp}
    String path = "sensorData/" + deviceId + "/readings/" + String(millis());
    
    // Send to Firebase
    if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), json.raw())) {
      Serial.println("Data sent successfully!");
    } else {
      Serial.println("Failed to send data: " + fbdo.errorReason());
    }
    
    // Update last active timestamp
    String devicePath = "devices/" + deviceId;
    FirebaseJson deviceJson;
    deviceJson.set("fields/lastActive/timestampValue/.sv", "timestamp");
    
    Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", devicePath.c_str(), deviceJson.raw(), "lastActive");
  }
}
