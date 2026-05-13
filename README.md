# AUTOMAHOME
## An Intelligent Home System Automation Security with Keypad, Smart Clothes Drying Detection, and Human-sensing Outdoor Lights

---

## Project Overview

AUTOMAHOME is an Arduino-based intelligent home automation system that integrates three independent real-time subsystems into a single embedded platform:

- **Keypad Security & Door Control** — password-protected door access using a 4x4 keypad and servo motor
- **Smart Clothes Drying Detection** — automatic clothes rack control based on rain sensor readings
- **Human-sensing Outdoor Lights** — ultrasonic-based presence detection that controls an outdoor LED

All three subsystems run in parallel without blocking each other, using non-blocking millis() timers throughout.

The sketch folder and .ino file must share the same name (AUTOMAHOME) for the Arduino IDE to open it correctly.

---

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

### Step 1 — Install Arduino IDE

Download and install the Arduino IDE from https://www.arduino.cc/en/software. Version 1.8.x or 2.x both work.

### Step 2 — Install Required Libraries

1. Open Arduino IDE.
2. Go to Sketch → Include Library → Manage Libraries.
3. Search for and install:
   - LiquidCrystal I2C by Frank de Brabander
   - Keypad by Mark Stanley & Alexander Brevig

### Step 3 — Open the Sketch

1. In Arduino IDE, go to File → Open.
2. Navigate to the AUTOMAHOME/AUTOMAHOME/ folder.
3. Select AUTOMAHOME.ino and click Open.

### Step 4 — Connect the Hardware

Wire all components to the Arduino Uno according to the Pin Mapping table above. Double-check servo power — if servos cause resets, power them from an external 5V supply with a shared ground to the Arduino.

### Step 5 — Find the I2C Address of Your LCD

If the LCD does not display anything after uploading, your I2C module may use address 0x27 instead of 0x3F. To check:

1. Upload the I2C scanner sketch (available at https://playground.arduino.cc/Main/I2cScanner/).
2. Open Serial Monitor at 9600 baud.
3. Note the address shown and update this line in AUTOMAHOME.ino:

```cpp
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // change 0x3F to your address
```

### Step 6 — Select Board and Port

1. Go to Tools → Board and select Arduino Uno.
2. Go to Tools → Port and select the COM port your Arduino is connected to (e.g. COM3 on Windows, /dev/ttyUSB0 on Linux/Mac).

### Step 7 — Upload

Click the Upload button (→ arrow icon) or press Ctrl + U. Wait for "Done uploading." to appear in the status bar.

### Step 8 — Test the System

Once uploaded, the LCD will show AUTOMAHOME for 1.5 seconds then switch to Enter Password.

---

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



## Troubleshooting

**LCD shows nothing**
The I2C address may be 0x27 instead of 0x3F. Run an I2C scanner sketch to confirm and update the address in the code.

**Servo jitters or Arduino resets**
Servos draw more current than the Arduino's 5V pin can provide. Power them from an external 5V supply with a common ground to the Arduino.

**Rain sensor always reads wet**
Check the sensitivity potentiometer on the rain sensor module and adjust it with a small screwdriver until it correctly distinguishes wet from dry.

**Ultrasonic always reads 999**
Verify TRIG is on pin 13 and ECHO is on A0. Ensure nothing is blocking the sensor face and there are no loose connections.

**Keypad keys not registering**
Confirm row pins (9, 8, 7, 6) and column pins (5, 4, 3, 2) match the physical keypad wiring. Some keypads have rows and columns swapped — try reversing the row/column pin arrays in the code if needed.

---

*AUTOMAHOME — Arduino Intelligent Home Automation System*
