#include <iostream>
#include <cmath>

using namespace std;

int main(){

	const float RADIUS = 0.5;
	const float X_UPPER = -0.5;
	const float Y_UPPER = 0.5;
	const float X_MIDDLE = 0;
	const float Y_MIDDLE = 0;
	const float X_LOWER = 0.5;
	const float Y_LOWER = -0.5;
	const unsigned short REGION_UPPER = 1;
	const unsigned short REGION_JOINT_CENTER_UPPER = 2;
	const unsigned short REGION_CENTER = 3;
	const unsigned short REGION_JOINT_CENTER_LOWER = 4;
	const unsigned short REGION_LOWER = 5;
	const unsigned short SCORE_ON_EDGE = 100;
	const unsigned short SCORE_OUTSIDE = 0;
	const double ERROR = 1e-9;

	double x,y;

	cin>>x>>y;

	double dist_to_central = sqrt(x*x+y*y);
	double dist_to_upper = sqrt((x-X_UPPER)*(x-X_UPPER) + (y-Y_UPPER)*(y-Y_UPPER));
	double dist_to_lower = sqrt((x-X_LOWER)*(x-X_LOWER) + (y-Y_LOWER)*(y-Y_LOWER));

	bool is_in_upper = 0;
	bool is_in_central = 0;
	bool is_in_lower = 0;

	if(RADIUS - dist_to_central > ERROR){

		is_in_central = 1;
	}

	if(RADIUS - dist_to_lower > ERROR){

		is_in_lower = 1;
	}

	if(RADIUS - dist_to_upper > ERROR){

		is_in_upper = 1;
	}

	//check if point is in 1 or 3 quadrant - if true - print SCORE_OUTSIDE

	if((x - 0. > ERROR && y - 0. > ERROR) || (0. - x > ERROR && 0. - y > ERROR )){

		cout << SCORE_OUTSIDE << endl;
		return 0;
	}


	if(abs(RADIUS - dist_to_upper) < ERROR || abs(RADIUS - dist_to_central) < ERROR || abs(RADIUS - dist_to_lower) < ERROR){

		cout << SCORE_ON_EDGE << endl;

	}

	else if(is_in_upper){

		if(is_in_central){

			cout << REGION_JOINT_CENTER_UPPER << endl;
		}
		else{

			cout << REGION_UPPER << endl;
		}

	}

	else if(is_in_lower){

		if(is_in_central){

			cout << REGION_JOINT_CENTER_LOWER << endl;
		}
		else{

			cout << REGION_LOWER << endl;
		}
	}

	else if(is_in_central){

		cout << REGION_CENTER << endl;
	}

	else{

		cout << SCORE_OUTSIDE << endl;
	}

	return 0;

}