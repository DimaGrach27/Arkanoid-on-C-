#pragma once
#include "Framework.h"
#include "Helpers.h"

class Block
{
public:
    bool is_show = true;
    
    vector2_int pos {0,0};

    void init_block(int sprite_num, vector2_int position);
    void start();
    void show_block();
    void destroy_block();
private:

    Sprite* block_sprite = nullptr;
};
