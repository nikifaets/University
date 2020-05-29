#include "User.h"

User::User(int id, std::string name, int age, Rank rank) : USER_ID(id), name(name), AGE(age), RANK(rank){}

std::string User::get_name() const{

    return name;
}

void User::set_name(std::string new_name){

    this->name = new_name;
}
bool User::operator == (const User& other) const{

		return this->get_name().compare(other.get_name());
}

bool User::operator == (const User* other) const{

    return this->get_name().compare(other->get_name());
}

User User::operator = (const User& other) const{

    if (this->name.compare(other.get_name()) && this->RANK == other.RANK && this->USER_ID == other.USER_ID) return *this;

    return User(USER_ID, name, AGE, RANK);
}
