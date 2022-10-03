#pragma once
#include "Framework.h"
#include "Helpers.h"

class Platform
{
    
public:
    Platform();

    void move();
    void draw();
    void restart();
    void restore_speed();
    void restore_size();
    
    void set_speed(int speed);
    void set_size(int size);
    void set_move_direction(int value);
    
    AABB get_AABB() const;
    vector2_int get_size() const;
    vector2_int get_pos() const;
    
private:
    int tick_ = 0;
    int tick_count_ = 0;
    int move_direction_ = 0;

    constexpr static int count_platform_sprites = 3;
    
    int platform_speed_ = 2;
    
    vector2_int pos {0,0};
    vector2_int size {200,40};
    
    Sprite* animation_sprite_platform_[count_platform_sprites] = {};
    
    void init_platform_sprites(int count_sprites);
};
