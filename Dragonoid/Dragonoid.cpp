#include <iostream>
#include <string>

#include "BackGround.h"
#include "Framework.h"
#include "Helpers.h"
#include "Ball.h"
#include "Block.h"
#include "Constans.h"
#include "Platform.h"
#include "SafeZone.h"

using std::string;

class Drogonoid final : public Framework {

public:

    void PreInit(int& width, int& height, bool& fullscreen) override
    {
        width = 800;
        height = 600;

        bottom_edge = height;
        fullscreen = false;
    }

    bool Init() override
    {
        back_ground_.init();
        platform_.init();
        ball_.init();
        safe_zone_.init();
        
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
                block_[count].init_block(13, spawn_pos);
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
        back_ground_.draw();

        if(is_ball_on_platform_)
        {
            ball_.ball_on_platform_position_update(platform_.get_pos(), platform_.get_size());
            ball_.draw_line_to_mouse(mouse_pos);
        }
        else
        {
            ball_.ball_move();
            calculate_near_element();
        }

        for (int i = 0; i < count; i++)
        {
            block_[i].show_block();
        }
        
        platform_.move();
        platform_.draw();
        safe_zone_.draw();
        ball_.draw_ball();

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
            const vector2_float vec_dir(
                mouse_pos.x - ball_.pos.x + ball_.size / 2,
                mouse_pos.y - ball_.pos.y + ball_.size / 2);
            
            const float vec_mag_ = sqrt(vec_dir.x * vec_dir.x + vec_dir.y * vec_dir.y);
            const float vec_inv_mag_ = 1 / vec_mag_;
            
            ball_.set_dir_x(vec_dir.x * vec_inv_mag_);
            ball_.set_dir_y(vec_dir.y * vec_inv_mag_);
            
            is_ball_on_platform_ = false;
        }
    }

    void onKeyPressed(FRKey k) override
    {
        if(k == FRKey::LEFT)
        {
            platform_.set_move_direction(-1);
        }

        if(k == FRKey::RIGHT)
        {
            platform_.set_move_direction(1);
        }
    }

    void onKeyReleased(FRKey k) override
    {
        if(k == FRKey::LEFT || k == FRKey::RIGHT)
        {
            platform_.set_move_direction(0);
        }
    }
	
    const char* GetTitle() override
    {
        return "Dragonoid";
    }

private:

    Ball ball_;
    Platform platform_;
    Block block_[count_bricks];
    SafeZone safe_zone_;
    BackGround back_ground_;
    
    vector2_int mouse_pos {0,0};

    bool is_ball_on_platform_ = true;
    bool is_game_end = false;
    
    int count = 0;
    int bottom_edge = 0;

    void start_game()
    {
        if(is_game_end)
        {
            is_game_end = false;
            is_ball_on_platform_ = true;
            
            platform_.restart();
            ball_.restart();
            safe_zone_.restart();

            for (int i = 0; i < 64; i++)
            {
                block_[i].start();
            }
        }
    }
    
    void calculate_near_element()
    {
        if (ball_.pos.y + ball_.size == bottom_edge)
        {
            if(safe_zone_.isAlive())
            {
                safe_zone_.death();
                return;    
            }
            
            is_game_end = true;
            //TODO: lose game
            start_game();
            std::cout << "LOSE!" << std::endl;
            return;
        }

        for (int i=0, j=0; i<count_bricks; i++)
        {
            if (!block_[i].is_show)
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

        if (ball_.is_can_contact_platform() &&
            intersects(ball_.get_ball_AABB(), platform_.get_ball_AABB())) {
            
            float direction = -0.6f +
            static_cast<float>(ball_.pos.x + ball_.size / 2 - platform_.get_ball_AABB().x_min) /
                (platform_.get_ball_AABB().x_max - platform_.get_ball_AABB().x_min) * 1.2f;

            if(direction > 0.6f) direction= 0.6f;
            if(direction < -0.6f) direction = -0.6f;

            ball_.set_dir_x(direction);
        
            direction = 1.0f - abs(direction);
            direction *= -1;

            ball_.set_dir_y(direction);
            return;
        }

        for (int i = 0; i < count_bricks; i++) {

            if (intersects(ball_.get_ball_AABB(), block_[i].get_ball_AABB()))
            {
                vector2_int point_right(ball_.pos.x + ball_.size, ball_.pos.y + ball_.size / 2);
                vector2_int point_left(ball_.pos.x, ball_.pos.y + ball_.size / 2);
                vector2_int point_top(ball_.pos.x + ball_.size / 2, ball_.pos.y);
                vector2_int point_bottom(ball_.pos.x + ball_.size / 2, ball_.pos.y + ball_.size);
                
                if (block_[i].is_show)
                {
                    if(contains(point_right, block_[i].get_ball_AABB()) ||
                        contains(point_left, block_[i].get_ball_AABB()))
                    {
                        ball_.invert_dir_x();
                    }

                    if(contains(point_top, block_[i].get_ball_AABB()) ||
                        contains(point_bottom, block_[i].get_ball_AABB()))
                    {
                        ball_.invert_dir_y();
                    }
          
                    block_[i].destroy_block();
                    ball_.set_can_contact_platform(true);
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
