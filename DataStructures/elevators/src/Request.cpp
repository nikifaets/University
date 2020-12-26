#include "Request.h"

bool Request::operator==(const Request& other) const{

    return this->call_time == other.call_time &&
            this->call_floor == other.call_floor &&
            this->go_floor == other.go_floor;
}