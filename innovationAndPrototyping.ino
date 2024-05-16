#include <WiFi.h>
#include "time.h"
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIXEL_PIN 15 // Pin for NeoPixel
#define NUMPIXELS 8 // Number of NeoPixels

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int delayval = 5; // Timing delay for NeoPixels

// Buzzer
int buzzer = 26; // Pin for the buzzer

// Servo
Servo myservo;
int angle;

// Joystick
#define VRY_PIN  32 // ESP32 pin connected to VRY pin
#define VRX_PIN  33 // ESP32 pin connected to VRX pin
int xValue = 0;
int yValue = 0;
int xDefaultValue = 0;
int yDefaultValue = 0;

// LCD screen
int lcdColumns = 16; // Number of columns
int lcdRows = 2; // Number of rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

int counter = 0;
unsigned long lastTime = 0;
unsigned long elapsedTime = 0;
unsigned long elapsedTimeMinutes = 0;
unsigned long currentTime = 0;
unsigned long lastUsageTime = 0;
unsigned long interval = 45 * 60 * 1000; // Interval in milliseconds (45 minutes)

const char* ssid = "Alisa";
const char* password = "8758758758";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 2 * 3600;
const int daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.setBrightness(25);
  pixels.show(); // Initialize all pixels to 'off'

  Wire.begin(21, 22); // SDA to GPIO 21, SCL to GPIO 22

  myservo.attach(14); // Pin for connecting the servo
  xDefaultValue = analogRead(VRX_PIN);
  yDefaultValue = analogRead(VRY_PIN);
  Serial.print("Default x value is ");
  Serial.println(xDefaultValue);
  Serial.print("Default y value is ");
  Serial.println(yDefaultValue);

  pinMode(buzzer, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fed");
  lcd.setCursor(0, 1);
  lcd.print("Counter:");
}

void loop() {
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(", y = ");
  Serial.println(yValue);

  currentTime = millis();
  if(counter <= 20 || currentTime - lastUsageTime >= interval) {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0, 250, 0)); // Moderately bright green color
      pixels.show();
      delay(delayval);
    }

    if(xValue > xDefaultValue + 5 || xValue < xDefaultValue - 5 || yValue > yDefaultValue + 5 || yValue < yDefaultValue - 5) {
      for (angle = 0; angle <= 145; angle++) {
        myservo.write(angle);
        delay(5);
      }
      delay(2000);
      for (angle = 145; angle >= 0; angle--) {
        myservo.write(angle);
        delay(5);
      }
      counter += 1;
      currentTime = millis();
      elapsedTime = currentTime - lastTime;
      elapsedTimeMinutes = elapsedTime / 1000;
      lastTime = currentTime;
      lastUsageTime = currentTime;
    }
  } else {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Bright red color
      pixels.show();
      delay(delayval);
    }
    //tone(buzzer, 1000); // Emit a tone for 100ms
    delay(100);
    noTone(buzzer);
  }

  lcd.setCursor(9, 1);
  lcd.print(counter);
  lcd.setCursor(4, 0);
  lcd.print(elapsedTimeMinutes);
  lcd.print(" secs ago");
  delay(1000);
}

