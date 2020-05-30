#include "Parser/Parser.h"
#include "Platform/Platform.h"
#include "Users/UserFactory.h"
#include "Post/Post.h"
#include <fstream>

void test(){

    Platform::add_user("Admin", "Niki", 20);
    Platform::add_user("Niki", "Gosho", 20);
    Platform::add_moderator("Admin", "Pipi", 15);
    Platform::add_post("Niki", "text", "haha");
    Platform::add_post("Niki", "image", "link");
    Platform::add_post("Pipi", "url", "sdfd", "haha");
    Platform::block_user("Pipi", "Niki");
    Platform::show_info();
}
int main(){

    
    User admin = UserFactory::create_admin("Admin", 20);
    Platform::add_admin(admin);
    //Parser parser;
    test();

    Parser parser;
    parser.parse();
    
}