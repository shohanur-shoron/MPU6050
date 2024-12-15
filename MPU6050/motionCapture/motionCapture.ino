#include <Wire.h>
#include <math.h>
#include <Servo.h>

#define MPU6050_I2C_ADDRESS 0x68
#define FREQ  30.0  // Sample frequency in Hz

Servo roll_servo;

// Global angle, gyro derived
double gSensitivity = 65.5;  // For 500 deg/s
double gx = 0, gy = 0, gz = 0;
double gyrX = 0, gyrY = 0, gyrZ = 0;
int16_t accX = 0, accY = 0, accZ = 0;

double gyrXoffs = -281.00, gyrYoffs = 18.00, gyrZoffs = -83.00;

void setup() {
  uint8_t sample_div;

  Serial.begin(115200);  // Use 115200 baud rate for ESP8266
  Wire.begin(D2, D1);    // SDA -> D2 (GPIO4), SCL -> D1 (GPIO5)

  // Servo pin adjusted for ESP8266
  roll_servo.attach(D4, 550, 2550);  // Attach to GPIO2 (D4)

  // Initialize MPU6050
  i2c_write_reg(MPU6050_I2C_ADDRESS, 0x6b, 0x00);  // Wake up MPU6050
  i2c_write_reg(MPU6050_I2C_ADDRESS, 0x1a, 0x01);  // Low-pass filter
  i2c_write_reg(MPU6050_I2C_ADDRESS, 0x1b, 0x08);  // 500 deg/s setting

  // Set sample rate
  sample_div = 1000 / FREQ - 1;
  i2c_write_reg(MPU6050_I2C_ADDRESS, 0x19, sample_div);

  // Calibrate MPU6050
  calibrate();
}

void loop() {
  unsigned long start_time = millis();
  read_sensor_data();

  // Calculate angles from accelerometer
  double ay = atan2(accX, sqrt(pow(accY, 2) + pow(accZ, 2))) * 180 / M_PI;
  double ax = atan2(accY, sqrt(pow(accX, 2) + pow(accZ, 2))) * 180 / M_PI;

  // Gyro-based angles
  gx = gx + gyrX / FREQ;
  gy = gy - gyrY / FREQ;
  gz = gz + gyrZ / FREQ;

  // Complementary filter
  gx = gx * 0.96 + ax * 0.04;
  gy = gy * 0.96 + ay * 0.04;

  // Check for Serial input
  if (Serial.available()) {
    char rx_char = Serial.read();
    if (rx_char == '.') {
      Serial.print(gx, 2); Serial.print(", ");
      Serial.print(gy, 2); Serial.print(", ");
      Serial.println(gz, 2);
    }
    if (rx_char == 'z') {
      gz = 0;
    }
  }

  // Move servo based on calculated angle
  roll_servo.write(-gx + 90);

  unsigned long end_time = millis();
  delay(((1 / FREQ) * 1000) - (end_time - start_time));  // Delay to maintain sample rate
}

void calibrate() {
  int x;
  long xSum = 0, ySum = 0, zSum = 0;
  uint8_t i2cData[6];
  int num = 500;

  for (x = 0; x < num; x++) {
    i2c_read(MPU6050_I2C_ADDRESS, 0x43, i2cData, 6);

    xSum += ((int16_t)(i2cData[0] << 8) | i2cData[1]);  // Cast to int16_t
    ySum += ((int16_t)(i2cData[2] << 8) | i2cData[3]);  // Cast to int16_t
    zSum += ((int16_t)(i2cData[4] << 8) | i2cData[5]);  // Cast to int16_t
  }

  gyrXoffs = xSum / num;
  gyrYoffs = ySum / num;
  gyrZoffs = zSum / num;
}

void read_sensor_data() {
  uint8_t i2cData[14];
  i2c_read(MPU6050_I2C_ADDRESS, 0x3b, i2cData, 14);

  accX = ((int16_t)(i2cData[0] << 8) | i2cData[1]);  // Cast to int16_t
  accY = ((int16_t)(i2cData[2] << 8) | i2cData[3]);  // Cast to int16_t
  accZ = ((int16_t)(i2cData[4] << 8) | i2cData[5]);  // Cast to int16_t

  // Fixing gyro readings
  gyrX = (((int16_t)(i2cData[8] << 8) | i2cData[9]) - gyrXoffs) / gSensitivity;
  gyrY = (((int16_t)(i2cData[10] << 8) | i2cData[11]) - gyrYoffs) / gSensitivity;
  gyrZ = (((int16_t)(i2cData[12] << 8) | i2cData[13]) - gyrZoffs) / gSensitivity;
}

// I2C Read
int i2c_read(int addr, int start, uint8_t *buffer, int size) {
  Wire.beginTransmission(addr);
  Wire.write(start);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)addr, (size_t)size, (bool)true);  // Cast to avoid warning
  for (int i = 0; i < size && Wire.available(); i++) {
    buffer[i] = Wire.read();
  }
  return 0;
}

// I2C Write
int i2c_write_reg(int addr, int reg, uint8_t data) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  return Wire.endTransmission(true);
}
