#pragma once
#include <queue>
#include <list>
#include "Request.h"
using namespace std;

const int TIME_PER_FLOOR = 5;

class Elevator{

    //-1 if elevator has reached destination and is stationary
    int destination;
    int curr_pos;
    int start_pos;
    int req_direction;
    int move_direction;
    //time since change of final destination
    int elapsed_time;
    int capacity;

    vector<bool> stops;
    //stores amount of passengers for each destination
    int num_passengers;
    vector<int> passenger_destinations;
    vector<list<Request> > floor_requests;

    void add_stop(int floor);
    void remove_stop(int floor);

    public:

    Elevator(int capacity, int num_floors);
    void accept_call_request(Request& r);
    bool can_accept_call(int floor, int req_direction);
    bool accept_passenger(Request& r);
    bool has_no_stops();
    bool is_on_floor();
    int get_curr_floor();
    void unload_passengers();
    void update();
    bool has_to_stop();
    bool has_req_direction();
    int get_direction();
    list<Request>* get_requests_for_floor(int floor);

    void print_stops();
    void print_pos();
    void print_requests();



};