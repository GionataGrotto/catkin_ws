#include "ros/ros.h"
#include "exercize1/room.h"
#include <sstream>
#include <vector>

int main(int argc, char **argv) {

    ros::init(argc, argv, "vacuum");

    ros::NodeHandle n;

    ros::Publisher p = n.advertise<exercize1::room>("message", 1000);

    ros::Rate loop_rate(5);

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

    int ID = 1;
    std::string room_name = rooms[ID-1];
    int battery = 100;

    while(ros::ok()) {
        exercize1::room msg;

        msg.ID = ID;
        msg.name = room_name;
        msg.battery = battery;

        if (battery == 1) {
            ID = 1;
            battery = 100;
        } else {
            int index = rand() % possibilities[ID].size();
            ID = possibilities[ID][index];
            battery--;
        }

        room_name = rooms[ID-1];

        p.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;

}