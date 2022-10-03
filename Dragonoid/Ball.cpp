#include "Ball.h"

#include <ostream>

#include "Helpers.h"

Ball::Ball()
{
    ball_sprite_ = createSprite(Helpers::get_path_to_sprite(58).c_str());

    for (int i = 0; i < count_balls_; i++)
    {
        ball_small_sprite_[i] = createSprite(Helpers::get_path_to_sprite(63).c_str());
    }
}

void Ball::restart()
{
    is_was_init_ = false;
    is_alive_ = true;
    set_can_contact_platform(false);
}

void Ball::set_can_contact_platform(bool value)
{
    is_can_contact_platform_ = value;
}

void Ball::set_is_was_init(bool value)
{
    is_was_init_ = value;
}

bool Ball::get_is_can_contact_platform() const
{
    return is_can_contact_platform_;
}

bool Ball::get_is_was_init() const
{
    return is_was_init_;
}

bool Ball::get_is_alive() const
{
    return is_alive_;
}

void Ball::destroy_ball()
{
    is_alive_ = false;
    is_was_init_ = false;
}

void Ball::draw_line_to_mouse(vector2_int mouse_pos) const
{
    vector2_int delta = mouse_pos;
    
    delta.x -= pos.x - size / 2;
    delta.y -= pos.y - size / 2;

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

AABB Ball::get_AABB() const
{
    return  {pos.x, pos.y, pos.x + size, pos.y + size};
}


void Ball::ball_on_platform_position_update(vector2_int platform_pos, vector2_int platform_size)
{
    // ball_pos_X_ = 0;
    // ball_pos_Y_ = 0;
    pos.x = platform_pos.x + platform_size.x / 2 - size / 2;
    pos.y = platform_pos.y;
    pos.y -= platform_size.y / 2 + size / 3;
}

void Ball::set_dir_x(float dir_x)
{
    direction_.x = part_pos_.x = dir_x;
}

void Ball::set_dir_y(float dir_y)
{
    direction_.y = part_pos_.y = dir_y;
}

void Ball::invert_dir_y()
{
    direction_.y *= -1;
    part_pos_.y = direction_.y;
}

void Ball::invert_dir_x()
{
    direction_.x *= -1;
    part_pos_.x = direction_.x;
}

AABB Ball::get_right_side() const
{
    return {pos.x + size, pos.y + 1, pos.x + size, pos.y + size - 1};
}

AABB Ball::get_left_side() const
{
    return {pos.x, pos.y + size + 1, pos.x, pos.y + size - 1};
}

AABB Ball::get_top_side() const
{
    return {pos.x + 1, pos.y, pos.x + size - 1, pos.y};
}

AABB Ball::get_bottom_side() const
{
    return {pos.x + 1, pos.y + size, pos.x + size - 1, pos.y + size};
}

void Ball::draw_ball() const
{
    if(!is_alive_) return;
    
    drawSprite(ball_sprite_, pos.x, pos.y);
    setSpriteSize(ball_sprite_, size, size);
}


void Ball::ball_move()
{
    part_pos_.x += direction_.x * ball_speed_;
        
    if(part_pos_.x > 1)
    {
        part_pos_.x--;
        pos.x += 1;
    }

    if(part_pos_.x < -1)
    {
        part_pos_.x++;
        pos.x -= 1;
    }

    part_pos_.y += direction_.y * ball_speed_;
        
    if(part_pos_.y > 1)
    {
        part_pos_.y--;
        pos.y += 1;
    }

    if(part_pos_.y < -1)
    {
        part_pos_.y++;
        pos.y -= 1;
    }
    
    int screen_x;
    int screen_y;
    getScreenSize(screen_x, screen_y);
    
    if(pos.x <= 0)
    {
        set_dir_x(0.5f);
        if(direction_.y >= 0) set_dir_y(0.5f);
        else set_dir_y(-0.5f);
        is_can_contact_platform_ = true;
    }

    if(pos.x >= screen_x - size)
    {
        set_dir_x(-0.5f);
        if(direction_.y >= 0) set_dir_y(0.5f);
        else set_dir_y(-0.5f);
        is_can_contact_platform_ = true;
    }

    if(pos.y == 0 || pos.y == screen_y - size)
    {
        direction_.y *= -1;
        part_pos_.y = direction_.y;
        is_can_contact_platform_ = true;
    }
}

