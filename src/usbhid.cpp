#include "usbhid.h"
#include <pthread.h>
#include <string.h>
hid_device *handle;
void *usbhidRecDataHandle_thread(void* ptr);
usbHid::usbHid():private_nh_("~")
{
    int rc1;
    private_nh_.param("ultrasonic_distance1_Max", m_distance1_Max, 0.5);
    private_nh_.param("ultrasonic_distance1_Min",m_distance1_Min, 0.02);
    m_distance1 = 0;
    private_nh_.param("ultrasonic_distance2_Max", m_distance2_Max, 0.5);
    private_nh_.param("ultrasonic_distance2_Min",m_distance2_Min, 0.02);
    m_distance2 = 0;
    private_nh_.param("ultrasonic_distance3_Max", m_distance3_Max, 0.5);
    private_nh_.param("ultrasonic_distance3_Min",m_distance3_Min, 0.02);
    m_distance3 = 0;
    private_nh_.param("ultrasonic_distance4_Max", m_distance4_Max, 0.5);
    private_nh_.param("ultrasonic_distance4_Min",m_distance4_Min, 0.02);
    m_distance4 = 0;
    devs = new struct hid_device_info();
    cur_dev = new struct hid_device_info();
    irecCnt = 0;
    iwriteCnt = 0;
    if(hid_init())
    {
//        return -1;
        printf("init usb hid error \n");
        return;
    }
//    devs = hid_enumerate(0x0, 0x0);
//    cur_dev = devs;
//    while (cur_dev) {
//        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
//        printf("\n");
//        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
//        printf("  Product:      %ls\n", cur_dev->product_string);
//        printf("  Release:      %hx\n", cur_dev->release_number);
//        printf("  Interface:    %d\n",  cur_dev->interface_number);
//        printf("\n");
//        cur_dev = cur_dev->next;
//    }
//    hid_free_enumeration(devs);

    // Set up the command buffer.
    memset(buf,0x00,sizeof(buf));
    buf[0] = 0x01;
    buf[1] = 0x81;
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    ////handle = hid_open(0x4d8, 0x3f, L"12345");
    //handle = hid_open(0x4d8, 0x3f, NULL);
    handle = hid_open(0x483, 0x5750, NULL); //stm 32 VID PID
    if (!handle) {
//        printf("unable to open device\n");
//        perror("unable to open device");
        return;
    }
    // Read the Manufacturer String
    wstr[0] = 0x0000;
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    if (res < 0)
    {
//        printf("Unable to read manufacturer string\n");
    }
//    printf("Manufacturer String: %ls\n", wstr);

    // Read the Product String
    wstr[0] = 0x0000;
    res = hid_get_product_string(handle, wstr, MAX_STR);
    if (res < 0)
    {
//        printf("Unable to read product string\n");
    }
//    printf("Product String: %ls\n", wstr);

    // Read the Serial Number String
    wstr[0] = 0x0000;
    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    if (res < 0)
    {
    	//        printf("Unable to read serial number string\n");
    }
//    printf("Serial Number String: (%d) %ls", wstr[0], wstr);
//    printf("\n");

    // Read Indexed String 1
    wstr[0] = 0x0000;
    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    if (res < 0)
    {
    	//        printf("Unable to read indexed string 1\n");
    }
//    printf("Indexed String 1: %ls\n", wstr);

    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1);

    // Try to read from the device. There shoud be no
    // data here, but execution should not block.
    res = hid_read(handle, buf, 17);

    // Send a Feature Report to the device
    buf[0] = 0x2;
    buf[1] = 0xa0;
    buf[2] = 0x0a;
    buf[3] = 0x00;
    buf[4] = 0x00;
    res = hid_send_feature_report(handle, buf, 17);
    if (res < 0) {
//        printf("Unable to send a feature report.\n");
    }

    memset(buf,0,sizeof(buf));

    // Read a Feature Report from the device
    buf[0] = 0x2;
    res = hid_get_feature_report(handle, buf, sizeof(buf));
    if (res < 0) {
//        printf("Unable to get a feature report.\n");
//        printf("%ls", hid_error(handle));
    }
    else {
        // Print out the returned buffer.
//        printf("Feature Report\n   ");
//        for (i = 0; i < res; i++)
//            printf("%02hhx ", buf[i]);
//        printf("\n");
    }

    memset(buf,0,sizeof(buf));

    // Toggle LED (cmd 0x80). The first byte is the report number (0x1).
    buf[0] = 0x1;
    buf[1] = 0x80;
    res = hid_write(handle, buf, 17);
    if (res < 0) {
//        printf("Unable to write()\n");
//        printf("Error: %ls\n", hid_error(handle));
    }


    // Request state (cmd 0x81). The first byte is the report number (0x1).
    buf[0] = 0x1;
    buf[1] = 0x81;
    hid_write(handle, buf, 17);
    if (res < 0)
    {
    	//        printf("Unable to write() (2)\n");
    }
	if ((rc1 = pthread_create(&usbhidRec_thread, NULL, usbhidRecDataHandle_thread, (void*)(this))))
	{
//		printf("Thread2 creation failed: %d\n", rc1);
	}

}
double usbHid::get_mdistance(int index)
{
	double d_distance;
	switch(index)
	{
	case 1:
		d_distance = m_distance1;
		break;
	case 2:
		d_distance = m_distance2;
		break;
	case 3:
		d_distance = m_distance3;
		break;
	case 4:
		d_distance = m_distance4;
		break;
	default:
		d_distance = 0;
		break;
	}
	return d_distance;
}

double usbHid::get_mdistanceMax(int index)
{
	double d_distance_Max;
	switch(index)
	{
	case 1:
		d_distance_Max = m_distance1_Max;
		break;
	case 2:
		d_distance_Max = m_distance2_Max;
		break;
	case 3:
		d_distance_Max = m_distance3_Max;
		break;
	case 4:
		d_distance_Max = m_distance4_Max;
		break;
	default:
		d_distance_Max = 0;
		break;
	}
	return d_distance_Max;
}


double usbHid::get_mdistanceMin(int index)
{
	double d_distance_Min;
	switch(index)
	{
	case 1:
		d_distance_Min = m_distance1_Min;
		break;
	case 2:
		d_distance_Min = m_distance2_Min;
		break;
	case 3:
		d_distance_Min = m_distance3_Min;
		break;
	case 4:
		d_distance_Min = m_distance4_Min;
		break;
	default:
		d_distance_Min = 0;
		break;
	}
	return d_distance_Min;
}

void usbHid::set_mdistance(unsigned int uiDistance, int index)
{
	double d_Distance;
	d_Distance = uiDistance * 0.001;
	switch(index)
	{
	case 1:
		if((d_Distance > m_distance1_Max) || (d_Distance < m_distance1_Min))
		{
			m_distance1 = 0;
		}
		else
		{
			m_distance1 = d_Distance;
		}
		break;
	case 2:
		if((d_Distance > m_distance2_Max) || (d_Distance < m_distance2_Min))
		{
			m_distance2 = 0;
		}
		else
		{
			m_distance2 = d_Distance;
		}
		break;
	case 3:
		if((d_Distance > m_distance3_Max) || (d_Distance < m_distance3_Min))
		{
			m_distance3 = 0;
		}
		else
		{
			m_distance3 = d_Distance;
		}
		break;
	case 4:
		if((d_Distance > m_distance4_Max) || (d_Distance < m_distance4_Min))
		{
			m_distance4 = 0;
		}
		else
		{
			m_distance4 = d_Distance;
		}
		break;
	default:
		break;
	}

}

void *usbhidRecDataHandle_thread(void* ptr)
{
    static double dDistanceBak1,dDistanceBak2,dDistanceBak3,dDistanceBak4;
    static bool bUsbEnable = true;
    double dDistanceCur1,dDistanceCur2,dDistanceCur3,dDistanceCur4;
    usbHid *pComm=(usbHid*) ptr;
//    printf("usbhidRecDataHandle_thread \n");
    while(bUsbEnable)
    {
		pComm->res = hid_read(handle, pComm->buf, sizeof(pComm->buf));
		if ((pComm->res == 0) || (pComm->res < 0))
		{
			printf("waiting...\n");
			memset(pComm->buf,0,sizeof(pComm->buf) / sizeof(unsigned char));
		}
		dDistanceCur1 = ((pComm->buf[1]*256 + pComm->buf[0]) );
		dDistanceCur2 = ((pComm->buf[3]*256 + pComm->buf[2]) );
		dDistanceCur3 = ((pComm->buf[5]*256 + pComm->buf[4]) );
		dDistanceCur4 = ((pComm->buf[7]*256 + pComm->buf[6]) );
		if(dDistanceCur1 != dDistanceBak1)
		{
			 dDistanceBak1 = dDistanceCur1;
			 pComm->set_mdistance(dDistanceCur1,1);
		}
		if(dDistanceCur2 != dDistanceBak2)
		{
			 dDistanceBak2 = dDistanceCur2;
			 pComm->set_mdistance(dDistanceCur2,2);
		}
		if(dDistanceCur3 != dDistanceBak3)
		{
			 dDistanceBak3 = dDistanceCur3;
			 pComm->set_mdistance(dDistanceCur3,3);
		}
		if(dDistanceCur4 != dDistanceBak4)
		{
			 dDistanceBak4 = dDistanceCur4;
			 pComm->set_mdistance(dDistanceCur4,4);
		}
		for(int i = 0; i < 64; i++)
		{
			pComm->buf[i] = i;
		}
		pComm->res = hid_write(handle, pComm->buf, 64);
		if (pComm->res < 0) {
			bUsbEnable =  false;
			hid_close(handle);
			hid_exit();
			printf("Unable to write()\n");
			memset(pComm->buf,0,sizeof(pComm->buf) / sizeof(unsigned char));
			printf("Error: %ls\n", hid_error(handle));
		}
		usleep(20000);//1ms read
    }
}

usbHid::~usbHid()
{

	printf("hid_close\n");
    hid_close(handle);

    /* Free static HIDAPI objects. */
    hid_exit();
}


