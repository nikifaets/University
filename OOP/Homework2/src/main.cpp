#include "Parser/Parser.h"
#include "Platform/Platform.h"
#include "Users/UserFactory.h"


int main(){

    User admin = UserFactory::create_admin("Admin", 20);
    Platform::add_admin(admin);
    Parser parser;

    parser.parse();
    
}