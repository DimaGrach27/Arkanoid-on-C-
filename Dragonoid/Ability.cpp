#include "Ability.h"

void Ability::destroy()
{
    is_alive_ = false;

    destroySprite(sprite_);
}

void Ability::draw() const
{
    if(!is_alive_) return;
    if(sprite_ == nullptr) return;
    drawSprite(sprite_, position_.x, position_.y);
}

bool Ability::get_is_alive() const
{
    return is_alive_;
}

bool Ability::spawn()
{
    int random = Helpers::get_random_num();
    if(random < 80) return false;

    if(random >= 80 && random < 85)
    {
        sprite_ = createSprite(Helpers::get_path_to_sprite(47).c_str());
        ability_ = big_platform;
    }

    if(random >= 85 && random < 90)
    {
        sprite_ = createSprite(Helpers::get_path_to_sprite(46).c_str());
        ability_ = small_platform;
    }

    if(random >= 90 && random < 95)
    {
        sprite_ = createSprite(Helpers::get_path_to_sprite(42).c_str());
        ability_ = fast_platform;
    }

    if(random >= 95)
    {
        sprite_ = createSprite(Helpers::get_path_to_sprite(41).c_str());
        ability_ = slow_platform;
    }

    random = Helpers::get_random_num();

    vector2_int screen = {0,0};
    getScreenSize(screen.x, screen.y);
    
    position_.x = (screen.x - 50) * random / 100;
    position_.y = (screen.y - 300) * random / 100;
    
    is_alive_ = true;
    
    setSpriteSize(sprite_, size_.x, size_.y);
    return true;
}

void Ability::move()
{
    if(!is_alive_) return;
    
    position_.y += 1;
}


ability_type Ability::get_ability_type() const
{
    return ability_;
}

AABB Ability::get_ability_AABB() const
{
    return  {
        position_.x, position_.y,
        position_.x + size_.x, position_.y + size_.y};
}


