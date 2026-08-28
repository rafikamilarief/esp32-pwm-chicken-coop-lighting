#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>  
#include <DS3231.h>
#include <time.h>

#define LDR_pin 36
#define IN1   16
#define IN2   17
#define ENA   4

#define SD_CS 5    // GPIO 5 untuk Chip Select
#define SD_MOSI 23 // GPIO 23 untuk MOSI
#define SD_MISO 19 // GPIO 19 untuk MISO
#define SD_SCK 18  // GPIO 18 untuk SCK

DS3231 myRTC;  // Corrected the object name to myRTC

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");
  Wire.begin(25, 26);

  // Set the RTC with the current date and time from PC
  setRTC();
}

void loop() {
  // Your main loop code here
}

void setRTC() {

}
