#include <iostream>
using namespace std;

const unsigned short POS_EMPTY = 10;

const unsigned short CARRIER_ID = 0;
const unsigned short BATTLESHIP_ID = 1;
const unsigned short FRIGATE_ID = 2;
const unsigned short SUBMARINE_ID = 3;
const unsigned short CUTTER_ID = 4;

const unsigned short SHIP_LEN[5] = {5,4,3,3,2};

const unsigned short CARRIER_LEN = 5;
const unsigned short BATTLESHIP_LEN = 4;
const unsigned short FRIGATE_LEN = 3;
const unsigned short SUBMARINE_LEN = 3;
const unsigned short CUTTER_LEN = 2;

const unsigned short NUM_CARRIERS = 1;
const unsigned short NUM_BATTLESHIPS = 1;
const unsigned short NUM_FRIGATES = 2;
const unsigned short NUM_SUBMARINES = 2;
const unsigned short NUM_CUTTERS = 4;

const char SHIP_NAME[5][20] = {"Carrier", "Battleship", "Frigate", "Submarine", "Cutter"};
const char CARDINAL[4][5] = {"1st", "2nd", "3rd", "4th"};

unsigned short get_rand(unsigned short max);
void fit_array(unsigned short** board, unsigned short board_len, unsigned short array_len);
void fill_board(unsigned short** board, unsigned short board_len);
bool array_fits(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short array_len, unsigned short propagation);
void write_array(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short rotation, unsigned short ship_id);

void player_fill_board(unsigned short** board, unsigned short board_len);
void player_input(unsigned short**board, unsigned short board_len, unsigned short ship_id, unsigned short num);

unsigned short get_array_len(unsigned short ship_id);

unsigned short** create_matrix(unsigned short rows_num, unsigned short cols_num);
void delete_matrix(unsigned short** matrix, unsigned short rows_num, unsigned short cols_num);


int main(){


	int len = 6;
	unsigned short** computer_board = create_matrix(len, len);
	unsigned short** player_board = create_matrix(len, len);

	fill_board(computer_board, len);
	player_fill_board(player_board, len);

	for(int i=0; i<len; i++){

		for(int j=0; j<len; j++){

			cout << player_board[i][j] << " ";
		}

		cout << endl;
	}

}

void player_fill_board(unsigned short** board, unsigned short board_len){

	//fit carrier x1
	for(int i=0; i<NUM_CARRIERS; i++){

		player_input(board, board_len, CARRIER_ID, i);
	}

	//fit battleship x1
	for(int i=0; i<NUM_BATTLESHIPS; i++){

		player_input(board, board_len, BATTLESHIP_ID, i);
	}

	//fit frigate x2
	for(int i=0; i<NUM_FRIGATES; i++){

		player_input(board, board_len, FRIGATE_ID, i);
	}

	//fit submarine x2
	for(int i=0; i<NUM_SUBMARINES; i++){

		player_input(board, board_len, SUBMARINE_ID, i);
	}

	//fit cutter x4
	for(int i=0; i<NUM_CUTTERS; i++){

		player_input(board, board_len, CUTTER_ID, i);
	}
}

void player_input(unsigned short** board, unsigned short board_len, unsigned short ship_id, unsigned short num){

	cout << "Input the starting field of your " << CARDINAL[num] << " " << SHIP_NAME[ship_id] << ":"<<endl;

	char* input_field = new char[4];
	cin >> input_field;

	cout << "Input rotation: \n Right: 0 \n Left: 1 \n Up: 2 \n Down: 3." << endl;

	unsigned short rotation;
	cin >> rotation;

	unsigned short row = input_field[0] - 'A' - 1;
	unsigned short col = atoi(input_field + 1) - 1;

	if(array_fits(board, board_len, row, col, SHIP_LEN[ship_id], rotation)){

		write_array(board, board_len, row, col, rotation, ship_id);
	}

	else{

		cout << "No space on the given coordinates. Try again." << endl;
		player_input(board, board_len, ship_id, num);
	}
}
void fill_board(unsigned short** board, unsigned short board_len){

	//fit carrier x1
	for(int i=0; i<NUM_CARRIERS; i++){

		fit_array(board, board_len, CARRIER_ID);
	}

	//fit battleship x1
	for(int i=0; i<NUM_BATTLESHIPS; i++){

		fit_array(board, board_len, BATTLESHIP_ID);
	}

	//fit frigate x2
	for(int i=0; i<NUM_FRIGATES; i++){

		fit_array(board, board_len, FRIGATE_ID);
	}

	//fit submarine x2
	for(int i=0; i<NUM_SUBMARINES; i++){

		fit_array(board, board_len, SUBMARINE_ID);
	}

	//fit cutter x4
	for(int i=0; i<NUM_CUTTERS; i++){

		fit_array(board, board_len, CUTTER_ID);
	}
}

void fit_array(unsigned short** board, unsigned short board_len, unsigned short ship_id){


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
			if(array_fits(board, board_len, i, j, array_len, 0)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 0;
				next_fit ++;
			}

			//try to fit left
			if(array_fits(board, board_len, i, j, array_len, 1)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 1;
				next_fit ++;
			}

			//try to fit up
			if(array_fits(board, board_len, i, j, array_len, 2)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 2;
				next_fit ++;
			}

			//try to fit down
			if(array_fits(board, board_len, i, j, array_len, 3)){

				positions[next_fit][0] = i;
				positions[next_fit][1] = j;
				positions[next_fit][2] = 3;
				next_fit ++;
			}
		}
	}

	if(next_fit == 0){

		cout << "NO SPACE" << endl;
		return;
	}

	//get random valid position
	unsigned short idx = get_rand(next_fit);
	unsigned short row = positions[idx][0];
	unsigned short col = positions[idx][1];
	unsigned short rotation = positions[idx][2];

	write_array(board, board_len, row, col, rotation, ship_id);

	delete_matrix(positions, possible_positions, 3);


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
bool array_fits(unsigned short** board, unsigned short board_len, unsigned short start_row, unsigned short start_col, unsigned short array_len, unsigned short propagation){

	//propagation: {r:0; l:1; u:2; d:3}

	//right
	if(propagation == 0){

		if(start_col + array_len > board_len) return false;

		for(int i=start_col; i<start_col+array_len; i++){

			if(board[start_row][i] != POS_EMPTY){

				return  false;
			}
		}

	}

	//left
	else if(propagation == 1){

		if(start_col - array_len < -1) return false;

		for(int i=start_col; i>start_col-array_len; i--){

			if(board[start_row][i] != POS_EMPTY){

				return false;
			}
		}

	}

	//up
	else if(propagation == 2){

		if(start_row - array_len < -1) return false;

		for(int i = start_row; i>start_row-array_len; i--){

			if(board[i][start_col] != POS_EMPTY){

				return false;
			}
		}

	}

	//down
	else if(propagation == 3){

		if(start_row + array_len > board_len) return false;

		for(int i=start_row; i<start_row+array_len; i++){

			if(board[i][start_col] != POS_EMPTY){

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