#include <iostream>
#include <cstring>
using namespace std;

const unsigned short PLAYER_ID = 0;
const unsigned short COMPUTER_ID = 1;

const char ADDRESS[2][15] = {"You", "The computer"};

const unsigned short POS_EMPTY = 10;

const unsigned short BOARD_STATUS_HIDDEN = 11;
const unsigned short BOARD_STATUS_HIT = 12;

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

bool AI_REFRESH = false;

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
void ai_register_hit(unsigned short* pos, unsigned short hit, unsigned short** board, unsigned short board_len, unsigned short** history_board, unsigned short* ships_left, unsigned short shots_on_target);

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
	unsigned short** computer_history_board = create_matrix(len,len);

	for(int i=0; i<len; i++){

		for(int j=0; j<len; j++){

			player_board[i][j] = POS_EMPTY;
			computer_board[i][j] = POS_EMPTY;
			player_history_board[i][j] = BOARD_STATUS_HIDDEN;
			computer_history_board[i][j] = BOARD_STATUS_HIDDEN;
		}
	}


	fill_board(computer_board, len);
	player_fill_board(player_board, len);

	while(!GAME_OVER){

		player_attack(computer_board, len, player_history_board);
		ai_attack(player_board, len, computer_history_board);

		cout << "Your board: " << endl;
		print_board(player_board, len);

		cout << "Your history of attacks " << endl;
		print_board(player_history_board, len);

	}

	delete_matrix(player_board, len, len);
	delete_matrix(player_history_board, len, len);
	delete_matrix(computer_board, len, len);
	delete_matrix(computer_history_board, len, len);


}

void player_fill_board(unsigned short** board, unsigned short board_len){

	for(int i=0; i<SHIPS_AMOUNT; i++){

		for(int j=0; j<SHIP_AMOUNT[i]; j++){

			player_input(board, board_len, i, j);
			print_board(board, board_len);
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
	}

	else{

		cout << "No space on the given coordinates. Try again." << endl;
		player_input(board, board_len, ship_id, num);
	}

	delete [] input_field;
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

	delete [] positions;

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
	static unsigned short shots_on_target = 0;

	if(first){
	
		memcpy(ships_left, SHIP_AMOUNT, sizeof(unsigned short)*SHIPS_AMOUNT);
	}

	cout << " Choose a field to attack." << endl;

	char* input = new char[4];

	cin >> input;

	unsigned short row = input[0] - 'A';
	unsigned short col = atoi(input+1) - 1;

	unsigned short board_status = board[row][col];

	delete [] input;
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

		shots_on_target ++;

		if(shots_on_target == SHIP_LEN[ship_id]){

			ships_left[ship_id] --;
			cout << "You destroyed the opponent's " << CARDINAL[hit] << SHIP_NAME[ship_id] << endl;


		}

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
	static unsigned short* first_attack = new unsigned short[2];
	static unsigned short* last_attack = new unsigned short[2];
	static unsigned short attack_target = POS_EMPTY; 
	static unsigned short shots_on_target = 0;
	static unsigned short direction = 0; //0 - right; 1 - left; 2 - up; 3 - down;

	static unsigned short curr_state = STATE_ATTACK_RANDOM;

	if(first){

		memcpy(ships_left, SHIP_AMOUNT, SHIPS_AMOUNT*sizeof(unsigned short));
		first = false;
	}

	if(AI_REFRESH){

		shots_on_target = 0;
		attack_target = POS_EMPTY;
		curr_state = STATE_ATTACK_RANDOM;

		bool all_destroyed = true;
		for(int i=0; i<SHIPS_AMOUNT; i++){

			if(ships_left[i] > 0) all_destroyed = false;

		}

		if(all_destroyed){

			delete [] first_attack;
			delete [] ships_left;
			delete [] last_attack;

		}

	}


	if(curr_state == STATE_ATTACK_RANDOM){

		unsigned short* res = ai_attack_random(board, board_len, history_board, ships_left);

		unsigned short hit_row = res[0];
		unsigned short hit_col = res[1];
		unsigned short hit_direction = res[2];

		first_attack[0] = hit_row;
		first_attack[1] = hit_col;
		last_attack[0] = hit_row;
		last_attack[1] = hit_col;
		direction = hit_direction;
		attack_target = res[3];

		if(attack_target != POS_EMPTY){

			curr_state = STATE_DECIDE_DIRECTION;
			shots_on_target ++;

		}
		ai_register_hit(last_attack, attack_target, board, board_len, history_board, ships_left, shots_on_target);

		delete [] res;


	}

	else if(curr_state == STATE_DECIDE_DIRECTION){

		unsigned short* attack_pos = new unsigned short[2];
		unsigned short hit;

		if(direction == 0 && last_attack[1] < board_len - 1){

			last_attack[1] ++;
			attack_pos[0] = last_attack[0];
			attack_pos[1] = last_attack[0];

			hit = ai_hit(board, board_len, last_attack);

			if(hit != POS_EMPTY && hit == attack_target){

				curr_state = STATE_ATTACK_HORIZONTAL;
				direction = 0;
			}

			else{

				direction = 1;
				last_attack[1] --;
			}
		}

		else if(direction == 1 && last_attack[1] > 0){

			last_attack[1] --;
			attack_pos[0] = last_attack[0];
			attack_pos[1] = last_attack[0];

			hit = ai_hit(board, board_len, last_attack);

			if(hit != POS_EMPTY && hit == attack_target){

				curr_state = STATE_ATTACK_HORIZONTAL;
				direction = 1;
			}

			else{

				direction = 2;
				last_attack[1] ++;
			}
		}

		else if(direction == 2 && last_attack[0] > 0){

			last_attack[0] --;
			attack_pos[0] = last_attack[0];
			attack_pos[1] = last_attack[0];

			hit = ai_hit(board, board_len, last_attack);

			if(hit != POS_EMPTY && hit == attack_target){

				curr_state = STATE_ATTACK_VERTICAL;
				direction = 2;
			}

			else{

				direction = 3;
				last_attack[0] ++;
			}
		}

		else if(direction == 3 && last_attack[0] < board_len-1){

			last_attack[0] ++;
			attack_pos[0] = last_attack[0];
			attack_pos[1] = last_attack[0];

			hit = ai_hit(board, board_len, last_attack);

			if(hit != POS_EMPTY && hit == attack_target){

				curr_state = STATE_ATTACK_HORIZONTAL;
				direction = 3;
			}

			else{

				direction = 2;
				last_attack[0] --;
			}
		}

		ai_register_hit(attack_pos, hit, board, board_len, history_board, ships_left, shots_on_target);

		delete [] attack_pos;
	}

	else if(curr_state == STATE_ATTACK_HORIZONTAL){

		unsigned short hit;

		if(direction == 0){

			if(last_attack[1] == board_len-1){

				last_attack[1] = first_attack[1] --;
				direction = 1;
				hit = ai_hit(board, board_len, last_attack);
			}

			else{

				last_attack[1] ++;
				hit = ai_hit(board, board_len, last_attack);
				if(hit != attack_target) direction = 1;
			}
		}

		else{

			if(last_attack[1] == 0){

				last_attack[1] = first_attack[1] ++;
				direction = 0;
				hit = ai_hit(board, board_len, last_attack);
			}

			else{

				last_attack[1] --;
				hit = ai_hit(board, board_len, last_attack);
				if(hit != attack_target) direction = 0;
			}
		}

		ai_register_hit(last_attack, hit, board, board_len, history_board, ships_left, shots_on_target);

	}

	else if(curr_state == STATE_ATTACK_VERTICAL){

		unsigned short hit;

		if(direction == 2){

			if(last_attack[0] == board_len-1){

				last_attack[0] = first_attack[0] --;
				direction = 3;
				hit = ai_hit(board, board_len, last_attack);
			}

			else{

				last_attack[0] ++;
				hit = ai_hit(board, board_len, last_attack);
				if(hit != attack_target) direction = 3;
			}
		}

		else{

			if(last_attack[0] == 0){

				last_attack[0] = first_attack[0] ++;
				direction = 2;
				hit = ai_hit(board, board_len, last_attack);
			}

			else{

				last_attack[0] --;
				hit = ai_hit(board, board_len, last_attack);
				if(hit != attack_target) direction = 2;
			}
		}

		ai_register_hit(last_attack, hit, board, board_len, history_board, ships_left, shots_on_target);
	}


}

unsigned short* ai_attack_random(unsigned short** board, unsigned short board_len, unsigned short** history_board, unsigned short* ships_left){

	unsigned short fit_ship_id = 0;
	for(int i=SHIPS_AMOUNT-1; i>=0; i--){

		if(ships_left[i] != 0){

			fit_ship_id = i;
		}
	}

	unsigned short* pos_to_attack = fit_array(history_board, board_len, fit_ship_id, BOARD_STATUS_HIDDEN);
	cout << "got pos to attack" << endl;
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

	delete [] pos_to_attack;
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

void ai_register_hit(unsigned short* pos, unsigned short hit, unsigned short** board, unsigned short board_len, unsigned short** history_board, unsigned short* ships_left, unsigned short shots_on_target){

	unsigned short row = pos[0];
	unsigned short col = pos[1];

	history_board[row][col] = hit;

	if(hit != POS_EMPTY){

		board[row][col] = BOARD_STATUS_HIT;
	}

	bool final = false;

	if(shots_on_target == SHIP_LEN[hit]) final = true;

	if(final){

		cout << "The Computer destroyed a " << SHIP_NAME[hit] << endl;
		ships_left[hit] --;
		AI_REFRESH = true;
		for(int i=0; i<SHIPS_AMOUNT; i++){

			if(ships_left[i] != 0) return;
		}

		
		print_message_game_over(COMPUTER_ID);
	}

}
void print_board(unsigned short** board, unsigned short board_len){

	cout << "   ";
	for(int i=0; i<board_len; i++){

		cout << i+1 << " ";;
	}

	cout << endl;
	for(int i=0; i<board_len; i++){

		cout << char('A' + i )<< "| ";
		for(int j=0; j<board_len; j++){

			if(board[i][j] == POS_EMPTY){

				cout << "_ ";
			}

			else if(board[i][j] == BOARD_STATUS_HIDDEN || board[i][j] == BOARD_STATUS_HIT){

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