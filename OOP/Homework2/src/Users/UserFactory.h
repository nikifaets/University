#pragma once 

#include "User.h"

class UserFactory{

    static int free_id;

    public:

    UserFactory();
    static User create_user(std::string name, int age);
    static User create_admin(std::string name, int age);
    static User create_moderator(std::string name, int age);
    

};