# Smart Cat Feeder 
A Connected IoT Solution for Pet Care
This project is an IoT-enabled Smart Cat Feeder, designed to make pet feeding more efficient and customizable. <br>
The device combines servo-controlled mechanisms, LED indicators, and a user-friendly LCD display to create an innovative and interactive feeding system. Built using ESP32, this project integrates scheduling, real-time feedback, and smart notifications for a complete pet-care solution.

## Features 
- Customizable Feeding Schedule:
Configure feeding times and control the number of daily feedings.

- Portion Control:
Adjust the amount of food dispensed with precision servo motor control.

- Interactive Joystick Control:
Feed your pet manually by moving the joystick, providing flexibility and interaction.

- Real-Time Status Display:
The LCD screen tracks the current feed count and displays the last feeding time.

- LED and Sound Alerts:

  - Red LED + buzzer when feeding is restricted.
  - Green LED + Nokia ringtone when feeding is allowed.
- Energy Efficiency:
Automatically disconnects Wi-Fi after syncing the time with NTP servers to conserve power.

## Technical Stack

### Hardware:

- ESP32: Microcontroller for managing the feeder.
- Servo Motor: For dispensing food.
- Adafruit NeoPixel Ring: LED indicators for feeding status.
- Joystick: For manual feeding.
- LCD Display (I2C): Real-time feed count and status.
- Buzzer: Audio notifications.

### Software:

- C++: Core programming for ESP32.

### Libraries Used:
- Adafruit_NeoPixel
- ESP32Servo
- LiquidCrystal_I2C
- pitches.h
- WiFi.h


## Here’s a breakdown of how the Smart Cat Feeder works:

- At the start of the day Counter resets to zero.
- Nokia ringtone plays to signal feeding availability.

#### Feeding Allowed:

Green LED indicates feeding is enabled.
Joystick movement activates the servo motor to dispense food.

#### Feeding Restricted:

Red LED and buzzer sound notify that the daily limit is reached.

#### Real-Time Updates:

LCD displays feed count and last feeding time.

## How to Use 
### Hardware Setup:

Connect the ESP32 microcontroller to:

- Servo motor for dispensing food.
- Adafruit NeoPixel Ring for LED notifications.
- Joystick for manual feeding.
- LCD display via I2C.
- Buzzer for sound alerts.
- Load the program onto the ESP32 using the Arduino IDE.

### Software Configuration:

- Replace the placeholders in config.h with your Wi-Fi SSID and password.
- Install required libraries:
- Adafruit_NeoPixel
- ESP32Servo
- LiquidCrystal_I2C
- Upload the code and monitor the serial output for debugging.


## Future Enhancements 
- Add a mobile app for remote feeding control.
- Integrate weight sensors for precise portion measurement.
- Enable real-time notifications via MQTT.
- Expand functionality for multi-pet households.
