#include "Platform.h"
#include <algorithm>
#include <iostream>
#include "../MessageSystem/MessageSystem.h"
#include "../Users/UserFactory.h"

std::vector<User> Platform::users;
std::unordered_set <std::string> Platform::usernames;

void Platform::add_admin(User user){

    users.push_back(user);
    usernames.insert(user.get_name());
}
void Platform::add_user(std::string actor_name, std::string name, int age){


    if(! validate_rights(actor_name, Rank::ADMIN)) return;

    if(is_name_used(name)){

        message::print_string("Username is taken.");
        return;
    }

    User user = UserFactory::create_user(name, age);
    usernames.insert(name);
    users.push_back(user);

    std::cout << users.size() << std::endl;
}

void Platform::add_moderator(std::string actor_name, std::string name, int age){

    if(! validate_rights(actor_name, Rank::ADMIN)) return;

    if(is_name_used(name)){

        message::print_string("Username is taken.");
    }

    User user = UserFactory::create_moderator(name, age);
    users.push_back(user);
    usernames.insert(name);
}

void Platform::delete_user(std::string actor_name, std::string target_name){

    if(! validate_rights(actor_name, Rank::ADMIN)) return;

    usernames.erase(target_name);
    users.erase(std::find(users.begin(), users.end(), get_user_by_name(target_name)));


}
void Platform::block_user(std::string actor_name, std::string target_name){}
void Platform::unblock_user(std::string actor_name, std::string target_name){}

void Platform::change_name(std::string actor_name, std::string new_name){

    if(is_name_used(new_name)){

        message::print_string("Name is already in use.");
        return;
    }

    User* user = get_user_by_name(actor_name);
    user->set_name(new_name);

    usernames.erase(actor_name);
    usernames.insert(new_name);
 
}

bool Platform::is_name_used(std::string name){

    return usernames.find(name) != usernames.end();
}



void Platform::show_info(){

    int users_count = 0;
    int mod_count = 0;

    for(int i=0; i<users.size(); i++){

        User curr = users[i];

        if(curr.RANK == Rank::USER){

            users_count ++;
        }

        else if(curr.RANK == Rank::MODERATOR){

            mod_count ++;
        }

    }

    message::print_info("Users count:", users_count);
    message::print_info("Moderators count:", mod_count);
}

User* Platform::get_user_by_name(std::string name){

    for(int i=0; i<users.size(); i++){

        if(!users[i].get_name().compare(name)){

            return &users[i];
        }
    }

    User* res_user = new User(-1, "Invalid", 0, Rank::USER);
    return res_user;

}

bool Platform::validate_rights(std::string name, Rank rank){

    if(is_name_used(name)){

        User* user = get_user_by_name(name);
        
        if(user->RANK >= rank) return true;

        else{

            message::print_string("User does not have the appropriate rights.");
            return false;
        }

    }

    message::print_string("Invalid name of actor.");
    return false;
}