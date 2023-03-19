#ifndef Cuervito_h
#define Cuervito_h

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include "Adafruit_BMP3XX.h"

#include "Elevon.h"
#include "Motor.h"

class Cuervito {
public:
  Cuervito(int bow_motor_pin_a, int bow_motor_pin_b, int aft_motor_pin_a, int aft_motor_pin_b, int portside_servo_pin, int starboard_servo_pin);
  Cuervito(int bow_motor_pin, int aft_motor_pin, int portside_servo_pin, int starboard_servo_pin);
  
  void calibrate();
  void dual_prop_calibrate(int start, int stop, int step, int delay_millis);

  void joy_in(int x, int y);
  void motor_in(int m);

private:
  void initializeBMP();
  void readBMP();
  void initializeMPU();
  void readMPU();
  
  
  int bow_motor_pin_a, bow_motor_pin_b, aft_motor_pin_a, aft_motor_pin_b;
  int bow_motor_pin, aft_motor_pin;
  int portside_servo_pin, starboard_servo_pin;
  bool canReverse;
  Motor bow_prop, aft_prop;
  Elevon portside_elevon, starboard_elevon;
  Adafruit_BMP3XX bmp;
  Adafruit_MPU6050 mpu;
};
#endif
