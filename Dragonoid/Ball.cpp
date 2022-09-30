#include "Ball.h"

#include <ostream>

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

AABB Ball::get_ball_AABB() const
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

void Ball::set_dir_x(float dir_x)
{
    direction.x = part_pos.x = dir_x;
}

void Ball::set_dir_y(float dir_y)
{
    direction.y = part_pos.y = dir_y;
}

void Ball::invert_dir_y()
{
    direction.y *= -1;
    part_pos.y = direction.y;
}

void Ball::invert_dir_x()
{
    direction.x *= -1;
    part_pos.x = direction.x;
}

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
    

    if(pos.x <= 0)
    {
        set_dir_x(0.5f);
        if(direction.y >= 0) set_dir_y(0.5f);
        else set_dir_y(-0.5f);
    }

    if(pos.x >= screen_x - ball_size_)
    {
        set_dir_x(-0.5f);
        if(direction.y >= 0) set_dir_y(0.5f);
        else set_dir_y(-0.5f);
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

