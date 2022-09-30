#include "Ball.h"

#include <iostream>
#include <ostream>

#include "Constans.h"
#include "Helpers.h"

void Ball::Init()
{
    init_ball_sprite();
}

void Ball::Tick()
{
    if(!is_ball_on_platform_ || is_game_end)
    {
        ball_move();
    }

    draw_ball();
}

void Ball::start()
{
    is_ball_on_platform_ = true;
}

void Ball::draw_line_to_mouse(vector2_int mouse_pos) const
{
    vector2_int delta = mouse_pos;
    
    delta.x -= pos.x - ball_size_ / 2;
    delta.y -= pos.y - ball_size_ / 2;

    int line_size = delta.x * delta.x + delta.y * delta.y;
    line_size = sqrt(line_size); 

    constexpr int size_ball = 20;
    const int count_balls = line_size / (size_ball * 1.25f);
        
    for (int i = 0; i < count_balls; i++)
    {
        const float t = i / (float)count_balls;
        const int x = t * (mouse_pos.x - pos.x) + pos.x;
        const int y = t * (mouse_pos.y - pos.y) + pos.y;

        if(i < count_balls_ && t > 0.05f)
        {
            drawSprite(ball_small_sprite_[i], x, y);
            setSpriteSize(ball_small_sprite_[i], size_ball, size_ball);
        }
    }
}

AABB Ball::get_ball_AABB()
{
    return  {pos.x, pos.y, pos.x + ball_size_, pos.y + ball_size_};
}


void Ball::ball_on_platform_position_update(vector2_int platform_pos, vector2_int platform_size)
{
    // ball_pos_X_ = 0;
    // ball_pos_Y_ = 0;
    pos.x = platform_pos.x + platform_size.x / 2;
    pos.y = platform_pos.y;
    pos.y -= platform_size.y / 2 + ball_size_ / 2;
}
void Ball::set_direction(vector2_float dir)
{
    direction = part_pos = dir;
}

// void Ball::set_object_proection(AABB proect, bool is_player_platform, bool &is_was_collision)
// {
//     proection_ = proect;
//     is_was_collision = false;
//     
//     mine_proection_.x_min = pos.x;
//     mine_proection_.x_max = pos.x + ball_size_;
//     mine_proection_.y_min = pos.y;
//     mine_proection_.y_max = pos.y + ball_size_;
//
//     // if(!is_ball_in_field_) return;
//
//     if(mine_proection_.x_max >= proection_.x_min &&
//         mine_proection_.y_max > proection_.y_min &&
//         mine_proection_.y_max < proection_.y_max &&
//         mine_proection_.y_min < proection_.y_max &&
//         mine_proection_.y_min > proection_.y_min &&
//         mine_proection_.x_min < proection_.x_min)
//     {
//         // from right
//         direction.x *= -1;
//         part_pos.x = direction.x;
//         is_was_collision = true;
//         // is_ball_in_field_ = false;
//     }
//
//     if(mine_proection_.x_min <= proection_.x_max &&
//         mine_proection_.y_max > proection_.y_min &&
//         mine_proection_.y_max < proection_.y_max &&
//         mine_proection_.y_min < proection_.y_max &&
//         mine_proection_.y_min > proection_.y_min &&
//         mine_proection_.x_max > proection_.x_max)
//     {
//         // from left
//         direction.x *= -1;
//         part_pos.x = direction.x;
//         is_was_collision = true;
//         // is_ball_in_field_ = false;
//     }
//
//     if(mine_proection_.y_max >= proection_.y_min &&
//         mine_proection_.x_min < proection_.x_max &&
//         mine_proection_.x_min > proection_.x_min &&
//         mine_proection_.x_max > proection_.x_min &&
//         mine_proection_.x_max < proection_.x_max &&
//         mine_proection_.y_min < proection_.y_min)
//     {
//         // from up
//         direction.y *= -1;
//         part_pos.y = direction.y;
//         is_was_collision = true;
//         // is_ball_in_field_ = false;
//     }
//
//     if(mine_proection_.y_min <= proection_.y_max &&
//         mine_proection_.x_min < proection_.x_max &&
//         mine_proection_.x_min > proection_.x_min &&
//         mine_proection_.x_max > proection_.x_min &&
//         mine_proection_.x_max < proection_.x_max &&
//         mine_proection_.y_max > proection_.y_max)
//     {
//         // from down
//         direction.y *= -1;
//         part_pos.y = direction.y;
//         is_was_collision = true;
//         // is_ball_in_field_ = false;
//     }
//
//     if(!is_ball_in_field_) return;
//     
//     if(is_was_collision && is_player_platform)
//     {
//         direction.x = -0.6f +
//             static_cast<float>(pos.x + ball_size_ / 2 - proection_.x_min) /
//                 (proection_.x_max - proection_.x_min) * 1.2f;
//
//         if(direction.x > 0.6f) direction.x = 0.6f;
//         if(direction.x < -0.6f) direction.x = -0.6f;
//         
//         direction.y = 1.0f - abs(direction.x);
//         direction.y *= -1;
//
//         part_pos.x = direction.x;
//         part_pos.y = direction.y;
//     }
// }

void Ball::draw_ball() const
{
    drawSprite(ball_sprite_, pos.x, pos.y);
    setSpriteSize(ball_sprite_, ball_size_, ball_size_);
}

void Ball::init_ball_sprite()
{
    ball_sprite_ = createSprite(Helpers::get_path_to_sprite(58).c_str());

    for (int i = 0; i < count_balls_; i++)
    {
        ball_small_sprite_[i] = createSprite(Helpers::get_path_to_sprite(63).c_str());
    }
}

void Ball::ball_move()
{
    part_pos.x += direction.x * ball_speed_;
        
    if(part_pos.x > 1)
    {
        part_pos.x--;
        pos.x += 1;
        is_ball_in_field_ = true;
    }

    if(part_pos.x < -1)
    {
        part_pos.x++;
        pos.x -= 1;
        is_ball_in_field_ = true;
    }

    part_pos.y += direction.y * ball_speed_;
        
    if(part_pos.y > 1)
    {
        part_pos.y--;
        pos.y += 1;
        is_ball_in_field_ = true;
    }

    if(part_pos.y < -1)
    {
        part_pos.y++;
        pos.y -= 1;

        is_ball_in_field_ = true;
    }
    
    int screen_x;
    int screen_y;
    getScreenSize(screen_x, screen_y);

    // if(!is_ball_in_field_) return;

    // std::cout
        // << "BALL pos: " << pos.x << " " << pos.y << "\n"
        // << "BORDER pos: " << border.x << " " << border.y << "\n";

    if(pos.x == 0 || pos.x == screen_x - ball_size_)
    {
        direction.x *= -1;
        part_pos.x = direction.x;

        is_ball_in_field_ = false;
    }

    if(pos.y == 0 || pos.y == screen_y - ball_size_)
    {
        direction.y *= -1;
        part_pos.y = direction.y;

        is_ball_in_field_ = false;
    }

    if(pos.y == screen_y - ball_size_)
    {
        is_game_end = true;
    }
}

