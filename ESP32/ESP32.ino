#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi credentials
#define WIFI_SSID "Redmi Note 10S"
#define WIFI_PASSWORD "AkuSayangKamu"

// Firebase credentials
#define FIREBASE_HOST "dsm2024-e8b0c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "VLcC1MhirGZ7KMRCkkc0kuQZietKOMH2tX81dMnP"

// Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// Function declarations
String getValue(String data, char separator, int index);
bool isValidData(String data);

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Set Firebase configuration
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  if (!Firebase.ready()) {
    Serial.println("Firebase initialization failed!");
    while (true);
  }
  Serial.println("Firebase initialized successfully.");
}

void loop() {
  if (Serial.available()) {
    delay(100); // Ensure complete data reception
    String data = Serial.readStringUntil('\n');
    Serial.println("Raw data received: " + data);
    uploadToFirebase(data);
  }
}

void uploadToFirebase(String data) {
  // Validate data format
  if (!isValidData(data)) {
    Serial.println("Invalid data format!");
    return;
  }

  // Parse data
  String temp = getValue(data, ',', 0);
  String hum = getValue(data, ',', 1);
  String gas = getValue(data, ',', 2);
  String fire = getValue(data, ',', 3);
  String led = getValue(data, ',', 4);
  String buzzer = getValue(data, ',', 5);
  String motor = getValue(data, ',', 6);
  String message = getValue(data, ',', 7);

  // Debug parsed data
  Serial.println("Parsed Data:");
  Serial.println("Suhu: " + temp);
  Serial.println("Kelembapan: " + hum);
  Serial.println("Gas: " + gas);
  Serial.println("Api: " + fire);
  Serial.println("LED: " + led);
  Serial.println("Buzzer: " + buzzer);
  Serial.println("Motor: " + motor);
  Serial.println("Pesan: " + message);

  // Upload data to Firebase
  if (Firebase.setFloat(firebaseData, "/Suhu", temp.toFloat())) {
    Serial.println("Suhu updated successfully.");
  } else {
    Serial.println("Error updating Suhu: " + firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, "/Kelembapan", hum.toFloat())) {
    Serial.println("Kelembapan updated successfully.");
  } else {
    Serial.println("Error updating Kelembapan: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/Gas", gas.toInt())) {
    Serial.println("Gas updated successfully.");
  } else {
    Serial.println("Error updating Gas: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/Api", fire.toInt())) {
    Serial.println("Api updated successfully.");
  } else {
    Serial.println("Error updating Api: " + firebaseData.errorReason());
  }

  if (Firebase.setString(firebaseData, "/LED", led)) {
    Serial.println("LED updated successfully.");
  } else {
    Serial.println("Error updating LED: " + firebaseData.errorReason());
  }

  if (Firebase.setString(firebaseData, "/Buzzer", buzzer)) {
    Serial.println("Buzzer updated successfully.");
  } else {
    Serial.println("Error updating Buzzer: " + firebaseData.errorReason());
  }

  if (Firebase.setString(firebaseData, "/Motor", motor)) {
    Serial.println("Motor updated successfully.");
  } else {
    Serial.println("Error updating Motor: " + firebaseData.errorReason());
  }

  FirebaseJson json;
  json.set("/Pesan", message);

  if (Firebase.updateNode(firebaseData, "/", json)) {
    Serial.println("Pesan berhasil dikirim.");
  } else {
    Serial.println("Error updating Pesan: " + firebaseData.errorReason());
  }
}

bool isValidData(String data) {
  int count = 0;
  for (int i = 0; i < data.length(); i++) {
    if (data.charAt(i) == ',') count++;
  }
  return count == 7; // Must have 7 commas
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void testParsing() {
  String testData = "29.10,81.30,305,1,off,off,off,Semua sensor aman.";
  Serial.println("Testing parsing with: " + testData);

  Serial.println("Suhu: " + getValue(testData, ',', 0));
  Serial.println("Kelembapan: " + getValue(testData, ',', 1));
  Serial.println("Gas: " + getValue(testData, ',', 2));
  Serial.println("Api: " + getValue(testData, ',', 3));
  Serial.println("LED: " + getValue(testData, ',', 4));
  Serial.println("Buzzer: " + getValue(testData, ',', 5));
  Serial.println("Motor: " + getValue(testData, ',', 6));
  Serial.println("Pesan: " + getValue(testData, ',', 7));
}
