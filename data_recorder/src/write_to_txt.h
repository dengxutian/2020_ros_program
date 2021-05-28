
// Created by Deng-Xutian on 2020/9/22.
// Email address: dengxutian@126.com.

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "ros/ros.h"
#include "geometry_msgs/WrenchStamped.h"
#include "geometry_msgs/Quaternion.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float64.h"

using namespace std;

//输入文件名，自动添加路径前缀
string input_file_name(string dir_path)
{
    string file_name;
    string txt_path;
    cout << "Input the doc name of txt:" << endl;
    cin >> file_name;
    txt_path = dir_path + file_name + ".txt";
    return txt_path;
}

//整形转字符串
string int_to_string(int msg)
{
   stringstream ss;
   string str;
   ss << msg;
   ss >> str;
   return str;
}

//浮点型转字符串
string double_to_string(double msg)
{
   stringstream ss;
   string str;
   ss << msg;
   ss >> str;
   return str;
}

//传入topic消息参数，拼接为字符串并返回
string force_value_to_str(geometry_msgs::WrenchStamped force_value)
{
    string space = " ";
    string result;
    result = double_to_string(force_value.wrench.force.x) + space + double_to_string(force_value.wrench.force.y) + space + double_to_string(force_value.wrench.force.z) + space + double_to_string(force_value.wrench.torque.x) + space + double_to_string(force_value.wrench.torque.y) + space + double_to_string(force_value.wrench.torque.z);
    return result;
}

//传入topic消息参数，拼接为字符串并返回
string imu_value_to_str(sensor_msgs::Imu imu_value)
{
    string space = " ";
    string result;
    result = double_to_string(imu_value.orientation.x) + space + double_to_string(imu_value.orientation.y) + space + double_to_string(imu_value.orientation.z) + space +double_to_string(imu_value.orientation.w);
    return result;
}

class callback_function
//回调函数请添加到这个类里面
//回调函数格式固定
{
public:

    void imu_callback_function(const sensor_msgs::Imu::ConstPtr& msg)
    {
        imu_value = *msg;
    }

    void force_callback_function(const geometry_msgs::WrenchStamped::ConstPtr& msg)
    {
        force_value = *msg;
    }

    geometry_msgs::WrenchStamped force_value;
    sensor_msgs::Imu imu_value;
    
};