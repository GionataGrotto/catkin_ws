#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <exercise3/ChargerAction.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "station");

    actionlib::SimpleActionClient<exercise3::ChargerAction> ac("charger", true);

    ROS_INFO("Waiting for action server to start");

    ac.waitForServer();

    ROS_INFO("Action server started, sending goal");

    exercise3::ChargerGoal goal;

    goal.battery = 100;
    goal.h.stamp = ros::Time::now();
    goal.h.frame_id = "Start charging";

    ac.sendGoal(goal);

    bool finished_before_timeout = ac.waitForResult(ros::Duration(70.0));

    if (finished_before_timeout) {
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished: %s",state.toString().c_str());
    } else {
        ROS_INFO("Action did not finish before the time out");
    }

    return 0;
}