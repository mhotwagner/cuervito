#include <Arduino.h>

#include "Cuervito.h"

#define SEALEVELPRESSURE_HPA (820)

Cuervito::Cuervito(int bow_motor_pin_a,
                   int bow_motor_pin_b,
                   int aft_motor_pin_a,
                   int aft_motor_pin_b,
                   int portside_servo_pin,
                   int starboard_servo_pin)
                   :bow_prop(bow_motor_pin_a, bow_motor_pin_b), aft_prop(aft_motor_pin_a, aft_motor_pin_b),
                   portside_elevon(portside_servo_pin, "PORTSIDE", -1), starboard_elevon(starboard_servo_pin, "STARBOARD", 1)
{
      bow_motor_pin_a = bow_motor_pin_a;
      bow_motor_pin_b = bow_motor_pin_b;
      aft_motor_pin_a = aft_motor_pin_a;
      aft_motor_pin_b = aft_motor_pin_b;
      portside_servo_pin = portside_servo_pin;
      starboard_servo_pin = starboard_servo_pin;
      canReverse = true;

};


Cuervito::Cuervito(int bow_motor_pin,
                   int aft_motor_pin,
                   int portside_servo_pin,
                   int starboard_servo_pin)
                   :bow_prop(bow_motor_pin), aft_prop(aft_motor_pin),
                   portside_elevon(portside_servo_pin, "PORTSIDE", -1), starboard_elevon(starboard_servo_pin, "STARBOARD", 1)
{
      bow_motor_pin = bow_motor_pin;
      aft_motor_pin = aft_motor_pin;
      portside_servo_pin = portside_servo_pin;
      starboard_servo_pin = starboard_servo_pin;
      canReverse = false;
};


void Cuervito::dual_prop_calibrate(int start, int stop, int step, int delay_millis) {
  Serial.println("CALIBRATING BOTH PROPS");
  for (int power = start; power < stop; power += step) {
    bow_prop.set(power, true);
    aft_prop.set(power, true);
    delay(delay_millis);
  }
  delay(1000);
  for (int power = stop; power > start; power -= step) {
    bow_prop.set(power, true);
    aft_prop.set(power, true);
    delay(delay_millis);
  }
}

void Cuervito::calibrate() {
  Serial.println("CALIBRATING DRONE");
  portside_elevon.calibrate();
  starboard_elevon.calibrate();
  bow_prop.calibrate(0, 50, 1, 100);
  aft_prop.calibrate(0, 50, 1, 100);
  dual_prop_calibrate(0, 50, 1, 100);

  initializeBMP();
  initializeMPU();
  
};

void Cuervito::joy_in(int x, int y) {
  
  //this is the mixing 'magic' for elevons...change the sign in front of the variables if anything is reversed for either servo:
  //note you can also reduce the 'amount' of roll or pitch here by multiplying either of the variables by a float from 0.0 (no contribution) to 1.0 (full contribution)  
  //constrain the servo command variables to stay within -1 to 1 range
  int pc = constrain(x + y,-128, 128);
  int sc = constrain(x - y,-128, 128);
  
  //write the command values out using the servo library (assuming you've already created/attached your servo objects in the void setup):
  portside_elevon.set(map(pc,-128, 128, 0, 180), true); //we remapped the normalized command from -1-1 to 0-180, which is what the servo library expects as an input
  starboard_elevon.set(map(sc,-128, 128, 0, 180), true);
}

void Cuervito::motor_in(int m) {
  bow_prop.set(m);
  aft_prop.set(m);
}

void Cuervito::initializeBMP() {
  if (!bmp.begin_I2C()) {
    Serial.println("OH NO");
    return;
  }
  Serial.println("still going?");
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  Serial.println("still going?");
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  readBMP();
  Serial.println("still going?");
}

void Cuervito::readBMP() {
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
}

void Cuervito::initializeMPU() {
  if (!mpu.begin()) {
    Serial.println("OH NOOOO");
  }

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
  readMPU();
  
}

void Cuervito::readMPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
}
