#include <chrono>
#include <iostream>
#include <string>

#include "Framework.h"
#include "IStart.h"
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
    Block block[64];

    IStart starts[66];
    
    void PreInit(int& width, int& height, bool& fullscreen) override
    {
        width = 800;
        height = 600;
        fullscreen = false;
    }

    bool Init() override
    {
        starts[0] = static_cast<IStart>(platform);
        starts[1] = static_cast<IStart>(ball);

        
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
                starts[count + 2] = static_cast<IStart>(block[count]);
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
            

            ball.set_direction({vec_dir_x * vec_inv_mag_, vec_dir_y * vec_inv_mag_});
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

    void start_game()
    {
        if(is_game_end)
        {
            is_game_end = false;

            for (auto i_start : starts)
            {
                i_start.start();
            }
        }
    }

private:

    vector2_int mouse_pos {0,0};

    int count = 0;
    
    Sprite* field_sprite_ = nullptr;
    

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
        // vector2_int box(ball.pos.x - ball.ball_size_ / 2,ball.pos.y - ball.ball_size_ / 2);

        // int part = 0;
        // float angle = sin(ball.direction.x);
        //
        // if(ball.direction.x < 0.25f && ball.direction.y)
        // {
        //     
        // }
        proection element_proection;
        bool is_was_collision = false;
        
        for (int i = 0; i < count; i++)
        {
            block[i].show_block();

            if(!block[i].is_show) continue;
            
            constexpr int offset = 3;
            element_proection.x_min = block[i].pos.x + offset;
            element_proection.x_max = block[i].pos.x + block_size.x + offset;
            element_proection.y_min = block[i].pos.y + offset;
            element_proection.y_max = block[i].pos.y + block_size.y + offset;

            ball.set_object_proection(element_proection, false, is_was_collision);
            
            if(is_was_collision)
            {
                block[i].destroy_block();
                return;
            }
        }


        constexpr int offset = 10;
        element_proection.x_min = platform.pos.x + offset;
        element_proection.x_max = platform.pos.x + platform.size.x + offset;
        element_proection.y_min = platform.pos.y + offset;
        element_proection.y_max = platform.pos.y + platform.size.y + offset;

        ball.set_object_proection(element_proection, true, is_was_collision);
        
        // bool is_first_half = true;
        // for (int i = 1; i < 3; i++)
        // {
        //     for (int j = 1; j < 5; j++)
        //     {
        //         const int mult = is_first_half ? 1 : -1;
        //         if(j < 3) box.x += mult * ball.ball_size_;
        //         else box.y += mult * ball.ball_size_;
        //
        //         vector2_int result= {0, 0};
        //         
        //         if(check_element(box, 0, result))
        //         {
        //             ball.set_object_border(result);
        //             return;
        //         }
        //     }
        //     
        //     is_first_half = !is_first_half;
        // }
        //
        // ball.set_object_border({0, 0});
    }
    
    // bool check_element(vector2_int border, int index, vector2_int &out)
    // {
    //     if(border.x > platform.pos.x && border.x < platform.pos.x + platform.size.x &&
    //         border.y > platform.pos.y && border.y < platform.pos.y + platform.size.y)
    //     {
    //         if(ball.pos.x > platform.pos.x + platform.size.x) out.x = platform.pos.x;
    //         if(ball.pos.x - ball.ball_size_ < platform.pos.x) out.x = platform.pos.x;
    //         if(ball.pos.y > platform.pos.y + platform.size.y) out.y = platform.pos.y + platform.size.y;
    //         if(ball.pos.y - ball.ball_size_ < platform.pos.y) out.y = platform.pos.y;
    //         return true;
    //     }
    //     
    //     return false;
    // }
};


int main(int argc, char* argv[])
{
    return run(new Drogonoid);
}
