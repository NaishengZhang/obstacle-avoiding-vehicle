#include <Servo.h>
#include "ESP8266WiFi.h"
#define WIFINAME "xxxxxx"
#define WIFIPW   "xxxxxx"

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4
#define TURNLEFT_WIFI 5

// setup servo
#define RIGHT   55
#define FRONT  100
#define LEFT    160
#define DIS      35
//Motor pin
int leftMotor1 = 15;
int leftMotor2 = 0;
int rightMotor1 = 5;
int rightMotor2 = 2;
int leftPWM = 13;
int rightPWM = 4;
int stby = 16;

// RSSI Variable
int RSSIs[5] = { -100, -100, -100, -100, -100}; // used as a queue

Servo myServo;  //servo
int ser = 12;
int echo = 14;  // echo pin
int trig = 3; // trig pin

void setup() {
  //GPIO 3 (RX) swap the pin to a GPIO.
  pinMode(3, FUNCTION_3);
  //initialize upload spead
  Serial.begin(115200);
  //servo
  myServo.attach(ser);
  //Motor
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(stby, OUTPUT);
  //Sensor
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  //WIFI
  Serial.println("");
  WiFi.begin(WIFINAME, WIFIPW);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP Address:");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.RSSI());
}

void loop() {
  //make car stop before scan
  motorRun(STOP, 0, 0);
  delay(500);
  //devide which action should take
  char action = scan();
  for (int i = 0; i < 4; i++) {
    RSSIs[i] = RSSIs[i + 1]; // construct a queue to store the RSSIs record
  }
  RSSIs[4] = getRSSI();// update the current RSSI
  switch (action) {
    // try to avoid obstacle at first.
    case 'TURNLEFT':
      motorRun(TURNLEFT, 600, 600);
      delay(700);
      break;
    case 'TURNRIGHT':
      motorRun(TURNRIGHT, 600, 600);
      delay(700);
      break;
    case 'BACKWARD':
      motorRun(BACKWARD, 700, 550);
      delay(700);
      motorRun(TURNRIGHT, 600, 600);
      delay(600);
      break;
    default:
      //the default is to forward which means there is no abstacle around. So in this case, let car try to find beacon.
      char action_find = find();
      if (action_find == 'STOP') {
        motorRun(STOP, 0, 0);
        delay(100000);
      }
      else if (action_find == 'FORWARD') {
        motorRun(FORWARD, 700, 550);
        delay(600);
      }
      else if (action_find == 'TURNLEFT') {
        motorRun(TURNLEFT, 600, 600);
        delay(600);
      }
      else {
        motorRun(TURNRIGHT, 600, 600);
        delay(600);
      }
  }
}

char scan()
{
  char action;
  int left, right, front;

  // scan left
  myServo.write(LEFT);
  delay(500);
  left = getDistance();

  // scan right
  myServo.write(RIGHT);
  delay(800);
  right = getDistance();

  // scan straight ahead
  myServo.write(FRONT);
  delay(500);
  front = getDistance();
  if (right < DIS || left < DIS || front < DIS) {
    // if one of the distances are less than the safe distance 35, make car back.
    action = 'BACKWARD';
  }
  else if (left > right && left > front) {
    //if there are obstacbles at the front and left, make car turn left.
    action = 'TURNLEFT';
  }
  else {
    //if there are obstacbles at the front and right, make car turn right.
    action = 'TURNRIGHT';
  }
  return action;
}



char find()
{
  char action;
  if (RSSIs[4] > -50) {
    // The car is close to the beacon, stop
    action = 'STOP';
  }
  else if (RSSIs[4] >= RSSIs[3]) {
    // when the value of RSSI become more and more larger(i.e. the car is closer to the beacon), let car forward.
    action = 'FORWARD';
  }
  else {
    // when the value of RSSI become more and more smaller(i.e. the car is farther and farther away from the beacon), let car turnleft.
    action = 'TURNLEFT';
  }
  return action;
}

void motorRun(int cmd, int leftVal, int rightVal)
{
  analogWrite(leftPWM, leftVal);  //set pwm => set speed
  analogWrite(rightPWM, rightVal);
  switch (cmd) {
    case FORWARD:
      Serial.println("FORWARD"); //FORWARD
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      digitalWrite(stby, HIGH); // set stby high to make motor run
      break;
    case TURNRIGHT:
      Serial.println("TURNRIGHT"); //TURNRIGHT
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      digitalWrite(stby, HIGH);
      break;
    case TURNLEFT_WIFI:
      Serial.println("TURNLEFT_WIFI"); //TURNLEFT_WIFI
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      digitalWrite(stby, HIGH);
      break;
    case BACKWARD:
      Serial.println("BACKWARD"); //BACKWARD
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      digitalWrite(stby, HIGH);
      break;
    case TURNLEFT:
      Serial.println("TURNLEFT"); //TURNLEFT
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      digitalWrite(stby, HIGH);
      break;
    default:
      Serial.println("STOP"); //STOP
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}

int getRSSI() // get the value of RSSI.
{
  int sum = 0;
  int maxVal = -100;
  int minVal = 0;
  for (int i = 0; i < 10; i++) {
    int temp = WiFi.RSSI(); // read 10 times RSSI
    sum += temp; // sum
  }
  return sum / 10; // get the average value of 10 RSSIs
}

int getDistance()
{
  digitalWrite(trig, LOW); // Make the interface send out the ultrasonic signal < 2μs
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // Make the interface send out the ultrasonic signal < 10μs, here is at least 10μs
  delayMicroseconds(10);
  digitalWrite(trig, LOW); // keep trig low
  int distance = pulseIn(echo, HIGH); // read the length of the pulse
  distance = distance / 58; //calculate distance based on the length of the pulse
  Serial.println(distance); //print distance

  if (distance >= 50)
  {
    //return 50 if distance > 50
    return 50;
  }
  else {
    return distance;
  }
}
