#pragma once

#include "../Deck/Deck.h"
#include "../Card/Card.h"
#include "../Parser/Parser.h"


class Game{

	int ask_move();
	int ask_deck();
	void ask_player();
	void draw();
	int calculate_score(Card card, int curr_score);
	void skip();
	void get_probability();
	int score_count(int score);
	void ai_draw();
	void exit();
	void begin();

	void player_lose();
	void player_win();

	const int SCORE_PLAYER_LOSE = 21;
	Deck* deck;
	int score = 0;
	bool exit_flag = false;
	const int SHUFFLE_ITERATIONS = 50;
	const int DECK_UPPER_LIMIT = 520;
	const int DECK_LOWER_LIMIT = 10;
	Parser parser;


public:

	Game();
	void loop();


};
