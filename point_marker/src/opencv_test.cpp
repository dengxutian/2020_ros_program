#include <ros/ros.h>
#include "telcontrol/Posxy.h"
#include <iostream>
#include <typeinfo>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void find_center(Mat img, int *a)
{
    int x, y;
    int xs = 0, ys = 0, count = 0;
    for(x=0; x<img.rows; x++)
    {
        #pragma omp parallel for    
        for(y=0; y<img.cols; y++)
        {
            if(img.at<uchar>(x, y) ==255)
            {
                xs = xs + x;
                ys = ys + y;
                count = count +1;
            }
        }
    }
    if(count != 0)
    {
        a[0] = int(xs / count);
        a[1] = int(ys / count);        
    }
    else
    {
        a[0] = 0;
        a[1] = 0;
    }
}

void image_deal(Mat img_color, int *b)
{
    int a[2];
    Mat img_gray, binary, dealed;
    cvtColor(img_color, img_gray, CV_BGR2GRAY);
    threshold(img_gray, binary, 250, 255, 0);
    imshow("1", img_color);
    imshow("2", img_gray);
    imshow("3", binary);
    find_center(binary, a);
    b[0] = a[0];
    b[1] = a[1];
    waitKey(10);
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker1");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<telcontrol::Posxy>("/posxy",1);
    int a[2];
    telcontrol::Posxy msg;
    VideoCapture capture(0);
    // capture.set(CAP_PROP_FRAME_WIDTH, 1920);
    // capture.set(CAP_PROP_FRAME_HEIGHT, 1080);
    capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));

    Mat frame, result;
    while(1)
    {
        capture >> frame;
        image_deal(frame, a);
        printf("%d,%d\n",a[0],a[1]);
        printf("fbs = %.2f\n",capture.get(CV_CAP_PROP_FPS));
        pub.publish(msg);
    }

    return 0;
}