#pragma once
#include "BackGround.h"
#include "Helpers.h"

class Ability
{
public:
    ability_type get_ability_type() const;

    bool spawn();
    bool get_is_alive() const;
    void move();
    void destroy();
    void draw() const;

    AABB get_ability_AABB() const;
    
private:
    bool is_alive_ = false;
    
    vector2_int position_ = {0,0};
    vector2_int size_ = {80,20};
    
    ability_type ability_ = none;
    
    Sprite* sprite_ = nullptr;
};
