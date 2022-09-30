#include "Block.h"
#include "Constans.h"

void Block::restart()
{
    is_show = true;
}

AABB Block::get_ball_AABB()
{
    return  {pos.x, pos.y, pos.x + block_size.x, pos.y + block_size.y};
}

void Block::destroy_block()
{
    is_show = false;
}

void Block::init_block(int sprite_num, vector2_int position)
{
    pos = position;
    block_sprite = createSprite(Helpers::get_path_to_sprite(sprite_num).c_str());
}

void Block::show_block()
{
    if(!is_show) return;

    drawSprite(block_sprite, pos.x, pos.y);
    setSpriteSize(block_sprite, block_size.x, block_size.y);
}

