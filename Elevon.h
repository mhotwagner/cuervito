#ifndef Elevon_h
#define Elevon_h

#include <Arduino.h>
#include <Servo.h>


class Elevon {
public:
  String name;
  Servo servo;
  int dir;

  Elevon(int servo_pin, String n, int d);
  void set(int us, bool debug);
  void set(int us);
  void calibrate(int step, int delay_millis);
  void calibrate();
};
#endif
