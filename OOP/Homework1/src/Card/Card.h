#pragma once

#include "Rank.h"

enum Suit {Hearts, Diamonds, Spades, Clubs};

const Rank ranks[13] = {
	Rank(Two, "Two"),
	Rank(Three, "Three"),
	Rank(Four, "Four"),
	Rank(Five, "Five"),
	Rank(Six, "Six"),
	Rank(Seven, "Seven"),
	Rank(Eight, "Eight"),
	Rank(Nine, "Nine"),
	Rank(Ten, "Ten"),
	Rank(J, "Jack"),
	Rank(K, "King"),
	Rank(Q, "Queen").
	Rank(A, "Ace")
	};

class Card{

	Rank rank;
	Suit suit;
	char* serial_num;

public:

	Card(RankValues r, Suit s, char* sn);
	char* get_serial_num();



};
