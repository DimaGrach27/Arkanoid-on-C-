#include "BackGround.h"

BackGround::BackGround()
{
    getScreenSize(size.x, size.y);
    field_sprite = createSprite(Helpers::get_path_to_sprite(67).c_str());
}

void BackGround::draw() const
{
    drawSprite(field_sprite, 0, 0);
    setSpriteSize(field_sprite, size.x, size.y);
}

