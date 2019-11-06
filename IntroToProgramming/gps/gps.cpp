#include <iostream>
#include <cmath>
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

		float save_b_x = b_x;
		float save_b_y = b_y;



		//check if points are in or outside
		bool a_out = 0;
		bool b_out = 0;
		if(a_x < LEFT_X - ERROR || a_x > RIGHT_X + ERROR || a_y < BOTTOM_Y - ERROR || a_y > TOP_Y + ERROR){

			a_out = 1;

		}

		if(b_x < LEFT_X - ERROR || b_x > RIGHT_X + ERROR || b_y < BOTTOM_Y - ERROR || b_y > TOP_Y + ERROR){

			b_out = 1;
		}

		if(a_out && b_out){ ; } 

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
				bool on_top = 0;
				bool on_bottom = 0;
				bool on_right = 0;
				bool on_left = 0;

				if((intersection_top_x < RIGHT_X - ERROR && intersection_top_x > LEFT_X + ERROR) ||
				  fabs(intersection_top_x - RIGHT_X) < ERROR ||
				  fabs(intersection_top_x - LEFT_X) < ERROR){ //also check for the two corners

					on_top = 1;

				} 

				else if((intersection_bottom_x < RIGHT_X - ERROR && intersection_bottom_x > LEFT_X + ERROR) || 
				  fabs(intersection_bottom_x - RIGHT_X) < ERROR ||
				  fabs(intersection_bottom_x - LEFT_X) < ERROR){ //check for the other two corners

					on_bottom = 1;
				}

				else if(intersection_left_y < TOP_Y - ERROR && intersection_left_y > BOTTOM_Y + ERROR){

					on_left = 1;
				}

				else if(intersection_right_y < TOP_Y - ERROR && intersection_right_y > BOTTOM_Y + ERROR){

					on_right = 1;
				}

				if(on_top){

					b_x = intersection_top_x;
					b_y = TOP_Y; 
				}

				else if(on_bottom){

					b_x = intersection_bottom_x;
					b_y = BOTTOM_Y;
				}

				else if(on_left){

					b_x = LEFT_X;
					b_y = intersection_left_y;

				}

				else if(on_right){

					b_x = RIGHT_X;
					b_y = intersection_right_y;
				}

				distance += calculate_distance(a_x, a_y, b_x, b_y);
			}

		}
		a_x = save_b_x;
		a_y = save_b_y;
	}

	cout << distance << endl;
}