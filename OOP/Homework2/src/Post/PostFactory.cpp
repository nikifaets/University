#include "PostFactory.h"

int PostFactory::free_id = 1;

Post PostFactory::create_image(int creator_id, std::string path){

    return Post(creator_id, ++free_id, "<img src= \"" + path + "\">");
}

Post PostFactory::create_text(int creator_id, std::string text){

    return Post(creator_id, ++free_id, "<p>" + text + "</p>");
}

Post PostFactory::create_url(int creator_id, std::string url, std::string desc){

    return Post(creator_id, ++free_id, "<a href=\"" + url + "\">" + desc + "</a>");
}