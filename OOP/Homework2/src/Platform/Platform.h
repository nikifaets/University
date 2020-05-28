#pragma once

#include <vector>
#include <map>
#include <string>
class Platform{

    
    public:

    static void show_info();
    static void add_user(int actor_id, int target_id);
    static void delete_user(int actor_id, int target_id);
    static void block_user(int actor_id, int target_id);
    static void unblock_user(int actor_id, int target_id);
    static void add_moderator(int actor_id, int target_id);
    static void change_name(int actor_id);


};