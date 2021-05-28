
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
    string txt_path;
    fstream file;
    txt_path = "/home/dxt/recorder/src/write_to_txt/config/parameter.yaml";
    file.open(txt_path.c_str(), ios::out);
    ros::Rate loop_rate(10);
    ros::Duration(2).sleep();

    int sec;
    init_time(sec);

    ros::spinOnce();
    double x_max = cb.force_value.wrench.force.x, x_min = cb.force_value.wrench.force.x;
    double y_max = cb.force_value.wrench.force.y, y_min = cb.force_value.wrench.force.y;
    double z_max = cb.force_value.wrench.force.z, z_min = cb.force_value.wrench.force.z;

    while(cb.keyboard_value.data != ' ')
    {
        re_define(&x_max, &x_min, cb.force_value.wrench.force.x, 0.5);
        re_define(&y_max, &y_min, cb.force_value.wrench.force.y, 0.5);
        re_define(&z_max, &z_min, cb.force_value.wrench.force.z, 0.5);
        ros::spinOnce();
        loop_rate.sleep();
    }
    std::cout <<  "Recording data!" << std::endl;
    file << "sec: " << sec << std::endl;
    file << "x_max: " << x_max << std::endl;
    file << "x_min: " << x_min << std::endl;
    file << "y_max: " << y_max << std::endl;
    file << "y_min: " << y_min << std::endl;
    file << "z_max: " << z_max << std::endl;
    file << "z_min: " << z_min << std::endl;
    file.close();
    return 0;


}    