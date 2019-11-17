#include <iostream>
#include <bitset>
using namespace std;

int main(){

	unsigned short n;
	cin >> n;

	if(n < 1 || n > 32){

		cout << " Ivalid input. ";
		return 0;
	}

	unsigned long long max_id = 0;
	unsigned short ones_count = 0;
	int bitmask = 0;

	for(int i=0; i<n; i++){

		unsigned long long id;
		cin >> id;

		if(id > max_id){

			max_id = id;
			ones_count += 1;
			bitmask |= (1<<i);
		}



	}

	unsigned short middle = ones_count;

	if(ones_count % 2 == 1){

		middle ++;
	}

	unsigned short zeros_count_left = 0;
	unsigned short zeros_count_right = 0;

	for(int i=0; i<n; i++){

		if( !(bitmask & (1<<i)) ){

			if(n < middle){

				zeros_count_left++;
			}
			else{

				zeros_count_right++;
			}
		}

	}

	if(zeros_count_left > zeros_count_right) cout << "Left side: ";

	else if(zeros_count_left < zeros_count_right) cout << "Right side: ";

	else cout << "Both sides: ";

	cout << max(zeros_count_right, zeros_count_left) << endl;
	return 0;

}