#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;

const double EPS = 1e-5;
const double MAX_RAD = 999999;

void handle_input(double **benches, const size_t num_benches, double** lamps, const size_t num_lamps){


    for(int i=0; i<num_benches; i++){

        double x,y;
        cin >> x >> y;
        benches[i][0] = x;
        benches[i][1] = y;    
    }

    for(int i=0; i<num_lamps; i++){

        double x,y,r;
        cin >> x >> y >> r;
        lamps[i][0] = x;
        lamps[i][1] = y;
        lamps[i][2] = r;
    }
    
}

bool is_point_in_circle(double px, double py, double cx, double cy, double cr){

    double dist_to_center = sqrt((px-cx)*(px-cx) + (py-cy)*(py-cy));

    return dist_to_center <= cr || abs(dist_to_center - cr) <= EPS;
}

bool all_benches_lit(double** benches, const size_t num_benches, double** lamps, const size_t num_lamps){

    //test all benches
    bool exit_flag = false;

    for(int i=0; i<num_benches;  i++){

        double px = benches[i][0];
        double py = benches[i][1];

        bool is_lit = false;

        //try for all lamps
        for(int j=0; j<num_lamps;  j++){

            double cx = lamps[j][0];
            double cy = lamps[j][1];
            double cr = lamps[j][2];

            is_lit = is_point_in_circle(px, py, cx, cy, cr);
            if(is_lit) break;
        }

        if(!is_lit){

            exit_flag = true;
            break;
        }
    }

    if(exit_flag){

        return false;
    }

    return true;

}

int dp(double** lamps, const size_t num_lamps, const int original_num_lamps, double** benches, const size_t num_benches, int curr_lamp_idx, int num_removed){

    if(curr_lamp_idx < num_lamps) cout << "checking lamp " << lamps[curr_lamp_idx][0] << " " << lamps[curr_lamp_idx][1] << " " << lamps[curr_lamp_idx][2] << endl;
    
    if(! all_benches_lit(benches, num_benches, lamps, num_lamps)){

        return num_removed-1;
    }
    if(curr_lamp_idx >= num_lamps){

        return num_removed;

    }

 

    
    //remove next and continue
    int next_idx = curr_lamp_idx + 1;

    //make a new array without the curr lamp
    const size_t num_new_lamps = num_lamps - 1;
    double** new_lamps = new double* [num_new_lamps];
    for(int i=0; i<num_new_lamps; i++){

        new_lamps[i] = new double[3];

        if(i<curr_lamp_idx){

            new_lamps[i][0] = lamps[i][0];
            new_lamps[i][1] = lamps[i][1];
            new_lamps[i][2] = lamps[i][2];
        }
        else{

            new_lamps[i][0] = lamps[i+1][0];
            new_lamps[i][1] = lamps[i+1][1];
            new_lamps[i][2] = lamps[i+1][2];
        }

    }

    return max(
        dp(new_lamps, num_new_lamps, original_num_lamps, benches, num_benches, curr_lamp_idx, num_removed+1),
        dp(lamps, num_lamps, original_num_lamps, benches, num_benches, next_idx, num_removed));
    //update lamps array

    
    
}


int main(){

    //handle input
    int n, m;
    cin >> n >> m;

    const size_t num_benches = n;
    const size_t num_lamps = m;

    double** benches = new double* [num_benches];
    for(int i=0; i<num_benches; i++) benches[i] = new double[2];

    double** lamps = new double* [num_lamps];
    for(int i=0; i<num_lamps; i++) lamps[i] = new double[3];

    handle_input(benches, num_benches, lamps, num_lamps);


    //test if all lamps are lit
    bool all_lit = all_benches_lit(benches, num_benches, lamps, num_lamps);

    if(! all_lit){

        cout << "Not all benches are lit!" << endl;

    }

    else{

        cout << "All benches are lit." << endl;

        //count maximum possible lamp removals

        //get min radius
        double min_rad = MAX_RAD;
        for(int i=0; i<num_lamps; i++){

            if(lamps[i][2] < min_rad){
                min_rad = lamps[i][2];

            } 
        }

        for(int i=0; i<num_lamps; i++){

            lamps[i][2] = min_rad;
        }


        int max_removed = dp(lamps, num_lamps, num_lamps, benches, num_benches, 0, 0);

        if(max_removed > 0){
            cout << "Max lamps to be removed: " << max_removed << endl;
        }

        else
        {
            cout << "No lamps can be removed" << endl;
        }
        
    }
    


    //clear allocated memory
    for(int i=0; i<num_benches; i++){

        delete[] benches[i];
    }

    delete [] benches;

    for(int i=0; i<num_lamps; i++){

        delete [] lamps[i];
    }

    delete [] lamps;
    return 0;

}