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

        bottom_edge_ = height;
        fullscreen = false;
    }

    bool Init() override
    {
        back_ground_.init();
        platform_.init();
        ball_1_.init();
        // ball_2_.init();
        // ball_3_.init();
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
                
                if(i == 2 && (j == 2 || j == 5 || j == 0 || j == 7))
                {
                    block_[count_].init_block(11, spawn_pos);
                    block_[count_].set_is_transparent_ability(true);
                }
                else if(i == 4 && (j == 2 || j == 5))
                {
                    block_[count_].init_block(11, spawn_pos);
                    block_[count_].set_is_transparent_ability(true);
                }
                else if(i == 6 && (j == 3 || j == 4))
                {
                    block_[count_].init_block(11, spawn_pos);
                    block_[count_].set_is_transparent_ability(true);
                }
                else
                {
                    block_[count_].init_block(13, spawn_pos);
                }
               
                count_++;
            }
        }

        return true;
    }

    void Close() override
    {

    }

    bool Tick() override
    {
        if(prev_time_ < helpers_.play_timer())
        {
            timer_tick();
            prev_time_ = helpers_.play_timer();
            cout << "Play time: " << prev_time_<< "(s)" << endl;
        }
        
        back_ground_.draw();

        if(is_ball_on_platform_)
        {
            ball_1_.ball_on_platform_position_update(platform_.get_pos(), platform_.get_size());
            ball_1_.draw_line_to_mouse(mouse_pos_);
        }
        else
        {
            ball_1_.ball_move();
            // ball_2_.ball_move();
            // ball_3_.ball_move();
            
            calculate_near_element(&ball_1_);
            // calculate_near_element(&ball_2_);
            // calculate_near_element(&ball_3_);
        }

        for (int i = 0; i < count_; i++)
        {
            block_[i].show_block();
        }
        
        platform_.move();
        platform_.draw();
        safe_zone_.draw();
        
        ball_1_.draw_ball();
        // ball_2_.draw_ball();
        // ball_3_.draw_ball();
        
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
                mouse_pos_.x - ball_1_.pos.x + ball_1_.size / 2,
                mouse_pos_.y - ball_1_.pos.y + ball_1_.size / 2);
            
            const float vec_mag_ = sqrt(vec_dir.x * vec_dir.x + vec_dir.y * vec_dir.y);
            const float vec_inv_mag_ = 1 / vec_mag_;

            start_direction_shot_ = {vec_dir.x * vec_inv_mag_, vec_dir.y * vec_inv_mag_};
            
            ball_1_.set_dir_x(vec_dir.x * vec_inv_mag_);
            ball_1_.set_dir_y(vec_dir.y * vec_inv_mag_);
            
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

    Ball ball_1_;
    // Ball ball_2_;
    // Ball ball_3_;
    
    Platform platform_;
    Block block_[count_bricks];
    SafeZone safe_zone_;
    BackGround back_ground_;
    Helpers helpers_;
    
    vector2_int mouse_pos_ {0,0};
    vector2_float start_direction_shot_ {0,0};

    bool is_ball_on_platform_ = true;
    bool is_game_end_ = false;
    
    int count_ = 0;
    int bottom_edge_ = 0;
    int prev_time_ = 0;

    void start_game()
    {
        if(is_game_end_)
        {
            is_game_end_ = false;
            is_ball_on_platform_ = true;
            
            platform_.restart();
            ball_1_.restart();
            // ball_2_.restart();
            // ball_3_.restart();
            safe_zone_.restart();

            for (int i = 0; i < 64; i++)
            {
                block_[i].restart();
            }
        }
    }

    void timer_tick()
    {
        for (int i = 0; i < count_bricks; i++)
        {
            block_[i].transparent_timer_tick();
        }
    }
    
    void calculate_near_element(Ball *ball)
    {
        if (ball->pos.y + ball->size == bottom_edge_)
        {
            if(safe_zone_.isAlive())
            {
                safe_zone_.death();
                return;    
            }

            ball->destroy_ball();

            if(is_all_ball_destroyed())
            {
                is_game_end_ = true;
                start_game();
                std::cout << "LOSE!" << std::endl;
            }
            
            return;
        }

        for (int i = 0, j = 0; i < count_bricks; i++)
        {
            if (!block_[i].get_is_show())
            {
                j++;
            }

            if (j == count_bricks)
            {
                start_game();
                std::cout << "WIN!" << std::endl;
                return;
            }
        }

        if (ball->is_can_contact_platform() &&
            intersects(ball->get_ball_AABB(), platform_.get_ball_AABB()))
        {
            
            float direction = -0.6f +
            static_cast<float>(ball->pos.x + ball->size / 2 - platform_.get_ball_AABB().x_min) /
                (platform_.get_ball_AABB().x_max - platform_.get_ball_AABB().x_min) * 1.2f;

            if(direction > 0.6f) direction= 0.6f;
            if(direction < -0.6f) direction = -0.6f;

            ball->set_dir_x(direction);
        
            direction = 1.0f - abs(direction);
            direction *= -1;

            ball->set_dir_y(direction);
            return;
        }

        bool is_was_collision = false;
        
        for (int i = 0; i < count_bricks; i++) {

            if (!block_[i].get_is_transparent_now() &&
                intersects(ball->get_ball_AABB(), block_[i].get_ball_AABB()))
            {
                if (block_[i].get_is_show())
                {
                    if(!is_was_collision &&
                        (contains(ball->get_right_side(), block_[i].get_ball_AABB()) ||
                        contains(ball->get_left_side(), block_[i].get_ball_AABB())))
                    {
                        ball->invert_dir_x();
                        is_was_collision = true;
                    }

                    if(!is_was_collision &&
                        (contains(ball->get_top_side(), block_[i].get_ball_AABB()) ||
                        contains(ball->get_bottom_side(), block_[i].get_ball_AABB())))
                    {
                        ball->invert_dir_y();
                        is_was_collision = true;
                    }
          
                    block_[i].destroy_block();
                    ball->set_can_contact_platform(true);
                }
            }
        }
    }

    bool is_all_ball_destroyed() const
    {
        return !ball_1_.is_alive()/* && !ball_2_.is_alive() && !ball_3_.is_alive()*/;
    }

    static bool intersects(AABB one, AABB two)
    {
        if(one.x_max < two.x_min || one.x_min > two.x_max) return false;
        if(one.y_max < two.y_min || one.y_min > two.y_max) return false;
        
        return true;
    }

    static bool contains(AABB side, AABB box)
    {
        for (int x = side.x_min; x <= side.x_max; x++)
        {
            for (int y = side.y_min; y <= side.y_max; y++)
            {
                const vector2_int point = {x, y};
                if(contains(point, box))
                {
                    return true;
                }
            }
        }
        return false;
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
