/*
  Kelompok 45
  Irdina Rusyda Kamila Arsil
  Rafi Kamil Arief

  PWM-Based Light Control System for Chicken Coop
*/

//Inisialisasi library
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

//Inisialisasi variabel RTC
RTC_DS3231 rtc;
int tanggal = 10;
int bulan = 12;
int tahun = 2023;
int jam = 12;
int menit = 0;
int detik = 0;

//Inisialisasi pin LDR dan Driver L289N
#define LDR_pin 36
#define IN1   13
#define IN2   12
#define ENA   14

//inisialisasi parameter PWM
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 10;

// Inisialisasi pin untuk modul SD Card
#define SD_CS 5   

//fungsi untuk menulis ke dalam file
void writeFile(fs::FS &fs, const char *path, const char *message) {
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Setup started");

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    Serial.println("Pins set to OUTPUT");

    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin(ENA, pwmChannel);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.println("LED setup done");

    //Setup komunikasi I2C untuk RTC
    Wire.begin(25, 26); // SDA pada GPIO 25 dan SCL pada GPIO 26
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(tahun, bulan, tanggal, jam, menit, detik));
    }

  //Konfigurasi SD Card dan komunikasi SPI
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card mount failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // inisialisasi SD Card gagal
  }
  // Jika file log.txt tidak ada maka membuat file baru
  File file = SD.open("/log.txt");
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/log.txt", "Tanggal-Waktu\t Kondisi\t PWM Value\r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

}

// Fungsi untuk menambahkan pesan ke dalam file yang sudah ada
void appendFile(fs::FS &fs, const char *path, const char *message) {
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message)) {
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void loop() {
    Serial.println("Loop started");
    DateTime time = rtc.now();
    Serial.println(String("TIMESTAMP:\t") + time.timestamp(DateTime::TIMESTAMP_FULL));

    // Membaca LDR Value dan memetakannya ke dalam duty cycle
    int LDRValue = analogRead(LDR_pin);
    int dutyCycle = map(LDRValue, 3000, 4095, 0, 1023);
    dutyCycle = constrain(dutyCycle, 0, 1023);

    Serial.print("LDR Value = ");
    Serial.println(LDRValue);
    Serial.print("PWM Value = ");
    Serial.println(dutyCycle);

    // Mengatur LED sesuai dengan PWM
    ledcWrite(pwmChannel, dutyCycle);
    Serial.println("LED value written");

    // Menentukan kondisi pencahayaan lingkungan sesuai dengan nilai yang diterima dari sensor LDR
    Serial.print("\nKondisi lingkungan");
    if (LDRValue < 1000) {
        Serial.println(" => Sangat Cerah");
        appendFile(SD, "/log.txt", (time.timestamp(DateTime::TIMESTAMP_FULL) + "," + " \tSangat Cerah \t" + String(dutyCycle) + "\n").c_str());
    } else if (LDRValue < 2000) {
        Serial.println(" => Cerah");
        appendFile(SD, "/log.txt", (time.timestamp(DateTime::TIMESTAMP_FULL) + "," + " \tCerah \t" + String(dutyCycle) + "\n").c_str());
    } else if (LDRValue < 3000) {
        Serial.println(" => Terang");
        appendFile(SD, "/log.txt", (time.timestamp(DateTime::TIMESTAMP_FULL) + "," + " \tTerang \t" + String(dutyCycle) + "\n").c_str());
    } else if (LDRValue < 4000) {
        Serial.println(" => Redup");
        appendFile(SD, "/log.txt", (time.timestamp(DateTime::TIMESTAMP_FULL) + "," + " \tRedup \t" + String(dutyCycle) + "\n").c_str());
    } else {
        Serial.println(" => Gelap");
        appendFile(SD, "/log.txt", (time.timestamp(DateTime::TIMESTAMP_FULL) + "," + " \tGelap \t" + String(dutyCycle) + "\n").c_str());
    }

    delay(500);

    if (millis() % 2000 == 0) {
        Serial.println("System is running...");
    }

    Serial.println("Loop ended");
    delay(1500);
}
