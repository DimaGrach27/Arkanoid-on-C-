#include "AbilityController.h"

AbilityController::AbilityController(Platform* platform, Helpers* helpers)
{
    platform_ = platform;
    helpers_ = helpers;
}

void AbilityController::catch_ability(ability_type ability)
{
    if(ability == fast_platform || ability == slow_platform) change_speed(ability);
    if(ability == big_platform || ability == small_platform) change_size(ability);
}

void AbilityController::change_size(ability_type ability)
{
    if(current_ability_size_ == big_platform && ability == small_platform)
    {
        current_ability_size_ = none;
    }
    else if(current_ability_size_ == small_platform && ability == big_platform)
    {
        current_ability_size_ = none;
    }
    else
    {
        current_ability_size_ = ability;
    }

    switch (current_ability_size_)
    {
    case small_platform:
        platform_->set_size(160);
        timer_ability_size_ = 10;
        break;
    case big_platform:
        platform_->set_size(240);
        timer_ability_size_ = 10;
        break;
    default:
        platform_->restore_size();
        break;
    }
}

void AbilityController::change_speed(ability_type ability)
{
    if(current_ability_speed_ == slow_platform && ability == fast_platform)
    {
        current_ability_speed_ = none;
    }
    else if(current_ability_speed_ == fast_platform && ability == slow_platform)
    {
        current_ability_speed_ = none;
    }
    else
    {
        current_ability_speed_ = ability;
    }
    
    switch (current_ability_speed_)
    {
    case slow_platform:
        platform_->set_speed(1);
        timer_ability_speed_ = 3;
        break;
    case fast_platform:
        platform_->set_speed(3);
        timer_ability_speed_ = 3;
        break;
    default:
        platform_->restore_speed();
        break;
    }
}

void AbilityController::tick()
{
    timer_active_ability();
}

void AbilityController::timer_active_ability()
{
    if(prev_time_ < helpers_->play_timer())
    {
        prev_time_ = helpers_->play_timer();

        if(current_ability_size_ != none)
        {
            timer_ability_size_--;
            if(timer_ability_size_ <= 0)
            {
                current_ability_size_ = none;
                platform_->restore_size();
            }
        }

        if(current_ability_speed_ != none)
        {
            timer_ability_speed_--;
            if(timer_ability_speed_ <= 0)
            {
                current_ability_speed_ = none;
                platform_->restore_speed();
            }
        }
    }
}


void AbilityController::disable_ability()
{
    current_ability_size_ = none;
    current_ability_speed_ = none;
    
    platform_->restore_size();
    platform_->restore_speed();
}

