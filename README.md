# AUTOMAHOME
## An Intelligent Home System Automation Security with Keypad, Smart Clothes Drying Detection, and Human-sensing Outdoor Lights

---

## Project Overview

AUTOMAHOME is an Arduino-based intelligent home automation system that integrates three independent real-time subsystems into a single embedded platform:

- **Keypad Security & Door Control** — password-protected door access using a 4x4 keypad and servo motor
- **Smart Clothes Drying Detection** — automatic clothes rack control based on rain sensor readings
- **Human-sensing Outdoor Lights** — ultrasonic-based presence detection that controls an outdoor LED

All three subsystems run in parallel without blocking each other, using non-blocking millis() timers throughout.

