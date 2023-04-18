#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include "Adafruit_LEDBackpack.h"
#include<stdio.h>
#include <string.h>

//Defines pin number for respective parts
#define leftPot1 A0
#define leftPot2 A1
#define leftPot3 A2
#define rightPot3 A3
#define rightPot2 A4
#define rightPot1 A5

#define speakerPin 2
#define leftPhotoPin A6
#define rightPhotoPin A7
#define laserPin 30

int leftTeamScore;
int rightTeamScore;
int leftTeamScoreB;
int rightTeamScoreB;
unsigned long previousMillis = 0; // variable to store the last time "x" was read

//Declares servo objects
Servo leftRow1;
Servo leftRow2;
Servo leftRow3;
Servo laserServo;
Servo rightRow3;
Servo rightRow2;
Servo rightRow1;

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
  leftRow1.attach(13);
  leftRow2.attach(12);
  leftRow3.attach(11);
  laserServo.attach(10);
  rightRow3.attach(9);
  rightRow2.attach(8);
  rightRow1.attach(7);

  //Declares potentiometer pins as inputs
  pinMode(leftPot1, INPUT);
  pinMode(leftPot2, INPUT);
  pinMode(leftPot3, INPUT);
  pinMode(rightPot3, INPUT);
  pinMode(rightPot2, INPUT);
  pinMode(rightPot1, INPUT);

  //Declares photoresistor and buzzer pins
  pinMode(leftPhotoPin, INPUT);
  pinMode(rightPhotoPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(laserPin, OUTPUT);

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

void moveServos(){
  //Reads potentiometer values
  int leftLoc1 = analogRead(leftPot1);
  int leftLoc2 = analogRead(leftPot2);
  int leftLoc3 = analogRead(leftPot3);
  int rightLoc3 = analogRead(rightPot3);
  int rightLoc2 = analogRead(rightPot2);
  int rightLoc1 = analogRead(rightPot1);

  //Maps analog values to degrees
  leftLoc1 = map(leftLoc1, 0, 1023, 0 , 180);
  leftLoc2 = map(leftLoc2, 0, 1023, 0 , 180);
  leftLoc3 = map(leftLoc3, 0, 1023, 0 , 180);
  rightLoc3 = map(rightLoc3, 0, 1023, 0 , 180);
  rightLoc2 = map(rightLoc2, 0, 1023, 0 , 180);
  rightLoc1 = map(rightLoc1, 0, 1023, 0 , 180);

  //Moves Servos to new locations
  leftRow1.write(leftLoc1);
  leftRow2.write(leftLoc2);
  leftRow3.write(leftLoc3);
  rightRow3.write(rightLoc3);
  rightRow2.write(rightLoc2);
  rightRow1.write(rightLoc1);
}

//Moves laser for gameplay
//TODO: Create better algorithm for laser movement
void moveLaser(){
  int laserLoc = rand() % 181;
  laserServo.write(laserLoc);
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

void controlLaser(){
  if(Serial.read() == 'x')
    {
      digitalWrite(laserPin, HIGH);      
    }
  if(Serial.read() == 'v')
    {
      digitalWrite(laserPin, LOW);
    }
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

void loop() {
  //Turn off display and reset score if "x" is not read after 5 minutes
  if ((millis() - previousMillis) >= 300000) {
    resetDisplay();
  }
  //Turn display back on if "x" is read after the display is turned off
  if (Serial.read() == 'x') {
    if ((millis() - previousMillis) >= 300000) {
      setDisplay();
  }
   //Increment the score if the display is on and "x" is read
    else {
      incrementScore("left");
    }
    // update previousMillis to current time
    previousMillis = millis();
  }
  //moveServos()
  //moveLaser();
  //checkLight();
  //setDisplay();
  //countDown();
  //controlLaser();
}
