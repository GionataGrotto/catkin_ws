#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char **argv) {
    // initialize ros to use parameters
    ros::init(argc, argv, "example1_a");

    // initialize node
    ros::NodeHandle n;

    // generate publisher to publish messages
    // first parameter define the name of the publisher
    // second parameter define the dimension of the buffer
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("message", 1000);

    // frequency of loop
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        // create a standard message
        std_msgs::String msg;

        // create a string
        std::stringstream ss;
        ss << " Hello World! ";

        // assign the string to the message
        msg.data = ss.str();

        // print our message to check
        ROS_INFO("%s", msg.data.c_str());

        // publish the message in the publisher object
        chatter_pub.publish(msg);
        ros::spinOnce();

        // to have the frequency specified in loop_rate
        loop_rate.sleep();
    }
    return 0; 
}