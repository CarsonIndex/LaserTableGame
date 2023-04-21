#include <Servo.h>

// Define the pins for the servos and potentiometers
int servoPin[] = {13, 12, 11, 9, 8, 7}; // add more pins as needed
int potPin[] = {A0, A1, A2, A3, A4, A5}; // add more pins as needed

int potValue[6] = {0, 0, 0, 0, 0, 0};

//PotValueSum takes the average of 20 readings to lower noise due to the electricity
long potValueSum[6] = {0, 0, 0, 0, 0, 0};

//Define variables to keep track of previous inputs and milliseconds since last input
long previousSum[6] = {0, 0, 0, 0, 0, 0};

unsigned long previousMillis[6] = {0, 0, 0, 0, 0, 0};

bool stopCheck[6] = {false, false, false, false, false, false};

//Change to 300000
const int expireTime = 300000;

// Create servo objects to control the servo
Servo servo[6];

//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------


void setup() {
  // Attach the servos to the appropriate pins
  Serial.begin(9600);
  //Change to 6
  for (int i = 0; i < 6; i++) {
    servo[i].attach(servoPin[i]);
    pinMode(potPin[i], INPUT);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------


void moveServos(int potValue, int j){
    // If the potentiometer is turned left, rotate the servo counterclockwise; vice-versa if the potentiometer is turned right.
    if (potValue >= 0 && potValue <= 61) {
    servo[j].writeMicroseconds(1000);
    }
    else if (potValue >= 62 && potValue <= 161) {
    servo[j].writeMicroseconds(1100);
    }
    else if (potValue >= 162 && potValue <= 261) {
    servo[j].writeMicroseconds(1200);
    }
      else if (potValue >= 262 && potValue <= 361) {
    servo[j].writeMicroseconds(1300);
    }
      else if (potValue >= 362 && potValue <= 461) {
    servo[j].writeMicroseconds(1400);
    }
      //Dead zone
    else if (potValue >= 462 && potValue <= 561) {
      servo[j].writeMicroseconds(1500);
    }
      else if (potValue >= 562 && potValue <= 661) {
    servo[j].writeMicroseconds(1600);
    }
      else if (potValue >= 662 && potValue <= 761) {
    servo[j].writeMicroseconds(1700);
    }
      else if (potValue >= 762 && potValue <= 861) {
    servo[j].writeMicroseconds(1800);
    }
      else if (potValue >= 862 && potValue <= 961) {
    servo[j].writeMicroseconds(1900);
    }
        else if (potValue >= 962 && potValue <= 1023) {
    servo[j].writeMicroseconds(2000);
    }
  }

void servoNoiseReduction(int j) {
  //For loop to take the average of readings to reduce noise
  for (int i = 0; i < 20; i++) {
    potValue[j] = analogRead(potPin[j]);
    potValueSum[j] += potValue[j];
    delay(5);
  }

  //Divide the sum by 20 to take the average
  potValueSum[j] = (potValueSum[j] / 20);

  //If variance is more than 5, set previousMillis
  if (abs(potValueSum[j] - previousSum[j]) > 30) {
    Serial.println(abs(potValueSum[j] - previousSum[j]));
    for (int i = 0; i < 6; i++) {
      previousMillis[i] = millis();
      if (stopCheck[i] == true) {
        stopCheck[i] = false;
      }
    }
    delay(10);
  }
}

void updateMillis(int j) {
    stopCheck[j] = true;
    (stopCheck[0] && stopCheck[1] && stopCheck[2] && stopCheck[3] && stopCheck[4] && stopCheck[5] == true) {
      for (int i = 0; i < 6; i++) {
        servo[i].writeMicroseconds(1500);
      }
    }
}


void beginServo(int j) {
  servoNoiseReduction(j);
  
  //Has more than 5 seconds passed since an input? If so, detach the servo and set stopCheck to true.
  if ((millis()) - previousMillis[j] > expireTime) {
    updateMillis(j);
  }
  //Otherwise, run the servo normally.
  else {
    moveServos(potValueSum[j], j);
    stopCheck[j] = false;
  }

  //Update the sum values after calculating everything
  previousSum[j] = potValueSum[j];
  potValueSum[j] = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------

void loop() {
  beginServo(0);
  delay(10);
  beginServo(1);
  delay(10);
  beginServo(2);
  delay(10);
  beginServo(3);
  delay(10);
  beginServo(4);
  delay(10);
  beginServo(5);
  delay(10);
}
