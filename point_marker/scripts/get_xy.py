#!/usr/bin/env python

import rospy

from telcontrol.msg import Posxy
import numpy as np




import sys
sys.path.remove('/opt/ros/kinetic/lib/python2.7/dist-packages')
import cv2

class Posget:

    def __init__(self):

        rospy.init_node('posxy_publisher',anonymous=True)
        self.pub_posxyinfo = rospy.Publisher('/posxy_info',Posxy,queue_size=10)
        
        self.threshold = 245
        self.mark_size = 5
        self.mark_color = (0,0,255)
        self.mark_type = -1
        self.cap = cv2.VideoCapture(0)

    def check_open(self):

        if self.cap.isOpened() is False:
            rospy.loginfo('Failed to open usb cam\n')
            exit(1)

    def image_deal(self):

        _,original_img = self.cap.read()
        cv2.imshow('original_img',original_img)
        gray=cv2.cvtColor(original_img,cv2.COLOR_BGR2GRAY)
        _, binary = cv2.threshold(gray, self.threshold, 255, cv2.THRESH_BINARY)
        H,W = binary.shape
        target_pos = np.where(binary == 255)
        if target_pos[0].size == 0:
            H_var = 0
            W_var = 0
            pos = [0,0]
        else:
            H_var = np.var(target_pos[0])
            W_var = np.var(target_pos[1])
            pos = [W - round(np.mean(target_pos[1])),round(np.mean(target_pos[0]))] 

        var = (H_var + W_var)/2
        imgshow = np.zeros((H,W,3),np.uint8)
        if 200 < var < 1500 and 200 < H_var < 1500 and 200 < W_var < 1500:
        # if target_pos[0].size > 5000 and target_pos[0].size < 9000:
            cv2.circle(imgshow,(W - round(np.mean(target_pos[1])),round(np.mean(target_pos[0]))),self.mark_size,self.mark_color,self.mark_type)
        cv2.imshow('imgshow',imgshow)
    
        return pos
    def Pos_publisher(self):
        posxy_msg = Posxy()# 发布消息

        posxy = self.image_deal()
        posxy_msg.x = posxy[0]
        posxy_msg.y = posxy[1]
        self.pub_posxyinfo.publish(posxy_msg)
        rospy.loginfo("Publsh person message[%f, %f]",posxy_msg.x, posxy_msg.y)
        

        # rospy.spin()
def run():
    robot_posxy = Posget()
    while not rospy.is_shutdown():
        robot_posxy.Pos_publisher()
        if cv2.waitKey(3) & 0xFF == ord('q'):
            break

if __name__ == '__main__':
    try:
        run()
    except rospy.ROSInterruptException:
        pass






