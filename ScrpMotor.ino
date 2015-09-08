#include <EEPROM.h>
#include <ScrpSlave.h>
#include <digitalWriteFast.h>

#define REDE_PIN 9

#define MTR1_FWD 6
#define MTR1_RVS 5
#define MTR2_FWD 10
#define MTR2_RVS 11

#define LED1     2
#define LED2     13

ScrpSlave<REDE_PIN> slave(EEPROM.read(0), changeID);

void setup(){
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  slave.addCMD(2, driveMtr1);
  slave.addCMD(3, driveMtr2);
}

void loop(){
  slave.check();
}

void changeID(byte new_id){
  EEPROM.write(0, new_id);
}

boolean safeOperation(int rx_data, int& tx_data){
  digitalWriteFast(MTR1_FWD, LOW);
  digitalWriteFast(MTR1_RVS, LOW);
  digitalWriteFast(MTR2_FWD, LOW);
  digitalWriteFast(MTR2_RVS, LOW);
  return true;
}

boolean driveMtr1(int rx_data, int& tx_data){
  rx_data = constrain(rx_data, -255, 255);
  if(!rx_data){
    digitalWriteFast(MTR1_FWD, LOW);
    digitalWriteFast(MTR1_RVS, LOW);
    digitalWriteFast(LED1, LOW);
  }else if(0 < rx_data){
    digitalWriteFast(MTR1_RVS, LOW);
    analogWrite(MTR1_FWD, rx_data);
    digitalWriteFast(LED1, HIGH);
  }else{
    digitalWriteFast(MTR1_FWD, LOW);
    analogWrite(MTR1_RVS, -rx_data);
    digitalWriteFast(LED1, HIGH);
  }
}

boolean driveMtr2(int rx_data, int& tx_data){
  rx_data = constrain(rx_data, -255, 255);
  if(!rx_data){
    digitalWriteFast(MTR2_FWD, LOW);
    digitalWriteFast(MTR2_RVS, LOW);
    digitalWriteFast(LED2, LOW);
  }else if(0 < rx_data){
    digitalWriteFast(MTR2_RVS, LOW);
    analogWrite(MTR1_FWD, rx_data);
    digitalWriteFast(LED2, HIGH);
  }else{
    digitalWriteFast(MTR2_FWD, LOW);
    analogWrite(MTR1_RVS, -rx_data);
    digitalWriteFast(LED2, HIGH);
  }
}

