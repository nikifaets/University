#pragma once

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include "../Platform/Platform.h"

class Parser{

    std::unordered_set <std::string> independent_requests = {"info", "exit", "change_name"};

    std::unordered_set <std::string> user_actions = {"block_user", "unblock_user", "add_user", "add_moderator",
                                                    "delete_user"};

    std::unordered_set<std::string> user_creation_actions = {"add_user", "add_moderator"}; 
    std::unordered_set <std::string> post_actions = {"post", "remove_post"};
    
    typedef void (*user_func_ptr)(std::string actor_name, std::string target_name);
    typedef void (*user_cr_ptr)(std::string actor_name, std::string name, int age);
    //typedef void (Parser::*post_func_ptr)(int actor_id, int )

    std::unordered_map <std::string, user_func_ptr> map_user_action;
    std::unordered_map <std::string, user_cr_ptr> map_user_creation_action;

    void fill_maps();

    public:

    Parser();
    
};