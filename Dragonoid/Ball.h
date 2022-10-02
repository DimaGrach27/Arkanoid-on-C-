#pragma once
#include "Framework.h"
#include "Helpers.h"

class Ball
{
    
public:
    const int size = 30;

    vector2_int pos {0,0};
    
    void init();
    void restart();
    void ball_move();
    void destroy_ball();
    void ball_on_platform_position_update(vector2_int platform_pos, vector2_int platform_size);
    void set_dir_x(float dir_x);
    void set_dir_y(float dir_y);
    void invert_dir_y();
    void invert_dir_x();
    void set_can_contact_platform(bool value);
    void set_is_was_init(bool value);
    
    void draw_ball() const;
    void draw_line_to_mouse(vector2_int mouse_pos) const;

    bool get_is_can_contact_platform() const;
    bool get_is_was_init() const;
    bool get_is_alive() const;
    
    AABB get_ball_AABB() const;
    AABB get_right_side() const;
    AABB get_left_side() const;
    AABB get_top_side() const;
    AABB get_bottom_side() const;

private:
    bool is_can_contact_platform_ = false;
    bool is_alive_ = true;
    bool is_was_init_ = false;
    
    constexpr static int count_balls_ = 100;
    
    const float ball_speed_ = 3;

    vector2_float direction_ {0,0};
    vector2_float part_pos_ {0,0};

    Sprite* ball_sprite_ = nullptr;
    Sprite* ball_small_sprite_[count_balls_] = {};

};
