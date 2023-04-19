#include <Servo.h>

// Define the pins for the servo and potentiometer
int servoPin = 2;
int servoPinB = 4;
int potPin = A0;
//int potPinB = A2;
int potValue = 0;
//PotValueSum takes the average of 20 readings to lower noise due to the electricity
long potValueSum = 0;
//Define variables to keep track of previous inputs and milliseconds since last input
long previousSum = 0;
unsigned long previousMillis = 0;
bool stopCheck = false;
const int expireTime = 300000;

// Create a Servo object to control the servo
Servo servo;

void setup() {
  // Attach the servo to the appropriate pin
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(potPin, INPUT);
}

void moveServos(int potValue){
    // If the potentiometer is turned left, rotate the servo counterclockwise; vice-versa if the potentiometer is turned right.
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
      servo.detach();
      stopCheck = true;
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
  }

void beginServo() {
  //For loop to take the average of readings to reduce noise
  for (int i = 0; i < 20; i++) {
    potValue = analogRead(potPin);
    potValueSum += potValue;
    delay(5);
  }

  //Divide the sum by 20 to take the average
  potValueSum = (potValueSum / 20);

  //If variance is more than 5, set previousMillis
  if (abs(potValueSum - previousSum) > 10) {
    previousMillis = millis();
    //Re-attach the servo if it is currently detached
    if (stopCheck == true) {
      servo.attach(2);
      stopCheck = false;
    }
    delay(10);
  }

  //Has more than 5 seconds passed since an input? If so, detach the servo and set stopCheck to true.
  if ((millis()) - previousMillis > expireTime) {
    servo.detach();
    stopCheck = true;
  }

  //Otherwise, run the servo normally.
  else {
    moveServos(potValueSum);
  }

  //Update the sum values after calculating everything
  previousSum = potValueSum;
  potValueSum = 0;
}

void loop() {
  beginServo();
}
