/*
  Obstacle Avoidance Car (Arduino UNO + L298N + HC-SR04 + LED/Buzzer)
  ------------------------------------------------------------------
  - Reads distance from HC-SR04 ultrasonic sensor (TRIG/ECHO).
  - Drives two DC motors via L298N H-Bridge driver (EN A/EN B PWM + IN1..IN4 direction).
  - LED + Buzzer provide visual/audible indication when an obstacle is detected.
  - If distance is below a threshold, performs an avoidance maneuver:
      INDICATORS ON -> STOP -> BACKWARD -> TURN RIGHT -> STOP
  - Otherwise, moves forward with indicators OFF.

  NOTE: Comments are in English as required by the assignment.
*/

#define PIN_TRIG 2
#define PIN_ECHO 3

// L298N control pins (must match wiring)
#define ENA 5   // PWM enable for Motor A  -> L298N "EN A"
#define IN1 8
#define IN2 7

#define ENB 6   // PWM enable for Motor B  -> L298N "EN B"
#define IN3 4
#define IN4 9

// Indicators
#define LED_PIN    12
#define BUZZER_PIN 13

// Tunable parameters
const int SPEED_FWD  = 200;   // 0..255
const int SPEED_TURN = 180;
const int SPEED_BACK = 180;

const int THRESHOLD_CM = 20;                 // obstacle distance threshold (cm)
const unsigned long ECHO_TIMEOUT_US = 30000; // 30ms timeout

long readDistanceCm() {
  // Generate a 10us trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(3);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure echo pulse duration (microseconds)
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) return -1; // timeout

  // Convert microseconds to centimeters (approx. duration/58)
  return (long)(duration / 58UL);
}

void indicatorsOn() {
  digitalWrite(LED_PIN, HIGH);
  // Use tone for passive piezo. If you use an active buzzer, digitalWrite HIGH also works.
  tone(BUZZER_PIN, 2000);
}

void indicatorsOff() {
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void forward(int spd) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void backward(int spd) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void turnRight(int spd) {
  // Pivot right: left motor forward, right motor backward
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  indicatorsOff();
  stopMotors();

  Serial.println("READY: distance < 20cm => avoidance + LED/Buzzer ON.");
}

void loop() {
  long cm = readDistanceCm();

  Serial.print("Distance(cm): ");
  Serial.println(cm);

  if (cm > 0 && cm < THRESHOLD_CM) {
    indicatorsOn();

    stopMotors();
    delay(150);

    backward(SPEED_BACK);
    delay(350);

    stopMotors();
    delay(80);

    turnRight(SPEED_TURN);
    delay(350);

    stopMotors();
    delay(80);

    // Keep indicators on briefly, then off
    delay(100);
    indicatorsOff();
  } else {
    indicatorsOff();
    forward(SPEED_FWD);
  }

  delay(60);
}
