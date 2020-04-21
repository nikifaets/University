#include <iostream>
#include "Card/Card.h"
#include "Deck/Deck.h"

using namespace std;

int main(){

	char sn[] = "abcdef";
	Card c(Four, Hearts, sn);

	cout << c.get_serial_num() << endl;

	Deck deck;
	int size = deck.get_size();
	cout << deck.get_size()<<endl;
	Card* cards = deck.get_cards();

	for(int i=0; i<size; i++){

		cout << "new card " << endl;
		cards[i].get_rank().print_rank();
		cards[i].get_suit().print_suit();
		cout << endl;
	}
}