#include "Platform.h"

#include "Helpers.h"

Platform::Platform()
{
    init_platform_sprites(count_platform_sprites);
    tick_count_ = getTickCount();

    int screen_x;
    int screen_y;
    getScreenSize(screen_x, screen_y);

    // pos.y = 0;
    // pos.x = 0;
    pos.y = screen_y - 75;
    pos.x = screen_x / 2 - size.x / 2;
}

vector2_int Platform::get_size() const
{
    return size;
}

vector2_int Platform::get_pos() const
{
    return pos;
}

void Platform::restart()
{
    int screen_x;
    int screen_y;
    getScreenSize(screen_x, screen_y);
    
    pos.y = screen_y - 75;
    pos.x = screen_x / 2 - size.x / 2;
}

void Platform::move()
{
    const int newPosition = pos.x + move_direction_ * platform_speed_;
    int screen_x;
    int screen_y;
    getScreenSize(screen_x, screen_y);
    
    if(newPosition >= 0 && newPosition <= screen_x - size.x)
    {
        pos.x = newPosition;
    }
}

void Platform::draw()
{
    if(getTickCount() - tick_count_ > 150)
    {
        tick_count_ = getTickCount();
        tick_++;
    }

    if(tick_ >= count_platform_sprites)
        tick_ = 0;
        
    setSpriteSize(animation_sprite_platform_[tick_], size.x, size.y);
    drawSprite(animation_sprite_platform_[tick_], pos.x, pos.y);
}
void Platform::restore_size()
{
    size.x = 200;
    size.y = 40;
}

void Platform::restore_speed()
{
    platform_speed_ = 2;
}

void Platform::set_size(int newSize)
{
    size.x = newSize;
    std::cout << "SIZE: " << size.x << std::endl;
}

void Platform::set_speed(int speed)
{
    platform_speed_ = speed;
}

AABB Platform::get_AABB() const
{
    return  {pos.x, pos.y, pos.x + size.x, pos.y + size.y};
}

void Platform::set_move_direction(int value)
{
    move_direction_ = value;
}

void Platform::init_platform_sprites(int count_sprites)
{
    for (int i = 0; i < count_sprites; i++)
    {
        const int number = 50 + i;
        animation_sprite_platform_[i] = createSprite(Helpers::get_path_to_sprite(number).c_str());
    }
}