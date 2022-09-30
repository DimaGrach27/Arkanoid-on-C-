#include "BackGround.h"

void BackGround::init()
{
    getScreenSize(size.x, size.y);
    field_sprite = createSprite(Helpers::get_path_to_sprite(29).c_str());
}

void BackGround::draw() const
{
    drawSprite(field_sprite, 0, 0);
    setSpriteSize(field_sprite, size.x, size.y);
}

