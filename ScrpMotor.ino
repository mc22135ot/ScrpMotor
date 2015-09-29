#include <Utility.h>
#include <EEPROM.h>
#include <ScrpSlave.h>
#include "config.h"

ScrpSlave slave(REDE_PIN, EEPROM.read(0), changeID);

void setup(){
  pinMode(REDE_PIN, OUTPUT);
  pinMode(MTR1_FWD, OUTPUT);
  pinMode(MTR1_RVS, OUTPUT);
  pinMode(MTR2_FWD, OUTPUT);
  pinMode(MTR2_RVS, OUTPUT);
  pinMode(MTR1_LED, OUTPUT);
  pinMode(MTR2_LED, OUTPUT);
  setPWMFrequency(5, PWM_DIV8);
  setPWMFrequency(6, PWM_DIV8);
  setPWMFrequency(9, PWM_DIV8);
  setPWMFrequency(10, PWM_DIV8);
  setPWMFrequency(3, PWM_DIV8);
  setPWMFrequency(11, PWM_DIV8);
  Serial.begin(BAUDRATE);
  slave.addCMD(2, driveMtr1);
  slave.addCMD(3, driveMtr2);
  slave.addCMD(4, brakeMtr1);
  slave.addCMD(5, brakeMtr2);
  slave.addCMD(255, safeOperation);
}

boolean mtr1brake = false;
boolean mtr2brake = false;
boolean mtrbrake  = false;
byte mtr1speed = 0;
byte mtr2speed = 0;
boolean mtr1rotation = true;
boolean mtr2rotation = true;

unsigned long prev_time = millis();
void loop(){
  slave.check();
  if(mtrbrake){
    unsigned long now_time = millis();
    if(20 <= now_time-prev_time){
      if(mtr1brake){
        mtr1speed = (1<=mtr1speed)?mtr1speed-1:0;
        if(!mtr1speed){
          mtr1brake = false;
          digitalWrite(MTR1_LED, LOW);
        }
        if(mtr1rotation){
          digitalWrite(MTR1_RVS, LOW);
          analogWrite(MTR1_FWD, mtr1speed);
        }else{
          digitalWrite(MTR1_FWD, LOW);
          analogWrite(MTR1_RVS, mtr1speed);
        }
      }
      if(mtr2brake){
        mtr2speed = (1<=mtr2speed)?mtr2speed-1:0;
        if(!mtr2speed){
          mtr2brake = false;
          digitalWrite(MTR2_LED, LOW);
        }
        if(mtr2rotation){
          digitalWrite(MTR2_RVS, LOW);
          analogWrite(MTR2_FWD, mtr2speed);
        }else{
          digitalWrite(MTR2_FWD, LOW);
          analogWrite(MTR2_RVS, mtr2speed);
        }
      }
      mtrbrake = mtr1brake || mtr2brake;
      prev_time = now_time;
    } 
  }
}

void changeID(byte new_id){
  EEPROM.write(0, new_id);
}

boolean safeOperation(int rx_data, int& tx_data){
  digitalWrite(MTR1_FWD, LOW);
  digitalWrite(MTR1_RVS, LOW);
  digitalWrite(MTR2_FWD, LOW);
  digitalWrite(MTR2_RVS, LOW);
  digitalWrite(MTR1_LED, LOW);
  digitalWrite(MTR2_LED, LOW);
  return true;
}

boolean driveMtr1(int rx_data, int& tx_data){
  rx_data = constrain(rx_data, -255, 255);
  if(!rx_data){
    digitalWrite(MTR1_FWD, LOW);
    digitalWrite(MTR1_RVS, LOW);
    digitalWrite(MTR1_LED, LOW);
    mtr1speed = 0;
  }else if(0 < rx_data){
    digitalWrite(MTR1_RVS, LOW);
    analogWrite(MTR1_FWD, rx_data);
    digitalWrite(MTR1_LED, HIGH);
    mtr1speed = rx_data;
    mtr1rotation = true;
  }else{
    digitalWrite(MTR1_FWD, LOW);
    analogWrite(MTR1_RVS, -rx_data);
    digitalWrite(MTR1_LED, HIGH);
    mtr1speed = -rx_data;
    mtr1rotation = false;
  }
}

boolean driveMtr2(int rx_data, int& tx_data){
  rx_data = constrain(rx_data, -255, 255);
  if(!rx_data){
    digitalWrite(MTR2_FWD, LOW);
    digitalWrite(MTR2_RVS, LOW);
    digitalWrite(MTR2_LED, LOW);
    mtr2speed = 0;
  }else if(0 < rx_data){
    digitalWrite(MTR2_RVS, LOW);
    analogWrite(MTR2_FWD, rx_data);
    digitalWrite(MTR2_LED, HIGH);
    mtr2speed = rx_data;
    mtr2rotation = true;
  }else{
    digitalWrite(MTR2_FWD, LOW);
    analogWrite(MTR2_RVS, -rx_data);
    digitalWrite(MTR2_LED, HIGH);
    mtr2speed = -rx_data;
    mtr2rotation = false;
  }
}

boolean brakeMtr1(int rx_data, int& tx_data){
  if(!mtr1speed) return false;
  mtrbrake = true;
  mtr1brake = true;
}

boolean brakeMtr2(int rx_data, int& tx_data){
  if(!mtr2speed) return false;
  mtrbrake = true;
  mtr2brake = true;
}

