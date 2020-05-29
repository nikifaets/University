#include "Parser/Parser.h"
#include "Platform/Platform.h"


int main(){

    for(int i=0; i<5; i++){

        Platform::add_user("Mama", "Gosho", i);
        Platform::add_moderator("Tate", "Pesho", i);
    }

    Platform::show_info();
    
}