#include <iostream>
#include <string.h>
using namespace std;

const char CHAR_EOF = '#';
const int MAX_WORD_LEN = 65;

bool is_delimeter(char c){

    return 
    (c < 'a' || c > 'z') &&
    (c < 'A' || c > 'Z') &&
    c != '-';

}

void to_lowercase(char* word){

    const size_t len = strlen(word);

    for(int i=0; i<len; i++){

        word[i] =  tolower(word[i]);
    }
    
}

char** resize_dict(char** dict, int &dict_len){

    //copy dict into temp
    char** temp_dict = new char* [dict_len];
    for(int i=0; i<dict_len; i++){

        temp_dict[i] = new char[strlen(dict[i])+1];
        strcpy(temp_dict[i], dict[i]);
    }

    //resize dict
    const size_t new_dict_len = dict_len+1;
    dict = new char* [new_dict_len];

    //copy temp into resized dict
    for(int i=0; i<dict_len; i++){

        dict[i] = new char[strlen(temp_dict[i])+1];
        strcpy(dict[i], temp_dict[i]);

    }

    dict_len ++;

    //delete temp
    for(int i=0; i<dict_len-1; i++){

        delete[] temp_dict[i];
    }

    delete[] temp_dict;

    return dict;
}

char** add_word(char* input, char** dict, int &dict_len){

    dict = resize_dict(dict, dict_len);
    const size_t new_len = strlen(input);

    char* new_word = new char[new_len+1];

    strcpy(new_word, input);
    dict[dict_len-1] = new char[new_len];
    dict[dict_len-1] = new_word;

    return dict;

}

bool is_in_set(char* str, char** set, int set_len){

    if(!set_len) return false;

    for(int i=0; i<set_len; i++){

        if(!strcmp(str, set[i])) return true;
    }

    return false;
}

void print_dict(char** dict, const int dict_len){

    cout << "Dict size " << dict_len << endl;
    for(int i=0; i<dict_len; i++){

        cout << dict[i] << endl;
    }
}

int count_occurences(char* str, char** dict, const int dict_len){

    int res = 0;

    for(int i=0; i<dict_len; i++){

        if(! strcmp(str, dict[i])) res++;
    }

    return res;
}

void print_sorted(char** dict, const int dict_len, char** set, const int set_len){

    int str_occurences[set_len];
    memset(str_occurences, 0, sizeof(int)*set_len);
    
    for(int i=0; i<set_len; i++){

        str_occurences[i] = count_occurences(set[i], dict, dict_len);
    }

    //sort using bubble sort
    for(int i=0; i<set_len; i++){

        for(int j=i; j<set_len; j++){

            if(str_occurences[i] < str_occurences[j]){

                swap(str_occurences[i], str_occurences[j]);
                swap(set[i], set[j]);
            }

            else if(str_occurences[i] == str_occurences[j]){

                if(strcmp(set[i], set[j]) > 0){

                    swap(str_occurences[i], str_occurences[j]);
                    swap(set[i], set[j]);
                }
            }
        }

    }

    for(int i=0; i<set_len; i++){

        cout << set[i] << " - " << str_occurences[i] << endl;
    }

}
int main(){


    char input;
    int word_len = 0;
    char* word = new char[MAX_WORD_LEN];

    int dict_len = 0;
    char** dict = new char* [0];

    char** dict_set = new char* [0];
    int set_len = 0;

    while(input != CHAR_EOF){

        cin.get(input);

        bool is_delim = is_delimeter(input);

        if(is_delim){

            if(word_len > 0){
                
                to_lowercase(word);

                //populate dict with repetitions
                dict = add_word(word, dict, dict_len);

                //populate dict without repetitions
                if(!is_in_set(word, dict_set, set_len)){

                    dict_set = add_word(word, dict_set, set_len);
                }

                word = new char[MAX_WORD_LEN];
                word_len = 0;

            }

        }

        else{

            word[word_len] = input;
            word_len ++;
        }
    } 

    cout << "Word in text: " << dict_len << endl;
    cout << "Unique words in text: " << set_len << endl;
    print_sorted(dict, dict_len, dict_set, set_len);

    //free allocated dynamic memory
    for(int i=0; i<set_len; i++){

        delete[] dict_set[i];
    }

    delete[] dict_set;

    for(int i=0; i<dict_len; i++){

        delete[] dict[i];
    }

    delete[] dict;

    
}