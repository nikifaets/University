#pragma once
#include "../Card/Card.h"

class Deck{

	static const int DEFAULT_DECK_SIZE = 52;
	int deck_size = DEFAULT_DECK_SIZE;

	int last_idx = 0;
	bool all_drawn = false;

	Card* cards;

	const char* DEFAULT_SERIAL = "123456987";
	const int CARD_SERIAL_APPEND = 3;
	char* serial_num;

	Card* generate_deck(const int length = DEFAULT_DECK_SIZE);
	Card* generate_random_set(const int length);


public:

	Deck();
	Deck(int length, const char* serial_num);
	int get_size();
	Card* get_cards();
	void swap(int first, int second);
	Card draw();
	Card get_top();
	int suit_count(SuitValues s);
	int rank_count(RankValues r);
	int get_last_idx();
	void print_deck();
	char* generate_serial();
	void shuffle(int iterations);


};