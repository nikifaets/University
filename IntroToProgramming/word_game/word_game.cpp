#include <iostream>
#include <string.h>

using namespace std;

bool is_sorted(char** dict, const size_t dict_len, char* word){

    for(int i=0; i<dict_len; i++){

        if(strcmp(dict[i], word) > 0) return false;
    }

    return true;

}
char** input_dict(const size_t dict_len){


    char** dict = new char* [dict_len];

    for(int i=0; i<dict_len; i++){

        char word[101];
        cin >> word;

        if(! is_sorted(dict, i, word)){

            cout << "Not sorted! ;)" << endl;
        }

        const size_t word_len = strlen(word) + 1;
        dict[i] = new char[word_len];
        strcpy(dict[i], word);
    }

    return dict;
}

char** input_board(const size_t len){

    char** board = new char* [len];

    for(int i=0; i<len; i++){

        board[i] = new char[len];

        for(int j=0; j<len; j++){
            char c;
            cin >> c;
            board[i][j] = c;

        }
        
    }

    return board;
}

bool find_next_char(char** dict, const size_t dict_len, char** board, const size_t board_len, bool** visited, char* word, const int word_len, const int word_pos, int curr_x, int curr_y){

     
    if(curr_x < 0 || curr_y < 0 || curr_y >= board_len || curr_x >= board_len) return false;
    visited[curr_x][curr_y] = true;

    if(word_pos >= word_len) return false;
    if(board[curr_x][curr_y] != word[word_pos]) return false;
    if(word[word_pos] == word[word_len-1] && word_pos == word_len-1) return true;

    //look for next letter in the word in the adjacent fields
    for(int i=-1; i<=1; i++){

        if(curr_x + i < 0 || curr_x + i >= board_len) continue;
        for(int j=-1; j<=1; j++){

            if(curr_y + j < 0 || curr_y + j >= board_len) continue;

            if(i == 0 && j == 0) continue;
            if(visited[curr_x+i][curr_y+j]) continue;

            if (find_next_char(dict, dict_len, board, board_len, visited, word, word_len, word_pos+1, curr_x+i, curr_y+j)){

                return true;
            }
        }
    }

    return false;
}
int main(){

    int n;
    cin >> n;
    
    const size_t dict_len = n;
    char** dict = input_dict(dict_len);

    int k;
    cin >> k;

    const size_t board_len = k;
    char** board = input_board(board_len);

    bool* found = new bool[dict_len];

    memset(found, false, dict_len*sizeof(bool));

    //created visited matrix
    bool** visited = new bool* [board_len];
    for(int i=0; i<k; i++){

        visited[i] = new bool[board_len];

        for(int j=0; j<board_len; j++){

            visited[i][j] = false;
        }
    }

    int longest_word = -1;

    //start searching for words
    for(int i=0; i<dict_len; i++){

        cout << "Searching for word: " << dict[i] << endl;
        //clear visited
        for(int vi=0; vi<k; vi++){
            for(int vj=0; vj<k; vj++){
                visited[vi][vj] = false;
            }
        }

        for(int w=0; w<k; w++){
            
            for(int h=0; h<k; h++){

                if(board[w][h] == dict[i][0]){

                    if(find_next_char(dict, dict_len, board, board_len, visited, dict[i], strlen(dict[i]), 0, w, h)){

                        cout << "Found " << dict[i] << endl;
                        found[i] = true;
                        int word_len = strlen(dict[i]);
                        if(word_len > longest_word) longest_word = strlen(dict[i]);
                    }
                }
            }
        }
    }

    if(longest_word == -1){

        cout << "No words found " << endl;
 
    }

    else{
        cout << "Max length " << longest_word << endl;

        cout << "Found words with max length: " << endl;
        for(int i=0; i<dict_len; i++){

            if(strlen(dict[i]) == longest_word && found[i]){

                cout << dict[i] << endl;
            }
        }
    }
    
    //delete allocated memory
    for(int i=0; i<dict_len; i++){

        delete[] dict[i];
    }

    delete[] dict;

    for(int i=0; i<k; i++){

        delete[] board[i];
        delete[] visited[i];
    }

    delete[] board;
    delete[] visited;
    delete[] found;

    return 0;
    

}