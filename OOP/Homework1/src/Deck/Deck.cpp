#include "Deck.h"
#include <iostream>
#include <cstring>

Deck::Deck(){

	serial_num = new char[strlen(DEFAULT_SERIAL)];
	strcpy(serial_num, DEFAULT_SERIAL);
	cards = generate_deck();

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

	return cards;
}

int Deck::get_size(){

	return deck_size;
}

Card* Deck::get_cards(){

	return cards;
}
