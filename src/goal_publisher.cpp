#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <std_msgs/Int8.h>

float tolerance = 1.0;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
std::vector<move_base_msgs::MoveBaseGoal> goals;
tf::TransformListener *listener;
ros::Publisher pub;

//navigates to a goal
void navigate(const std_msgs::BoolConstPtr& nav) { // begins navigation

  if (!nav) { // if given a false, cancel navigation and clear the goal list
    goals.clear();
    return;
  }

  MoveBaseClient ac("move_base", true);
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server");
  } 
  ROS_INFO("Proceeding to next goal");

  auto it = goals.begin();// jackal navigates to next goal
  if(it == goals.end()){
    ROS_INFO("No more goals in list");
    return;
  }
  ROS_INFO("Navigating to goal");
  ac.sendGoal(*it);
  ac.waitForResult(ros::Duration(0,0));

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("The jackal reached the goal");
  }
  else if(ac.getState() == actionlib::SimpleClientGoalState::ACTIVE) {
    ROS_INFO("The goal state is ACTIVE");
  }
  else if(ac.getState() == actionlib::SimpleClientGoalState::PENDING) {
    ROS_INFO("The goal state is PENDING");
  }
  else if(ac.getState() == actionlib::SimpleClientGoalState::ABORTED) {
    ROS_INFO("The goal state is ABORTED");
  }
  else if(ac.getState() == actionlib::SimpleClientGoalState::RECALLED) {
    ROS_INFO("The goal state is RECALLED");
  }
  else if(ac.getState() == actionlib::SimpleClientGoalState::PREEMPTED) {
    ROS_INFO("The goal state is PREEMPTED");
  }
  else if(ac.getState() == actionlib::SimpleClientGoalState::REJECTED) {
    ROS_INFO("The goal state is REJECTED");
  }
  else {
    ROS_INFO("The jackal failed to reach the goal");
  }

  goals.erase(goals.begin());

  //continue to the next goal
  std_msgs::Bool msg;
  msg.data = true;
  pub.publish(msg);

}

void addGoal(float x, float y, float z, float w) { // adds goal to the list from the cb_goal topic
  
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;
  goal.target_pose.pose.position.z = 0.0;
  goal.target_pose.pose.orientation.x = 0.0;
  goal.target_pose.pose.orientation.y = 0.0;
  goal.target_pose.pose.orientation.z = z;
  goal.target_pose.pose.orientation.w = w;
  
  goals.push_back(goal);

  ROS_INFO("added goal");  
}

int main(int argc, char **argv) 
{
  
  ros::init(argc, argv, "goal_publisher");
  ros::NodeHandle nh("~");
  listener = new tf::TransformListener();
  nh.setParam("move_base/actionlib_client_sub_queue_size", 30);
  nh.setParam("move_base/actionlib_client_pub_queue_size", 30);

  ros::Subscriber sub = nh.subscribe("navigate", 1000, navigate); // subscribes to "navigate", move to next goal
  pub = nh.advertise<std_msgs::Bool>("navigate", 1000);
  
  std::vector<float> goal_list;
  nh.getParam("goals", goal_list);

  ROS_INFO_STREAM("Parameters:" << std::endl <<
            "tolerance: " << tolerance << std::endl <<
            "goal_list size: " << goal_list.size()
            );

  if(goal_list.size() > 0 && goal_list.size() % 4 == 0){
    for(int i = 0; i < goal_list.size(); i = i+4){
      addGoal(goal_list[i+0], goal_list[i+1], goal_list[i+2], goal_list[i+3]);
    }
  }
  
  ros::spin();
  return 0;
}
