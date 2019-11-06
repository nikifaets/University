#include <iostream>
#include <math.h>

using namespace std;



int main(){


	const double ERROR = 1e-3;

	const unsigned short SMALLEST_CIRCLE_RADIUS = 1;
	const unsigned short MIDDLE_CIRCLE_RADIUS = 3;
	const unsigned short BIGGEST_CIRCLE_RADIUS = 8;

	const unsigned short SMALLEST_CIRCLE_REWARD = 60;
	const unsigned short MIDDLE_CIRCLE_REWARD = 20;
	const unsigned short BIGGEST_CIRCLE_REWARD = 10;

	const unsigned short INVALID_SHOT_SCORE = 0;

	double x, y, x_offset, y_offset;
	cin >> x >> y >> x_offset >> y_offset;

	x += x_offset;
	y += y_offset;


	double dist = sqrt(x*x + y*y);

	if(dist-SMALLEST_CIRCLE_RADIUS < -ERROR){

		cout << SMALLEST_CIRCLE_REWARD << endl;
	}

	else if(dist-SMALLEST_CIRCLE_RADIUS > ERROR && dist-MIDDLE_CIRCLE_RADIUS < -ERROR){

		cout << MIDDLE_CIRCLE_REWARD << endl;
	}

	else if(dist-BIGGEST_CIRCLE_RADIUS < -ERROR && dist-MIDDLE_CIRCLE_RADIUS > ERROR){

		cout << BIGGEST_CIRCLE_REWARD << endl;
	}

	else{

		cout << INVALID_SHOT_SCORE << endl;
	}

	return 0;
}