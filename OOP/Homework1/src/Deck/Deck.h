#pragma once
#include "../Card/Card.h"

class Deck{

	static const int DEFAULT_DECK_SIZE = 52;
	int deck_size = DEFAULT_DECK_SIZE;
	Card* cards;
	const char* DEFAULT_SERIAL = "123456987";
	char* serial_num;
	Card* generate_deck(const int length = DEFAULT_DECK_SIZE);
	Card* generate_random_set(const int length);


public:

	Deck();
	Deck(int length, const char* serial_num);
	int get_size();
	Card* get_cards();

};