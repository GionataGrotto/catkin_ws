#include "ros/ros.h"
#include "exercise2/rooms.h"


int main(int argc, char **argv) {
    
    ros::init(argc, argv, "station");

    ros::NodeHandle n;
    
    // variables to get value from launch files
    int ID; // ID of the station
    double rate; // rate of the station

    // get the name of the node
    std::string name_node = ros::this_node::getName().substr(ros::this_node::getNamespace().size());
    // get ID of the node
    ros::param::get("/"+name_node+"/ID", ID);
    // get rate of the node
    ros::param::get("/"+name_node+"/freq", rate);


    ros::ServiceClient client = n.serviceClient<exercise2::rooms>("vacuum_serv");

    ros::Rate loop_rate(rate);

    while(ros::ok()) {
        exercise2::rooms srv;
        srv.request.ID_station = ID;

        // not used
        srv.request.h.stamp = ros::Time::now();
        srv.request.h.frame_id = name_node;
        
        if (client.call(srv)) {
            ROS_INFO("The vacuum current state is: room ID = %d, room name = %s, battery level = %d", 
            (int)srv.response.ID, srv.response.name.c_str(), (int)srv.response.battery);
        } else {
            ROS_ERROR("Failed to contact the vacuum");
            return 1;
        }

        ros::spinOnce();

        loop_rate.sleep();
    }

    
    return 0;

}