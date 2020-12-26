#include "Parser.h"
#include <fstream>
#include <iostream>

Parser::Parser(std::string filename): filename(filename){}

std::list<Request>* Parser::get_requests(){
    std::ifstream inp(this->filename);

    int dummy_h, dummy_size_l, dummy_size_s;

    inp >> dummy_h >> dummy_size_l >> dummy_size_s;

    std::list<Request>* requests = new std::list<Request>;
    int num_req;
    inp >> num_req;

    for(int i=0; i<num_req/2; i++){

        std::string dummy_req_name;


        //call request
        inp >> dummy_req_name;
        std::string direction_str;
        inp >> direction_str;
        
        int call_floor, call_time, go_destination, go_time;
        inp >> call_floor >> call_time;

        //go request
        inp >> dummy_req_name;
        inp >> go_destination >> go_time;
        int direction = direction_str == "UP" ? 1 : -1;

        Request r = {direction, call_floor, call_time, go_destination, go_time};
        requests->push_back(r);

    }

    inp.close();
    return requests;



}

int Parser::get_num_floors(){

    std::ifstream inp(this->filename);
    int h;
    inp >> h;

    inp.close();

    return h+2;

}

std::pair<int, int> Parser::get_capacities(){

    std::ifstream inp(this->filename);
    int dummy, l_size, s_size;
    inp >> dummy >> l_size >> s_size;
    inp.close();

    return std::make_pair(l_size, s_size);


}

bool Parser::validate_filename(){

    std::ifstream inp;
    inp.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try{

        inp.open(filename);
        inp.close();
    }

    catch(std::ifstream::failure e){

        return false;
    }

    return true;
}
