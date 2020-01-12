#include <iostream>
#include <cstring>

using namespace std;

void decompress(char* input, int *input_it, char* res, int *res_it, int *res_len);
int read_num(char* input, int *it);

char* get_new_buffer(char* buffer, int old_len, int new_len){


	char* temp[new_len];
	
	memcpy(temp, buffer, sizeof(char)*old_len);

	buffer = new char[new_len];
	
	memcpy(buffer, temp, sizeof(char)*new_len);

	return buffer;
}


int main(){

	int buffer_len = 4;

	char *buffer = new char[16]; 

	char input;
	char delim = '\n';
	int input_len = 0;

	while(input != delim){

		input = cin.get();


		buffer[input_len] = input;
		input_len++;

		if(input_len >= buffer_len){

			buffer = get_new_buffer(buffer, buffer_len, buffer_len*2);
			buffer_len *= 2;
			
		} 
	}

	cout << buffer << endl;

	int input_it = 0;
	int res_it = 0;
	int res_len = buffer_len;
	char* res = new char[buffer_len];

	decompress(buffer, &input_it, res, &res_it, &res_len);

	for(int i=0; i<res_len; i++){

		cout << res[i] << " ";
	}

	cout << endl;
}

void decompress(char* input, int *input_it, char* res, int *res_it, int *res_len){

	while(input[*input_it] != '\n' && input[*input_it] != ')'){



		char curr = input[*input_it];

		if(curr >= '1' && curr <= '9'){

			int num = read_num(input, input_it);

			for(int i=0; i<num-1; i++){

				int static_it = *input_it;
				decompress(input, &static_it, res, res_it, res_len);
			} 

			decompress(input, input_it, res, res_it, res_len);

		}

		else if(curr >= 'A' && curr <= 'Z'){

			if(*res_it >= *res_len){

				res = get_new_buffer(res, *res_len, (*res_len)*2);
				(*res_len) *= 2;
			}

			res[*res_it] = curr;
			(*res_it) ++;

		}

		(*input_it) ++;
	}

	return;
}

int read_num(char* input, int *it){

	char digits[16];
	int digits_counter = 0;

	while(input[*it] != '('){

		digits[digits_counter] = input[*it];
		digits_counter ++;
		(*it)++;
	}

	int res_num = 0;
	int tens = 1;

	for(int i=digits_counter-1; i>=0; i--){

		res_num += (digits[i] - '0')*tens;
		tens *= 10;

	}

	return res_num;
}