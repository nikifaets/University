#include <iostream>
using namespace std;

int main(){

	int n;
	cin >> n;

	unsigned int hemming_nums = 0;
	unsigned long long count = 1;
	while(hemming_nums < n){

		unsigned long long curr_num = count;
		while(curr_num % 5 == 0) curr_num /= 5;
		while(curr_num % 3 == 0) curr_num /= 3;
		while(curr_num % 2 == 0) curr_num /= 2;

		if(curr_num == 1){

			cout << count << endl;
			hemming_nums++;
		} 

		count ++;


	}
	
}