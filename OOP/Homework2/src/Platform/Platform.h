#pragma once

#include <vector>
#include <unordered_set>
#include <set>
#include <string>
#include "../Users/User.h"

class Platform{


    static std::vector<User> users;
    static std::unordered_set<std::string> usernames;

    static User* get_user_by_name(std::string name);
    static bool validate_rights(std::string name, Rank rank);


    public:

    static bool is_name_used(std::string name);
    static void show_info();
    static void add_user(std::string actor_name, std::string name, int age);
    static void delete_user(std::string actor_name, std::string target_name);
    static void block_user(std::string actor_name, std::string target_name);
    static void unblock_user(std::string actor_name, std::string target_name);
    static void add_moderator(std::string actor_name, std::string name, int age);
    static void change_name(std::string actor_name, std::string new_name);
    static void add_admin(User user);


};