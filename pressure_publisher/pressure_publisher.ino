#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include "HX711.h"

float weight = 0;
boolean buttonState;
ros::NodeHandle nh;
std_msgs::Int16 msg1;
std_msgs::Float32 msg2;
ros::Publisher chatter1("up_button", &msg1);
ros::Publisher chatter2("Pressure", &msg2);

void setup() {
//pinMode(5, OUTPUT);
//digitalWrite(5, HIGH);
Init_Hx711();
Serial.begin(57600);
nh.initNode();
nh.advertise(chatter1);
nh.advertise(chatter2);
delay(3000);
Get_Maopi();
}

void loop() {
weight = Get_Weight();
msg2.data = weight;
buttonState = digitalRead(12);
msg1.data = 0;
if(buttonState == HIGH)
{
  msg1.data = 1;
}
chatter1.publish(&msg1);
chatter2.publish(&msg2);
nh.spinOnce();
delay(10);
}
