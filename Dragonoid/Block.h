#pragma once
#include "Framework.h"
#include "Helpers.h"

class Block
{
public:
    Block(int sprite_num, vector2_int position, bool is_transparent);
    
    void restart();
    void show_block() const;
    void destroy_block();
    AABB get_ball_AABB() const;
    bool get_is_show() const;
    bool get_is_transparent_ability() const;
    bool get_is_transparent_now() const;
    void transparent_timer_tick();
    
private:
    bool is_show_ = true;
    bool is_transparent_ability_ = false;
    bool is_transparent_now_ = false;

    int current_sec_ = 0;
    
    vector2_int pos_ {0,0};
    
    Sprite* block_sprite_ = nullptr;
    Sprite* block_transparent_sprite_ = nullptr;
};
