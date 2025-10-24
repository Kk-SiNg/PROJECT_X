
#include <Arduino.h>

// --- Pin Definitions ---
// IR Sensors
const int S1 = 32;
const int S2 = 33;
const int S3 = 25;
const int S4 = 26;
const int S5 = 27;

// Motor Driver (Right Motor)
const int IN1 = 14;
const int IN2 = 12;
const int ENA = 13;

// Motor Driver (Left Motor)
const int IN3 = 15;
const int IN4 = 2;
const int ENB = 4;

//PID constants
float Kp = 0.05; // Proportional gain: Governs the reaction to the current error.
float Ki = 0.001; // Integral gain: Corrects for long-term, systematic errors.
float Kd = 0.1;  // Derivative gain: Dampens the response to prevent overshooting.


// --- Motor & Control Settings ---
int maxSpeed = 200; // The maximum speed of the motors (0-255)

// The integral should not be allowed to grow larger in magnitude than this value.
// This prevents "integral windup".
float integralLimit = 200.0;


// --- Global PID Variables ---
float error = 0;
float integral = 0;
float derivative = 0;
float lastError = 0; // Required to calculate the derivative


// --- ESP32 PWM Configuration ---
const int pwmFrequency = 5000; // Hz
const int pwmResolution = 8;   // 8-bit resolution (0-255)
const int rightMotorPWMChannel = 0;
const int leftMotorPWMChannel = 1;


void setup() {

  Serial.begin(115200); //set burd rate according to ir sensor array

  // Set pin modes
  pinMode(S1, INPUT); pinMode(S2, INPUT); pinMode(S3, INPUT); pinMode(S4, INPUT); pinMode(S5, INPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Configure the PWM functionalities for motor speed control using ESP32's LEDC peripheral
  ledcSetup(rightMotorPWMChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(ENA, rightMotorPWMChannel);
  ledcSetup(leftMotorPWMChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(ENB, leftMotorPWMChannel);

}

void loop() {
  // 1. CALCULATE THE ERROR
  // This section reads the sensors and calculates a single 'error' value,
  // which represents how far the bot is from the center of the line.
  // A negative error means the bot is to the right of the line.
  // A positive error means the bot is to the left of the line.(as is clear from code below)

  // Read sensor values (assuming LOW/0 = black line, HIGH/1 = white surface)
  int s[5];
  s[0] = digitalRead(S1); s[1] = digitalRead(S2); s[2] = digitalRead(S3);
  s[3] = digitalRead(S4); s[4] = digitalRead(S5);

  // Use the Weighted Average method for a smooth, continuous error value.
  long weightedSum = 0;
  int activeSensorCount = 0;
  bool onLine = false;

  if (s[0] == 0) { weightedSum += -20; activeSensorCount++; onLine = true; }
  if (s[1] == 0) { weightedSum += -10; activeSensorCount++; onLine = true; }
  if (s[2] == 0) { weightedSum += 0;   activeSensorCount++; onLine = true; }
  if (s[3] == 0) { weightedSum += 10;  activeSensorCount++; onLine = true; }
  if (s[4] == 0) { weightedSum += 20;  activeSensorCount++; onLine = true; }

  if (onLine) {
    // If at least one sensor is on the line, calculate the error
    error = (float)weightedSum / activeSensorCount;
  } 
  else {
    // If no sensors see the line, use the last known error(lastError saved) to decide
    // which way to turn. A large error value forces a sharp turn back.
    if (lastError > 0) {
      error = 25; // A value larger than the max weight
    } 
    else {
      error = -25; // A value smaller than the min weight
    }
  }

  // 2. COMPUTE PID TERMS
  integral = integral + error;
  // CLAMP the integral to prevent "windup"
  integral = constrain(integral, -integralLimit, integralLimit);

  // Derivative term is the rate of change of the error, used to dampen the response.
  derivative = error - lastError;

  // 3. CALCULATE THE FINAL CORRECTION
  // The final correction is the sum of the three PID terms multiplied by their gains.
  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

  // 4. UPDATE lastError FOR THE NEXT ITERATION
  lastError = error;

  // 5. APPLY THE CORRECTION TO THE MOTORS
  int rightMotorSpeed = maxSpeed - correction;
  int leftMotorSpeed = maxSpeed + correction;

  // Constrain the motor speeds to be within a valid, controllable range (0 to maxSpeed)
  rightMotorSpeed = constrain(rightMotorSpeed, 0, maxSpeed);
  leftMotorSpeed = constrain(leftMotorSpeed, 0, maxSpeed);

  moveMotors(leftMotorSpeed, rightMotorSpeed);
}


void moveMotors(int leftSpeed, int rightSpeed) {
  // Control Left Motor (Motor B: IN3, IN4, ENB)
  digitalWrite(IN3, HIGH); // Set Direction: Forward
  digitalWrite(IN4, LOW);
  ledcWrite(leftMotorPWMChannel, leftSpeed); // Set Speed

  // Control Right Motor (Motor A: IN1, IN2, ENA)
  digitalWrite(IN1, HIGH); // Set Direction: Forward
  digitalWrite(IN2, LOW);
  ledcWrite(rightMotorPWMChannel, rightSpeed); // Set Speed
}