# Challanges
1. Dimentions of line vs the seperation between sensors in linear array

# learning sources for pid
1. https://www.youtube.com/watch?v=tFVAaUcOm4I
2. https://youtu.be/4Y7zG48uHRo?si=n-j8FnBH6SiFu-us
3. https://youtu.be/UR0hOmjaHp0?si=hpL7QxhogHAIR6cR

# Project description
 * --- FEATURES ---
 *  - Microcontroller used: ESP32
 *  - Sensors: 5-channel IR array
 *  - Motor Driver: L298N(*see my research on motor drivers*)
 *  - Control Algorithm: Full PID (Proportional, Integral, Derivative)
 *  - Error Calculation: Weighted Average for smooth, continuous error values
 *  - Stability Feature: Integral Clamping (Anti-Windup) to prevent overshoot(*by adding an integralinit*)
 
### Weighted Average method:

1. I first used this error calculation method:

'''
  if(s[0]==0 && s[1]==1 && s[2]==1 && s[3]==1 && s[4]==1) error = -4; 
  else if (s[0]==0 && s[1]==0 && s[2]==1 && s[3]==1 && s[4]==1) error = -3;
  else if (s[0]==1 && s[1]==0 && s[2]==1 && s[3]==1 && s[4]==1) error = -2;
  else if (s[0]==1 && s[1]==0 && s[2]==0 && s[3]==1 && s[4]==1) error = -1;
  else if (s[0]==1 && s[1]==1 && s[2]==0 && s[3]==1 && s[4]==1) error = 0;   // Perfectly centered
  else if (s[0]==1 && s[1]==1 && s[2]==0 && s[3]==0 && s[4]==1) error = 1;
  else if (s[0]==1 && s[1]==1 && s[2]==1 && s[3]==0 && s[4]==1) error = 2;
  else if (s[0]==1 && s[1]==1 && s[2]==1 && s[3]==0 && s[4]==0) error = 3;
  else if (s[0]==1 && s[1]==1 && s[2]==1 && s[3]==1 && s[4]==0) error = 4; /
  
  // If the bot is completely off the line (all sensors see white),
  // we can assume it went off in the direction of the last known error.
  // This helps it turn back towards the line.
  else if (s[0]==1 && s[1]==1 && s[2]==1 && s[3]==1 && s[4]==1) {
      if (lastError > 0) {
          error = 5; // Or a value larger than max
      } else {
          error = -5; // Or a value smaller than min
      }
  }'''

but this has some problems like sudden jumps in error values ex:

consider a transition of line from under s[2] to s[3] then:

- State A: Line is under S3. Sensor values: [1, 1, 0, 1, 1]. Error = 0.
- State B: Line is under S4. Sensor values: [1, 1, 1, 0, 1]. Error = 2.

The error value jumps from 0 directly to 2. It never has a value of 0.5, 1.0, or 1.5. The robot's physical position, however, changes continuously. The PID controller has to react to a sudden change in error rather than a smooth one.

2. weighted avg method in same example:

- Line under Center (S3):
Sensors ON: {S3}
Sum = 0
Count = 1
Error = 0 / 1 = 0

- Line under S4:
Sensors ON: {S4}
Sum = 10
Count = 1
Error = 10 / 1 = 10

- Advantage part- Line is BETWEEN S3 and S4:
Sensors ON: {S3, S4}
Sum = 0 + 10 = 10
Count = 2
Error = 10 / 2 = 5 (This is our "in-between" value! It's perfectly continuous.)

 ### pin setup
 * IR Sensors:
 * S1 (Far Left) -> GPIO 32
 * S2 (Left)     -> GPIO 33
 * S3 (Center)   -> GPIO 25
 * S4 (Right)    -> GPIO 26
 * S5 (Far Right)-> GPIO 27
 *
 * L298N Motor Driver Inputs:
 * -- Right Motor --
 * IN1 -> GPIO 14
 * IN2 -> GPIO 12
 * ENA -> GPIO 13 (PWM)
 *
 * -- Left Motor --
 * IN3 -> GPIO 15
 * IN4 -> GPIO 2
 * ENB -> GPIO 4  (PWM)
 

*please note that i had no idea about data like PID constants and what should be the value of integaralimit so i used gemini for that*