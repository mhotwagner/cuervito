#include <Arduino.h>

#include "Elevon.h"

#define MIN_DEG 10
#define MAX_DEG 135

Elevon::Elevon(int servo_pin, String n, int d) {
  servo.attach(servo_pin);
  name = n;
  dir = d;
}

void Elevon::set(int deg, bool debug) {
  if (dir == -1) {
    deg = 180 - deg;
  }
  if (debug) {
    Serial.print("Setting to ");
    Serial.print(deg);
    Serial.println("deg");
  }
  servo.write(deg);
}

void Elevon::set(int deg) {
  set(deg, false);
}

void Elevon::calibrate(int step, int delay_millis) {
  Serial.println("CALIBRATING " + name + " ELEVON");
  int current = 90;
  while (current < MAX_DEG) {
    current += step;
    set(current, true);
    delay(delay_millis);
  }

  while (current > MIN_DEG) {
    current -= step;
    Serial.print("Setting to ");
    Serial.print(current);
    Serial.println("deg");
    set(current, true);
    delay(delay_millis);
  }

  while (current < 90) {
    current += step;
    Serial.print("Setting to ");
    Serial.print(current);
    Serial.println("deg");
    set(current, true);
    delay(delay_millis);
  }
  Serial.println("COMPLETE");
}

void Elevon::calibrate() {
  calibrate(45, 100);
}
