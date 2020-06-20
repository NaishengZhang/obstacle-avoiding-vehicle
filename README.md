# [Obstacle Avoiding Vehicle](https://github.com/NaishengZhang/obstacle_avoiding_vehicle)

[Video](https://youtu.be/46QFpG3M0sA)

- Navigate through an obstacle course successfully. The obstacles will be walls higher than the vehicle height.

- The vehicle are based on a microcontroller with WiFi Communication built-in. This is used to guide the vehicle towards a WiFi Access Point Beacon.

- There will be a central Wi-Fi beacon access point in the obstacle course, the vehicle will need to calculate their relative distance to the beacon through signal strength measurements.

- They will then navigate through the course to meet at the beacon. The meeting area will be circular and will have a radius calibrated to the strength of the WiFi beacon.

- The vehicle  is able to navigate autonomously to the beacon, without human intervention. 

- The vehicle can do this irrespective of their starting positions, in a certain range of distance, where

  the signal is perceivable.

- Radio Communication enabled devices usually have a perceived signal strength indicator parameter.

- This parameter doesn’t require need for much data exchange as just establishing and

  maintaining a stable connection will give the RSSI value.

## LIST OF MATERIALS:

- **Motor Driver:** Pololu Dual DC Motor Driver 1A, 4.5V- 13.5V- TB6612FNG

- **Car kit:** EMO Smart Robot Car Chassis Kit with Motors, Speed Encoder and Battery Box for DIY

- **Micro controller:** Adafruit Feather HUZZAH with ESP8266 - Loose Headers

- **Battery:** Lithium Ion Polymer Battery - 3.7v 1200mAh
- **Ultrosonic sensor:** ELEGOO 5PCS HC-SR04 Ultrasonic Module Distance Sensor for Arduino UNO MEGA2560 Nano Robot XBee ZigBee
- **Servo:** 4Pcs SG90 9g Micro Servos for RC Robot Heli-copter Airplane Controls Car Boat
- **Wi-Fi Beacon:** iPhone 11 Pro personal Hot Pot

## IMPLEMENTATION

- Use one ultrasonic sensor to measure the distance between car and obstacles, and one motor driver to transfer the signal to power energy to enable the car to move.
- Use one servo to control the direction the sensor face so that the vehicle measure the distance from different directions, which can help the car to decide which direction it can go, one Wi-Fi beacon to help the car to fix the direction. 
- Use the Jumper Wire to connect the microcontroller and the motor driver, and give the pin with High and Low voltage to make our car move, meanwhile, use the Jump Wire to connect the ultrasonic sensor and the microcontroller, to make it output the wave.
- Use the tape and glue to combine the servo and the sensor. Use the 3.7V battery to charge the motor driver first, but we found that this voltage cannot satisfy the motor driver, so we change our battery to four 4AA, and use the 3.7V battery to charge the microcontroller.