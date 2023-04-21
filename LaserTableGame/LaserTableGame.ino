#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include "Adafruit_LEDBackpack.h"
#include <stdio.h>
#include <string.h>



// miscellaneous pins
#define speakerPin 2
#define leftPhotoPin A6
#define rightPhotoPin A7
#define laser1Pin 30
#define laser2Pin 32

// constants
#define LASER_PERIOD 8000 // includes laser off time
#define LASER_OFF_TIME 3000 // ms laser is turned off when moved

int leftTeamScore;
int rightTeamScore;
int leftTeamScoreB;
int rightTeamScoreB;
unsigned long previousMillis_B = 0; // variable to store the last time "x" was read


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

//Declares LED Matrices
Adafruit_IS31FL3731_Wing redMatrix = Adafruit_IS31FL3731_Wing();
Adafruit_IS31FL3731_Wing blueMatrix = Adafruit_IS31FL3731_Wing();

//Declares AlphNum Displays
Adafruit_AlphaNum4 redDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 blueDisplay = Adafruit_AlphaNum4();

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //Attaches servo motors to respective pins
  //Declares potentiometer pins as inputs
  for (int i = 0; i < 6; i++) {
    servo[i].attach(servoPin[i]);
    pinMode(potPin[i], INPUT);
  }

  //Declares photoresistor and buzzer pins
  pinMode(leftPhotoPin, INPUT);
  pinMode(rightPhotoPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(laser1Pin, OUTPUT);
  pinMode(laser2Pin, OUTPUT);

  //Instantiates LED Matrices
  redMatrix.begin(0x77);
  blueMatrix.begin();

  //Instantiates AlphaNum Displays
  redDisplay.begin(0x70);
  //Will need to change address after soldering address bridge pin
  blueDisplay.begin(0x71);
  setDisplay();
  leftTeamScore = 0;
  rightTeamScore = 0;
  leftTeamScoreB = 0;
  rightTeamScoreB = 0;
}

//Moves laser for gameplay
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
  laserServo.write(angle);
}

//Checks if light is over threshold
//Will need to change a little bit for actual photoresistors
void checkLight(){
  int leftLevel = analogRead(leftPhotoPin);
  int rightLevel = analogRead(rightPhotoPin);
  Serial.print(leftLevel);
  Serial.print(" ");
  Serial.println(rightLevel);
  if(leftLevel < 511){
    celebrationSequence("left");
  }
  if(rightLevel < 511){
    celebrationSequence("right");
  }
}

//Calls relevant celebration functions
void celebrationSequence(String teamName){
  incrementScore(teamName);
  playSound();
  ledAnimation(teamName);
}

//Increments score on Display
//TODO: Write code to change score on Display.
//Check if Team Score > 10
void incrementScore(String teamName){
  if(teamName == "left")
    {
      //Single digit incrementing, 0-9
      if (leftTeamScore < 9) {
      //Increases Score Integer for left/red team
      leftTeamScore++;
      //Sets all values of red AlphaNum Display to new values and writes to display
      redDisplay.writeDigitAscii(0, '1');
      redDisplay.writeDigitAscii(1, 'P');
      redDisplay.writeDigitAscii(2, '0');
      redDisplay.writeDigitAscii(3, '0' + leftTeamScore);
      redDisplay.writeDisplay();
      delay(100);
      }
      //Resets the score if it reaches 100
      else if (leftTeamScore == 99) {
        leftTeamScore = 0;
        leftTeamScoreB = 0;
        //Sets all values of red AlphaNum Display to new values and writes to display
        redDisplay.writeDigitAscii(0, '1');
        redDisplay.writeDigitAscii(1, 'P');
        redDisplay.writeDigitAscii(2, '0');
        redDisplay.writeDigitAscii(3, '0');
        redDisplay.writeDisplay();
        delay(100);        
      }
      //Incrementation for 10-99
      else {
        leftTeamScore++;
        //leftTeamScoreB is for the tenths place. Increment it if the score reaches 10, 20, 30, etc.
        if (leftTeamScore % 10 == 0) {
          leftTeamScoreB++;
        }
        //Sets all values of red AlphaNum Display to new values and writes to display
        redDisplay.writeDigitAscii(0, '1');
        redDisplay.writeDigitAscii(1, 'P');
        redDisplay.writeDigitAscii(2, '0' + leftTeamScoreB);
        //Divide rightTeamScore (ones place) by 10 and take the remainder. Ex: If the score is 15, it will output the remainder of 5.
        redDisplay.writeDigitAscii(3, '0' + (leftTeamScore % 10));
        redDisplay.writeDisplay();
        delay(100);
      }
    }
  if(teamName == "right")
    {
      //Single digit output, 0-9
      if (rightTeamScore < 9) {
      //Increases Score Integer for left/red team
      rightTeamScore++;
      //Sets all values of red AlphaNum Display to new values and writes to display
      redDisplay.writeDigitAscii(0, '2');
      redDisplay.writeDigitAscii(1, 'P');
      redDisplay.writeDigitAscii(2, '0');
      redDisplay.writeDigitAscii(3, '0' + rightTeamScore);
      redDisplay.writeDisplay();
      delay(100);
      }
      //Resets score to 0 if score reaches 100  
      else if (rightTeamScore == 99) {
        rightTeamScore = 0;
        rightTeamScoreB = 0;
        //Sets all values of red AlphaNum Display to new values and writes to display
        redDisplay.writeDigitAscii(0, '2');
        redDisplay.writeDigitAscii(1, 'P');
        redDisplay.writeDigitAscii(2, '0');
        redDisplay.writeDigitAscii(3, '0');
        redDisplay.writeDisplay();
        delay(100);        
      }
      //Double digit display
      else {
        //Increment the score  
        rightTeamScore++;
        //If the score is 10, 20, 30, etc, increment rightTeamSCoreB, which is the tenths digit
        if (rightTeamScore % 10 == 0) {
          rightTeamScoreB++;
        }
        //Sets all values of red AlphaNum Display to new values and writes to display
        redDisplay.writeDigitAscii(0, '2');
        redDisplay.writeDigitAscii(1, 'P');
        redDisplay.writeDigitAscii(2, '0' + rightTeamScoreB);
        //Divide rightTeamScore (ones place) by 10 and take the remainder. Ex: If the score is 15, it will output the remainder of 5.
        redDisplay.writeDigitAscii(3, '0' + (rightTeamScore % 10));
        redDisplay.writeDisplay();
        delay(100);
      }
    }
}

void setDisplay(){
  redDisplay.writeDigitAscii(0, '1');
  redDisplay.writeDigitAscii(1, 'P');
  redDisplay.writeDigitAscii(2, '0');
  redDisplay.writeDigitAscii(3, '0' + leftTeamScore);
  redDisplay.writeDisplay();
  blueDisplay.writeDigitAscii(0, '2');
  blueDisplay.writeDigitAscii(1, 'P');
  blueDisplay.writeDigitAscii(2, '0');
  blueDisplay.writeDigitAscii(3, '0' + rightTeamScore);
  blueDisplay.writeDisplay();
  //delay(100);


}

//Turn off the display
void resetDisplay(){
  //Reset the score
  leftTeamScore = 0;
  leftTeamScoreB = 0;
  redDisplay.writeDigitAscii(0, ' ');
  redDisplay.writeDigitAscii(1, ' ');
  redDisplay.writeDigitAscii(2, ' ');
  redDisplay.writeDigitAscii(3, ' ');
  redDisplay.writeDisplay();
  blueDisplay.writeDigitAscii(0, ' ');
  blueDisplay.writeDigitAscii(1, ' ');
  blueDisplay.writeDigitAscii(2, ' ');
  blueDisplay.writeDigitAscii(3, ' ');
  blueDisplay.writeDisplay();
  //delay(100);
}

//Flashes LEDS
//Write code to make leds write out which team scores
void ledAnimation(String teamName){
  if(teamName == "red")
    {
      //Sets parameters of red LED board
      redMatrix.setTextSize(1);
      redMatrix.setTextWrap(false);
      redMatrix.setTextColor(100);
      //Prints Red Scored and scrolls across Screen
      for (int8_t x=0; x>=-32; x--){
        redMatrix.clear();
        redMatrix.setCursor(x,0);
        redMatrix.print("Red Scored");
        delay(100);
      }
      delay(20);
    }
  else
    {
      //Sets parameters of blue LED board
      blueMatrix.setTextSize(1);
      blueMatrix.setTextWrap(false);
      blueMatrix.setTextColor(100);
      //Prints Blue Scored and scrolls across Screen
      for (int8_t x=0; x>=-32; x--){
        blueMatrix.clear();
        blueMatrix.setCursor(x,0);
        blueMatrix.print("Blue Scored");
        delay(100);
      }
      delay(20);      
    }
}

//Flashes entire led matrices
void flashLights(String teamName){
  if(teamName == "red"){
      //redTeam full matrix flash
      for(int i = 0; i < 3; i++)
        {
          redMatrix.drawRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          redMatrix.fillRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          delay(500);
          redMatrix.clear();
          delay(500);
        }
    }
  else if(teamName == "blue"){

    //blueTeam full matrix flash
    for(int i = 0; i < 3; i++)
        {
          blueMatrix.drawRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          blueMatrix.fillRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          delay(500);
          blueMatrix.clear();
          delay(500);
        }
  }
  else if(teamName == "both"){

    //Both sides full matrix flash
    for(int i = 0; i < 3; i++)
      {
          redMatrix.drawRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          redMatrix.fillRect(0,0, redMatrix.width(), redMatrix.height(), 255);
          blueMatrix.drawRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          blueMatrix.fillRect(0,0, blueMatrix.width(), blueMatrix.height(), 255);
          delay(500);
          redMatrix.clear();
          blueMatrix.clear();
          delay(500);
      }
  }
      
}

//Counts down from 3 on led matrices
void countDown(){
    //Sets parameters for led matrices
    blueMatrix.setTextSize(1);
    blueMatrix.setTextWrap(false);
    blueMatrix.setTextColor(100);
    redMatrix.setTextSize(1);
    redMatrix.setTextWrap(false);
    redMatrix.setTextColor(100);

    //Flashes countdown on led matrices
    for(int i = 3; i>= 0; i--)
      {
        redMatrix.setCursor(5,0);
        blueMatrix.setCursor(5,0);
        String intString = String(i);
        redMatrix.print(intString);
        blueMatrix.print(intString);
        delay(500);
        redMatrix.clear();
        blueMatrix.clear();
        delay(500);
      }
    
}
//Plays celebration sound
//TODO possibly create more sounds, or a game winning sound
void playSound(){
  //Declares notes and durations
  int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int durations[] = {100, 100, 100, 100, 100, 100, 100, 100};
  //Plays notes for declared durations
  for (int i = 0; i < 8; i++) {
    tone(speakerPin, notes[i]);
    delay(durations[i]);
  }
  // Stop playing the tune
  noTone(speakerPin);
  delay(3000);
}


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
    if (stopCheck[0] && stopCheck[1] && stopCheck[2] && stopCheck[3] && stopCheck[4] && stopCheck[5] == true) {
      setDisplay();
    }
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
    if (stopCheck[0] && stopCheck[1] && stopCheck[2] && stopCheck[3] && stopCheck[4] && stopCheck[5] == true) {
      resetDisplay();
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

void callServos() {
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
}


void loop() {
  //callServos();
  //moveLaser();
  //checkLight();

  //I don't think we need this function
  //countDown();
}
