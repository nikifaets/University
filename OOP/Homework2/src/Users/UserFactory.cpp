#include "UserFactory.h"

int UserFactory::free_id = 0;
UserFactory::UserFactory(){

    free_id = 0;
}

User UserFactory::create_user(std::string name, int age){

    free_id ++;
    return User(free_id, name, age, Rank::USER);
}

User UserFactory::create_moderator(std::string name, int age){

    free_id ++;
    return User(free_id, name, age, Rank::MODERATOR);
}

User UserFactory::create_admin(std::string name, int age){

    free_id ++;
    return User(free_id, name, age, Rank::ADMIN);
}