#pragma once
#include "Framework.h"
#include "Helpers.h"

class Ball
{
    
public:
    bool is_ball_on_platform_ = true;
    bool is_game_end = false;
    
    const int ball_size_ = 30;

    vector2_int pos {0,0};
    vector2_float direction {0,0};
    
    void Init();
    void Tick();
    void start();
    void draw_line_to_mouse(vector2_int mouse_pos) const;
    void ball_on_platform_position_update(vector2_int platform_pos, vector2_int platform_size);
    void set_direction(vector2_float dir);
    void set_object_proection(proection proect, bool is_player_platform, bool &is_was_collision);

private:
    bool is_ball_in_field_ = false;

    proection proection_ {0,0, 0,0};
    proection mine_proection_ {0,0, 0,0};
    
    constexpr static int count_balls_ = 100;

    const float ball_speed_ = 3;

    vector2_float part_pos {0,0};

    Sprite* ball_sprite_ = nullptr;
    Sprite* ball_small_sprite_[count_balls_] = {};

    void draw_ball() const;
    void init_ball_sprite();
    void ball_move();
};
