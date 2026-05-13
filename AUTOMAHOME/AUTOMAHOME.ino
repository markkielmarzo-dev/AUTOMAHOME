#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

Servo doorServo;
Servo rainServo;

const String CORRECT_PASSWORD = "1234";
String inputBuffer = "";

const int RAIN_PIN  = 12;
const int TRIG_PIN  = 13;
const int ECHO_PIN  = A0;
const int LED_PIN   = A1;

const int DETECT_DISTANCE = 100;

bool doorOpen = false;
unsigned long doorOpenTime = 0;
const unsigned long DOOR_OPEN_DURATION = 5000;

bool msgShowing = false;
unsigned long msgStartTime = 0;
unsigned long msgDuration  = 0;

#define MSG_NONE      0
#define MSG_DOOR_OPEN 2

int pendingMsg = MSG_NONE;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool lastRainState = false;

int wrongAttempts = 0;
const int MAX_ATTEMPTS = 3;
bool lockedOut = false;
unsigned long lockoutStart = 0;
const unsigned long LOCKOUT_DURATION = 10000;

void showLCD(String line1, String line2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  if (line2.length() > 0) {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}

void startMsg(String line1, String line2, unsigned long duration, int next = MSG_NONE) {
  showLCD(line1, line2);
  msgShowing   = true;
  msgStartTime = millis();
  msgDuration  = duration;
  pendingMsg   = next;
}

int readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;
  return (int)(duration * 0.034 / 2);
}

void setup() {
  lcd.init();
  lcd.backlight();

  doorServo.attach(10);
  rainServo.attach(11);
  doorServo.write(0);
  rainServo.write(0);

  pinMode(RAIN_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN,  OUTPUT);
  digitalWrite(LED_PIN, LOW);

  showLCD("  AUTOMAHOME  ", "Enter Password");
  delay(1500);
  showLCD("Enter Password");
}

void loop() {

  unsigned long now = millis();

  // Rain sensor
  bool raining = (digitalRead(RAIN_PIN) == LOW);
  if (raining && !lastRainState) {
    rainServo.write(90);
    lastRainState = true;
    if (!msgShowing) startMsg("Rain Detected!", "Clothes covered", 2000);
  } else if (!raining && lastRainState) {
    rainServo.write(0);
    lastRainState = false;
    if (!msgShowing) startMsg("Rain stopped", "Rack opened", 2000);
  }

  // Ultrasonic + LED
  int distance = readDistance();
  digitalWrite(LED_PIN, (distance > 0 && distance <= DETECT_DISTANCE) ? HIGH : LOW);

  // LCD message timer
  if (msgShowing && (now - msgStartTime >= msgDuration)) {
    msgShowing = false;
    if (pendingMsg == MSG_DOOR_OPEN) {
      showLCD("Door Open");
      pendingMsg = MSG_NONE;
    } else {
      if (!doorOpen) showLCD("Enter Password");
    }
  }

  // Door auto-lock
  if (doorOpen && (now - doorOpenTime >= DOOR_OPEN_DURATION)) {
    doorServo.write(0);
    doorOpen     = false;
    msgShowing   = true;
    msgStartTime = now;
    msgDuration  = 1200;
    pendingMsg   = MSG_NONE;
    showLCD("Door Locked");
  }

  // Lockout timer
  if (lockedOut && (now - lockoutStart >= LOCKOUT_DURATION)) {
    lockedOut     = false;
    wrongAttempts = 0;
    showLCD("Enter Password");
  }

  // Keypad
  if (!lockedOut) {
    char key = keypad.getKey();

    if (key) {

      if (key == 'D') {
        doorServo.write(0);
        doorOpen    = false;
        inputBuffer = "";
        startMsg("Door Locked", "", 1200);
        return;
      }

      if (key == '*') {
        if (inputBuffer.length() > 0) {
          inputBuffer.remove(inputBuffer.length() - 1);
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          for (int i = 0; i < (int)inputBuffer.length(); i++) lcd.print('*');
        }
        return;
      }

      if (key == '#') {
        if (inputBuffer.length() == 0) return;

        if (inputBuffer == CORRECT_PASSWORD) {
          wrongAttempts = 0;
          doorServo.write(90);
          doorOpen     = true;
          doorOpenTime = now;
          inputBuffer  = "";
          startMsg("Access Granted!", "Door Opening...", 1500, MSG_DOOR_OPEN);
        } else {
          wrongAttempts++;
          inputBuffer = "";
          lcd.setCursor(0, 1);
          lcd.print("                ");

          if (wrongAttempts >= MAX_ATTEMPTS) {
            lockedOut    = true;
            lockoutStart = now;
            startMsg("LOCKED OUT", "Wait 10 seconds", 10000);
          } else {
            startMsg("Wrong Password", "Tries left: " + String(MAX_ATTEMPTS - wrongAttempts), 1500);
          }
        }
        return;
      }

      if (key >= '0' && key <= '9') {
        if (inputBuffer.length() < 8) {
          inputBuffer += key;
          lcd.setCursor(inputBuffer.length() - 1, 1);
          lcd.print('*');
        }
      }
    }
  }
}
