#pragma once

struct Request{

    //call
    int call_dir;
    int call_floor;
    int call_time;

    //go
    int go_floor;
    int go_time;

    //note if this request has been returned to the list
    bool returned = false;

    bool operator ==(const Request& other) const;
};
