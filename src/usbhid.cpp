#include "usbhid.h"
#include <pthread.h>
hid_device *handle;
void *usbhidRecDataHandle_thread(void* ptr);
usbHid::usbHid()
{
    int rc1;
    m_distanceMax = 500;//unit mm
    m_distanceMin = 20;//unit mm
    m_distance = 0;
    devs = new struct hid_device_info();
    cur_dev = new struct hid_device_info();
    irecCnt = 0;
    iwriteCnt = 0;
    if(hid_init())
    {
//        return -1;
        printf("init error");
        return;
    }
    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev) {
        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        printf("\n");
        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Release:      %hx\n", cur_dev->release_number);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

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
        printf("unable to open device\n");
        perror("unable to open device");
        return;
    }

    // Read the Manufacturer String
    wstr[0] = 0x0000;
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    if (res < 0)
        printf("Unable to read manufacturer string\n");
    printf("Manufacturer String: %ls\n", wstr);

    // Read the Product String
    wstr[0] = 0x0000;
    res = hid_get_product_string(handle, wstr, MAX_STR);
    if (res < 0)
        printf("Unable to read product string\n");
    printf("Product String: %ls\n", wstr);

    // Read the Serial Number String
    wstr[0] = 0x0000;
    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    if (res < 0)
        printf("Unable to read serial number string\n");
    printf("Serial Number String: (%d) %ls", wstr[0], wstr);
    printf("\n");

    // Read Indexed String 1
    wstr[0] = 0x0000;
    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    if (res < 0)
        printf("Unable to read indexed string 1\n");
    printf("Indexed String 1: %ls\n", wstr);

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
        printf("Unable to send a feature report.\n");
    }

    memset(buf,0,sizeof(buf));

    // Read a Feature Report from the device
    buf[0] = 0x2;
    res = hid_get_feature_report(handle, buf, sizeof(buf));
    if (res < 0) {
        printf("Unable to get a feature report.\n");
        printf("%ls", hid_error(handle));
    }
    else {
        // Print out the returned buffer.
        printf("Feature Report\n   ");
        for (i = 0; i < res; i++)
            printf("%02hhx ", buf[i]);
        printf("\n");
    }

    memset(buf,0,sizeof(buf));

    // Toggle LED (cmd 0x80). The first byte is the report number (0x1).
    buf[0] = 0x1;
    buf[1] = 0x80;
    res = hid_write(handle, buf, 17);
    if (res < 0) {
        printf("Unable to write()\n");
        printf("Error: %ls\n", hid_error(handle));
    }


    // Request state (cmd 0x81). The first byte is the report number (0x1).
    buf[0] = 0x1;
    buf[1] = 0x81;
    hid_write(handle, buf, 17);
    if (res < 0)
        printf("Unable to write() (2)\n");
	if ((rc1 = pthread_create(&usbhidRec_thread, NULL, usbhidRecDataHandle_thread, (void*)(this))))
	{
		printf("Thread2 creation failed: %d\n", rc1);
	}

}

void usbHid::set_mdistance(unsigned int uiDistance)
{
	if((uiDistance > m_distanceMax) || (uiDistance < m_distanceMin))
	{
		m_distance = 0;
	}
	else
	{
		m_distance = uiDistance;
	}
}

void *usbhidRecDataHandle_thread(void* ptr)
{
    static double dDistanceBak;
    double dDistanceCur;
    usbHid *pComm=(usbHid*) ptr;
    printf("usbhidRecDataHandle_thread \n");
    while(true)
    {
		pComm->res = hid_read(handle, pComm->buf, sizeof(pComm->buf));
		if (pComm->res == 0)
			printf("waiting...\n");
		if (pComm->res < 0)
			printf("Unable to read()\n");

		dDistanceCur = ((pComm->buf[1]*256 + pComm->buf[0]) );
		if(dDistanceCur != dDistanceBak)
		{
			 dDistanceBak = dDistanceCur;
			 pComm->set_mdistance(dDistanceCur);
		}
		for(int i = 0; i < 64; i++)
		{
			pComm->buf[i] = i;
		}
		pComm->res = hid_write(handle, pComm->buf, 64);
		if (pComm->res < 0) {
			printf("Unable to write()\n");
			printf("Error: %ls\n", hid_error(handle));
		}
		usleep(1000);//1ms read
    }
}

usbHid::~usbHid()
{

    hid_close(handle);

    /* Free static HIDAPI objects. */
    hid_exit();
}


