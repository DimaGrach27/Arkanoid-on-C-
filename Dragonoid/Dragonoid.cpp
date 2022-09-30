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
        balls_.init();
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
            balls_.ball_on_platform_position_update(platform_.get_pos(), platform_.get_size());
            balls_.draw_line_to_mouse(mouse_pos_);
        }
        else
        {
            balls_.ball_move();
            calculate_near_element();
        }

        for (int i = 0; i < count; i++)
        {
            block_[i].show_block();
        }
        
        platform_.move();
        platform_.draw();
        safe_zone_.draw();
        balls_.draw_ball();

        start_game();
        
        return false;
    }

    void onMouseMove(int x, int y, int x_relative, int y_relative) override
    {
        mouse_pos_.x = x;
        mouse_pos_.y = y;

        // std::cout << "POS X: " << mouse_pos.x << " POS Y: " << mouse_pos.y << std::endl;
    }

    void onMouseButtonClick(FRMouseButton button, bool isReleased) override
    {
        if(button == FRMouseButton::LEFT && isReleased)
        {
            const vector2_float vec_dir(
                mouse_pos_.x - balls_.pos.x + balls_.size / 2,
                mouse_pos_.y - balls_.pos.y + balls_.size / 2);
            
            const float vec_mag_ = sqrt(vec_dir.x * vec_dir.x + vec_dir.y * vec_dir.y);
            const float vec_inv_mag_ = 1 / vec_mag_;

            start_direction_shot_ = {vec_dir.x * vec_inv_mag_, vec_dir.y * vec_inv_mag_};
            
            balls_.set_dir_x(vec_dir.x * vec_inv_mag_);
            balls_.set_dir_y(vec_dir.y * vec_inv_mag_);
            
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

    Ball balls_;
    Platform platform_;
    Block block_[count_bricks];
    SafeZone safe_zone_;
    BackGround back_ground_;
    
    vector2_int mouse_pos_ {0,0};
    vector2_float start_direction_shot_ {0,0};

    bool is_ball_on_platform_ = true;
    bool is_game_end_ = false;
    
    int count = 0;
    int bottom_edge = 0;

    void start_game()
    {
        if(is_game_end_)
        {
            is_game_end_ = false;
            is_ball_on_platform_ = true;
            
            platform_.restart();
            balls_.restart();
            safe_zone_.restart();

            for (int i = 0; i < 64; i++)
            {
                block_[i].restart();
            }
        }
    }
    
    void calculate_near_element()
    {
        if (balls_.pos.y + balls_.size == bottom_edge)
        {
            if(safe_zone_.isAlive())
            {
                safe_zone_.death();
                return;    
            }
            
            is_game_end_ = true;
            //TODO: lose game
            start_game();
            std::cout << "LOSE!" << std::endl;
            return;
        }

        for (int i = 0, j = 0; i<count_bricks; i++)
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

        if (balls_.is_can_contact_platform() &&
            intersects(balls_.get_ball_AABB(), platform_.get_ball_AABB())) {
            
            float direction = -0.6f +
            static_cast<float>(balls_.pos.x + balls_.size / 2 - platform_.get_ball_AABB().x_min) /
                (platform_.get_ball_AABB().x_max - platform_.get_ball_AABB().x_min) * 1.2f;

            if(direction > 0.6f) direction= 0.6f;
            if(direction < -0.6f) direction = -0.6f;

            balls_.set_dir_x(direction);
        
            direction = 1.0f - abs(direction);
            direction *= -1;

            balls_.set_dir_y(direction);
            return;
            }

        for (int i = 0; i < count_bricks; i++) {

            if (intersects(balls_.get_ball_AABB(), block_[i].get_ball_AABB()))
            {
                vector2_int point_right(balls_.pos.x + balls_.size, balls_.pos.y + balls_.size / 2);
                vector2_int point_left(balls_.pos.x, balls_.pos.y + balls_.size / 2);
                vector2_int point_top(balls_.pos.x + balls_.size / 2, balls_.pos.y);
                vector2_int point_bottom(balls_.pos.x + balls_.size / 2, balls_.pos.y + balls_.size);
                
                if (block_[i].is_show)
                {
                    if(contains(point_right, block_[i].get_ball_AABB()) ||
                        contains(point_left, block_[i].get_ball_AABB()))
                    {
                        balls_.invert_dir_x();
                    }

                    if(contains(point_top, block_[i].get_ball_AABB()) ||
                        contains(point_bottom, block_[i].get_ball_AABB()))
                    {
                        balls_.invert_dir_y();
                    }
          
                    block_[i].destroy_block();
                    balls_.set_can_contact_platform(true);
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
