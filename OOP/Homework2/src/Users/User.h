#pragma once

#include <iostream>
#include <string>

enum Rank {User, Moderator, Admin};

class User{

	const int USER_ID;
	std::string name;
	Rank rank = Rank::User;

	public:

	User(int id, std::string name, Rank rank) : USER_ID(id), name(name), rank(rank){}
	

};
