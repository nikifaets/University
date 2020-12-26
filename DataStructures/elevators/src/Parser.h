#pragma once
#include <list>
#include "Request.h"
#include <string>
#include <algorithm>

class Parser{

    std::string filename;

    public:
    Parser(std::string filename);
    std::list<Request>* get_requests();
    std::pair<int, int> get_capacities();
    int get_num_floors();
    bool validate_filename();
};