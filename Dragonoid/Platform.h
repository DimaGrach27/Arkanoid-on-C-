#pragma once
#include "Framework.h"
#include "Helpers.h"
#include "IStart.h"

class Platform : public IStart
{
    
public:
    vector2_int pos {0,0};
    const vector2_int size {200,40};
    
    void Init();
    void Tick();
    void start() override;
    void set_move_direction(int value);
    
private:
    int tick_ = 0;
    int tick_count_ = 0;
    int move_direction_ = 0;

    constexpr static int count_platform_sprites = 3;
    
    const int platform_speed_ = 1;
    
    Sprite* animation_sprite_platform_[count_platform_sprites] = {};
    
    void init_platform_sprites(int count_sprites);
    void move_platform();
    void animation_platform();
};
