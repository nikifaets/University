#include "Platform.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "../MessageSystem/MessageSystem.h"
#include "../Users/UserFactory.h"
#include "../Post/PostFactory.h"

std::vector<User> Platform::users;
std::unordered_set <std::string> Platform::usernames;
std::unordered_set <int> Platform::blocked_users;
std::vector<Post> Platform::posts;

void Platform::add_admin(User user){

    users.push_back(user);
    usernames.insert(user.get_name());
}
void Platform::add_user(std::string actor_name, std::string name, int age){


    if(! validate_rights(actor_name, Rank::ADMIN)) return;

    if(is_name_used(name)){

        message::print_string("Username is taken.");
        return;
    }

    User user = UserFactory::create_user(name, age);
    usernames.insert(name);
    users.push_back(user);

}

void Platform::add_moderator(std::string actor_name, std::string name, int age){

    if(! validate_rights(actor_name, Rank::ADMIN)) return;

    if(is_name_used(name)){

        message::print_string("Username is taken.");
    }

    User user = UserFactory::create_moderator(name, age);
    users.push_back(user);
    usernames.insert(name);
}

void Platform::delete_user(std::string actor_name, std::string target_name){

    if(! validate_rights(actor_name, Rank::ADMIN)) return;

    usernames.erase(target_name);
    users.erase(std::find(users.begin(), users.end(), get_user_by_name(target_name)));


}
void Platform::block_user(std::string actor_name, std::string target_name){

    if(! validate_rights(actor_name, Rank::MODERATOR) || !is_name_used(target_name)) return;

    User* user = get_user_by_name(target_name);
    blocked_users.insert(user->USER_ID);

}
void Platform::unblock_user(std::string actor_name, std::string target_name){

    if(!validate_rights(actor_name, Rank::MODERATOR) || !is_name_used(target_name)) return;

    User* user = get_user_by_name(target_name);
    blocked_users.erase(user->USER_ID);
}

void Platform::change_name(std::string actor_name, std::string new_name){

    if(is_name_used(new_name)){

        message::print_string("Name is already in use.");
        return;
    }

    User* user = get_user_by_name(actor_name);
    user->set_name(new_name);

    usernames.erase(actor_name);
    usernames.insert(new_name);
 
}

void Platform::view_all_posts_by_user(std::string actor_name, std::string target_name){

    User* user = get_user_by_name(actor_name);

    std::string contents;

    //search for posts by this user
    for(int i=0; i<posts.size(); i++){

        if(posts[i].CREATOR_ID == user->USER_ID){

            contents += (posts[i].CONTENT + "\n");
        }
    }

    Post::add_content(contents, target_name);
}

void Platform::view_post(std::string actor_name, int post_id){

    

}
bool Platform::is_name_used(std::string name){

    return usernames.find(name) != usernames.end();
}


void Platform::add_post(std::string actor_name, std::string post_type, std::string content, std::string optional){

    typedef Post (*ptr)(int creator_id, std::string content);
    std::unordered_map<std::string, ptr> map_action = { 
                                            {"text", &PostFactory::create_text}, 
                                            {"image", &PostFactory::create_image},
                                            };   

    User* actor = get_user_by_name(actor_name);
    if(blocked_users.find(actor->USER_ID) != blocked_users.end()){

        message::print_string("Actor is blocked and cannot post.");

    }

    if(!post_type.compare("url")){

        Post post = PostFactory::create_url(actor->USER_ID, content, optional);
        posts.push_back(post);
        return;
        
    }

    Post post = map_action[post_type](actor->USER_ID, content);
    posts.push_back(post);

}

void Platform::remove_post(std::string actor_name, int post_id){

    //find post by id
    for(int i=0; i<posts.size(); i++){

        Post* curr = &posts[i];
        if(curr->POST_ID == post_id){

            posts.erase(posts.begin()+i);
            return;
        }
    }
}

void Platform::show_info(){

    int users_count = 0;
    int mod_count = 0;
    int blocked_count = 0;
    int most_posts_per_user = 0;
    
    std::string most_active = users[0].get_name();
    std::string youngest = users[0].get_name();
    std::string oldest = users[0].get_name();

    int youngest_age = users[0].AGE;
    int oldest_age = users[0].AGE;

    for(int i=0; i<users.size(); i++){

        User* curr = &users[i];

        if(curr->RANK == Rank::USER){

            users_count ++;
        }

        else if(curr->RANK == Rank::MODERATOR){

            mod_count ++;
        }

        if(curr->AGE < youngest_age){

            youngest_age = curr->AGE;
            youngest = curr->get_name();
        }

        if(curr->AGE > oldest_age){

            oldest_age = curr->AGE;
            oldest = curr->get_name();
        }

        int curr_user_posts = 0;
        //count posts per user
        for(int j=0; j<posts.size(); j++){

            Post* curr_post = &posts[j];

            if(curr_post->CREATOR_ID == curr->USER_ID) curr_user_posts ++;
        }

        if(curr_user_posts > most_posts_per_user){

            most_posts_per_user = curr_user_posts;
            most_active = curr->get_name();
        }
    }




    message::print_info("Users count: ", users_count);
    message::print_info("Moderators count: ", mod_count);
    message::print_info("Youngest user: " + youngest + ": ", youngest_age);
    message::print_info("Oldest user:" + oldest + ": ", oldest_age);
    message::print_info("User with the most posts is " + most_active + ": ", most_posts_per_user);
    message::print_info("Blocked users count: ", blocked_users.size());
    message::print_string("Blocked users:");

    std::vector<std::string> blocked_names = get_blocked_names();

    for(int i=0; i<blocked_names.size(); i++){

        message::print_string(blocked_names[i]);
    }
    
}

std::vector<std::string> Platform::get_blocked_names(){

    std::vector<std::string> res;

    std::unordered_set<int>::iterator it;

    for(it = blocked_users.begin(); it!=blocked_users.end(); it++){

        int id = *it;

        //find user with this id
        for(int i=0; i<users.size(); i++){

            if(users[i].USER_ID == id) res.push_back(users[i].get_name());
        }
    }

    return res;
}
User* Platform::get_user_by_name(std::string name){

    for(int i=0; i<users.size(); i++){

        if(!users[i].get_name().compare(name)){

            return &users[i];
        }
    }

    User* res_user = new User(-1, "Invalid", 0, Rank::USER);
    return res_user;

}

bool Platform::validate_rights(std::string name, Rank rank){

    if(is_name_used(name)){

        User* user = get_user_by_name(name);
        
        if(user->RANK >= rank) return true;

        else{

            message::print_string("User does not have the appropriate rights.");
            return false;
        }

    }

    message::print_string("Invalid name of actor.");
    return false;
}