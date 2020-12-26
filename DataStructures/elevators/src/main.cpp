#include "Elevator.h"
#include "Parser.h"
#include "Request.h"
#include <iostream>
#include <queue>

using namespace std;

struct Stop{

    int time;
    bool elevator;
    int floor; 
};

bool assign_to_elevator(Request& r, Elevator& l, Elevator& s){

    bool l_can_take = l.can_accept_call(r.call_floor, r.call_dir);
    bool s_can_take = s.can_accept_call(r.call_floor, r.call_dir);

    l_can_take = (!l.has_req_direction() && s.has_req_direction() && s.get_direction() && s_can_take == r.call_dir) ? false : l_can_take;
    s_can_take = (!s.has_req_direction() && l.has_req_direction() && l.get_direction() && l_can_take == r.call_dir) ? false : s_can_take;

    if(l_can_take){

        if(s_can_take){

            if(abs(l.get_curr_floor() - r.call_floor) > abs(s.get_curr_floor() - r.call_floor)){

                s.accept_call_request(r);
                return true;

            }
        }
        l.accept_call_request(r);
        return true;

    }

    else if(s_can_take){
        s.accept_call_request(r);
        return true;
    }

    return false;

}

list<Request>* handle_load_unload(Elevator& el, vector<queue<Request> >& floor_requests){

    el.unload_passengers();

    int curr_floor = el.get_curr_floor();
    while(!floor_requests[curr_floor].empty() && el.accept_passenger(floor_requests[curr_floor].front())){

        floor_requests[curr_floor].pop();
    }

    list<Request>* rejected = el.get_requests_for_floor(curr_floor);
    for(list<Request>::iterator it = rejected->begin(); it != rejected->end(); ++it){

        it->returned = true;
    }

    return rejected;
}

void print_queue_sizes(vector<queue<Request*> >& floor_requests){

    for(int i=0; i<floor_requests.size(); i++){

        cout << "Queue size at floor " << i << endl;
        cout << floor_requests[i].size() << endl;
    }
}

int main(int argc, char** argv){

    if(argc < 2 ){

        cout << "Please give the file name as an argument!" << endl;
        return 1;
    }
    string filename = argv[1];
    cout << filename << endl;
    Parser p(filename);

    if(!p.validate_filename()){

        cout << "Cannot open file. Use relative path to the executable." << endl;
        return 1;
    }
    int num_floors = p.get_num_floors();
    std::pair<int, int> sizes = p.get_capacities();
    std::list<Request>* requests = p.get_requests();

    Elevator l(sizes.first, num_floors);
    Elevator s(sizes.second, num_floors);

    vector<queue<Request> > floor_requests(num_floors);
    vector<Stop> recorded_stops;

    int t = 0;
    while(!requests->empty() || !l.has_no_stops() || !s.has_no_stops()){
        
        list<Request>::iterator it = requests->begin();

        while(it != requests->end()){

            if(it->call_time > t) break;
            
            //put the request on the floor queue
            int call_floor = it->call_floor;
            
            if(!it->returned){

                it->returned = true;
                floor_requests[call_floor].push(*it);
            }

            //can one of the elevators accept the request?
            //is so, take it and store it for the appropriate floor
            bool assigned = assign_to_elevator(*it, l, s);
            if(assigned) requests->erase(it++);
            else ++it;



        }

        //logic for loading and unloading
        //if there is loading or unloading logic, SAVE THE STOP IN THE ANS
        //if less than expected people loaded on the floor, return the untaken requests to the original list     
        if(l.is_on_floor() && l.has_to_stop()){
            
            recorded_stops.push_back({t, 0, l.get_curr_floor()});

            list<Request>* rejected = handle_load_unload(l, floor_requests);
            requests->splice(requests->begin(), *rejected);

            
        }

        if(s.is_on_floor() && s.has_to_stop()){

            recorded_stops.push_back({t, 1, s.get_curr_floor()});
            list<Request>* rejected = handle_load_unload(s, floor_requests);
            requests->splice(requests->begin(), *rejected);
        }

        t++;
        l.update();
        s.update();


    }

    cout << "\nColumns: Second | Elevator | Floor \n" << endl;
    for(int i=0; i<recorded_stops.size(); i++){

        Stop s = recorded_stops[i];
        cout << "recorded stop: " << s.time << " " << s.elevator << " " << s.floor << endl; 
    }

    delete requests;

    return 0;

}