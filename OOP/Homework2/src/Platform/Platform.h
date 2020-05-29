#pragma once

#include <vector>
#include <unordered_set>
#include <set>
#include <string>
#include "../Users/User.h"

class Platform{

    public:
    class UserHash{

        public:
        int operator()(const User& user) const;
    };

    private:
    static std::unordered_set<User, UserHash> users;

    public:

    static void show_info();
    static void add_user(std::string actor_name, std::string name, int age);
    static void delete_user(std::string actor_name, std::string target_name);
    static void block_user(std::string actor_name, std::string target_name);
    static void unblock_user(std::string actor_name, std::string target_name);
    static void add_moderator(std::string actor_name, std::string name, int age);
    static void change_name(std::string actor_name, std::string new_name);


};