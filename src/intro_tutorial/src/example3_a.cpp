#include "ros/ros.h"
#include "intro_tutorial/srv1.h"

// callback function that take as parameters the request and the response of the service
bool add(intro_tutorial::srv1::Request &req, intro_tutorial::srv1::Response &res) {
    // save the sum of the variable of the request in the response
    res.sum = req.A + req.B + req.C;
    ROS_INFO("request: A=%d, B=%d C=%d", (int)req.A, (int)req.B, (int)req.C);
    ROS_INFO("sending back response: [%ld]", (int)res.sum);
    // return true if everything is ok
    return true;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "add_3_ints_server");
    ros::NodeHandle n;
    // object to which the client connec
    // take as parameter the name and the callback function
    ros::ServiceServer service = n.advertiseService("add_3_ints", add);
    ROS_INFO("Ready to add 3 ints.");
    ros::spin();
    return 0;
}