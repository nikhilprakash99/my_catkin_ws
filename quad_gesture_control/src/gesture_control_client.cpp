#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <geometry_msgs/Twist.h>
#include <quad_gesture_control/TakeoffAction.h>

#define TAKEOFF_DISTANCE 10
#define TIMEOUT 5.0

typedef actionlib::SimpleActionClient<quad_gesture_control::TakeoffAction> takeoff_ac;

void takeoff()
{
	// create the action client
  // true causes the client to spin its own thread
	takeoff_ac ac("Quad_Takeoff",true);

	ROS_INFO("Waiting for the Quad_Takeoff action server to start.");

	// wait for the action server to start
	ac.waitForServer();

	ROS_INFO("Quad_Takeoff Action server started, sending goal ");

	// send a goal to the action
	quad_gesture_control::TakeoffGoal goal;
	goal.destination.position.z = TAKEOFF_DISTANCE ;
	ac.sendGoal(goal);

	//wait for the action to return
    bool finished_before_timeout = ac.waitForResult(ros::Duration(TIMEOUT));

    if (finished_before_timeout)
    {
      actionlib::SimpleClientGoalState state = ac.getState();
      ROS_INFO("YaY :D Takeoff Successful \n Status: %s",state.toString().c_str());
    }
    else
      ROS_INFO("Takeoff did not finish before the time out :/.");

}

void MessageReceived(const geometry_msgs::Twist& msg) {
  if(msg.linear.x==2)
  	takeoff();
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "gesture_control_client");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/command", 1000,&MessageReceived);

  ros::spin();
}