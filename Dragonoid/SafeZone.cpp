#include "SafeZone.h"

#include "Helpers.h"

void SafeZone::init()
{
    save_zone_sprite = createSprite(Helpers::get_path_to_sprite(10).c_str());

    int width = 0;
    int height = 0;
    getScreenSize(width, height);

    size.x = width;
    size.y = 32;

    position = height - 32;
}

void SafeZone::restart()
{
    save_zone_sprite = createSprite(Helpers::get_path_to_sprite(10).c_str());
    is_alive = true;
}

void SafeZone::death()
{
    is_alive = false;
    destroySprite(save_zone_sprite);
}

void SafeZone::draw() const
{
    if(!is_alive) return;
    
    drawSprite(save_zone_sprite, 0, position);
    setSpriteSize(save_zone_sprite, size.x, size.y);
}

bool SafeZone::isAlive() const
{
    return  is_alive;
}




