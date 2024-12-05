#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define FIRE_SENSOR_PIN 3
#define MQ135_PIN A0

#define LED_PIN 10
#define BUZZER_PIN 9
#define MOTOR_PIN 11

// Thresholds
#define TEMP_THRESHOLD 36.0
#define GAS_THRESHOLD 450
#define FIRE_THRESHOLD 2

unsigned long lastBlinkTime = 0;  // Waktu terakhir LED berkedip
bool ledState = LOW;             // Status LED (ON/OFF)
const int blinkInterval = 50;    // Interval kedipan dalam milidetik (50 ms)

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(FIRE_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);
}

void loop() {
  // Membaca data sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasLevel = analogRead(MQ135_PIN);
  int fireStatus = digitalRead(FIRE_SENSOR_PIN);

  // Variabel kontrol aktuator
  bool motorActive = false;
  bool ledBlinking = false;
  bool ledOn = false;
  bool buzzerActive = false;

  String statusMessage;

  // Logika pengendalian aktuator berdasarkan kondisi sensor
  if (temperature > TEMP_THRESHOLD && gasLevel > GAS_THRESHOLD && fireStatus == FIRE_THRESHOLD) {
    // Semua sensor di atas ambang batas
    ledBlinking = true;
    buzzerActive = true;
    motorActive = true;
    statusMessage = "Semua sensor di atas ambang batas! LED berkedip, kipas aktif, buzzer aktif.";
  } else if (fireStatus == FIRE_THRESHOLD) {
    // Flame sensor di atas ambang batas
    ledBlinking = true;
    buzzerActive = true;
    motorActive = true;
    statusMessage = "Flame sensor mendeteksi api! LED berkedip, kipas aktif, buzzer aktif.";
  } else if (gasLevel > GAS_THRESHOLD) {
    // Gas sensor di atas ambang batas
    ledOn = true;
    buzzerActive = true;
    motorActive = true;
    statusMessage = "Kualitas udara memburuk! LED menyala, kipas aktif, buzzer aktif.";
  } else if (temperature > TEMP_THRESHOLD) {
    // Suhu di atas ambang batas
    ledOn = true;
    motorActive = true;
    statusMessage = "Suhu terlalu panas! LED menyala, kipas aktif.";
  } else {
    // Semua sensor aman
    statusMessage = "Semua sensor aman. Semua aktuator dimatikan.";
  }

  // Cetak hasil pembacaan sensor dan status aktuator
  printSensorData(temperature, humidity, gasLevel, fireStatus, ledOn, ledBlinking, buzzerActive, motorActive, statusMessage);

  // Kirim data ke ESP32
  sendToESP32(temperature, humidity, gasLevel, fireStatus, ledOn, ledBlinking, buzzerActive, motorActive, statusMessage);

  // Kontrol aktuator
  digitalWrite(BUZZER_PIN, buzzerActive ? HIGH : LOW);
  digitalWrite(MOTOR_PIN, motorActive ? HIGH : LOW);

  // Kontrol LED
  if (ledBlinking) {
    handleLedBlinking();  // LED berkedip dengan non-blocking
  } else {
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  }

  delay(2000);  // Delay utama untuk pembacaan sensor (tidak mempengaruhi kedipan LED)
}

// Fungsi untuk mengelola kedipan LED
void handleLedBlinking() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = currentMillis;
    ledState = !ledState;  // Toggle LED
    digitalWrite(LED_PIN, ledState);
  }
}

// Fungsi untuk mencetak data sensor dan status aktuator
void printSensorData(float temp, float hum, int gas, int fire, bool ledOn, bool ledBlink, bool buzzer, bool motor, String message) {
  Serial.println("=== Pembacaan Sensor ===");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Gas Level: ");
  Serial.println(gas);

  Serial.print("Flame Sensor: ");
  Serial.println(fire == HIGH ? "Terdeteksi" : "Tidak terdeteksi");

  Serial.println("=== Status Aktuator ===");
  Serial.print("LED: ");
  if (ledBlink) {
    Serial.println("Berkedip");
  } else if (ledOn) {
    Serial.println("Menyala");
  } else {
    Serial.println("Mati");
  }

  Serial.print("Buzzer: ");
  Serial.println(buzzer ? "Aktif" : "Mati");

  Serial.print("Kipas: ");
  Serial.println(motor ? "Aktif" : "Mati");

  Serial.println("Pesan Status: " + message);
  Serial.println("========================");
}

// Fungsi untuk mengirim data ke ESP32
void sendToESP32(float temp, float hum, int gas, int fire, bool ledOn, bool ledBlink, bool buzzer, bool motor, String message) {
  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(gas);
  Serial.print(",");
  Serial.print(fire);
  Serial.print(",");
  Serial.print(ledOn ? "on" : (ledBlink ? "blinking" : "off"));
  Serial.print(",");
  Serial.print(buzzer ? "on" : "off");
  Serial.print(",");
  Serial.print(motor ? "on" : "off");
  Serial.print(",");
  Serial.println(message);
}
