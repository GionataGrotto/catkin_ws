#include "ros/ros.h"
#include "exercise2/rooms.h"
#include <vector>
#include <time.h>

// update the state of the vacuum every second
#define SECONDS_TO_UPDATE_STATE 1

// callback that recive the request and send the respond
bool state(exercise2::rooms::Request &req, exercise2::rooms::Response &res, int &current_room, std::string &name_room, int &battery) {
    res.ID = current_room;
    res.name = name_room;
    res.battery = battery;

    // not used
    res.h.stamp = ros::Time::now();
    res.h.frame_id = "vacuum";

    ROS_INFO("sending back response to station number: [%d]", req.ID_station);
    return true;
}

// every room has a chargin station except room 4
int nearest_charging_station(int room_id) {
    if (room_id == 4) {
        return 3;
    }
    return room_id;
}

// update the state of the vacuum
void update_state(int &room_id, std::string &room_name, int &battery_level, std::vector<std::string> &rooms, std::map<int,std::vector<int>> &possibilities, time_t &t) {
    // get how many seconds passed
    double time_passed = difftime(time(0), t);

    // check if enough time passed
    if (time_passed > SECONDS_TO_UPDATE_STATE) {

        // restart timer
        t = time(0);

        // if the battery level is low go to the nearest charging station
        if (battery_level == 1) {
            room_id = nearest_charging_station(room_id);
            battery_level = 100;
        } else {
            // change room and update battery
            int index = rand() % possibilities[room_id].size();
            room_id = possibilities[room_id][index];
            battery_level--;
        }

        room_name = rooms[room_id-1];
    }
}

int main(int argc, char **argv) {
    
    ros::init(argc, argv, "vacuum");
    ros::NodeHandle n;

    std::vector<std::string> rooms = {"Robot Vision Lab",
                                      "SSL Lab",
                                      "Neurorobotics Lab",
                                      "IAS Lab",
                                      "Autonomous Robotics Lab"};

    std::map<int,std::vector<int>> possibilities {{1, std::vector<int>{1,2,3}},
                                                  {2, std::vector<int>{1,2,3}},
                                                  {3, std::vector<int>{1,2,3,4,5}},
                                                  {4, std::vector<int>{3,4,5}},
                                                  {5, std::vector<int>{3,4,5}}};

    // start from room 1 with full battery
    int current_room = 1;
    std::string name_room = "Robot Vision Lab";
    int battery = 100;

    // keep track of the time
    time_t start = time(0);
    
    ros::ServiceServer service = n.advertiseService<exercise2::rooms::Request,exercise2::rooms::Response>("vacuum_serv", boost::bind(state, _1, _2, boost::ref(current_room), boost::ref(name_room), boost::ref(battery)));

    while (ros::ok()) {

        update_state(current_room,name_room,battery,rooms,possibilities, start);

        ros::spinOnce();
    }
    return 0;

}