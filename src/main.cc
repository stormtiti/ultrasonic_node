/*
 * main.cc
 *
 *  Created on: Sep 12, 2016
 *      Author: root
 */
// ros header file
#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>
#include <tf/message_filter.h>
#include <sensor_msgs/Range.h>
#include "usbhid.h"
// local header file

int main(int argc, char **argv){

  ros::init(argc, argv, "ultrasonic_node");
  int odomPublish_Hz;
  ros::NodeHandle nh;
  sensor_msgs::Range ultrasonic_msg_1;
  ros::Publisher     ultrasonic_pub = nh.advertise<sensor_msgs::Range>   ("Range", 50);
  sleep(1);

  ros::Rate loop_rate(20);
  usbHid myusbhid;
  while (ros::ok())
  {
	ros::WallTime start = ros::WallTime::now();
	ultrasonic_msg_1.header.stamp = ros::Time::now();
	ultrasonic_msg_1.header.frame_id = "ultra_range";
	ultrasonic_msg_1.max_range = myusbhid.get_mdistanceMax();
	ultrasonic_msg_1.min_range = myusbhid.get_mdistanceMin();
	if(myusbhid.get_mdistance() == 0)
	{
		ultrasonic_msg_1.range = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		ultrasonic_msg_1.range = myusbhid.get_mdistance();
	}
	ultrasonic_pub.publish(ultrasonic_msg_1);
    ros::spinOnce();
    loop_rate.sleep();
    ros::WallTime stop = ros::WallTime::now();
  }
  return 0;
}




