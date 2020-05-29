#include "Platform.h"

#include <iostream>
#include "../MessageSystem/MessageSystem.h"
#include "../Users/UserFactory.h"

std::unordered_set<User, Platform::UserHash> Platform::users;

void Platform::add_user(std::string actor_name, std::string name, int age){

    User user = UserFactory::create_user(name, age);
    users.insert(user);
}

void Platform::add_moderator(std::string actor_name, std::string name, int age){

    User user = UserFactory::create_moderator(name, age);
    users.insert(user);
}

void Platform::delete_user(std::string actor_name, std::string target_name){}
void Platform::block_user(std::string actor_name, std::string target_name){}
void Platform::unblock_user(std::string actor_name, std::string target_name){}

void Platform::change_name(std::string actor_name, std::string new_name){}

void Platform::show_info(){

    std::unordered_set<User>::iterator it;
    int users_count = 0;
    int mod_count = 0;

    for(it=users.begin(); it!=users.end(); it++){

        User curr = *it;

        if(curr.RANK == Rank::USER){

            users_count ++;
        }

        else if(curr.RANK == Rank::MODERATOR){

            mod_count ++;
        }

        message::print_info("Users count:", users_count);
        message::print_info("Moderators count:", mod_count);

    }
}

int Platform::UserHash::operator()(const User& user) const{

    return user.USER_ID;
}