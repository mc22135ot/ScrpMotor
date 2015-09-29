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
  slave.addCMD(255, safeOperation);
}

void loop(){
  slave.check();
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
  }else if(0 < rx_data){
    digitalWrite(MTR1_RVS, LOW);
    analogWrite(MTR1_FWD, rx_data);
    digitalWrite(MTR1_LED, HIGH);
  }else{
    digitalWrite(MTR1_FWD, LOW);
    analogWrite(MTR1_RVS, -rx_data);
    digitalWrite(MTR1_LED, HIGH);
  }
}

boolean driveMtr2(int rx_data, int& tx_data){
  rx_data = constrain(rx_data, -255, 255);
  if(!rx_data){
    digitalWrite(MTR2_FWD, LOW);
    digitalWrite(MTR2_RVS, LOW);
    digitalWrite(MTR2_LED, LOW);
  }else if(0 < rx_data){
    digitalWrite(MTR2_RVS, LOW);
    analogWrite(MTR2_FWD, rx_data);
    digitalWrite(MTR2_LED, HIGH);
  }else{
    digitalWrite(MTR2_FWD, LOW);
    analogWrite(MTR2_RVS, -rx_data);
    digitalWrite(MTR2_LED, HIGH);
  }
}
