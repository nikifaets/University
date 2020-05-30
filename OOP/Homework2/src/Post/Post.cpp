#include "Post.h"
#include <fstream>
#include "../MessageSystem/MessageSystem.h"

const std::string Post::TEMPLATE_PATH = "../../utils/template.html";

Post::Post(int creator_id, int post_id, std::string content): CREATOR_ID(creator_id), POST_ID(post_id), CONTENT(content){}

void Post::write_content(){



    std::string parsed = parse_content(TEMPLATE_PATH);
    std::string inserted = insert_content(parsed, CONTENT);

    if(inserted.size() == 0) return;

    std::ofstream to_write("post_" + std::to_string(CREATOR_ID) + "_" + std::to_string(POST_ID));
    to_write << inserted;


   
}

void Post::add_content(std::string content, std::string creator){

    std::string parsed = parse_content(TEMPLATE_PATH);
    
    std::string inserted = insert_content(parsed, content);

    if(inserted.size() == 0) return;

    std::ofstream to_write("post_manifest_" + creator);
    to_write << inserted;

}

std::string Post::parse_content(std::string path){

    std::ifstream html_template(path);

    std::string parsed;

    if(html_template.is_open()){

        std::string line;
        while(std::getline(html_template, line)){

            parsed += (line + "\n");

        }
    }

    return parsed;
}

std::string Post::insert_content(std::string html_template, std::string content){

    int match_idx = html_template.find("#placeholder");
    if(match_idx == std::string::npos){

        message::print_string("Corrupt file.");
        return "";
    }

    html_template.erase(match_idx, std::string("#placeholder").size());
    html_template.insert(match_idx, content);

    return html_template;

}

Post Post::operator=(const Post& other) const{

    if(other.CREATOR_ID == CREATOR_ID && other.POST_ID == POST_ID && !other.CONTENT.compare(CONTENT)) return *this;
    return Post(CREATOR_ID, POST_ID, CONTENT);
    
}

bool Post::operator==(const Post& other) const{

    return other.CREATOR_ID == CREATOR_ID;
}