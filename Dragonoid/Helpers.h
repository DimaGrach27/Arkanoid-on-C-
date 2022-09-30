#pragma once
#include <iostream>
#include <ostream>
#include <string>
using std::to_string;
using std::string;
using std::cout;
using std::endl;

class Helpers
{
public:
    
    static string get_path_to_sprite(int number_sprite)
    {
        const string data_folder = "F:/Dima/Development/C++ projects/Dragonoid/Dragonoid/data/";
        const string sprite_name = "-Breakout-Tiles.png";
        
        string path = data_folder + to_string(number_sprite) + sprite_name;
        return path;
    }

    void fps()
    {
        const int current_tick = static_cast<int>(getTickCount());
        const time_t current_time = time(0);
    
        if(current_time - start_time_ > 0)
        {
            start_time_ = time(0);
    
            cout << "fps: " << current_tick - prev_tick_ << endl;
            prev_tick_ = getTickCount();
        }
    }

private:
    int prev_tick_ = 0;

    time_t start_time_ = 0;
};

struct vector2_int
{
    vector2_int(int X, int Y)
    {
       x = X; 
       y = Y; 
    }
    int x;
    int y;
};

struct vector2_float
{
    vector2_float(float X, float Y)
    {
        x = X; 
        y = Y; 
    }
    float x;
    float y;
};

struct AABB
{
    int x_min = 0;
    int y_min = 0;
    int x_max = 0;
    int y_max = 0;
};
