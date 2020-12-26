#include "Elevator.h"
#include <algorithm> //for upper_bound, find
#include <iostream> //for debugging only
#include "Utils.h"

Elevator::Elevator(int capacity, int num_floors): 
        elapsed_time(0), 
        destination(-1), 
        req_direction(0),
        move_direction(0),
        curr_pos(1),
        start_pos(1),
        num_passengers(0),
        capacity(capacity),
        passenger_destinations(num_floors, 0),
        stops(num_floors, 0),
        floor_requests(num_floors)
        {}


bool Elevator::accept_passenger(Request& r){

    int passenger_direction = r.call_dir;
    int passenger_destination = r.go_floor;

    bool can_accept = (req_direction == passenger_direction);
    
    if(can_accept && num_passengers < capacity){

        add_stop(passenger_destination);

        remove_stop(curr_pos);
        num_passengers ++;
        passenger_destinations[passenger_destination] ++;

        //remove request from elevator's inner queue
        list<Request>::iterator req_it = find(floor_requests[curr_pos].begin(), floor_requests[curr_pos].end(), r);
        if(req_it != floor_requests[curr_pos].end()){
            floor_requests[curr_pos].erase(req_it);

        } 

        //if desired destination is further than the already set one, make it the elevator destination
        if(Utils::sign(passenger_destination - destination) == req_direction){

            destination = passenger_destination;
        }

        elapsed_time = 0;
        start_pos = curr_pos;
        move_direction = Utils::sign(passenger_destination - curr_pos);
        if(abs(curr_pos - r.go_floor) > abs(curr_pos - destination)) destination = r.go_floor;
        return true;
    }

    return false;
}

bool Elevator::can_accept_call(int floor, int req_direction){

    if(destination == -1) return true;

    if(num_passengers >= capacity) return false;

    if(req_direction != this->req_direction) return false;

    //floor of the call request should be between current position and final destination
    return Utils::sign(destination - curr_pos) == Utils::sign(floor - curr_pos);

}
void Elevator::accept_call_request(Request &r){

    int req_destination = r.call_floor;
    this->req_direction =  r.call_dir;

    if(destination == -1){

        destination = req_destination;
        start_pos = curr_pos;
        elapsed_time = 0;
        move_direction = Utils::sign(req_destination - curr_pos);
    }

    else{

        //if desired destination is further than the already set one, make it the elevator destination
        if(Utils::sign(req_destination - destination) == this->req_direction ||
            abs(req_destination - curr_pos) > abs(destination - curr_pos)){

            destination = req_destination;
        }        
    }

    floor_requests[req_destination].push_back(r);
    add_stop(req_destination);


}

bool Elevator::is_on_floor(){

    return (elapsed_time % TIME_PER_FLOOR == 0);
}

int Elevator::get_curr_floor(){

    if(! is_on_floor()) return -1;
    if(destination == -1) return curr_pos;

    return start_pos + move_direction * (elapsed_time / TIME_PER_FLOOR);
}

void Elevator::unload_passengers(){

    //This method should only be called if the elevator is on a floor

    if(! is_on_floor()) return;

    num_passengers -= passenger_destinations[curr_pos];
    passenger_destinations[curr_pos] = 0;
    remove_stop(curr_pos);
    
}

void Elevator::add_stop(int floor){

    stops[floor] = true;

}

void Elevator::remove_stop(int floor){

    stops[floor] = false;

}

list<Request> * Elevator :: get_requests_for_floor(int floor){

    return &floor_requests[floor];
}

void Elevator::update(){

    if(destination == -1) return;

    elapsed_time ++;
    if (is_on_floor()) curr_pos = get_curr_floor();

    if(curr_pos == destination && has_no_stops()){

        destination = -1;
        move_direction = 0;
        req_direction = 0;
        elapsed_time = 0;
    } 
}

bool Elevator::has_to_stop(){

    return stops[curr_pos];
}
bool Elevator::has_no_stops(){

    return find(stops.begin(), stops.end(), true) == stops.end();
}

bool Elevator::has_req_direction(){

    return req_direction != 0;
}

int Elevator::get_direction(){

    return req_direction;
}
void Elevator::print_stops(){

    cout << "Time elapsed " << elapsed_time << endl;
    std::cout << "Destination " << destination << endl;

    std::cout << "Stops:" << endl;
    for(int i=0; i<stops.size(); i++){

        std::cout << stops[i] << endl;
    }

    std::cout << "Passengers per stop:" << endl;
    for(int i=0; i<passenger_destinations.size(); i++){

        std::cout << passenger_destinations[i] << endl;
    }
}

void Elevator::print_pos(){

    std::cout << "Current stop: " << curr_pos << endl;
}

void Elevator::print_requests(){

    for(int i=0; i<passenger_destinations.size(); i++){

        cout << "Floor " << i << " " << passenger_destinations[i] << endl;
    }
}