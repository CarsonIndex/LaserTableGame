#include <Servo.h>

// Define the pins for the servo and potentiometer
int servoPin = 2;
int potPin = A0;

// Create a Servo object to control the servo
Servo servo;

void setup() {
  // Attach the servo to the appropriate pin
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(potPin, INPUT);
}

void loop() {
  // Read the value of the potentiometer
  int potValue = analogRead(potPin);
  Serial.println(analogRead(A0));
  // If the potentiometer is turned left, rotate the servo counterclockwise
  if (potValue >= 0 && potValue <= 61) {
   servo.writeMicroseconds(1000);
  }
  else if (potValue >= 62 && potValue <= 161) {
   servo.writeMicroseconds(1100);
  }
  else if (potValue >= 162 && potValue <= 261) {
   servo.writeMicroseconds(1200);
  }
    else if (potValue >= 262 && potValue <= 361) {
   servo.writeMicroseconds(1300);
  }
    else if (potValue >= 362 && potValue <= 461) {
   servo.writeMicroseconds(1400);
  }
    //Dead zone
  else if (potValue >= 462 && potValue <= 561) {
    servo.writeMicroseconds(1500);
  }
    else if (potValue >= 562 && potValue <= 661) {
   servo.writeMicroseconds(1600);
  }
    else if (potValue >= 662 && potValue <= 761) {
   servo.writeMicroseconds(1700);
  }
    else if (potValue >= 762 && potValue <= 861) {
   servo.writeMicroseconds(1800);
  }
    else if (potValue >= 862 && potValue <= 961) {
   servo.writeMicroseconds(1900);
  }
      else if (potValue >= 962 && potValue <= 1023) {
   servo.writeMicroseconds(2000);
  }
  // Wait a short time before reading the potentiometer again
  delay(10);
}
