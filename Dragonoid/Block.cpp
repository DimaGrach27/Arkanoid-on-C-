#include "Block.h"

#include "BackGround.h"
#include "Constans.h"

Block::Block(int sprite_num, vector2_int position, bool is_transparent)
{
    pos_ = position;
    is_transparent_ability_ = is_transparent;
    block_sprite_ = createSprite(Helpers::get_path_to_sprite(sprite_num).c_str());
    
    if(!is_transparent) return;
    
    block_transparent_sprite_ = createSprite(Helpers::get_path_to_sprite(66).c_str());
}

void Block::restart()
{
    is_show_ = true;
}

AABB Block::get_AABB() const
{
    return  {pos_.x, pos_.y, pos_.x + block_size.x, pos_.y + block_size.y};
}

bool Block::get_is_show() const
{
    return is_show_;
}

bool Block::get_is_transparent_ability() const
{
    return is_transparent_ability_;
}

bool Block::get_is_transparent_now() const
{
    return is_transparent_now_;
}

void Block::destroy_block()
{
    is_show_ = false;
}

void Block::show_block() const
{
    if(!is_show_) return;

    Sprite* show_sprite = is_transparent_now_ ? block_transparent_sprite_ : block_sprite_;
    
    drawSprite(show_sprite, pos_.x, pos_.y);
    setSpriteSize(show_sprite, block_size.x, block_size.y);
}

void Block::transparent_timer_tick()
{
    if(!is_transparent_ability_) return;

    current_sec_++;

    if(is_transparent_now_)
    {
        if(current_sec_ >= 3)
        {
            is_transparent_now_  = false;
            current_sec_ = 0;
        }
    }
    else
    {
        if(current_sec_ >= 10)
        {
            is_transparent_now_ = true;
            current_sec_ = 0;
        }
    }
}


