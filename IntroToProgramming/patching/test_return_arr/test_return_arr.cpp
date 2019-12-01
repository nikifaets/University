#include <iostream>

using namespace std;
	
void foo(int *arr, int len){


	for(int i=0; i<len; i++){

		arr[i] ++;
	}

}
int main(){


	int n;
	cin>>n;

	int arr[n];

	for(int i=0; i<n; i++){

		cin >> arr[i];
	}
	foo(arr, n);

	for(int i=0; i<n; i++){

		cout << arr[i] << endl;
	}

}