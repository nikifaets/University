#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

const short LEFT_X = -5;
const short BOTTOM_Y = -2;
const unsigned short RIGHT_X = 5;
const unsigned short TOP_Y = 2;
const float ERROR = 1e-2;
const float PI = 3.14;

bool equal(float x, float y){

	return fabs(x-y) < ERROR;
}

bool is_on_left(float x){

	if(equal(x, LEFT_X)) return 0;

	return x < LEFT_X;
}

bool is_on_right(float x){

	if(equal(x, RIGHT_X)) return 0;

	return x > RIGHT_X;
}

bool is_above(float y){

	if(equal(y, TOP_Y)) return 0;

	return y > TOP_Y;
}

bool is_beneath(float y){

	if(equal(y, BOTTOM_Y)) return 0;

	return y < BOTTOM_Y;
}


long double calculate_distance(float a_x, float a_y, float b_x, float b_y){


	long double distance_arg = sqrt(((a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y)));
	return ((PI / 2)*distance_arg + distance_arg) / 2;

}
int main(){

	unsigned long int n;
	long double distance = 0;

	cin >> n;
	float a_x, a_y, b_x, b_y; 
	cin >> a_x >> a_y;
	for(int i=1; i<n; i++){

		cin >> b_x >> b_y;

		float saved_b_x = b_x; // save initial value of b because it may swap with a later
		float saved_b_y = b_y;


		//check for both points whether they are inside or outside
		bool a_out = 0;
		bool b_out = 0;
		if(is_on_left(a_x) || is_on_right(a_x) || is_beneath(a_y) || is_above(a_y)){

			a_out = 1;

		}

		if(is_on_left(b_x) || is_on_right(b_x) || is_beneath(b_y) || is_above(b_y)){

			b_out = 1;
		}

		//both points are outside
		if(a_out && b_out){

			//swap points and continue without incrementing distance
			a_x = saved_b_x;
			a_y = saved_b_y;

			continue;
		} 

		//both points are inside
		else if(!a_out && !b_out){

			distance += calculate_distance(a_x, a_y, b_x, b_y);

		}

		//just one point is outside
		else{

			// swap a and b so that always b is the point outside the rectangle
			if(a_out){

				swap(a_x, b_x);
				swap(a_y, b_y);
			}


			//handle case when a and b have the same y-coordinates
			if(equal(a_y, b_y)){ 

				double intersection_x;
				if(is_on_right(b_x)){

					intersection_x = RIGHT_X;

				}

				else{

					intersection_x = LEFT_X;
				}
				distance += calculate_distance(a_x, a_y, intersection_x, b_y);

			}

			//handle case when a and b have the same x-coordinates
			else if(equal(a_x, b_x)){

				double intersection_y;

				if(is_above(b_y)){

					intersection_y = TOP_Y;
				}

				else{

					intersection_y = BOTTOM_Y;
				}

				distance += calculate_distance(a_x, a_y, b_x, intersection_y);

			}

			//handle case when a and b are with different x and y coordinates
			else{

				//get line equation and find coordinates of intersections: y = mx+k
				double m = (a_y - b_y) / (a_x - b_x);
				double k = a_y - m * a_x;

				//get intersection coordinates; x = (y-k)/m
				double intersection_top_x = (TOP_Y - k) / m;
				double intersection_bottom_x = (BOTTOM_Y - k) / m;
				double intersection_right_y = RIGHT_X * m + k;
				double intersection_left_y = LEFT_X * m + k;

				//check which side does the intersection point lie on
				bool on_top = 0;
				bool on_bottom = 0;
				bool on_right = 0;
				bool on_left = 0;

				if((!is_on_right(intersection_top_x) && !is_on_left(intersection_top_x) && !is_beneath(b_y)) ||
				  equal(intersection_top_x, RIGHT_X) ||
				  equal(intersection_top_x, LEFT_X)){ //also check for the two corners - up, right and up, left

					on_top = 1;

				} 

				else if((!is_on_left(intersection_bottom_x) && !is_on_right(intersection_bottom_x) && !is_above(b_y)) || 
				  equal(intersection_bottom_x, RIGHT_X) ||
				  equal(intersection_bottom_x, LEFT_X)){ //check for the other two corners - bottom, right and bottom, left

					on_bottom = 1;
				}

				else if(!is_above(intersection_left_y) && !is_beneath(intersection_left_y) && !is_on_right(b_x)){

					on_left = 1;
				}

				else if(!is_beneath(intersection_right_y) && !is_above(intersection_right_y) && !is_on_left(b_x)){

					on_right = 1;
				}

				//transform b to lie on one of the sides

				float transformed_b_x = b_x;
				float transformed_b_y = b_y;
				if(on_top){

					transformed_b_x = intersection_top_x;
					transformed_b_y = TOP_Y; 
				}

				else if(on_bottom){

					transformed_b_x = intersection_bottom_x;
					transformed_b_y = BOTTOM_Y;
				}

				else if(on_left){

					transformed_b_x = LEFT_X;
					transformed_b_y = intersection_left_y;

				}

				else if(on_right){

					transformed_b_x = RIGHT_X;
					transformed_b_y = intersection_right_y;
				}

				distance += calculate_distance(a_x, a_y, transformed_b_x, transformed_b_y);
			}

		}
		a_x = saved_b_x;
		a_y = saved_b_y;
	}

	distance = floor(distance*1000);
	distance /= 1000;

	cout <<	 distance << endl;
}