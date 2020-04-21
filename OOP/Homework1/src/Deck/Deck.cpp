#include "Deck.h"
#include <iostream>
#include <cstring>
using namespace std;

Deck::Deck(){

	serial_num = new char[strlen(DEFAULT_SERIAL)];
	strcpy(serial_num, DEFAULT_SERIAL);
	cards = generate_deck();

}

Deck::Deck(const int length, const char* serial_num){

	this->deck_size = length;
	this->serial_num = new char[strlen(serial_num)];
	strcpy(this->serial_num, serial_num);
	cards = generate_deck(length);
}

Card* Deck::generate_deck(const int length){

	Card* cards = new Card[length];

	//create duplicate decks if length >= DFEAULT_SIZE*2
	for(int i=0; i<length/DEFAULT_DECK_SIZE; i++){

		for(int j=0; j<DEFAULT_DECK_SIZE; j++){

			int rank = j % RANKS_NUM;
			int suit = j % SUITS_NUM;

			char* serial_num = this->serial_num;
			const char* serial_append = "69";

			char card_serial[strlen(serial_num) + strlen(serial_append)];
			strcpy(card_serial, serial_num);
			strcat(card_serial, serial_append);

			cards[i*DEFAULT_DECK_SIZE + j] = Card(static_cast<RankValues>(rank), static_cast<SuitValues>(suit), card_serial);
		}

	}

	cout << "start appending" << endl;
	//append the rest
	const int APPEND_SIZE = length % DEFAULT_DECK_SIZE;

	if(APPEND_SIZE){

		Card* append_cards = generate_random_set(APPEND_SIZE);

		int next_idx = (length/DEFAULT_DECK_SIZE) * DEFAULT_DECK_SIZE;

		for(int i=0; i<APPEND_SIZE; i++){

			cards[next_idx+i] = append_cards[i];
		}

	}

	return cards;
}

Card* Deck::generate_random_set(const int length){

	Card* cards = new Card[length];
	bool taken[DEFAULT_DECK_SIZE];

	for(int i=0; i<length; i++){

		const int CARDS_LEFT = DEFAULT_DECK_SIZE - i;
		int free[CARDS_LEFT];

		int idx_count = 0;
		for(int j=0; j<DEFAULT_DECK_SIZE; j++){

			if(!taken[j]){

				free[idx_count] = j;
				idx_count++;
			}
		}

		int take_idx = rand() % CARDS_LEFT;
		int rank = i % RANKS_NUM;
		int suit = i % SUITS_NUM;
		const char serial_num[] = "234";

		cards[i] = Card(static_cast<RankValues>(rank), static_cast<SuitValues>(suit), serial_num);

	}

	return cards;

}

int Deck::get_size(){

	return deck_size;
}

Card* Deck::get_cards(){

	return cards;
}
