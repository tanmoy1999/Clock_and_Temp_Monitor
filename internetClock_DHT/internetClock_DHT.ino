#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11 
#include <NTPClient.h>               // Include NTPClient library
#include <TimeLib.h>                 // Include Arduino time library
#include <Wire.h>
#include<LCD.h>
#include <LiquidCrystal_I2C.h>      // Include LiquidCrystal_I2C library
 
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows

#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 
 
const char *ssid = "D-Link_DIR-600M";  //wifi ssid
const char *password = "alliswell";   //wifi password
 
WiFiUDP ntpUDP;
 
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "time.nist.gov", 19800, 60000);
 
char Time[] = "TIME:00:00:00";
byte last_second, second_, minute_, hour_;
 
void setup() {
 
  Serial.begin(115200);
  dht.begin();
  lcd.begin(0, 2);                   // Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
  lcd.backlight();                   // Turn backlight ON
  lcd.setCursor(1, 0);
  lcd.print(Time);
  
 
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED ) {
    
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
 
  timeClient.begin();
}
 
void loop() {
  int h = dht.readHumidity();
  float t = dht.readTemperature();
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
  second_ = second(unix_epoch);
  lcd.setCursor(1, 1);
  lcd.print("T:");
    //lcd.setCursor(9,1);
    lcd.print(t);
    lcd.print("C ");
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
  if (last_second != second_) {
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    
 
    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;
 
    // Send time and date to serial monitor
    Serial.println(Time);    
 
    // Display time and date on the 16x2 LCD
    lcd.setCursor(1, 0);
    lcd.print(Time);
    
 
    last_second = second_;
  }
 
  delay(200);
 
}
