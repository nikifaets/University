#include <iostream>
#include <string.h>
using namespace std;

void resize(char** dict, int len){

    char** temp = new char* [len];

    for(int i=0; i<len; i++){

        //cout << "copying from dict" << dict[i] << endl;

        temp[i] = new char[strlen(dict[i])];
        strcpy(temp[i], dict[i]);

    }

    //cout << "copied " << endl;

    dict = new char* [len+1];
    //cout << "hehe" << endl;
    for(int i=0; i<len; i++){

        //cout << "copying from temp " << temp[i] << endl;
        dict[i] = new char[strlen(temp[i])];
        strcpy(dict[i], temp[i]);
        delete temp[i];        
    }

    delete[] temp;


}

int main(){

    int len = 1;
    char** dict = new char* [0];

    for(int i=0; i<len+2; i++){

        resize(dict, i);
        char word[65];
        cin >> word;
        dict[i] = new char[strlen(word)];
        strcpy(dict[i], "hui");
        cout << "word from dict " << dict[i] << endl;
    }

    for(int i=0; i<len; i++){

        cout << dict[i] << endl;
        delete[] dict[i];
    }

    delete[] dict;

    
}