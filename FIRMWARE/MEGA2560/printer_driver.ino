#include <Arduino.h>

#include <AccelStepper.h>
#include <MultiStepper.h>
#include "xaar128.h"



#define commandSize 128
#define printStartPos 3630


//RLE && COMMAND PROCESSING VARIABLES//
int nozzleRLE[128][20];
char command[commandSize];
int serialCount;
byte cA;
byte cAn;
byte ncPos[128];
int nCount[128];

byte nozzleNum;
byte nComb1[8];
byte nComb2[8];
char temp[3];
bool hasData;
float xPos;
//MOTOR CONTROL VARIABLES//
AccelStepper stepperY(4, 40, 41, 42, 43);
AccelStepper stepperBuild(4, 44, 45, 46, 47);
AccelStepper stepperFeed(1, 22, 23);
AccelStepper xServo(1, 24, 25);
MultiStepper steppers;
long positions[4];

Xaar128 xaar128;



void setup() {

  //MOTOR INIT//
  pinMode(26, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  stepperBuild.setMaxSpeed(250);
  stepperBuild.setAcceleration(50);

  // stepperBuild.setSpeed(200);
  stepperFeed.setMaxSpeed(250);
  stepperFeed.setAcceleration(50);
  xServo.setMaxSpeed(8000);
  xServo.setAcceleration(500000);
  stepperY.setAcceleration(1000);
  stepperY.setMaxSpeed(300);
  digitalWrite(9, LOW);
  digitalWrite(6, HIGH);
  steppers.addStepper(stepperBuild);
  steppers.addStepper(stepperFeed);
  steppers.addStepper(xServo);

  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;
  positions[3] = 0;



  // put your setup code here, to run once:
  Serial.begin(250000);

  //HEAD INIT//
  xaar128.init();
  TCCR1A = _BV(COM1A0);              // toggle OC1A on compare match
  OCR1A = 7;                         // top value for counter
  // UNO //TCCR1B = _BV(WGM12) | _BV(CS10);   // CTC mode, prescaler clock/1
  TCCR1B = _BV(WGM12) | _BV(CS10);   // CTC mode, prescaler clock/1

}

void loop() {

  if (stepperBuild.currentPosition() != positions[0]) {
    digitalWrite(26, HIGH);
    delay(1000);
  }
  else {
    digitalWrite(26, LOW);
  }
  if (stepperFeed.currentPosition() != positions[1]) {
    digitalWrite(6, LOW);
    delay(1000);
  }
  else {
    digitalWrite(6, HIGH);
  }

  char c;
  if (Serial.available()) {
    c = Serial.read();

    if (c != '\n') {
      command[serialCount] = c;
      serialCount++;
    } else {
      processCommand(command);
      serialCount = 0;
      memset(&command[0], 0, sizeof(command));
    }

  }

}












