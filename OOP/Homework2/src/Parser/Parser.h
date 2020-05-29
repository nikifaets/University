#pragma once

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include "../Platform/Platform.h"

class Parser{

    std::unordered_set <std::string> independent_actions = {"info", "exit"};

    std::unordered_set <std::string> user_actions = {"block_user", "unblock_user",
                                                    "delete_user", "change_name"};

    std::unordered_set<std::string> user_creation_actions = {"add_user", "add_moderator"}; 
    std::unordered_set <std::string> post_actions = {"post", "remove_post"};
    
    typedef void (*user_func_ptr)(std::string actor_name, std::string target_name);
    typedef void (*user_cr_ptr)(std ::string actor_name, std::string name, int age);
    typedef void (*independent_func_ptr)();

    //typedef void (Parser::*post_func_ptr)(int actor_id, int )

    std::unordered_map <std::string, user_func_ptr> map_user_action;
    std::unordered_map <std::string, user_cr_ptr> map_user_creation_action;
    std::unordered_map <std::string, independent_func_ptr> map_independent_action;

    void fill_maps();
    std::vector<std::string> substring(std::string input);

    public:

    void parse();
    Parser();
    
};