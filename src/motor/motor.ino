#include <Servo.h>
Servo motor1;

int val=45;
void setup() {
  // put your setup code here, to run once:
  motor1.attach(5); // Connect motor through motor driver to pin 5. Make sure you choose pin with sck only.
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  motor1.write(val); // Angle of rotation. 0<=val<=180
  val++;
  if(val > 120)
    val = 45;
  delay(250);
  Serial.println(val);
}
