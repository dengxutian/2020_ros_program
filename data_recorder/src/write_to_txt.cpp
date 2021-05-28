
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

    string dir_path;
    string txt_path;
    string str;
    fstream file;
    ros::Rate loop_rate(10);

    dir_path = "/home/dxt/recorder/src/write_to_txt/doc/"; //文件夹的路径
    txt_path = input_file_name(dir_path); //txt文件的路径
    cout << "Writing file:" << txt_path << endl;
    file.open(txt_path.c_str(), ios::out); //打开文件

    while(ros::ok())
    {
        str = imu_value_to_str(cb.imu_value) + " " + force_value_to_str(cb.force_value); //消息拼接
        file << str << endl; //写入数据
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    file.close(); //关闭文件
    return 0;
}