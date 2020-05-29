#include "User.h"

User::User(int id, std::string name, int age, Rank rank) : USER_ID(id), name(name), AGE(age), RANK(rank){}

std::string User::get_name() const{

    return name;
}
bool User::operator == (const User& other) const{

		return this->get_name().compare(other.get_name());
}