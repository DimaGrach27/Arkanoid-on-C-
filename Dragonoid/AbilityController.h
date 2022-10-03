#pragma once
#include "Platform.h"

class AbilityController
{
public:
    AbilityController(Platform* platform, Helpers* helpers);
    
    void tick();
    void catch_ability(ability_type ability);

    void disable_ability();
    
private:
    Platform* platform_ = nullptr;
    Helpers* helpers_ = nullptr;
    
    ability_type current_ability_size_ = none;
    ability_type current_ability_speed_ = none;

    int timer_ability_speed_ = 3;
    int timer_ability_size_ = 3;

    int prev_time_;
    
    void timer_active_ability();
    void change_speed(ability_type ability);
    void change_size(ability_type ability);
};
