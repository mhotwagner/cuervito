#include <Arduino.h>
#include "Motor.h"

Motor::Motor(int p) {
  pin = p;
}

Motor::Motor(int p_a, int p_b) {
  // set pin a to negative
  analogWrite(p_a, 0);
  // set pin to pin_b
  pin = p_b;
  // and go about your business
}

void Motor::set_raw(int power) {
//  Serial.print("SETTING POWER TO ");
//  Serial.print(power);
//  Serial.println("%");
  analogWrite(pin, power);
}

void Motor::set(int power, bool debug) {
  if (debug) {
    Serial.print("SETTING POWER TO ");
    Serial.print(power);
    Serial.println("%");
  }
  set_raw(static_cast<int>((power / 100.0) * 256));
}
void Motor::set(int power) {
  set(power, false);
}
void Motor::set(float power) {
  set(static_cast<int>(power), false);
}
void Motor::stop() {
  set(0);
}

void Motor::calibrate(int start, int stop, int step, int delay_millis) {
  Serial.println("CALIBRATING MOTOR");
  for (int power = start; power < stop; power += step) {
    set(power, true);
    delay(delay_millis);
  }
  delay(500);
  for (int power = stop; power > start; power -= step) {
    set(power, true);
    delay(delay_millis);
  }
}

void Motor::calibrate() {
  calibrate(0, 25, 1, 25);
  stop();
}
