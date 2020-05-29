#include "Parser.h"
#include <iostream>
#include <exception>
#include "../MessageSystem/MessageSystem.h"

Parser::Parser(){

    fill_maps();
}

void Parser::fill_maps(){

    Parser::map_user_action["delete_user"] = &Platform::delete_user;
    Parser::map_user_action["block_user"] = &Platform::block_user;
    Parser::map_user_action["unblock_user"] = &Platform::unblock_user;
    
    Parser::map_user_creation_action["add_moderator"] = &Platform::add_moderator;
    Parser::map_user_creation_action["add_user"] = &Platform::add_user;

    Parser::map_independent_action["info"] = &Platform::show_info;
}

std::vector<std::string> Parser::substring(std::string input){

    std::vector<std::string> processed;
    
    std::unordered_set<char> delimeters {' ', '\t'};
    
    input.push_back(' ');
    for(int i=0; i<input.size(); i++){

        if(delimeters.find(input[i]) == delimeters.end() && i == input.size()-1){

            processed.push_back(input);
            continue;
        }

        if(delimeters.find(input[i]) != delimeters.end()){

            processed.push_back(input.substr(0, i));

            int delims = 1;
            int to_delete = i+1;

            while(delimeters.find(input[i+delims]) != delimeters.end() &&  i+delims < input.size()){

                delims ++;
                to_delete++;


            }

            input.erase(0, to_delete);
            i=0;
        }
    }
    return processed;
}

void Parser::parse(){

    bool exit_flag = false;

    while(!exit_flag){

        std::string input;
        std::getline(std::cin, input);
        std::vector<std::string>  inputs = substring(input);
        //for(int i=0; i<inputs.size(); i++) std::cout << inputs[i] << std::endl;

        if(independent_actions.find(inputs[0]) != independent_actions.end()){
            
            if(!input.compare("exit")){

                return;
                exit_flag = true;
            }

            map_independent_action[inputs[0]]();

            continue;
        }

        if(inputs.size() < 3){

            message::print_string("Invalid input.");
            continue;
        }

        std::string actor_name = inputs[0];
        std::string action = inputs[1];
        std::string subject = inputs[2];

        if(!Platform::is_name_used(actor_name)){

            message::print_string("Actor name is used.");
            continue;
        }

        if(user_actions.find(action) != user_actions.end()){

            map_user_action[action](actor_name, subject);
        }

        if(inputs.size() >= 4 && user_creation_actions.find(action) != user_creation_actions.end()){

            int age = 0;

            try{

                age = std::stoi(inputs[3]);
                map_user_creation_action[action](actor_name, subject, age);

            }

            catch(const std::invalid_argument &e){

                message::print_string("Invalid argument for age.");
                continue;
            }
            
        }
    }


}
