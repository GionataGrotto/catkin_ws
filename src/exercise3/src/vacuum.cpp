#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <exercise3/ChargerAction.h>


class ChargerAction {
protected:
    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<exercise3::ChargerAction> as_;
    std::string action_name_;

    exercise3::ChargerFeedback feedback_;
    exercise3::ChargerResult result_;

    // consider this level of battery when the goal is received
    const int low_battery_level = 5;

public:
    ChargerAction(std::string name) : as_(nh_, name, boost::bind(&ChargerAction::charge, this, _1), false), action_name_(name) {
        as_.start();
    }
    ~ChargerAction(void){}

    void charge(const exercise3::ChargerGoalConstPtr &goal) {

        // get the difference between the current battery level and the goal
        int difference_form_goal = goal->battery - low_battery_level;
        int counter_difference = difference_form_goal;

        // to compute the rate
        const int SECONDS_IN_A_MINUTE = 60;

        // the battery require a minute to recharge
        double rate = static_cast<double>(difference_form_goal) / SECONDS_IN_A_MINUTE;

        ros::Rate loop_rate(rate);

        // to check if the action succeeded
        bool success = true;
        feedback_.current_battery = low_battery_level;
        feedback_.h.stamp = ros::Time::now();
        feedback_.h.frame_id = "The vacuum will be rechared in " + std::to_string(counter_difference--) + " timestep";
        for (int i = 0; i < difference_form_goal; i++) {
            if (as_.isPreemptRequested() || !ros::ok()) {
                ROS_INFO("%s: Preempted", action_name_.c_str());
                as_.setPreempted();
                success = false;
                break;
            }
            feedback_.current_battery += 1;
            feedback_.h.stamp = ros::Time::now();
            feedback_.h.frame_id = "The vacuum will be rechared in " + std::to_string(counter_difference--) + " timestep";
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