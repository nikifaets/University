#include <iostream>
#include "Card/Card.h"

using namespace std;

int main(){

	char sn[] = "abcdef";
	Card c(Four, Hearts, sn);

	cout << c.get_serial_num() << endl;
}