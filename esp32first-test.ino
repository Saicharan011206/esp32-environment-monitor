#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ============================================
// WiFi credentials - CHANGE THESE TO YOUR WIFI
// ============================================
#define WIFI_SSID "hotspot"
#define WIFI_PASSWORD "password"

// ============================================
// Firebase credentials - ALREADY SET FOR YOU
// ============================================
#define API_KEY "AIzaSyAyL52mHEL1-M8e_ZMEdUK1XjBM03JcULT"
#define FIREBASE_PROJECT_ID "sky-pulse-fed3e"
#define USER_EMAIL "saicharan011206@gmail.com"        // Email from Authentication
#define USER_PASSWORD "gssc@2006"  // Password from Authentication

// ============================================
// Sensor pins - ALREADY SET FROM YOUR WIRING
// ============================================
#define DHTPIN 4          // DHT22 data pin (GPIO 4)
#define DHTTYPE DHT22
#define MQ135PIN 34       // MQ135 analog pin (GPIO 34)

DHT dht(DHTPIN, DHTTYPE);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Device configuration
String deviceId = "Device-001";
unsigned long sendDataPrevMillis = 0;
const long sendDataInterval = 10000; // Send data every 10 seconds (for testing)

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n========================================");
  Serial.println("    Sky Pulse - Sensor System Starting");
  Serial.println("========================================\n");
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("✓ DHT22 sensor initialized");
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected!");
    Serial.print("  IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n✗ WiFi Connection Failed!");
    Serial.println("  Check your SSID and password");
    return;
  }

  // Configure Firebase
  Serial.println("\nConfiguring Firebase...");
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Serial.println("✓ Firebase initialized\n");
  Serial.println("========================================");
  Serial.println("System ready! Starting sensor readings...");
  Serial.println("========================================\n");
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > sendDataInterval || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    
    Serial.println("\n--- Reading Sensors ---");
    
    // Read sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int airQuality = analogRead(MQ135PIN);
    
    // Calculate CO2 (MQ135 approximation)
    float co2Level = map(airQuality, 0, 4095, 400, 2000);
    
    // Check if DHT readings are valid
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("✗ Failed to read from DHT sensor!");
      Serial.println("  Check wiring and connections");
      return;
    }
    
    // Print readings to Serial Monitor
    Serial.printf("Temperature:  %.1f°C\n", temperature);
    Serial.printf("Humidity:     %.1f%%\n", humidity);
    Serial.printf("Air Quality:  %d (raw)\n", airQuality);
    Serial.printf("CO2 Level:    %.0f ppm\n", co2Level);
    
    // Prepare Firebase path
    String path = "SensorData/" + deviceId;
    
    Serial.println("\nSending to Firebase...");
    
    // Send temperature
    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), 
        "Temperature", String(temperature).c_str())) {
      Serial.println("  ✓ Temperature sent");
    } else {
      Serial.println("  ✗ Temperature failed");
      Serial.println("    " + fbdo.errorReason());
    }
    
    // Send humidity
    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), 
        "Humidity", String(humidity).c_str())) {
      Serial.println("  ✓ Humidity sent");
    } else {
      Serial.println("  ✗ Humidity failed");
    }
    
    // Send air quality
    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), 
        "AirQuality", String(airQuality).c_str())) {
      Serial.println("  ✓ Air Quality sent");
    } else {
      Serial.println("  ✗ Air Quality failed");
    }
    
    // Send CO2 level
    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), 
        "CO2 Level", String(co2Level).c_str())) {
      Serial.println("  ✓ CO2 Level sent");
    } else {
      Serial.println("  ✗ CO2 Level failed");
    }
    
    Serial.println("\n✓ Upload cycle complete!");
    Serial.println("----------------------------");
  }
}
