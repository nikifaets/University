#include "Rank.h"
#include <cstring>

#include <iostream>

Rank::Rank(RankValues r, const char* name){

	this->value = r;
	this->name = new char[strlen(name)];
	strcpy(this->name, name);
}

Rank::Rank(){}

void Rank::print_rank() const{

	std::cout << name;
}