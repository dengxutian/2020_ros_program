#!/usr/bin/env python
import rospy
from std_msgs.msg import Int64
import numpy as np
import cv2 as cv
from telcontrol.msg import Posxy


def talker(posxy):
    pub = rospy.Publisher('/posxy_info',Posxy,queue_size=1)
    rospy.init_node('posxy_publisher',anonymous=True)
    msg=Posxy()
    msg.x=posxy[0]
    msg.y=posxy[1]
    pub.publish(msg)



def image_deal(img_color):
    threshold= 245
    mark_size = 5
    mark_color = (0,0,255)
    mark_type = 2

    img_gray=cv.cvtColor(img_color,cv.COLOR_BGR2GRAY)
    binary = cv.threshold(img_gray, threshold, 255, cv.THRESH_BINARY)
    binary1=binary[1]
    H,W = binary1.shape
    target_pos = np.where(binary1 == 255)
    if target_pos[0].size == 0:
        H_var = 0
        W_var = 0
        pos = [0,0]
    else:
        H_var = np.var(target_pos[0])
        W_var = np.var(target_pos[1])
        pos = [W - round(np.mean(target_pos[1])),round(np.mean(target_pos[0]))]
    var = (H_var + W_var)/2
    print(var)
    img_dealed = np.zeros((H,W,3),np.uint8)
    if 20<var<1500:
    # if 200 < var < 1500 and 200 < H_var < 1500 and 200 < W_var < 1500:
    # if target_pos[0].size != 0:
        cv.circle(img_dealed,(W - int(np.mean(target_pos[1])),int(np.mean(target_pos[0]))),mark_size,mark_color,mark_type)
    # img_color = img_color[30:H,40:W,:]
    cv.imshow("1", img_color)
    cv.imshow('2',binary1)
    cv.imshow("3", img_dealed)
    cv.waitKey(1)
    return pos

go_on=True
try:
    capture=cv.VideoCapture(0)
    capture.set(6,cv.VideoWriter.fourcc('M','J','P','G'))
    capture.set(5,120)
    print("fps = " + str(capture.get(5)))
    print("formation = " + str(capture.get(6)))
    print("Read Successfully!!!")
except:
    print("USB camera error!!!")
    go_on=False
while go_on:
    ret,img_color = capture.read()
    posxy = image_deal(img_color)
    talker(posxy)
    cv.waitKey(1)


# img_color=cv.imread('1.png')
# img_dealed=image_deal(img_color)
# cv.imshow("1",img_color)
# cv.imshow("2",img_dealed)
# cv.waitKey(10000)


