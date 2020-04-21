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

			char* card_serial = generate_serial();

			cards[i*DEFAULT_DECK_SIZE + j] = Card(static_cast<RankValues>(rank), static_cast<SuitValues>(suit), card_serial);
		}

	}

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
	memset(taken, false, sizeof(bool)*DEFAULT_DECK_SIZE);

	for(int i=0; i<length; i++){

		const int CARDS_LEFT = DEFAULT_DECK_SIZE - i;
		int free[CARDS_LEFT];

		int idx_count = 0;
		for(int j=0; j<DEFAULT_DECK_SIZE; j++){

			if(!taken[j]){

				free[idx_count] = j;
				idx_count++;
				taken[j] = true;
			}
		}

		srand(time(0));
		int take_idx = rand() % CARDS_LEFT;
		int rank = i % RANKS_NUM;
		int suit = i % SUITS_NUM;
		
		char* card_serial = generate_serial();

		cards[i] = Card(static_cast<RankValues>(rank), static_cast<SuitValues>(suit), card_serial);

	}

	return cards;

}

int Deck::get_size(){

	return deck_size;
}

Card* Deck::get_cards(){

	return cards;
}

void Deck::print_deck(){

	cout << "Deck Size: " << deck_size << endl;
	for(int i=0; i<deck_size; i++){

		cout << "Card " << i << " Serial: " << cards[i].get_serial() << endl;
		cards[i].print_card();
	}
}
void Deck::swap(int first, int second){

	if(first < 0 || second < 0){

		cout << "Please enter two positive integers." << endl;
		return;
	}

	Card temp;
	temp = cards[first];
	cards[first] = cards[second];
	cards[second] = temp;
}

Card Deck::draw(){

	Card top = cards[deck_size-1];
	for(int i=deck_size-1; i>0; i--){

		cards[i] = cards[i-1];
	}

	cards[0] = top;

	last_idx ++;
	if(last_idx == 0){

		all_drawn = true;
	}

	return top;
}

int Deck::suit_count(SuitValues s){

	int count = 0;

	for(int i=deck_size-1; i>=last_idx; i--){

		if(cards[i].get_suit().value == s) count ++;
	}

	return count;
}

int Deck::rank_count(RankValues r){

	int count = 0;

	for(int i=deck_size-1; i>=last_idx; i--){

		if(cards[i].get_rank().value == r) count ++;
	}

	return count;
}

char* Deck::generate_serial(){

	static char num[6] = {35, 35, 35, 35, 35, '\0'};
	num[rand()%5] += 1;

	char* serial_append = new char[CARD_SERIAL_APPEND];
	strcpy(serial_append, num);

	char* serial = new char[strlen(serial_num) + strlen(num)];
	strcpy(serial, serial_num);
	strcat(serial, serial_append);

	return serial;

}

int Deck::get_last_idx(){

	return last_idx;
}

Card Deck::get_top(){

	return cards[deck_size-1];
}

void Deck::shuffle(int iterations){

	srand(time(0));

	for(int i=0; i<iterations; i++){

		//There is no protection against picking the same cards. This is ON PURPOSE since it adds unpredictability.

		int first = rand() % deck_size;
		int second = rand() % deck_size;

		swap(first, second);
	}
}