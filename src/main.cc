/*******************************************************************************
 * Copyright(C) Pi2E . All rights reserved 2016
 *
 * @file main.cc
 *
 * @brief ultrasonic
 * 		  1) publish ultrasonic topic
 *
 * @author Hyman XI <twlk8634109@126.com>
 * @version 1.0.0.0
 * @date oct 27,2016
 *
 * Maintained by Hyman XI
 ***********************************************************************************/
#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>
#include <tf/message_filter.h>
#include <sensor_msgs/Range.h>
#include "usbhid.h"
// local header file

int main(int argc, char **argv){

  ros::init(argc, argv, "ultrasonic_multiple");
  int odomPublish_Hz;
  ros::NodeHandle nh;
  sensor_msgs::Range ultrasonic_msg_1;
  sensor_msgs::Range ultrasonic_msg_2;
  sensor_msgs::Range ultrasonic_msg_3;
  sensor_msgs::Range ultrasonic_msg_4;
  ros::Publisher     ultrasonic_pub1 = nh.advertise<sensor_msgs::Range>   ("ultrasonic_msg_1", 50);
  ros::Publisher     ultrasonic_pub2 = nh.advertise<sensor_msgs::Range>   ("ultrasonic_msg_2", 50);
  ros::Publisher     ultrasonic_pub3 = nh.advertise<sensor_msgs::Range>   ("ultrasonic_msg_3", 50);
  ros::Publisher     ultrasonic_pub4 = nh.advertise<sensor_msgs::Range>   ("ultrasonic_msg_4", 50);
  ROS_INFO("[Ultrasonic] ultrasonic Initialization ...");
  sleep(1);
  ros::Rate loop_rate(5);
  usbHid myusbhid;
  while (ros::ok())
  {
	ros::WallTime start = ros::WallTime::now();
	ultrasonic_msg_1.header.stamp = ros::Time::now();
	ultrasonic_msg_1.header.frame_id = "ultrasonic_msg_1";
	ultrasonic_msg_1.radiation_type = sensor_msgs::Range::ULTRASOUND;
	ultrasonic_msg_1.field_of_view = 0.26; // 15 deg
	ultrasonic_msg_1.max_range = myusbhid.get_mdistanceMax(1);
	ultrasonic_msg_1.min_range = myusbhid.get_mdistanceMin(1);
	if(myusbhid.get_mdistance(1) == 0)
	{
		ultrasonic_msg_1.range = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		ultrasonic_msg_1.range = myusbhid.get_mdistance(1);
	}

	ultrasonic_msg_2.header.stamp = ros::Time::now();
	ultrasonic_msg_2.header.frame_id = "ultrasonic_msg_2";
	ultrasonic_msg_2.radiation_type = sensor_msgs::Range::ULTRASOUND;
	ultrasonic_msg_2.field_of_view = 0.26; // 15 deg
	ultrasonic_msg_2.max_range = myusbhid.get_mdistanceMax(2);
	ultrasonic_msg_2.min_range = myusbhid.get_mdistanceMin(2);
	if(myusbhid.get_mdistance(2) == 0)
	{
		ultrasonic_msg_2.range = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		ultrasonic_msg_2.range = myusbhid.get_mdistance(2);
	}

	ultrasonic_msg_3.header.stamp = ros::Time::now();
	ultrasonic_msg_3.header.frame_id = "ultrasonic_msg_3";
	ultrasonic_msg_3.radiation_type = sensor_msgs::Range::ULTRASOUND;
	ultrasonic_msg_3.field_of_view = 0.26; // 15 deg
	ultrasonic_msg_3.max_range = myusbhid.get_mdistanceMax(3);
	ultrasonic_msg_3.min_range = myusbhid.get_mdistanceMin(3);
	if(myusbhid.get_mdistance(3) == 0)
	{
		ultrasonic_msg_3.range = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		ultrasonic_msg_3.range = myusbhid.get_mdistance(3);
	}

	ultrasonic_msg_4.header.stamp = ros::Time::now();
	ultrasonic_msg_4.header.frame_id = "ultrasonic_msg_4";
	ultrasonic_msg_4.radiation_type = sensor_msgs::Range::ULTRASOUND;
	ultrasonic_msg_4.field_of_view = 0.26; // 15 deg
	ultrasonic_msg_4.max_range = myusbhid.get_mdistanceMax(4);
	ultrasonic_msg_4.min_range = myusbhid.get_mdistanceMin(4);
	if(myusbhid.get_mdistance(4) == 0)
	{
		ultrasonic_msg_4.range = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		ultrasonic_msg_4.range = myusbhid.get_mdistance(4);
	}

	ultrasonic_pub1.publish(ultrasonic_msg_1);
	ultrasonic_pub2.publish(ultrasonic_msg_2);
	ultrasonic_pub3.publish(ultrasonic_msg_3);
	ultrasonic_pub4.publish(ultrasonic_msg_4);
    ros::spinOnce();
    loop_rate.sleep();
    ros::WallTime stop = ros::WallTime::now();
  }
  return 0;
}




