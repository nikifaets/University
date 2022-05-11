#include "bits/stdc++.h"
using namespace std;

struct Tree{

    int val = -1;
    Tree* left = nullptr;
    Tree* right = nullptr;

    Tree(int val): val(val){}
    Tree(int val, Tree* left, Tree* right): val(val), left(left), right(right){}

};

void print_tree(Tree* root){

    //traverse the tree with bfs and print roots
    
    queue<Tree*> q;
    q.push(root);

    while(!q.empty()){

        Tree* curr = q.front();
        q.pop();
        cout << "Node: " << curr->val << endl;;

        if(curr->left != nullptr){
            
            cout << "Left child: " << curr->left->val << endl;
            q.push(curr->left);
        }

        if(curr->right != nullptr){
            cout << "Right child: " << curr->right->val << endl;
            q.push(curr->right);
        }

        cout << "------------------------------" << endl;
    }
}

void mirror(Tree* root){

    swap(root->left, root->right);
    if(root->left != nullptr) mirror(root->left);
    if(root->right != nullptr) mirror(root->right);
}
void get_arr(int*& arr, int len){

    cout << "arr last el " << arr[len-1] << endl;
}

void visit(int start, vector<int>* adj, int node_num, bool* visited, stack<int>* res){

    cout << "Visit " << start << endl;
    if(visited[start]) return;

    visited[start] = true;
    for(int i=0; i<adj[start].size(); i++){

        int neigh = adj[start][i];
        visit(neigh, adj, node_num, visited, res);
    }

    res->push(start);
}

stack<int>* topsort(vector<int>* adj, int len){

    stack<int>* res = new stack<int>();
    bool* visited = new bool[len];
    memset(visited, false, len*sizeof(bool));

    for(int i=0; i<len; i++){
        visit(i, adj, len, visited, res);
    }
    
    return res;
    
}


int main(){


    Tree root(5);
    Tree left(3);
    Tree right(15);
    Tree leftleft(1);
    Tree leftright(4);
    Tree rightright(16);
    root.left = &left;
    root.right = &right;
    root.left->left = &leftleft;
    root.left->right = &leftright;
    root.right->right = &rightright;
    
    print_tree(&root);
    mirror(&root);
    print_tree(&root);

    vector<int> adj[41];
    adj[10].push_back(5);
    adj[20].push_back(6);
    adj[6].push_back(20);
    adj[6].push_back(5);
    adj[5].push_back(6);
    adj[5].push_back(40);
    adj[40].push_back(0);
    adj[0].push_back(3);
    adj[40].push_back(3);

    stack<int>* sorted = topsort(adj, 41);

    while(!sorted->empty()){

        cout << sorted->top() << endl;
        sorted->pop();
    }   

    delete sorted;

}