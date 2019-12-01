#include <iostream>
using namespace std;

const unsigned short PROJ_AMOUNT = 64;
const unsigned short CABLE_LEN = 513;

const short ERR_CABLE_FULL = -1;
const short ERR_SN_NOT_FOUND = -404; //for the meme
const short ERR_PROJ_FAKE = -2;
const short ERR_OUTSIDE_CABLE = -3;
const short ERR_SN_USED = -4;
const short NO_ERRORS = 0;

const unsigned short STATUS_EMPTY = 0;
const unsigned short STATUS_GOOD = 1;
const unsigned short STATUS_DUPLICATE = 2;
const unsigned short STATUS_CONFLICT = 3;

int projectors[PROJ_AMOUNT][3]; //[n][0] -> SN; [n][1] -> start; [n][2] -> len
unsigned long long cable[CABLE_LEN][2]; //[n][0] -> status; [n][1] -> projectors on channel

void move_proj(unsigned int SN, unsigned short new_start);
void remove_proj(unsigned int SN);
void update_region(unsigned short start, unsigned short len);
void add_proj(unsigned int SN, unsigned short start, unsigned short len);

short SN_to_idx(unsigned int SN);
short find_SN(unsigned int SN);

short validate_input(unsigned int SN, unsigned short start, unsigned short len);
short validate_SN(unsigned int SN);

unsigned short get_furthest_connection_left(unsigned short start, unsigned short len);
unsigned short get_furthest_connection_right(unsigned short start, unsigned short len);

unsigned short get_projectors_in_range(unsigned short start, unsigned short len, short* projectors_in_range);

void handle_error(short error);

void print_state();

int main(){

	for(int i=0; i<PROJ_AMOUNT; i++){

		projectors[i][0] = -1;
		projectors[i][1] = -1;
		projectors[i][2] = -1;
	}

	for(int i=0; i<CABLE_LEN; i++){

		cable[i][0] = 0;
		cable[i][1] = 0;
	}

	char input;

	while(input != 'e'){

		cin >> input;

		switch(input){

			case 'a':

				{
					unsigned int SN;
					unsigned short len, start;
					
					cin >> SN >> len >> start;
					
					add_proj(SN, start, len);

					break;
				}

			case 'r':

				{
					unsigned int SN;
					
					cin >> SN;
					
					remove_proj(SN);
					
					break;

				}

			case 'm':

				{

					unsigned int SN;
					unsigned short new_start;
					
					cin >> SN >> new_start;
					
					move_proj(SN, new_start);
					
					break;

				}

			case 'p':

				{

					print_state();
					
					break;
				}

			default:
				
				break;

		}
	}
}

short find_SN(unsigned int SN){

	for(int i=0; i<PROJ_AMOUNT; i++){

		if(projectors[i][0] == SN){

			return i;
		}
	}

	return ERR_SN_NOT_FOUND;
}

short SN_to_idx(){

	for(int i=0; i<PROJ_AMOUNT; i++){

		if(projectors[i][0] == -1){

			return i;
		}
	}

	return ERR_CABLE_FULL;
}

short validate_SN(unsigned int SN){


	for(int i=0; i<PROJ_AMOUNT; i++){

		if(projectors[i][0] > -1){

			if(projectors[i][0] == SN) return ERR_SN_USED;
		}
	}

	unsigned short sum_of_bits = 0;

	unsigned int SN_save = SN;
	while(SN){

		sum_of_bits += SN%2;
		SN /= 2;

	}

	if(SN_save % sum_of_bits) return ERR_PROJ_FAKE;



	return NO_ERRORS;
}

short validate_input(unsigned int SN, unsigned short start, unsigned short len){

	short valid_SN = validate_SN(SN);

	if(valid_SN != NO_ERRORS){

		return valid_SN;
	}

	//check if new projector is in range 1 -- 512

	if(start < 1 || start + len > CABLE_LEN){

		return ERR_OUTSIDE_CABLE;
	}

	return NO_ERRORS;

}

void add_proj(unsigned int SN, unsigned short start, unsigned short len){

	short validity = validate_input(SN, start, len);

	if(validity != NO_ERRORS){

		handle_error(validity);
		return;
	}

	short idx_assigned = SN_to_idx();

	if(idx_assigned < 0){

		handle_error(idx_assigned);
		return;
	}

	projectors[idx_assigned][0] = SN;
	projectors[idx_assigned][1] = start;
	projectors[idx_assigned][2] = len;

	//attempt to duplicate

	//get status of first element
	unsigned short status_first = cable[start][0];
	bool duplicate = true;


	if((status_first == STATUS_GOOD || status_first == STATUS_DUPLICATE)){

		for(int i=start; i<start+len; i++){

			if(cable[i][0] != status_first){

				duplicate = false;
				break;
			}
		}

	}

	else duplicate = false;

	if(duplicate){

		//check if all the channels in the region belong to the same projector
		short projectors_in_range[PROJ_AMOUNT];

		unsigned short last_idx = get_projectors_in_range(start, len, projectors_in_range);
		unsigned short first_proj = projectors_in_range[0];

		for(unsigned short i=start; i<start+len; i++){

			if(! (cable[i][1] & (1<<first_proj)) ){

				duplicate = false;
				break;
			}
		}



		if(projectors[first_proj][1] != start || projectors[first_proj][2] != len) duplicate = false;

	}


	if(duplicate){

		for(int i=start; i<start+len; i++){

			cable[i][0] = STATUS_DUPLICATE;
		}
	}

	else{

		for(int i=start; i<start+len; i++){


			if(cable[i][0] == STATUS_DUPLICATE || cable[i][0] == STATUS_GOOD){

				cable[i][0] = STATUS_CONFLICT;
			}

			else if(cable[i][0] == STATUS_EMPTY){

				cable[i][0] = STATUS_GOOD;
	
			}

		}
	}

	//add the new projector to cable's bitmask

	for(unsigned short i=start; i<start+len; i++){

		cable[i][1] |= (1<<idx_assigned);
	}


	
}



void print_state(){

	for(int i=1; i<CABLE_LEN; i++){

		switch(cable[i][0]){

			case STATUS_EMPTY:

				cout << "-";
				break;

			case STATUS_GOOD:

				cout << "o";
				break;

			case STATUS_DUPLICATE:

				cout << "d";
				break;

			case STATUS_CONFLICT:

				cout << "x";
				break;
		}

	}
}

unsigned short get_projectors_in_range(unsigned short start, unsigned short len, short* projectors_in_range){

	for(int i=0; i<PROJ_AMOUNT; i++){

		projectors_in_range[i] = -1;
	}

	unsigned short last_idx = 0;
	bool visited[PROJ_AMOUNT];

	for(int i=0; i<PROJ_AMOUNT; i++) visited[i] = false; 

	//start decoding bitmask

	for(int i=start; i<start+len; i++){

		unsigned short channel_bitmask = cable[i][1];

		unsigned short count_idx = 0;

		while(channel_bitmask){

			if(channel_bitmask % 2 == 1){

				if(!visited[count_idx]){

					projectors_in_range[last_idx] = count_idx;
					visited[last_idx] = true;
					last_idx ++;
				}
			}

			channel_bitmask /= 2;
			count_idx ++;
		}
	}


	return last_idx;
}


void handle_error(short error){

	switch(error){

		case ERR_OUTSIDE_CABLE:

			cout << "New projector channels are outside the cable" << endl;
			break;

		case ERR_PROJ_FAKE:

			cout << "The projector is fake." << endl;
			break;

		case ERR_CABLE_FULL:

			cout << "Cable is full." << endl;
			break;

		case ERR_SN_NOT_FOUND:

			cout << "Error 404. Serial number not found." << endl; // haha
			break;

		case ERR_SN_USED:

			cout << "Serial number is already in use." << endl;
			break;
	}
}

void remove_proj(unsigned int SN){

	unsigned long long idx = find_SN(SN);

	if(idx < NO_ERRORS){

		handle_error(idx);
		return;
	}

	//clear from cable

	unsigned short proj_start = projectors[idx][1];
	unsigned short proj_len = projectors[idx][2];

	for(int i=proj_start; i<proj_start + proj_len; i++){

		cable[i][1] &= ~(1<<idx);
	}

	projectors[idx][0] = -1;
	projectors[idx][1] = -1;
	projectors[idx][2] = -1;

	//assumption: every row of channels, belonging to a single projector, which is directly or indirectly connected to the current one
	//might be affected by the removal

	unsigned short start = get_furthest_connection_left(proj_start, proj_len);
	unsigned short end = get_furthest_connection_right(proj_start, proj_len);
	unsigned short len = end - start + 1;

	update_region(start, len);


}

void move_proj(unsigned int SN, unsigned short new_start){

	short idx = find_SN(SN);
	unsigned int proj_len = projectors[idx][2];

	if(idx < NO_ERRORS){

		handle_error(idx);
		return;
	}

	if(projectors[idx][1] + new_start > CABLE_LEN || projectors[idx][1] < 1){

		handle_error(ERR_OUTSIDE_CABLE);
		return;
	}


	remove_proj(SN);
	add_proj(SN, new_start, proj_len);

}

void update_region(unsigned short start, unsigned short len){

	short projectors_in_range[PROJ_AMOUNT];
	unsigned short last_idx = get_projectors_in_range(start, len, projectors_in_range);

	for(int i=0; i<last_idx; i++){

		unsigned long long curr_proj_idx = projectors_in_range[i];
		unsigned int curr_proj_SN = projectors[curr_proj_idx][0];
		unsigned short curr_proj_start = projectors[curr_proj_idx][1];
		unsigned short curr_proj_len = projectors[curr_proj_idx][2];

		//clear cable bitmask
		for(int j=curr_proj_start; j<curr_proj_start + curr_proj_len; j++){

			cable[j][1] &= ~(1<<curr_proj_idx);
		}

	}

	//clear cable status
	for(int i=start; i<start+len; i++){

		cable[i][0] = STATUS_EMPTY;
	}

	//set all the channels anew

	for(int i=0; i<last_idx; i++){

		unsigned short curr_proj_idx = projectors_in_range[i];
		unsigned int curr_proj_SN = projectors[curr_proj_idx][0];
		unsigned short curr_proj_start = projectors[curr_proj_idx][1];
		unsigned short curr_proj_len = projectors[curr_proj_idx][2];

		projectors[curr_proj_idx][0] = -1;
		projectors[curr_proj_idx][1] = -1;
		projectors[curr_proj_idx][2] = -1;

		add_proj(curr_proj_SN, curr_proj_start, curr_proj_len);


	}
}
unsigned short get_furthest_connection_left(unsigned short start, unsigned short len){

	if(start == 1) return start;
	//get all projectors in the range

	short projectors_in_range[PROJ_AMOUNT];
	
	unsigned short last_idx = get_projectors_in_range(start,len, projectors_in_range);
	//if there are no projectors in range
	if(last_idx == 0){

		return start;

	}

	//get the leftmost starting point
	unsigned short min = start;

	for(int i=0; i<last_idx; i++){

		unsigned short curr_start = projectors[projectors_in_range[i]][1];

		if(curr_start < min){

			min = curr_start;
		}
	}

	if(min == start) return start;

	unsigned short new_range_len = start-min+1;

	return get_furthest_connection_left(min, new_range_len);

}

unsigned short get_furthest_connection_right(unsigned short start, unsigned short len){


	if(start + len - 1 == CABLE_LEN) return CABLE_LEN;

	//get all projectors in the range

	short projectors_in_range[PROJ_AMOUNT];

	unsigned short last_idx = get_projectors_in_range(start, len, projectors_in_range);

	//if there are no projectors in range
	if(last_idx == 0){

		return start + len -1;

	}

	//get the rightmost starting point
	unsigned short max = start + len -1;

	for(int i=0; i<last_idx; i++){

		unsigned short curr_proj = projectors_in_range[i];

		unsigned short curr_end = projectors[curr_proj][1] + projectors[curr_proj][2] - 1;

		if(curr_end > max){

			max = curr_end;
		}
	}

	if(max == len+start-1) return max;

	unsigned short curr_end = start+len-1;
	return get_furthest_connection_right(curr_end, max - curr_end + 1);

}