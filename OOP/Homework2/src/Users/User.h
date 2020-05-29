#pragma once

#include <iostream>
#include <string>

enum Rank {USER, MODERATOR, ADMIN};

class User{

	std::string name;

	public:

	const Rank RANK = Rank::USER;
	const int USER_ID;
	const int AGE;

	User(int id, std::string name, int age, Rank rank);
	std::string get_name() const;
	void set_name(std::string new_name);
	bool operator == (const User& other) const;
	bool operator == (const User* other) const;
	User operator = (const User& other) const;

};
