//Author: Deng-Xutian
//email address: dengxutian@126.com

This program runs in Arduino which connecting both HX711 and PC.
It will publish pressure signal into ROS topic.
You should firstly make sure Arduino could connect to ROS.
Hardware plot: pressure sensor -> HX711 -> Arduino -> PC.
Please run: rosrun rosserial_python serial_node.py.
Topic: "Pressure"

Beside, topic names "up_button" can listen the buttom signal.
