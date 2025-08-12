// Melakukan intallasi library sensor DHT-11 dan Thinger.io
#include <DHT.h>
#include <ThingerESP32.h>

// Untuk menyambungkan antara modul ESP-32 ke Thinger.io
#define USERNAME "nandasultan10"
#define DEVICE_ID "NodeMCUESP32"
#define DEVICE_CREDENTIAL "punyananda"

// Menyambungkan ESP-32 ke jaringan dengan memasukan ssdi dan pasword internet yang ingin dihubungkan
#define SSID "nanda"
#define SSID_PASSWORD "nanda290102"

// Melakukan identifikasi tipe dht dan pin berapa yang digunakan untuk menerima output suhu dan kelembaban
#define DHTPIN 25
#define DHTTYPE DHT11

// Melakukan identifikasi pin berapa yang digunakan untuk mengkontrol kipas dan misting
#define FAN_PIN 32
#define MIST_PIN 33

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
DHT dht(DHTPIN, DHTTYPE);

//mendeklarasi suhu dan kelembaban
float hum, temp;

void setup() {

  Serial.begin(115200);

  pinMode(FAN_PIN, OUTPUT);
  pinMode(MIST_PIN, OUTPUT);

  dht.begin();

  thing.add_wifi(SSID, SSID_PASSWORD);
  
  thing["dht11"] >> [](pson& out){
    out["humidity"] = hum;
    out["celsius"] = temp;
 };

}

void loop() {

thing.handle();
  //mendeklarasi suhu dan kelembaban
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  hum = h;
  temp = t;

  //memberikan tampilan di serial monitor 
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" °C, Humidity: ");
    Serial.print(h);
    Serial.println(" %");

    //mengatur relay kipas dimana diakan dinyalakan dan dimatikan
    if (t < 28.0) {
      digitalWrite(FAN_PIN, HIGH);
    } else {
      digitalWrite(FAN_PIN, LOW);
    }
    //mengatur relay misting dimana diakan dinyalakan dan dimatikan
    if (h > 65.0) {
      digitalWrite(MIST_PIN, HIGH);
    } else {
      digitalWrite(MIST_PIN, LOW);
    }
  }

  //waktu dia akan diupdate
  delay(5000);
}
