#include "Suit.h"
#include <cstring>

#include <iostream>

Suit::Suit(SuitValues r, const char* name){

	this->value = r;
	this->name = new char[strlen(name)];
	strcpy(this->name, name);
}

Suit::Suit(){}

void Suit::print_suit() const{

	std::cout << name << std::endl;
}