# AUTOMAHOME
## An Intelligent Home System Automation Security with Keypad, Smart Clothes Drying Detection, and Human-sensing Outdoor Lights

---

## Project Overview

AUTOMAHOME is an Arduino-based intelligent home automation system that integrates three independent real-time subsystems into a single embedded platform:

- **Keypad Security & Door Control** — password-protected door access using a 4x4 keypad and servo motor
- **Smart Clothes Drying Detection** — automatic clothes rack control based on rain sensor readings
- **Human-sensing Outdoor Lights** — ultrasonic-based presence detection that controls an outdoor LED


## Hardware Requirements

| Component | Quantity |
|---|---|
| Arduino Uno | 1 |
| LCD 16x2 with I2C module | 1 |
| 4x4 Matrix Keypad | 1 |
| Servo Motor (SG90 or MG90) | 2 |
| Rain Sensor Module | 1 |
| Ultrasonic Sensor HC-SR04 | 1 |
| LED | 1 |
| 220Ω Resistor (for LED) | 1 |
| Jumper Wires | — |
| Breadboard | 1 |
| USB Type-B Cable | 1 |

---

## Pin Mapping

| Component | Arduino Pin |
|---|---|
| Keypad Row 1–4 | 9, 8, 7, 6 |
| Keypad Col 1–4 | 5, 4, 3, 2 |
| Door Servo | 10 |
| Rain Servo | 11 |
| Rain Sensor (digital) | 12 |
| Ultrasonic TRIG | 13 |
| Ultrasonic ECHO | A0 |
| Outdoor LED | A1 |
| LCD SDA | A4 (I2C) |
| LCD SCL | A5 (I2C) |

---

## Required Libraries

Install all libraries through Arduino IDE Library Manager (Sketch → Include Library → Manage Libraries):

| Library | Install Name |
|---|---|
| Wire | Built-in (no install needed) |
| LiquidCrystal I2C | LiquidCrystal I2C by Frank de Brabander |
| Keypad | Keypad by Mark Stanley, Alexander Brevig |
| Servo | Built-in (no install needed) |

---

## How to Run the Project


## Keypad Controls

| Key | Action |
|---|---|
| 0–9 | Enter password digits (shown as *) |
| # | Submit / confirm password |
| * | Backspace — delete last digit |
| D | Manually lock the door immediately |
| A, B, C | Not used |

---

## System Behavior

### Door / Keypad
- Enter your password using the number keys, then press # to confirm.
- Correct password → door servo rotates to 90° (unlocked), LCD shows "Access Granted!" then "Door Open".
- Door auto-locks after 5 seconds.
- Wrong password → LCD shows remaining attempts.
- After 3 wrong attempts → system locks out for 10 seconds.
- Press D at any time to manually lock the door.

### Smart Clothes Drying
- When rain is detected, the clothes rack servo closes to 90° and the LCD briefly shows "Rain Detected! / Clothes covered".
- When rain stops, the servo returns to 0° (open) and the LCD shows "Rain stopped / Rack opened".
- This runs independently of the keypad at all times.

### Outdoor LED
- The HC-SR04 sensor continuously measures distance.
- If a person or object is detected within 100 cm, the LED turns ON.
- When nothing is detected, the LED turns OFF.
- Detection threshold can be adjusted by changing DETECT_DISTANCE in the source code.


## Customization

| What to change | Where in the code |
|---|---|
| Password | const String CORRECT_PASSWORD = "1234"; |
| Door open duration | const unsigned long DOOR_OPEN_DURATION = 5000; |
| Detection distance | const int DETECT_DISTANCE = 100; |
| Max wrong attempts | const int MAX_ATTEMPTS = 3; |
| Lockout duration | const unsigned long LOCKOUT_DURATION = 10000; |
| LCD I2C address | LiquidCrystal_I2C lcd(0x3F, 16, 2); |


