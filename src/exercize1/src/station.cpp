#include "ros/ros.h"
#include "exercise1/room.h"


void messageCallBack(const exercise1::room::ConstPtr &msg) {
    ROS_INFO("The robot is in the room with ID: [%d], name: [%s]. The current battery is [%d]",
    msg->ID, msg->name.c_str(), msg->battery);
}

int main(int argc, char **argv) {

    ros::init(argc, argv, "station");

    ros::NodeHandle n;
    
    ros::Subscriber sub = n.subscribe("message", 1000, messageCallBack);

    ros::spin();

    return 0;

}