#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <exercise3/ChargerAction.h>

#define LOW_BATTERY_LEVEL 5
#define SECONDS_IN_A_MINUTE 60

class ChargerAction {
protected:
    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<exercise3::ChargerAction> as_;
    std::string action_name_;
    exercise3::ChargerFeedback feedback_;
    exercise3::ChargerResult result_;
    int low_battery_level;

public:
    ChargerAction(std::string name) : as_(nh_, name, boost::bind(&ChargerAction::charge, this, _1), false), action_name_(name), low_battery_level(LOW_BATTERY_LEVEL) {
        as_.start();
    }
    ~ChargerAction(void){}

    void charge(const exercise3::ChargerGoalConstPtr &goal) {
        int difference_form_goal = goal->battery - low_battery_level;
        double rate = difference_form_goal / SECONDS_IN_A_MINUTE;
        ros::Rate loop_rate(rate);
        bool success = true;
        feedback_.current_battery = low_battery_level;
        feedback_.h.stamp = ros::Time::now();
        feedback_.h.frame_id = "vacuum";
        for (int i = 0; i < difference_form_goal; i++) {
            if (as_.isPreemptRequested() || !ros::ok()) {
                ROS_INFO("%s: Preempted", action_name_.c_str());
                as_.setPreempted();
                success = false;
                break;
            }
            feedback_.current_battery += 1;
            feedback_.h.stamp = ros::Time::now();
            feedback_.h.frame_id = "vacuum";
            as_.publishFeedback(feedback_);
            loop_rate.sleep();
        }
        result_.success = success;
        result_.h.stamp = ros::Time::now();
        if (success) {
            result_.h.frame_id = "The vacuum is now charged";
            as_.setSucceeded(result_);
        } else {
            result_.h.frame_id = "Failed to charge the vacuum";
        }
        
    }
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "vacuum");

    ChargerAction charger("charger");

    ros::spin();


    return 0;
}