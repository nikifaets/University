#pragma once

enum SuitValues {Hearts, Diamonds, Spades, Clubs};

struct Suit{

	Suit(SuitValues s, const char* name);
	Suit();
	void print_suit() const;

	SuitValues value;
	char* name;
};