#include <ros/ros.h>
#include <string>

int main(int argc, char  **argv)
{
	ros::init(argc,argv,"Display");
	ros::NodeHandle nh;

	std::string s;

	nh.getParam("mode",s);

	ROS_INFO(" mode = %s ", s.c_str());

	return 0;
}