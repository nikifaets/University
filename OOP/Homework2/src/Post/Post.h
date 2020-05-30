#pragma once
#include "../Users/User.h"

class Post{


    static const std::string TEMPLATE_PATH;
    static std::string parse_content(std::string path="template");
    static std::string insert_content(std::string html_template, std::string content);

    public:
    
    const int CREATOR_ID;
    const int POST_ID;
    const std::string CONTENT;
    
    Post(int creator_id, int post_id, std::string content);
    void write_content();
    static void add_content(std::string content, std::string creator);

    Post operator =(const Post& other) const;
    bool operator==(const Post& other) const;
};