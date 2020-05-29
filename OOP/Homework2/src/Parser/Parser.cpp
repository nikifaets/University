#include "Parser.h"
#include <iostream>


Parser::Parser(){

    fill_maps();
}

void Parser::fill_maps(){

    Parser::map_user_action["delete_user"] = &Platform::delete_user;
    Parser::map_user_action["block_user"] = &Platform::block_user;
    Parser::map_user_action["unblock_user"] = &Platform::unblock_user;
    
    Parser::map_user_creation_action["add_moderator"] = &Platform::add_moderator;
    Parser::map_user_creation_action["add_user"] = &Platform::add_user;


}
