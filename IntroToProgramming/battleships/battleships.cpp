#include <iostream>
#include <cstring>
using namespace std;

const unsigned short PLAYER_ID = 0;
const unsigned short COMPUTER_ID = 1;

const char ADDRESS[2][15] = {"You", "The computer"};

const unsigned short POS_EMPTY = 10;

const unsigned short BOARD_STATUS_HIDDEN = 11;

const unsigned short CARRIER_ID = 0;
const unsigned short BATTLESHIP_ID = 1;
const unsigned short FRIGATE_ID = 2;
const unsigned short SUBMARINE_ID = 3;
const unsigned short CUTTER_ID = 4;

const unsigned short SHIP_LEN[5] = {5,4,3,3,2};
const unsigned short SHIP_AMOUNT[5] = {1,1,2,2,4};

const unsigned short SHIPS_AMOUNT = 5;

const char SHIP_NAME[5][20] = {"Carrier", "Battleship", "Frigate", "Submarine", "Cutter"};
const char CARDINAL[4][5] = {"1st", "2nd", "3rd", "4th"};

bool GAME_OVER = false;

unsigned short get_rand(unsigned short max);
unsigned short* fit_array(unsigned short** board, unsigned short board_len, unsigned short array_len, unsigned short write_over);
void fill_board(unsigned short** board, unsigned short board_len);
bool array_fits(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short array_len, unsigned short propagation, unsigned short write_over);
void write_array(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short rotation, unsigned short ship_id);

void player_fill_board(unsigned short** board, unsigned short board_len);
void player_input(unsigned short**board, unsigned short board_len, unsigned short ship_id, unsigned short num);

void player_attack(unsigned short** board, unsigned short len, unsigned short** history_board);
void ai_attack(unsigned short** board, unsigned short len, unsigned short** history_board);
unsigned short* ai_attack_random(unsigned short** board, unsigned short board_len, unsigned short** history_board, unsigned short* ships_left);
unsigned short ai_hit(unsigned short** board, unsigned short board_len, unsigned short* pos);

unsigned short get_array_len(unsigned short ship_id);

unsigned short** create_matrix(unsigned short rows_num, unsigned short cols_num);
void delete_matrix(unsigned short** matrix, unsigned short rows_num, unsigned short cols_num);

void print_board(unsigned short** board, unsigned short board_len);

void player_fill_board_test(unsigned short** board, unsigned short len);

void print_message_game_over(unsigned short player);

int main(){


	int len;

	cout << "Input board size. " << endl;
	cin >> len;

	unsigned short** computer_board = create_matrix(len, len);
	unsigned short** player_board = create_matrix(len, len);
	unsigned short** player_history_board = create_matrix(len,len);

	for(int i=0; i<len; i++){

		for(int j=0; j<len; j++){

			player_board[i][j] = POS_EMPTY;
			computer_board[i][j] = POS_EMPTY;
			player_history_board[i][j] = BOARD_STATUS_HIDDEN;
		}
	}


	fill_board(computer_board, len);
	print_board(computer_board, len);
	print_board(player_history_board, len);

	player_fill_board_test(player_board, len);


	player_attack(computer_board, len, player_history_board);


}

void player_fill_board(unsigned short** board, unsigned short board_len){

	for(int i=0; i<SHIPS_AMOUNT; i++){

		for(int j=0; j<SHIP_AMOUNT[i]; j++){

			player_input(board, board_len, i, j);
		}
	}
}

void player_input(unsigned short** board, unsigned short board_len, unsigned short ship_id, unsigned short num){

	cout << "Input the starting field of your " << CARDINAL[num] << " " << SHIP_NAME[ship_id] << ": ";
	cout << "  (" << num << " / " << SHIP_AMOUNT[ship_id] << ")" << endl;

	char* input_field = new char[4];
	cin >> input_field;

	cout << "Input rotation: \n Right: 0 \n Left: 1 \n Up: 2 \n Down: 3." << endl;

	unsigned short rotation;
	cin >> rotation;

	unsigned short row = input_field[0] - 'A';
	unsigned short col = atoi(input_field + 1) - 1;

	if(array_fits(board, board_len, row, col, SHIP_LEN[ship_id], rotation, POS_EMPTY)){

		write_array(board, board_len, row, col, rotation, ship_id);
		print_board(board, board_len);
	}

	else{

		cout << "No space on the given coordinates. Try again." << endl;
		player_input(board, board_len, ship_id, num);
	}
}
void fill_board(unsigned short** board, unsigned short board_len){

	for(int i=0; i<SHIPS_AMOUNT; i++){

		for(int j=0; j<SHIP_AMOUNT[i]; j++){

			unsigned short* position = fit_array(board, board_len, i, POS_EMPTY);

			//get random valid position
			unsigned short row = position[0];
			unsigned short col = position[1];
			unsigned short rotation = position[2];

			write_array(board, board_len, row, col, rotation, i);
			delete [] position;
		}
	}
}

unsigned short* fit_array(unsigned short** board, unsigned short board_len, unsigned short ship_id, unsigned short write_over){


	unsigned short array_len = SHIP_LEN[ship_id];

	//array to store all possible positions
	//positions[n][0] -> start row
	//positions[n][1] -> start col
	//positions[n][2] -> rotation {r:0; l:1; u:2; d:3}

	const unsigned short possible_positions = board_len*board_len*4;

	unsigned short** positions = create_matrix(possible_positions, 3);

	//track index of next position
	unsigned short next_fit = 0;

	//try each element as start point
	for(int i=0; i<board_len; i++){

		for(int j=0; j<board_len; j++){

			//try to fit right
			if(array_fits(board, board_len, i, j, array_len, 0, write_over)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 0;
				next_fit ++;
			}

			//try to fit left
			if(array_fits(board, board_len, i, j, array_len, 1, write_over)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 1;
				next_fit ++;
			}

			//try to fit up
			if(array_fits(board, board_len, i, j, array_len, 2, write_over)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 2;
				next_fit ++;
			}

			//try to fit down
			if(array_fits(board, board_len, i, j, array_len, 3, write_over)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 3;
				next_fit ++;
			}
		}
	}

	unsigned short idx = get_rand(next_fit);
	unsigned short* res = new unsigned short[3];
	res = positions[idx];
	return res;

}

void write_array(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short rotation, unsigned short ship_id){

	unsigned short array_len = SHIP_LEN[ship_id];

	switch(rotation){

		case 0:

			for(int i=start_col; i<start_col+array_len; i++){

				board[start_row][i] = ship_id;
			}

			break;

		case 1:

			for(int i=start_col; i>start_col-array_len; i--){

				board[start_row][i] = ship_id;
			}

			break;

		case 2:

			for(int i=start_row; i>start_row-array_len; i--){

				board[i][start_col] = ship_id;
			}

			break;

		case 3:

			for(int i = start_row; i<start_row+array_len; i++){

				board[i][start_col] = ship_id;
			}

			break;

	}



}
bool array_fits(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short array_len, unsigned short propagation, unsigned short write_over){

	//propagation: {r:0; l:1; u:2; d:3}

	//right
	if(propagation == 0){

		if(start_col + array_len > board_len) return false;

		for(int i=start_col; i<start_col+array_len; i++){

			if(board[start_row][i] != write_over){

				return  false;
			}
		}

	}

	//left
	else if(propagation == 1){

		if(start_col - array_len < -1) return false;

		for(int i=start_col; i>start_col-array_len; i--){

			if(board[start_row][i] != write_over){

				return false;
			}
		}

	}

	//up
	else if(propagation == 2){

		if(start_row - array_len < -1) return false;

		for(int i = start_row; i>start_row-array_len; i--){

			if(board[i][start_col] != write_over){

				return false;
			}
		}

	}

	//down
	else if(propagation == 3){

		if(start_row + array_len > board_len) return false;

		for(int i=start_row; i<start_row+array_len; i++){

			if(board[i][start_col] != write_over){

				return false;
			}
		}

	}

	return true;
}

unsigned short** create_matrix(unsigned short rows_num, unsigned short cols_num){

	unsigned short** rows = new unsigned short* [rows_num];

	for(int i=0; i<rows_num; i++){

		rows[i] = new unsigned short [cols_num];

	}

	return rows;
}

void delete_matrix(unsigned short** matrix, unsigned short rows_num, unsigned short cols_num){

	for(int i=0; i<rows_num; i++){

		delete [] matrix[i];
	}

	delete [] matrix;
}

unsigned short get_rand(unsigned short max){


	static bool first = true;
	if (first) 
	{  
	  srand( time(NULL) ); 
	  first = false;
	}
	return rand() % max;

}

void player_attack(unsigned short** board, unsigned short board_len, unsigned short** history_board){

	static bool first = true;

	static unsigned short* ships_left = new unsigned short[SHIPS_AMOUNT];

	if(first){
	
		memcpy(ships_left, SHIP_AMOUNT, sizeof(unsigned short)*SHIPS_AMOUNT);
	}

	cout << " Choose a field to attack." << endl;

	char* input = new char[4];

	cin >> input;

	unsigned short row = input[0] - 'A';
	unsigned short col = atoi(input+1) - 1;

	unsigned short board_status = board[row][col];

	if(board[row][col] == POS_EMPTY){

		cout << "There are no ships at the current position." << endl;
		history_board[row][col] = POS_EMPTY;
	}

	else{

		unsigned short ship_id = board[row][col];
		unsigned short hit = SHIP_AMOUNT[ship_id] - ships_left[ship_id];

		history_board[row][col] = ship_id;

		cout << "You just hit the opponent's " << CARDINAL[hit] << " " << SHIP_NAME[ship_id] << " ";

		cout << "(" << hit+1 << " / " << SHIP_AMOUNT[ship_id] << ")" << endl; 

		ships_left[ship_id] --;

		if(ships_left[ship_id] == 0){

			cout << "You just destroyed all the opponent's " << SHIP_NAME[ship_id] << "s" << endl;
		}
	}

	for(int i=0; i<SHIPS_AMOUNT; i++){

		if(ships_left[i] != 0) return;
	}

	print_message_game_over(PLAYER_ID);
	GAME_OVER = true;
}

void ai_attack(unsigned short** board, unsigned short board_len, unsigned short** history_board){
	
	static bool first = true;
	const unsigned short STATE_ATTACK_RANDOM = 0;
	const unsigned short STATE_DECIDE_DIRECTION = 1;
	const unsigned short STATE_ATTACK_HORIZONTAL = 2;
	const unsigned short STATE_ATTACK_VERTICAL = 3;

	static unsigned short* ships_left = new unsigned short[SHIPS_AMOUNT];

	if(first){

		memcpy(ships_left, SHIP_AMOUNT, SHIPS_AMOUNT*sizeof(unsigned short));
		first = false;
	}


	static bool target_destroyed = false;
	static unsigned short curr_state = STATE_ATTACK_RANDOM;

	if(curr_state == STATE_ATTACK_RANDOM){

		unsigned short* res = ai_attack_random(board, board_len, history_board, ships_left);
		if(res[3] != POS_EMPTY)
	}
}

unsigned short* ai_attack_random(unsigned short** board, unsigned short board_len, unsigned short** history_board, unsigned short* ships_left){

	unsigned short fit_ship_id = 0;
	for(int i=0; i<SHIPS_AMOUNT; i++){

		if(ships_left[i] != 0){

			fit_ship_id = i;
		}
	}

	unsigned short* pos_to_attack = fit_array(history_board, board_len, fit_ship_id, BOARD_STATUS_HIDDEN);
	unsigned short row = pos_to_attack[0];
	unsigned short col = pos_to_attack[1];

	unsigned short* pos = new unsigned short[2];
	pos[0] = row;
	pos[1] = col;

	unsigned short hit = ai_hit(board, board_len, pos);

	unsigned short* res = new unsigned short[4];
	res[0] = row;
	res[1] = col;
	res[2] = pos_to_attack[2];
	res[3] = hit;

	return res;

}

unsigned short ai_hit(unsigned short** board, unsigned short board_len, unsigned short* pos){

	unsigned short row = pos[0];
	unsigned short col = pos[1];

	if(board[row][col] == POS_EMPTY){

		cout << ADDRESS[COMPUTER_ID] << " just missed. " << endl;
	}

	else{

		cout << ADDRESS[COMPUTER_ID] << " just hit a " << SHIP_NAME[board[row][col]] << endl;
	}

	return board[row][col];



}
void print_board(unsigned short** board, unsigned short board_len){

	for(int i=0; i<board_len; i++){

		for(int j=0; j<board_len; j++){

			if(board[i][j] == POS_EMPTY){

				cout << "_ ";
			}

			else if(board[i][j] == BOARD_STATUS_HIDDEN){

				cout << "X ";
			}

			else{

				cout << SHIP_NAME[board[i][j]][0] << " ";
			}
		}

		cout << endl;
	}
}

void print_message_game_over(unsigned short player){

	cout << ADDRESS[player] << " just won the game. Game over. " << endl;

	if(player == COMPUTER_ID){

		cout << "AI is gonna conquer the world.";
	}

}
void player_fill_board_test(unsigned short** board, unsigned short board_len){

	fill_board(board, board_len);
}