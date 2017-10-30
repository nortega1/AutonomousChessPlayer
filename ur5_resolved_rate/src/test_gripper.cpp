#include "ros/ros.h"
#include "ur_msgs/SetIO.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_gripper_client");
  if (argc != 4)
  {
    ROS_INFO("usage: test_gripper_client: function, pin, state");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<ur_msgs::SetIO>("ur_driver/set_io");
  ur_msgs::SetIO srv;
  srv.request.fun = atoll(argv[1]);
  srv.request.pin = atoll(argv[2]);
  srv.request.state = atoll(argv[3]);
  if (client.call(srv))
  {
    ROS_INFO("UR5 response: %b", srv.response.success);
  }
  else
  {
    ROS_ERROR("Failed to call service test_gripper");
    return 1;
  }

  return 0;
}
