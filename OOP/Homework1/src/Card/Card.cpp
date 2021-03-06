#include <iostream>
#include "Card.h"
#include <string.h>
using namespace std;


Card::Card(RankValues r, SuitValues s, const char* sn){

	this->rank = ranks[r];
	this->suit = suits[s];
	this->serial_num = new char[strlen(sn)];
	strcpy(serial_num, sn);
}

Card::Card(){}

char* Card::get_serial_num(){

	return this->serial_num;
}

Rank Card::get_rank(){

	return rank;
}

Suit Card::get_suit(){

	return suit;
}

void Card::print_card(){

	get_rank().print_rank();
	cout << " ";
	get_suit().print_suit();
	cout << endl;
}

char* Card::get_serial(){

	return serial_num;
}
