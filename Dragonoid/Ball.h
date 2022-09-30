#pragma once
#include "Framework.h"
#include "Helpers.h"

class Ball
{
    
public:
    const int ball_size_ = 30;

    vector2_int pos {0,0};
    
    void init();
    void restart();
    void ball_move();
    void draw_ball() const;
    void draw_line_to_mouse(vector2_int mouse_pos) const;
    void ball_on_platform_position_update(vector2_int platform_pos, vector2_int platform_size);
    void set_dir_x(float dir_x);
    void set_dir_y(float dir_y);
    void invert_dir_y();
    void invert_dir_x();
    
    AABB get_ball_AABB() const;

private:
    constexpr static int count_balls_ = 100;
    
    const float ball_speed_ = 3;
    
    AABB proection_ {0,0, 0,0};
    AABB mine_proection_ {0,0, 0,0};

    vector2_float direction {0,0};
    vector2_float part_pos {0,0};

    Sprite* ball_sprite_ = nullptr;
    Sprite* ball_small_sprite_[count_balls_] = {};

};
