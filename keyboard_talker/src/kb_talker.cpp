#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "ros/ros.h"
#include "std_msgs/Int16.h"

#define TTY_PATH
#define STTY_US
#define STTY_DEF

int get_char();
int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 20;

    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar();
    }
    return ch;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "keyboard_talker");
    ros::NodeHandle nh;
    ros::Publisher pub_keyboard = nh.advertise<std_msgs::Int16>("keyboard", 1);
    ros::Rate loop_rate(50);

    int ch = 0;
    std_msgs::Int16 msg;
    std::string str;
    std::stringstream ss;
    system(STTY_US TTY_PATH);

    while(1)
    {
        ch = get_char();
        if(ch != 'q')
        {
            msg.data = ch;
            pub_keyboard.publish(msg);
        }
        else
        {
            system(STTY_DEF TTY_PATH);
            return 0;
        }
        loop_rate.sleep();
    }
}
