#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor {
public:
  int pin;
  bool canReverse;

  Motor(int p_a, int p_b);
  Motor(int p);
  void set_raw(int power);
  void set(int power, bool debug);
  void set(int power);
  void set(float power);
  void stop();

  void calibrate(int start, int stop, int step, int delay_millis);
  void calibrate();
};
#endif
