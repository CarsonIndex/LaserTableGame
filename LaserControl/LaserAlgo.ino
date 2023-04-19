#include <Servo.h>
#define servoPin 4
#define laser1Pin 32
#define laser2Pin 30

#define LASER_PERIOD 8000 // includes laser off time
#define LASER_OFF_TIME 3000 //

Servo testServo;

void moveLaser() {
  static unsigned long lastRun = millis();
  static unsigned long laserOn = millis() + LASER_OFF_TIME;
  static bool whichLaser;
  if (millis() >= laserOn) {
    if (whichLaser) digitalWrite(laser1Pin, HIGH);
    else digitalWrite(laser2Pin, HIGH);
  }
  if (millis() - lastRun < LASER_PERIOD) return;
  lastRun = millis();
  laserOn = millis() + LASER_OFF_TIME;
  digitalWrite(laser1Pin, LOW);
  digitalWrite(laser2Pin, LOW);
  whichLaser = !whichLaser;
  int angle = random(181);
  testServo.write(angle);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(laser1Pin, OUTPUT);
  pinMode(laser2Pin, OUTPUT);
  testServo.attach(servoPin);
  Serial.begin(9060);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveLaser();
}
