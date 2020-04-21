#include <iostream>
#include "Card.h"
#include <string.h>
using namespace std;


Card::Card(RankValues r, Suit s, char* sn){

	this->rank = ranks[r];
	this->suit = s;
	this->serial_num = new char[strlen(sn)];
	strcpy(serial_num, sn);
}

char* Card::get_serial_num(){

	return this->serial_num;
}