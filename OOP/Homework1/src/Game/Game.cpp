#include "Game.h"
#include <iostream>
#include <iomanip>
using namespace std;

Game::Game(){

	begin();

}

void Game::begin(){

	exit_flag = false;
	score = 0;

	int deck_size = ask_deck();
	deck = new Deck(deck_size, "1234");
	deck->shuffle(SHUFFLE_ITERATIONS);

	draw();

	loop();


}
int Game::ask_deck(){

	int size = -1;

	while(!parser.validate_int(size, DECK_LOWER_LIMIT, DECK_UPPER_LIMIT)){

		cout << "Choose deck size between 10 and 520." << endl;

		cin >> size;
	}


	return size;
}

void Game::loop(){

	const int MOVE_HIT = 1;
	const int MOVE_SKIP = 2;
	const int MOVE_PROB = 3;

	while(!exit_flag){

		int choice = -1;
		while(!parser.validate_int(choice, MOVE_HIT, MOVE_PROB)){

			choice = ask_move();
		}

		switch(choice){

			case MOVE_HIT:

				draw();
				break;

			case MOVE_SKIP:

				ai_draw();
				break;

			case MOVE_PROB:

				get_probability();
				break;
		}


		if(score > SCORE_PLAYER_LOSE){

			exit_flag = true;
			player_lose();
		}


	}
}


int Game::ask_move(){

	cout << "Choose a move:" << endl;
	cout << "1) Hit;" << endl;
	cout << "2) Skip;" << endl;
	cout << "3) Probability." << endl;

	int choice = -1;
	while(!parser.validate_int(choice, 1, 3)){

		cout << "Type a number 1-3." << endl;

		cin >> choice;
	}

	return choice;

}

void Game::draw(){

	Card card = deck->draw();
	int new_score = calculate_score(card, this->score);

	score += new_score;
	cout << "Your draw: " << endl;
	card.print_card();
	cout << "Your score is now " << score << endl;

}

void Game::exit(){

	cout << "Do you want to play another game?" << endl;
	cout << "1) Yes" << endl;
	cout << "2) No" << endl;

	int ans = -1;
	while(!parser.validate_int(ans, 1, 2)){

		cout << "Choose 1-2." << endl;
		cin >> ans;
	}

	if(ans == 1){

		begin();
	}

	else exit_flag = true;
}

void Game::player_win(){

	cout << "You won. " << endl;
	exit();
}

void Game::player_lose(){

	cout << "You lost. " << endl;
	exit();
}


void Game::get_probability(){

	int score_diff = SCORE_PLAYER_LOSE - score;
	int win_cards = score_count(score_diff);

	int last_idx = deck->get_last_idx();
	int deck_size = deck->get_size();

	int cards_in_game = deck_size - last_idx;

	float prob = 0;

	if(cards_in_game > 0){

		prob = float(win_cards) / float(cards_in_game);
	}

	cout << "Probability is " << setprecision(3) << prob << endl;
}

int Game::score_count(int score){

	int last_idx = deck->get_last_idx();
	int deck_size = deck->get_size();

	int count = 0;
	Card* cards = deck->get_cards();
	for(int i=deck_size-1; i>=last_idx; i--){

		Card curr = cards[i];
		int card_score = calculate_score(curr, score);

		if(card_score == score){

			count ++;
		}
	}

	return count;
}

void Game::ai_draw(){

	cout << "Dealer's turn." << endl;
	int ai_score = 0;

	while(ai_score <= 17){

		Card top = deck->draw();
		cout << "Dealer draws " << endl;
		top.print_card();

		int new_score = calculate_score(top, ai_score);
		ai_score += new_score;
		cout << "Dealer's score is now " << ai_score << endl;
	}

	if(ai_score > SCORE_PLAYER_LOSE){

		player_win();
	}

	else if(ai_score == score){

		player_win();
	}

	else if(ai_score > score){

		player_lose();
	}

	else{

		player_win();
	}
}

int Game::calculate_score(Card card, int curr_score){

	RankValues r = card.get_rank().value;

	int new_score=0;

	switch(r){

		case Two:

			new_score = 2;
			break;

		case Three:

			new_score = 3;
			break;

		case Four:

			new_score = 4;
			break;

		case Five:

			new_score = 5;
			break;

		case Six:

			new_score = 6;
			break;

		case Seven:

			new_score = 7;
			break;

		case Eight:

			new_score = 8;
			break;

		case Nine:

			new_score = 9;
			break;

		case Ten:

			new_score = 10;
			break;

		case J:

			new_score = 10;
			break;

		case K:

			new_score = 10;
			break;

		case Q:

			new_score = 10;
			break;

		case A:

			if(score + 11 > SCORE_PLAYER_LOSE){

				new_score = 1;
			}

			else new_score = 11;
	}

	return new_score;
}