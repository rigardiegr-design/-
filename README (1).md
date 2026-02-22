# Arduino Obstacle Avoidance Car (UNO + L298N + HC-SR04 + LED/Buzzer)

## Components
- Arduino UNO
- L298N motor driver (H-Bridge)
- HC-SR04 ultrasonic sensor
- 2x DC motors + chassis
- LED indicator + 220Ω resistor
- Buzzer (piezo)
- Power: 5V logic + Vmotor for motors (common GND)

## Pin Mapping
### HC-SR04
- TRIG -> D2
- ECHO -> D3
- VCC -> 5V
- GND -> GND

### L298N
- EN A -> D5 (PWM)
- IN1 -> D8
- IN2 -> D7
- EN B -> D6 (PWM)
- IN3 -> D4
- IN4 -> D9
- 5V (logic) -> 5V Arduino
- GND -> Arduino GND (common)
- Vmotor/Vs -> Battery + (motor supply)

### Indicators
- LED: D12 -> LED -> 220Ω -> GND
- Buzzer: pin 1 -> GND, pin 2 -> D13

## Behavior
- If distance < 20 cm: LED+BUZZER ON, STOP -> BACKWARD -> TURN RIGHT -> STOP
- Else: LED+BUZZER OFF, move forward

## How to run
1. Open sketch.ino in Arduino IDE
2. Board: Arduino Uno
3. Upload
4. Serial Monitor @ 9600 for distance readings
