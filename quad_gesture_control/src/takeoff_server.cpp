#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <quad_gesture_control/TakeoffAction.h>
#include <geometry_msgs/PoseStamped.h>

typedef actionlib::SimpleActionServer<quad_gesture_control::TakeoffAction> takeoff_as;

class TakeoffAction
{
protected:

  ros::NodeHandle nh;
  takeoff_as as; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
  std::string action_name;
  // create messages that are used to published feedback/result
  quad_gesture_control::TakeoffFeedback feedback;
  quad_gesture_control::TakeoffResult result;

public:

  TakeoffAction(std::string name) :
    as(nh, name, boost::bind(&TakeoffAction::execute, this, _1), false),
    action_name(name)
  {
    as.start();
  }

  ~TakeoffAction(void)
  {
  }

  void execute(const quad_gesture_control::TakeoffGoalConstPtr &goal)
  {
    // helper variables
    ros::Rate r(1);
    bool success = true;

    geometry_msgs::PoseStamped currentlocation;

    currentlocation.pose.position.z = 0;
    feedback.trajectory.poses.push_back(currentlocation);

    // publish info to the console for the user
    ROS_INFO("Executing Quad_Takeoff Action, executing Takeoff to a distance of %fM",goal->destination.position.z);

    int i;
    // start executing the action
    for( i=1; i<=goal->destination.position.z; i++)
    {
      // check that preempt has not been requested by the client
      if (as.isPreemptRequested() || !ros::ok())
      {
        ROS_INFO("%s: Preempted", action_name.c_str());
        // set the action state to preempted
        as.setPreempted();
        success = false;
        break;
      }

      currentlocation.pose.position.z = i;
      feedback.trajectory.poses.push_back(currentlocation);
      as.publishFeedback(feedback);
      r.sleep();
    }

    if(success)
    {
      result.currrent_location.position.z = feedback.trajectory.poses[i-1].pose.position.z;
      ROS_INFO("%s:Quad_Takeoff ACtion Succeeded \n Result : %f", action_name.c_str(),result.currrent_location.position.z);
      // set the action state to succeeded
     as.setSucceeded(result);
    }
  }

};

int main(int argc, char** argv)
{
  ros::init(argc, argv,"Quad_Takeoff");

  TakeoffAction Quad_Takeoff("Quad_Takeoff");
  ros::spin();

  return 0;
}