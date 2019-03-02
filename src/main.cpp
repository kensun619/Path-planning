#include <uWS/uWS.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "helpers.h"
#include "json.hpp"
#include "spline.h"
#include "BehaviorTree.hpp"
// for convenience
using nlohmann::json;
using std::string;
using std::vector;
using namespace std;
using namespace helpers;

int main() {
    uWS::Hub h;
    
    // Load up map values for waypoint's x,y,s and d normalized normal vectors
    vector<double> map_waypoints_x;
    vector<double> map_waypoints_y;
    vector<double> map_waypoints_s;
    vector<double> map_waypoints_dx;
    vector<double> map_waypoints_dy;
    
    // Waypoint map to read from
    string map_file_ = "../data/highway_map.csv";
    // The max s value before wrapping around the track back to 0
    double max_s = 6945.554;
    
    std::ifstream in_map_(map_file_.c_str(), std::ifstream::in);
    
    string line;
    while (getline(in_map_, line)) {
        std::istringstream iss(line);
        double x;
        double y;
        float s;
        float d_x;
        float d_y;
        iss >> x;
        iss >> y;
        iss >> s;
        iss >> d_x;
        iss >> d_y;
        map_waypoints_x.push_back(x);
        map_waypoints_y.push_back(y);
        map_waypoints_s.push_back(s);
        map_waypoints_dx.push_back(d_x);
        map_waypoints_dy.push_back(d_y);
    }
    int lane = 1;
    double ref_vel = 0;
    
    
    
    Map map;
    map.waypoints_x = map_waypoints_x;
    map.waypoints_y = map_waypoints_y;
    map.waypoints_s = map_waypoints_s;
    map.waypoints_dx = map_waypoints_dx;
    map.waypoints_dy = map_waypoints_dy;
    
    
    Selector *root = new Selector;  // Note that root can be either a Sequence or a Selector, since it has only one child.
    Sequence * laneSwitch0 = new Sequence;  // In general there will be several nodes that are Sequence or Selector, so they should be suffixed by an integer to distinguish between them.
    IsLaneNumberTask * isLane0Task  = new IsLaneNumberTask(1);  // The door is initially closed and 5 meters away.
    Sequence *switchConditionsLane1 = new Sequence;  // Note that root can be either a Sequence or a Selector, since it has only one child.
    IsOtherLaneFeasibleTask *isOtherLaneFeasibleTaskFrom0 = new IsOtherLaneFeasibleTask(0);
    IsSomeoneCloseBeforeYouTask *isSomeoneCloseBeforeYouTask = new IsSomeoneCloseBeforeYouTask(1);
    //ChangeVelocityToTheClosestInLane *changeVelocityToTheClosestInLane  = new ChangeVelocityToTheClosestInLane(1);
    DriveTask *drive1_0 = new DriveTask(0);

    Sequence * laneSwitch2 = new Sequence;  // In general there will be several nodes that are Sequence or Selector, so they should be suffixed by an integer to distinguish between them.
    IsLaneNumberTask * isLane2Task  = new IsLaneNumberTask(1);  // The door is initially closed and 5 meters away.
    Sequence *switchConditionsLane1_2 = new Sequence;  // Note that root can be either a Sequence or a Selector, since it has only one child.
    IsOtherLaneFeasibleTask *isOtherLaneFeasibleTaskFrom1_2 = new IsOtherLaneFeasibleTask(2);
    IsSomeoneCloseBeforeYouTask *isSomeoneCloseBeforeYouTask1_2 = new IsSomeoneCloseBeforeYouTask(1);
    //ChangeVelocityToTheClosestInLane *changeVelocityToTheClosestInLane  = new ChangeVelocityToTheClosestInLane(1);
    DriveTask *drive1_2 = new DriveTask(2);
    
    
    Selector * laneSwitch1 = new Selector;  // In general there will be several nodes that are Sequence or Selector, so they should be suffixed by an integer to distinguish between them.
    
    Sequence * laneSwitch2_1 = new Sequence;
    IsLaneNumberTask * isLaneTask2_1  = new IsLaneNumberTask(2);  // The door is initially closed and 5 meters away.
    Sequence *switchConditionsLane2_1 = new Sequence;  // Note that root can be either a Sequence or a Selector, since it has only one child.
    IsOtherLaneFeasibleTask *isOtherLaneFeasibleTaskFrom2_1 = new IsOtherLaneFeasibleTask(1);
    IsSomeoneCloseBeforeYouTask *isSomeoneCloseBeforeYouTaskFrom2_1 = new IsSomeoneCloseBeforeYouTask(2);
    DriveTask *drive2_1 = new DriveTask(1);
    
    Sequence * laneSwitch0_1 = new Sequence;
    IsLaneNumberTask * isLaneTask0_1  = new IsLaneNumberTask(0);  // The door is initially closed and 5 meters away.
    Sequence *switchConditionsLane0_1 = new Sequence;  // Note that root can be either a Sequence or a Selector, since it has only one child.
    IsOtherLaneFeasibleTask *isOtherLaneFeasibleTaskFrom0_1 = new IsOtherLaneFeasibleTask(1);
    IsSomeoneCloseBeforeYouTask *isSomeoneCloseBeforeYouTaskFrom0_1 = new IsSomeoneCloseBeforeYouTask(0);
    DriveTask *drive0_1 = new DriveTask(1);
    
    
    root->addChild(laneSwitch0);
    
    laneSwitch0->addChild (isLane0Task);
    laneSwitch0->addChild (switchConditionsLane1);
    laneSwitch0->addChild(drive1_0);
    switchConditionsLane1->addChild (isSomeoneCloseBeforeYouTask);
    switchConditionsLane1->addChild (isOtherLaneFeasibleTaskFrom0);
    
    root->addChild(laneSwitch2);
    laneSwitch2->addChild (isLane2Task);
    laneSwitch2->addChild (switchConditionsLane1_2);
    laneSwitch2->addChild(drive1_2);
    switchConditionsLane1_2->addChild (isSomeoneCloseBeforeYouTask1_2);
    switchConditionsLane1_2->addChild (isOtherLaneFeasibleTaskFrom1_2);
    
    root->addChild(laneSwitch1);
    laneSwitch1->addChild(laneSwitch2_1);
    laneSwitch1->addChild(laneSwitch0_1);
//
    laneSwitch2_1->addChild(isLaneTask2_1);
    laneSwitch2_1->addChild(switchConditionsLane2_1);
    laneSwitch2_1->addChild(drive2_1);
    switchConditionsLane2_1->addChild(isSomeoneCloseBeforeYouTaskFrom2_1);

    switchConditionsLane2_1->addChild(isOtherLaneFeasibleTaskFrom2_1);
//
    laneSwitch0_1->addChild(isLaneTask0_1);
    laneSwitch0_1->addChild(switchConditionsLane0_1);
    laneSwitch0_1->addChild(drive0_1);
    switchConditionsLane0_1->addChild(isSomeoneCloseBeforeYouTaskFrom0_1);
    switchConditionsLane0_1->addChild(isOtherLaneFeasibleTaskFrom0_1);
//

    root->addChild(new DriveTask(-1));
    
    h.onMessage([&map_waypoints_x,&map_waypoints_y,&map_waypoints_s,
                 &map_waypoints_dx,&map_waypoints_dy, &lane, &ref_vel, root, &map]
                (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                 uWS::OpCode opCode) {
                    
                    // "42" at the start of the message means there's a websocket message event.
                    // The 4 signifies a websocket message
                    // The 2 signifies a websocket event
                    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
                        
                        auto s = hasData(data);
                        
                        if (s != "") {
                            auto j = json::parse(s);
                            
                            string event = j[0].get<string>();
                            
                            if (event == "telemetry") {
                                // j[1] is the data JSON object
                                
                                // Main car's localization Data
                                double car_x = j[1]["x"];
                                double car_y = j[1]["y"];
                                double car_s = j[1]["s"];
                                double car_d = j[1]["d"];
                                double car_yaw = j[1]["yaw"];
                                double car_speed = j[1]["speed"];
                                
                                // Previous path data given to the Planner
                                auto previous_path_x = j[1]["previous_path_x"];
                                auto previous_path_y = j[1]["previous_path_y"];
                                // Previous path's end s and d values
                                double end_path_s = j[1]["end_path_s"];
                                double end_path_d = j[1]["end_path_d"];
                                
                                // Sensor Fusion Data, a list of all other cars on the same side
                                //   of the road.
                                auto sensor_fusion = j[1]["sensor_fusion"];
                          
                                
                                CarStatus carStatus;
                                carStatus.car_x = car_x;
                                carStatus.car_y = car_y;
                                carStatus.car_s = car_s;
                                carStatus.car_d = car_d;
                                carStatus.car_yaw = car_yaw;
                                carStatus.end_path_s = end_path_s;
                                carStatus.end_path_d = end_path_d;
                                //CarStatus::car_speed = ref_vel;
                                carStatus.sensor_fusion = sensor_fusion;
                                carStatus.previous_path_x = previous_path_x;
                                carStatus.previous_path_y = previous_path_y;
                                root->run(map, carStatus, ws);
                            
                                
                                
                            }  // end "telemetry" if
                        } else {
                            // Manual driving
                            std::string msg = "42[\"manual\",{}]";
                            
                            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                        }
                    }  // end websocket if
                }); // end h.onMessage
    
    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        std::cout << "Connected!!!" << std::endl;
    });
    
    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                           char *message, size_t length) {
        ws.close();
        std::cout << "Disconnected" << std::endl;
    });
    
    int port = 4567;
    if (h.listen(port)) {
        std::cout << "Listening to port " << port << std::endl;
    } else {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }
    
    h.run();
}

