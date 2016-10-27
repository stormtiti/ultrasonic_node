#ifndef USBHID_H
#define USBHID_H


#include <stdio.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "hidapi.h"
#include <sys/stat.h>
#include <ros/ros.h>
#define VENDOR_ID  0x1314
#define PRODUCT_ID 0x1003
#define MAX_STR 255
//#define ULTRASONIC_DEBUG
class usbHid
{

public:
     usbHid();
    ~usbHid();
    int res;
    unsigned char buf[256];
    double get_mdistance(int index);
    double get_mdistanceMax(int index);
    double get_mdistanceMin(int index);
    void set_mdistance(unsigned int uiDistance,int index);
private:


    wchar_t wstr[MAX_STR];

    int i;
    struct hid_device_info *devs, *cur_dev;

    int irecCnt;
    int iwriteCnt;
    pthread_t usbhidRec_thread;
    ros::NodeHandle private_nh_;
    double m_distance_Max;
    double m_distance_Min;
    double m_distance1;
    double m_distance2;
    double m_distance3;
    double m_distance4;

};
#endif // USBHID_H
