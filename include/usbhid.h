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
    double get_mdistance(){return m_distance;}
    double get_mdistanceMax(){return m_distanceMax;}
    double get_mdistanceMin(){return m_distanceMin;}
    void set_mdistance(unsigned int uiDistance);
private:


    wchar_t wstr[MAX_STR];

    int i;
    struct hid_device_info *devs, *cur_dev;

    int irecCnt;
    int iwriteCnt;
    pthread_t usbhidRec_thread;
    unsigned int m_distance;
    unsigned int m_distanceMax;
    unsigned int m_distanceMin;
};
#endif // USBHID_H
