#include <Arduino.h>
#include "Cuervito.h"
#include <esp_now.h>

#include <Wire.h>

#define bow_motor_pin_a 16
#define bow_motor_pin_b 17
#define aft_motor_pin_a 18
#define aft_motor_pin_b 19
#define portside_pin 34
#define starboard_pin 35

#define CONTROLLER_MAC = "78:21:84:8D:1C:DC"
//#define 

Cuervito corvy(bow_motor_pin_a, bow_motor_pin_b, aft_motor_pin_a, aft_motor_pin_b, portside_pin, starboard_pin);

//  Have to define this here 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

typedef struct comm_data_in {
    int x;
    int y;
    int m;
} comm_data_in;

comm_data_in newData;

void printNewData() {
  Serial.print("joy(");
  Serial.print(newData.x);
  Serial.print(",");
  Serial.print(newData.y);
  Serial.print(") | motor(");
  Serial.print(newData.m);
  Serial.println(")");
}

// Callback when data is received
void OnDataRecv(const unsigned char * mac, const unsigned char * incomingData, int len) {
  memcpy(&newData, incomingData, sizeof(newData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  printNewData();
  corvy.motor_in(newData.m);
  corvy.joy_in(newData.x, newData.y);
}


void setup() {
  Serial.begin(9600);
  pinMode(bow_motor_pin_a, OUTPUT);
  pinMode(bow_motor_pin_b, OUTPUT);
  pinMode(aft_motor_pin_a, OUTPUT);
  pinMode(aft_motor_pin_b, OUTPUT);
//  initComms();
  corvy.calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
}
