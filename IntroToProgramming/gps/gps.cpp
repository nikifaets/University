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
		if(a_x < LEFT_X - ERROR || a_x > RIGHT_X + ERROR || a_y < BOTTOM_Y - ERROR || a_y > TOP_Y + ERROR){

			a_out = 1;

		}

		if(b_x < LEFT_X - ERROR || b_x > RIGHT_X + ERROR || b_y < BOTTOM_Y - ERROR || b_y > TOP_Y + ERROR){

			b_out = 1;
		}

		if(a_out && b_out){ ; } //do nothing - program will not enter any other if-statements but need to switch

		else if(!a_out && !b_out){

			distance += calculate_distance(a_x, a_y, b_x, b_y);

		}

		else{

			// swap a and b so that always b is the point outside the rectangle
			if(a_out){

				swap(a_x, b_x);
				swap(a_y, b_y);
			}


			//handle case if a and b have the same y-coordinates
			if(fabs(a_y - b_y) < ERROR){ 

				double intersection_x;
				if(b_x > RIGHT_X - ERROR){

					intersection_x = RIGHT_X;

				}

				else{

					intersection_x = LEFT_X;
				}
				distance += calculate_distance(a_x, a_y, intersection_x, b_y);

			}

			//handle case if a and b have the same x-coordinates
			else if(fabs(a_x - b_x) < ERROR){

				double intersection_y;

				if(a_y > TOP_Y - ERROR){

					intersection_y = TOP_Y;
				}

				else{

					intersection_y = BOTTOM_Y;
				}

				distance += calculate_distance(a_x, a_y, b_x, intersection_y);

			}

			//handle case if a and b are with different x and y coordinates
			else{

				//get line equation and find coordinates of intersections: y = mx+k
				double m = (a_y - b_y) / (a_x - b_x);
				double k = a_y - m * a_x;

				//get intersection coordinates; x = (y-k)/m
				double intersection_top_x = (TOP_Y - k) / m;
				double intersection_bottom_x = (BOTTOM_Y - k) / m;
				double intersection_right_y = RIGHT_X * m + k;
				double intersection_left_y = LEFT_X * m + k;

				//chech which side does the intersection point lie on
				bool on_top = 0;
				bool on_bottom = 0;
				bool on_right = 0;
				bool on_left = 0;

				if((intersection_top_x < RIGHT_X - ERROR && intersection_top_x > LEFT_X + ERROR) ||
				  fabs(intersection_top_x - RIGHT_X) < ERROR ||
				  fabs(intersection_top_x - LEFT_X) < ERROR){ //also check for the two corners - up, right and up, left

					on_top = 1;

				} 

				else if((intersection_bottom_x < RIGHT_X - ERROR && intersection_bottom_x > LEFT_X + ERROR) || 
				  fabs(intersection_bottom_x - RIGHT_X) < ERROR ||
				  fabs(intersection_bottom_x - LEFT_X) < ERROR){ //check for the other two corners - bottom, right and bottom, left

					on_bottom = 1;
				}

				else if(intersection_left_y < TOP_Y - ERROR && intersection_left_y > BOTTOM_Y + ERROR){

					on_left = 1;
				}

				else if(intersection_right_y < TOP_Y - ERROR && intersection_right_y > BOTTOM_Y + ERROR){

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