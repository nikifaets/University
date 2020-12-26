#include <vector>
#include <iostream>
#include <queue>
#include <cstring>
#include <limits.h>
#include <algorithm>
#include <fstream>
using namespace std;

const int MAX_NODES = 127; //ascii numbers between 33 and 126
const int MIN_NODE = 33;

int* get_shortest_paths(int start, vector<pair<int, int>>* adj){

    priority_queue<pair<int, int>> q;
    int* shortest_path = new int[MAX_NODES];
    memset(shortest_path, -1, sizeof(int)*MAX_NODES);

    q.push(make_pair(0, start));
    shortest_path[start] = 0;

    while(!q.empty()){

        int curr = q.top().second;
        q.pop();

        for(int i=0; i<adj[curr].size(); i++){

            int neighbor = adj[curr][i].second;
            int weight = adj[curr][i].first;

            if(shortest_path[neighbor] == -1 || shortest_path[curr] + weight < shortest_path[neighbor]){

                shortest_path[neighbor] = shortest_path[curr] + weight;
                q.push(adj[curr][i]);
            }
        }
    }

    return shortest_path;
}

int get_shortest_path_between(int from, int to, vector<pair<int,int>>* adj){

    int* shortest_paths = get_shortest_paths(from, adj);
    delete [] shortest_paths;

    return shortest_paths[to] > -1 ? shortest_paths[to] : INT_MAX;
}

pair<int, int> get_closest_common_neighbor(int start1, int start2, vector<pair<int, int>>* adj){

    int* shortest_paths1 = get_shortest_paths(start1, adj);
    int* shortest_paths2 = get_shortest_paths(start2, adj);

    int min_sum = INT_MAX;
    int closest = 0;

    for(int i=MIN_NODE; i<MAX_NODES; i++){

        if(shortest_paths1[i] == -1 || shortest_paths2[i] == -1) continue;
        
        int paths_sum = shortest_paths1[i] + shortest_paths2[i];
        if(paths_sum < min_sum){

            min_sum = paths_sum;
            closest = i;
        }
    }

    delete [] shortest_paths1;
    delete [] shortest_paths2;

    return make_pair(min_sum, closest);
}

void get_input(int &N, int &M, vector<pair<int, int>>* adj, int*& necklace1, int*& necklace2, string filename){

    ifstream f(filename);

    f >> N;
    f >> M;

    necklace1 = new int[N];
    necklace2 = new int[N];

    for(int i=0; i<N; i++){
        
        char c;
        f >> c;
        necklace1[i] = c;

    }

    for(int i=0; i<N; i++){
        
        char c;
        f >> c;
        necklace2[i] = c;

    }

    for(int i=0; i<M; i++){

        char from, to;
        int w;

        f >> from >> to;
        f >> w;
        
        adj[from].push_back(make_pair(w, to));
    }
    
}

bool file_exists(string filename){

    ifstream f(filename);
    return f.good();
}

int main(int argc, char** argv){

    if(argc < 2){

        cout << "Please give file name" << endl;
        return 1;
    }

    string filename = argv[1];

    if(!file_exists(filename)){

        cout << "Invalid file name" << endl;
        return 1;
    }

    int N, M;

    vector<pair<int, int>>adj[MAX_NODES]; // <first, second> := <weight, neighbor>

    int* necklace1, *necklace2;

    get_input(N, M, adj, necklace1, necklace2, filename);

    
    int ans = 0;
    string new_str;

    for(int i=0; i<N; i++){

        if(necklace1[i] == necklace2[i]){

            new_str += necklace1[i];
            continue;
        }

        pair<int, int> shortest_from_1 = make_pair(get_shortest_path_between(necklace1[i], necklace2[i], adj), necklace2[i]);
        pair<int, int> shortest_from_2 = make_pair(get_shortest_path_between(necklace2[i], necklace1[i], adj), necklace1[i]);
        pair<int, int> shortest_common = get_closest_common_neighbor(necklace1[i], necklace2[i], adj);
        pair<int, int> new_val = min({shortest_from_1, shortest_from_2, shortest_common});

        if(new_val.first == INT_MAX){

            cout << -1 << endl;
            cout << "Not possible" << endl;
            return 0;
        }
        
        ans += new_val.first;
        new_str += new_val.second;
    }

    cout << ans << endl;
    cout << new_str << endl;

    delete [] necklace1;
    delete [] necklace2;

    return 0;

}