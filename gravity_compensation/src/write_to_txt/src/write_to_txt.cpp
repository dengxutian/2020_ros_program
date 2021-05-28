
// Created by Deng-Xutian on 2020/9/22.
// Email address: dengxutian@126.com.

#include "write_to_txt.h"

using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "writer");
    ros::NodeHandle nh;
    callback_function cb;
    ros::Subscriber sub_imu = nh.subscribe("/imu", 1, &callback_function::imu_callback_function, &cb);
    ros::Subscriber sub_force = nh.subscribe("/netft_data", 1, &callback_function::force_callback_function, &cb);
    ros::Subscriber sub_keyboard = nh.subscribe("/keyboard", 1, &callback_function::keyboard_callback_function, &cb);
    ros::Rate loop_rate(10);//采样频率未经过优化

    double x_max, x_min, y_max, y_min, z_max, z_min;
    nh.getParam("x_max", x_max);
    nh.getParam("x_min", x_min);
    nh.getParam("y_max", y_max);
    nh.getParam("y_min", y_min);
    nh.getParam("z_max", z_max);
    nh.getParam("z_min", z_min);
    double x, y, z, g;
    x = (x_max + x_min)/2;
    y = (y_max + y_min)/2;
    z = (z_max + z_min)/2;
    g = (z_max - z_min)/2;
    int hour, min, sec;
    nh.getParam("sec", sec);

    string dir_path;
    string txt_path;
    string str;
    fstream file;

    time_t time_t_time;
    tm* tm_time;

    dir_path = "/home/dxt/recorder/src/write_to_txt/doc/";
    ros::Duration(5).sleep();//等待ATI和IMU数值稳定
    txt_path = input_file_name(dir_path);
    cout << "File to write: " << txt_path << endl;
    file.open(txt_path.c_str(), ios::out);

    int i;
    for(i=0;i<100;i++)
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    std::cout << "Start to record." << std::endl;

    Eigen::Matrix3d R;
    Eigen::Quaterniond Q;
    Eigen::Vector3d standard(x, y, z);
    Eigen::Matrix3d RT = imu_value_to_R(cb.imu_value).transpose();
    Eigen::Matrix3d M_Result;
    Eigen::Vector3d gravity_xyz;
    Eigen::Vector3d gravity(0, 0, -g);
    Eigen::Vector3d fix_value;

    int mark_by_hand = 0;
    
    while(ros::ok())
    {
        mark_by_hand = 0;
        if(cb.keyboard_value.data == ' ')
            mark_by_hand = 1;
        
        R = imu_value_to_R(cb.imu_value);
        M_Result = RT * R;
        Q = Eigen::Quaterniond(M_Result);
        gravity_xyz = (M_Result.transpose() * gravity);
        fix_value << standard.x() - gravity_xyz.y(), standard.y() + gravity_xyz.x(), standard.z() - gravity_xyz.z();
        time_t_time = time(NULL);
        tm_time = localtime(&time_t_time);
        str =  int_to_string(mark_by_hand) + " " + tm_to_string(tm_time, sec) + " " + Q_to_str(Q) + " " + force_value_to_str(gravity_compensation(cb.force_value, fix_value));
        std::cout << str << std::endl;
        file << str << endl;
        ros::spinOnce();
        loop_rate.sleep();
    }
    file.close();
    return 0;
}
