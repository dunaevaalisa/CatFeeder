//including libraries
#include <WiFi.h>
#include "time.h"
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"


//LED circle
#define PIXEL_PIN 15 // Pin for NeoPixel
#define NUMPIXELS 8 // Number of NeoPixels

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int delayval = 5; // Timing delay for NeoPixels


// Servo
Servo myservo;
int angle = 0; //defining a variable for angle that servo would spin for


// Joystick
#define VRY_PIN  32 // ESP32 pin connected to VRY pin
#define VRX_PIN  33 // ESP32 pin connected to VRX pin
int xValue = 0; //variable for x value
int yValue = 0; //variable for y value
int xDefaultValue = 0; //default variable for x value
int yDefaultValue = 0; //default variable for y value

// LCD screen
int lcdColumns = 16; // Number of columns
int lcdRows = 2; // Number of rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


int counter = 0; //defining a variable for counter
char lastUseTime[6] = "00:00"; // Store the last usage time as a string

const char* ssid = "Alisa"; // name of the network
const char* password = "8758758758"; //password for the network

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 2 * 3600;
const int daylightOffset_sec = 3600;

#define BUZZER_PIN 26 // Pin for buzzer
bool buzzerPlayed = false; // Flag to indicate whether the buzzer has played


int StartMelody[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
}; //array with melody for the buzzer that plays when the food is available again

int startDurations[] = {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
}; //array with duration of the notes of the starting melody 

void setup() {
  Serial.begin(115200);

  pixels.begin(); //starting the LED-circle
  pixels.setBrightness(25); //setting the brightness of the LED
  pixels.show(); // Initialize all pixels to 'off'

  // Initialize LCD
  lcd.init();
  lcd.backlight(); //turning on the backlight
  lcd.setCursor(0, 0); //setting the cursor to the first row and the first column

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  delay(2500);
  lcd.clear();


   // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  Wire.begin(21, 22); // SDA to GPIO 21, SCL to GPIO 22

  myservo.attach(14); // Pin for connecting the servo

//reading default values of the joystick
  xDefaultValue = analogRead(VRX_PIN); 
  yDefaultValue = analogRead(VRY_PIN);
  Serial.print("Default x value is ");
  Serial.println(xDefaultValue);
  Serial.print("Default y value is ");
  Serial.println(yDefaultValue);


  pinMode(BUZZER_PIN, OUTPUT); //connecting the buzzer

  //printing the base values for the LCD screen
  lcd.print("Fed: ");
  lcd.setCursor(0, 1);
  lcd.print("Counter:");
}


void loop() {
  //reading current values of the joystick
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(", y = ");
  Serial.println(yValue);

//getting the current time 
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  int currentHour = timeinfo.tm_hour;
  int currentMinutes = timeinfo.tm_min;
  int currentSeconds = timeinfo.tm_sec;


//reseting the counter and playing the melody when the new day starts   
if(currentHour == 11 &&  currentMinutes == 59 && currentSeconds == 0) {
  counter = 0; 
  int size = sizeof(startDurations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / startDurations[note];
    tone(BUZZER_PIN, StartMelody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
}


//checking the time and the value of counter
  if ((currentHour >= 11 && currentHour <= 19) && counter < 5) {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0, 250, 0)); // Moderately bright green color
      pixels.show();
      delay(delayval);
    }

//checking the value of the joystick
    if(xValue > xDefaultValue + 100 || xValue < xDefaultValue - 100 || yValue > yDefaultValue + 100 || yValue < yDefaultValue - 100) {
      //turning the servo
      for (angle = 0; angle <= 145; angle++) { 
        myservo.write(angle);
        delay(5);
      }
      delay(2000);
      for (angle = 145; angle >= 0; angle--) {
        myservo.write(angle);
        delay(5);
      }
      counter += 1; //increasing the counter value
      updateLastUseTime(); //updating the time
    }
  } else {
    if (!buzzerPlayed) {
    tone(BUZZER_PIN, 1000); // Send a 1 kHz sound signal to the buzzer
    delay(1000); // Wait for one second 
    noTone(BUZZER_PIN); // Stop the buzzer sound
    buzzerPlayed = true; // Set the flag to indicate the buzzer has played
  }


    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Bright red color
      pixels.show();
      delay(delayval);
    }
   
  }

//printing the counter to the LCD screen
  lcd.setCursor(9, 1);
  lcd.print(counter); 
  delay(1000);
}

//function to get the current time 
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


//function to update the last usage time
void updateLastUseTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(lastUseTime, sizeof(lastUseTime), "%H:%M", &timeinfo);
  //printing the last usage time to the LCD screen
  lcd.setCursor(5, 0);
  lcd.print(lastUseTime); 
}
