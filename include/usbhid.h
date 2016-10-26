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
#define VENDOR_ID  0x1314
#define PRODUCT_ID 0x1003
#define MAX_STR 255
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
    unsigned int m_distance1;
    unsigned int m_distance1_Max;
    unsigned int m_distance1_Min;
    unsigned int m_distance2;
    unsigned int m_distance2_Max;
    unsigned int m_distance2_Min;
    unsigned int m_distance3;
    unsigned int m_distance3_Max;
    unsigned int m_distance3_Min;
    unsigned int m_distance4;
    unsigned int m_distance4_Max;
    unsigned int m_distance4_Min;
};
#endif // USBHID_H
