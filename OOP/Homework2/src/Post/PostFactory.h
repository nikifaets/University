#pragma once

#include "Post.h"
class PostFactory{

    static int free_id;

    public:

    static Post create_image(int creator_id, std::string path);
    static Post create_text(int creator_id, std::string text);
    static Post create_url(int creator_id, std::string url, std::string desc);
};
