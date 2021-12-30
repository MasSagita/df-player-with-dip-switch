/*
   df-player
    
   author: Mas Sagita 2018
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;

//dip pin
//int dipPins[] = {3, 4, 5, 6, 7, 8, 9, 12};
int dipPins[] = {12, 9, 8, 7, 6, 5, 4, 3};
int transAddress;

//potensio for control volume
const int potVol = A0;
int potVolVal = 0;

//indicator led
const int ledPin = 13;

//button pin
const int btnPin = 2;
volatile int btnState = 0;

int ledState = 0;

void setup() {
  Serial.begin(115200);
  
  myDFPlayer.begin(Serial);
  pinMode(btnPin, INPUT);
  
  //interrupt button on pin 2
  attachInterrupt(digitalPinToInterrupt(btnPin), buttonISR, CHANGE);

  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPin, 1);
    delay(100);
    digitalWrite(ledPin, 0);
    delay(200);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(dipPins[i], INPUT);
    //digitalWrite(dipPins[i], HIGH);
  }
  myDFPlayer.setTimeOut(100); //Set serial communictaion time out 100ms
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  digitalWrite(ledPin, LOW);
}

void loop() {

  potVolVal = analogRead(potVol);
  potVolVal = map(potVolVal, 0, 1023, 0, 30);
  transAddress = address();

  myDFPlayer.volume(potVolVal);

  digitalWrite(ledPin, ledState);

  //if(read_button() == switched){
  if (btnState == 1) {
    digitalWrite(ledPin, btnState);
    if (transAddress == 1) {
      myDFPlayer.play(1);
    }
    if (transAddress == 2) {
      myDFPlayer.play(2);
    }
    if (transAddress == 4) {
      myDFPlayer.play(3);
    }
    if (transAddress == 8) {
      myDFPlayer.play(4);
    }
    if (transAddress == 16) {
      myDFPlayer.play(5);
    }
    if (transAddress == 32) {
      myDFPlayer.play(6);
    }
    if (transAddress == 64) {
      myDFPlayer.play(7);
    }
    if (transAddress == 128) {
      myDFPlayer.play(8);
    }
  }

  if (transAddress == 1 || transAddress == 2 || transAddress == 4 || transAddress == 8 || transAddress == 16 || transAddress == 32 || transAddress == 64 || transAddress == 128) {
    ledState = 1;
  }
  else {
    ledState = 0;
  }
}

void buttonISR() {
  btnState = digitalRead(btnPin);
  digitalWrite(ledPin, btnState);
}

//dip read function
byte address() {
  int i, j = 0;
  for (i = 0; i < 8; i++) {
    j = (j << 1) | digitalRead(dipPins[i]);   // read each input pin
  }
  return j;
}
