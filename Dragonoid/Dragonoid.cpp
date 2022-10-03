#include "Ability.h"
#include "AbilityController.h"
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
        ability_ = new Ability;
        
        for (auto& ball : balls_)
        {
            ball = new Ball;
        }
        
        platform_ = new Platform;
        safe_zone_ = new SafeZone;
        back_ground_ = new BackGround;
        helpers_ = new Helpers;

        ability_controller_ = new AbilityController(platform_, helpers_);
        
        balls_[0]->set_is_was_init(true);
        
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
                    block_[count_] = new Block(11, spawn_pos, true);
                }
                else if(i == 4 && (j == 2 || j == 5))
                {
                    block_[count_] = new Block(11, spawn_pos, true);
                }
                else if(i == 6 && (j == 3 || j == 4))
                {
                    block_[count_] = new Block(11, spawn_pos, true);
                }
                else
                {
                    block_[count_] = new Block(13, spawn_pos, false);
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
        init_balls();
        
        if(prev_time_ < helpers_->play_timer())
        {
            timer_tick();
            prev_time_ = helpers_->play_timer();
            // cout << "Play time: " << prev_time_<< "(s)" << endl;
        }
        
        back_ground_->draw();

        if(is_ball_on_platform_)
        {
            balls_[0]->ball_on_platform_position_update(platform_->get_pos(), platform_->get_size());
            balls_[0]->draw_line_to_mouse(mouse_pos_);
        }
        else
        {
            for (const auto ball : balls_)
            {
                move_ball(ball);
            }

            for (const auto ball : balls_)
            {
                calculate_near_element(ball);
            }

            if(intersects(platform_->get_AABB(), ability_->get_AABB()))
            {
                ability_controller_->catch_ability(ability_->get_ability_type());
                ability_->destroy();
            }
        }

        for (int i = 0; i < count_; i++)
        {
            block_[i]->show_block();
        }
        
        ability_->move();
        if(ability_->get_AABB().y_max > bottom_edge_)
        {
            ability_->destroy();
        }
        
        platform_->move();
        platform_->draw();
        safe_zone_->draw();
        ability_->draw();

        for (const auto ball : balls_)
        {
            draw_ball(ball);
        }

        start_game();

        ability_controller_->tick();
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
        if(!is_ball_on_platform_) return;
        
        if(button == FRMouseButton::LEFT && isReleased)
        {
            const vector2_float vec_dir(
                mouse_pos_.x - balls_[0]->pos.x + balls_[0]->size / 2,
                mouse_pos_.y - balls_[0]->pos.y + balls_[0]->size / 2);
            
            const float vec_mag_ = sqrt(vec_dir.x * vec_dir.x + vec_dir.y * vec_dir.y);
            const float vec_inv_mag_ = 1 / vec_mag_;

            start_direction_shot_ = {vec_dir.x * vec_inv_mag_, vec_dir.y * vec_inv_mag_};
            shot_count_ = 3;
            init_balls();
            
            is_ball_on_platform_ = false;
        }
    }

    void onKeyPressed(FRKey k) override
    {
        if(k == FRKey::LEFT)
        {
            platform_->set_move_direction(-1);
        }

        if(k == FRKey::RIGHT)
        {
            platform_->set_move_direction(1);
        }
    }

    void onKeyReleased(FRKey k) override
    {
        if(k == FRKey::LEFT || k == FRKey::RIGHT)
        {
            platform_->set_move_direction(0);
        }
    }
	
    const char* GetTitle() override
    {
        return "Dragonoid";
    }

private:

    Ability* ability_ = nullptr;
    Ball* balls_[3] = {};
    Block* block_[count_bricks] = {};
    Platform* platform_ = nullptr;
    SafeZone* safe_zone_ = nullptr;
    BackGround* back_ground_ = nullptr;
    Helpers* helpers_ = nullptr;
    AbilityController* ability_controller_ = nullptr;
    
    vector2_int mouse_pos_ {0,0};
    vector2_float start_direction_shot_ {0,0};

    bool is_ball_on_platform_ = true;
    bool is_game_end_ = false;
    
    int count_ = 0;
    int bottom_edge_ = 0;
    int prev_time_ = 0;
    int prev_time_shot_ = 0;
    int shot_count_ = 0;

    void start_game()
    {
        if(is_game_end_)
        {
            is_game_end_ = false;
            is_ball_on_platform_ = true;
            
            platform_->restart();
            for (const auto ball : balls_)
            {
                ball->restart();
            }
            balls_[0]->set_is_was_init(true);

            safe_zone_->restart();

            for (const auto& block : block_)
            {
                block->restart();
            }

            ability_controller_->disable_ability();
        }
    }

    void timer_tick()
    {
        for (const auto& block : block_)
        {
            block->transparent_timer_tick();
        }

        ability_spawner();
    }

    void ability_spawner() const
    {
        if(is_ball_on_platform_) return;
        
        if(ability_->get_is_alive()) return;
        
        ability_->spawn();
    }

    static void draw_ball(const Ball *ball)
    {
        if(!ball->get_is_was_init()) return;
        ball->draw_ball();
    }

    static void move_ball(Ball *ball)
    {
        if(!ball->get_is_was_init()) return;
        ball->ball_move();
    }
    
    void calculate_near_element(Ball *ball)
    {
        if(!ball->get_is_was_init()) return;
        
        if (ball->pos.y + ball->size == bottom_edge_)
        {
            if(safe_zone_->isAlive())
            {
                safe_zone_->death();
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
            if (!block_[i]->get_is_show())
            {
                j++;
            }

            if (j == count_bricks)
            {
                is_game_end_ = true;
                start_game();
                std::cout << "WIN!" << std::endl;
                return;
            }
        }

        if (ball->get_is_can_contact_platform() &&
            intersects(ball->get_AABB(), platform_->get_AABB()))
        {
            
            float direction = -0.6f +
            static_cast<float>(ball->pos.x + ball->size / 2 - platform_->get_AABB().x_min) /
                (platform_->get_AABB().x_max - platform_->get_AABB().x_min) * 1.2f;

            if(direction > 0.6f) direction= 0.6f;
            if(direction < -0.6f) direction = -0.6f;

            ball->set_dir_x(direction);
        
            direction = 1.0f - abs(direction);
            direction *= -1;

            ball->set_dir_y(direction);
            return;
        }

        bool is_was_collision = false;
        
        for (const auto& block : block_)
        {
            if (!block->get_is_transparent_now() &&
                intersects(ball->get_AABB(), block->get_AABB()))
            {
                if (block->get_is_show())
                {
                    if(!is_was_collision &&
                        (contains(ball->get_right_side(), block->get_AABB()) ||
                        contains(ball->get_left_side(), block->get_AABB())))
                    {
                        ball->invert_dir_x();
                        is_was_collision = true;
                    }

                    if(!is_was_collision &&
                        (contains(ball->get_top_side(), block->get_AABB()) ||
                        contains(ball->get_bottom_side(), block->get_AABB())))
                    {
                        ball->invert_dir_y();
                        is_was_collision = true;
                    }

                    block->destroy_block();
                    ball->set_can_contact_platform(true);
                }
            }
        }
    }

    bool is_all_ball_destroyed() const
    {
        bool is_all_destroyed = true;

        for (const auto ball : balls_)
        {
            is_all_destroyed &= !ball->get_is_alive();
        }

        return is_all_destroyed;
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

    void init_balls()
    {
        if(shot_count_ <= 0) return;
        
        const int current_time = helpers_->play_timer();
        if(current_time - prev_time_shot_ > 0)
        {
            prev_time_shot_ = helpers_->play_timer();
            
            switch (shot_count_)
            {
            case 3:
                shot_ball(balls_[0]);
                break;

            case 2:
                shot_ball(balls_[1]);
                break;
            case 1:
                shot_ball(balls_[2]);
                break;
            default: break;
            }

            shot_count_--;
        }
    }

    void shot_ball(Ball * ball) const
    {
        ball->ball_on_platform_position_update(platform_->get_pos(), platform_->get_size());
        ball->set_dir_x(start_direction_shot_.x);
        ball->set_dir_y(start_direction_shot_.y);

        ball->set_is_was_init(true);
    }
};


int main(int argc, char* argv[])
{
    return run(new Drogonoid);
    
    // return 0;
}
