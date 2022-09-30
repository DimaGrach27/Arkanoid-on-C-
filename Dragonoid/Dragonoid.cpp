#include <iostream>
#include <string>

#include "Framework.h"
#include "Helpers.h"
#include "Ball.h"
#include "Block.h"
#include "Constans.h"
#include "Platform.h"

using std::string;

class Drogonoid final : public Framework {

public:

    Ball ball;
    Platform platform;
    Block block[count_bricks];

    void PreInit(int& width, int& height, bool& fullscreen) override
    {
        width = 800;
        height = 600;
        fullscreen = false;
    }

    bool Init() override
    {
        init_bg_sprite();
        platform.Init();
        ball.Init();
        
        for (int i = 0; i < 8; i++)
        {
            int offset = i > 0 ? 8 : 0;
            vector2_int spawn_pos =
            {
                0,
                12 + i * block_size.y + i * offset
            };
            
            for (int j = 0; j < 8; j++)
            {
                offset = j > 0 ? 8 : 0;
                spawn_pos.x = 12 + j * block_size.x + j * offset;
                block[count].init_block(13, spawn_pos);
                count++;
            }
        }

        return true;
    }

    void Close() override
    {

    }

    bool Tick() override
    {

        draw_bg();

        if(ball.is_ball_on_platform_)
        {
            ball.ball_on_platform_position_update(platform.pos, platform.size);
            
            ball.draw_line_to_mouse(mouse_pos);
        }else
        {
            calculate_near_element();
        }

        for (int i = 0; i < count; i++)
        {
            block[i].show_block();
        }
        
        platform.Tick();
        ball.Tick();

        start_game();
        
        return false;
    }

    void onMouseMove(int x, int y, int x_relative, int y_relative) override
    {
        mouse_pos.x = x;
        mouse_pos.y = y;

        // std::cout << "POS X: " << mouse_pos.x << " POS Y: " << mouse_pos.y << std::endl;
    }

    void onMouseButtonClick(FRMouseButton button, bool isReleased) override
    {
        if(button == FRMouseButton::LEFT && isReleased)
        {
            const float vec_dir_x = mouse_pos.x - ball.pos.x;
            const float vec_dir_y = mouse_pos.y - ball.pos.y;
            
            const float vec_mag_ = sqrt(vec_dir_x * vec_dir_x + vec_dir_y * vec_dir_y);
            const float vec_inv_mag_ = 1 / vec_mag_;
            

            ball.set_dir_x(vec_dir_x * vec_inv_mag_);
            ball.set_dir_x(vec_dir_y * vec_inv_mag_);
            
            ball.is_ball_on_platform_ = false;
        }
    }

    void onKeyPressed(FRKey k) override
    {
        if(k == FRKey::LEFT)
        {
            platform.set_move_direction(-1);
        }

        if(k == FRKey::RIGHT)
        {
            platform.set_move_direction(1);
        }
    }

    void onKeyReleased(FRKey k) override
    {
        if(k == FRKey::LEFT || k == FRKey::RIGHT)
        {
            platform.set_move_direction(0);
        }
    }
	
    const char* GetTitle() override
    {
        return "Dragonoid";
    }

private:

    vector2_int mouse_pos {0,0};

    int count = 0;
    
    Sprite* field_sprite_ = nullptr;

    void start_game()
    {
        if(ball.is_game_end)
        {
            ball.is_game_end = false;
            
            platform.start();
            ball.start();

            for (int i = 0; i < 64; i++)
            {
                block[i].start();
            }
        }
    }

    void init_bg_sprite()
    {
        field_sprite_ = createSprite(Helpers::get_path_to_sprite(29).c_str());
    }

    void draw_bg() const
    {
        int screen_x;
        int screen_y;
        getScreenSize(screen_x, screen_y);
        
        drawSprite(field_sprite_, 0, 0);
        setSpriteSize(field_sprite_, screen_x, screen_y);
    }
    
    void calculate_near_element()
    {
        if (ball.pos.y + ball.ball_size_ > platform.pos.y + platform.size.y)
        {
            ball.is_game_end = true;
            //TODO: lose game
            start_game();
            std::cout << "LOSE!" << std::endl;
            return;
        }

        for (int i=0, j=0; i<count_bricks; i++)
        {
            if (!block[i].is_show)
            {
                j++;
            }

            if (j == count_bricks)
            {
                //TODO: win game
                start_game();
                std::cout << "WIN!" << std::endl;
                return;
            }
        }

        if (intersects(ball.get_ball_AABB(), platform.get_ball_AABB())) {
            
            float direction = -0.6f +
            static_cast<float>(ball.pos.x + ball.ball_size_ / 2 - platform.get_ball_AABB().x_min) /
                (platform.get_ball_AABB().x_max - platform.get_ball_AABB().x_min) * 1.2f;

            if(direction > 0.6f) direction= 0.6f;
            if(direction < -0.6f) direction = -0.6f;

            ball.set_dir_x(direction);
        
            direction = 1.0f - abs(direction);
            direction *= -1;

            ball.set_dir_y(direction);
            return;
        }

        for (int i = 0; i < count_bricks; i++) {

            if (intersects(ball.get_ball_AABB(), block[i].get_ball_AABB()))
            {
                vector2_int point_right(ball.pos.x + ball.ball_size_, ball.pos.y + ball.ball_size_ / 2);
                vector2_int point_left(ball.pos.x, ball.pos.y + ball.ball_size_ / 2);
                vector2_int point_top(ball.pos.x + ball.ball_size_ / 2, ball.pos.y);
                vector2_int point_bottom(ball.pos.x + ball.ball_size_ / 2, ball.pos.y + ball.ball_size_);
                
                if (block[i].is_show)
                {
                    if(contains(point_right, block[i].get_ball_AABB()) ||
                        contains(point_left, block[i].get_ball_AABB()))
                    {
                        ball.invert_dir_x();
                    }

                    if(contains(point_top, block[i].get_ball_AABB()) ||
                        contains(point_bottom, block[i].get_ball_AABB()))
                    {
                        ball.invert_dir_y();
                    }
          
                    block[i].destroy_block();
                }
            }
        }
    }

    static bool intersects(AABB one, AABB two)
    {
        if(one.x_max < two.x_min || one.x_min > two.x_max) return false;
        if(one.y_max < two.y_min || one.y_min > two.y_max) return false;
        
        return true;
    }

    static bool contains(vector2_int point, AABB box)
    {
        if(point.x < box.x_min || point.x > box.x_max) return false;
        if(point.y < box.y_min || point.y > box.y_max) return false;

        return true;
    }
};


int main(int argc, char* argv[])
{
    return run(new Drogonoid);
    
}
