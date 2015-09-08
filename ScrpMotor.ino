#include <EEPROM.h>
#include <ScrpSlave.h>
#include <digitalWriteFast.h>

#define REDE_PIN 2

#define MTR1_FWD 5
#define MTR1_RVS 6
#define MTR2_FWD 5
#define MTR2_RVS 6

ScrpSlave<REDE_PIN> slave(Serial, 115200, EEPROM.read(0), changeID);

void setup(){
  slave.addCMD(2, driveMtr1);
  slave.addCMD(3, driveMtr2);;
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
  }else if(0 < rx_data){
    digitalWriteFast(MTR1_RVS, LOW);
    analogWrite(MTR1_FWD, rx_data);
  }else{
    digitalWriteFast(MTR1_FWD, LOW);
    analogWrite(MTR1_RVS, -rx_data);
  }
}

boolean driveMtr2(int rx_data, int& tx_data){
  rx_data = constrain(rx_data, -255, 255);
  if(!rx_data){
    digitalWriteFast(MTR2_FWD, LOW);
    digitalWriteFast(MTR2_RVS, LOW);
  }else if(0 < rx_data){
    digitalWriteFast(MTR2_RVS, LOW);
    analogWrite(MTR1_FWD, rx_data);
  }else{
    digitalWriteFast(MTR2_FWD, LOW);
    analogWrite(MTR1_RVS, -rx_data);
  }
}

