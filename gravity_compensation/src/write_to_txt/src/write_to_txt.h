
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
#include "std_msgs/Int16.h"
#include <Eigen/Dense>
#include <time.h>

using namespace std;

int time_to_sec(int hour, int min, int sec)
{
    return (hour * 60 * 60) + (min * 60) + sec;
}

int sec_to_time(int& hour, int& min, int& sec, int time)
{
    sec = time % 60;
    time = (time - sec)/60;
    min = time % 60;
    time = (time - min)/60;
    hour = time;
}

string input_file_name(string dir_path)
{
    string file_name;
    string txt_path;
    cout << "Input the doc name of txt:" << endl;
    cin >> file_name;
    txt_path = dir_path + file_name + ".txt";
    return txt_path;
}

string int_to_string(int msg)
{
   stringstream ss;
   string str;
   ss << msg;
   ss >> str;
   return str;
}


string double_to_string(double msg)
{
   stringstream ss;
   string str;
   ss << msg;
   ss >> str;
   return str;
}

string tm_to_string(tm* msg, int sec_)
{
    int hour, min, sec;
    sec_to_time(hour, min, sec, time_to_sec(msg->tm_hour, msg->tm_min, msg->tm_sec) + sec_);
    std::cout <<  time_to_sec(msg->tm_hour, msg->tm_min, msg->tm_sec) + sec_ << std::endl;
    return int_to_string(hour) + "/" + int_to_string(min) + "/" + int_to_string(sec);
}

void init_time(int &sec)
{
    int hour_, min_, sec_;
    std::cout << "hour:" << std::endl;
    std::cin >> hour_;
    std::cout << "min:" << std::endl;
    std::cin >> min_;
    std::cout << "sec:" << std::endl;
    std::cin >> sec_;

    time_t time_t_time;
    time_t_time = time(NULL);
    tm* tm_time = localtime(&time_t_time);
    std::cout << tm_time->tm_hour << "/" << tm_time->tm_min << "/" << tm_time->tm_sec << std::endl;
    // std::cout<< tm_time->tm_hour << "/" << tm_time->tm_min << "/" << tm_time->tm_sec << std::endl;
    std::cout<< hour_ << "/" << min_ << "/" << sec_ << std::endl;
    sec = time_to_sec(hour_, min_, sec_) - time_to_sec(tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
}

void rebuild_time(tm* tm_time, int hour, int min, int sec)
{
    tm_time->tm_hour = tm_time->tm_hour - hour;
    tm_time->tm_min = tm_time->tm_min - min;
    tm_time->tm_sec = tm_time->tm_sec - sec;
}

string force_value_to_str(geometry_msgs::WrenchStamped force_value)
{
    return double_to_string(force_value.wrench.force.x) + " " + double_to_string(force_value.wrench.force.y) + " " + double_to_string(force_value.wrench.force.z) + " " + double_to_string(force_value.wrench.torque.x) + " " + double_to_string(force_value.wrench.torque.y) + " " + double_to_string(force_value.wrench.torque.z);
}

string Q_to_str(Eigen::Quaterniond Q)
{
    return double_to_string(Q.w()) + " " + double_to_string(Q.x()) + " " + double_to_string(Q.y()) + " " + double_to_string(Q.z());
}

Eigen::Matrix3d imu_value_to_R(sensor_msgs::Imu imu_value)
{
    Eigen::Quaterniond Q(imu_value.orientation.w, imu_value.orientation.x, imu_value.orientation.y, imu_value.orientation.z);
    return Q.normalized().toRotationMatrix();
}


geometry_msgs::WrenchStamped gravity_compensation(geometry_msgs::WrenchStamped origin_value, Eigen::Vector3d gravity_xyz)
{   
    origin_value.wrench.force.x = origin_value.wrench.force.x - gravity_xyz.x();
    origin_value.wrench.force.y = origin_value.wrench.force.y - gravity_xyz.y();
    origin_value.wrench.force.z = origin_value.wrench.force.z - gravity_xyz.z();
    return origin_value;
}


void re_define(double *max, double *min, double now, double factor)
{
    if(now > *max)
        if(now < *max + factor)
        {
            std::cout << now << " > " << *max << std::endl;
            *max = now;
        }

    if(now < *min)
        if(now > *min - factor)
        {
            std::cout << now << " < " << *min << std::endl;
            *min = now;
        }
}


class callback_function
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

    void keyboard_callback_function(const std_msgs::Int16::ConstPtr& msg)
    {
        keyboard_value = *msg;
    }

    geometry_msgs::WrenchStamped force_value;
    sensor_msgs::Imu imu_value;
    std_msgs::Int16 keyboard_value;
    
};