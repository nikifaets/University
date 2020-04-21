#pragma once

#include "Rank.h"
#include "Suit.h"


const int RANKS_NUM = 13;
const int SUITS_NUM = 4;

const Rank ranks[RANKS_NUM] = {

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
	Rank(Q, "Queen"),
	Rank(A, "Ace")
};

const Suit suits[SUITS_NUM] = {

	Suit(Hearts, "Hearts"),
	Suit(Diamonds, "Diamonds"),
	Suit(Spades, "Spades"),
	Suit(Clubs, "Clubs")
};

class Card{

	Rank rank;
	Suit suit;
	char* serial_num;

public:

	Card(RankValues r, SuitValues s, const char* sn);
	Card();
	char* get_serial_num();
	Rank get_rank();
	Suit get_suit();



};
