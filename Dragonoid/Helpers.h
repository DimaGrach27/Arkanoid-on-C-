#pragma once
#include <chrono>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
using std::to_string;
using std::string;
using std::cout;
using std::endl;

class Helpers
{
public:
    Helpers()
    {
        prev_time_ = 0;
        start_time_ = time(nullptr);
    }

    static string get_path_to_sprite(int number_sprite)
    {
        const string data_folder = "F:/Dima/Development/C++ projects/Dragonoid/Dragonoid/data/"; // for PC
        // const string data_folder = "D:/Development/Projects_on_CPP/Dragonoid/Dragonoid/data/"; // for Notebook
        const string sprite_name = "-Breakout-Tiles.png";
        
        string path = data_folder + to_string(number_sprite) + sprite_name;
        return path;
    }

    static int get_random_num()
    {
        std::random_device rd;
        std::mt19937::result_type seed = rd() ^ (
                (std::mt19937::result_type)
                std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                    ).count() +
                (std::mt19937::result_type)
                std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch()
                    ).count() );

        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> distrib(1, 100);
        return distrib(gen);
    }

    int play_timer()
    {
        const time_t current_time = time(nullptr);
        int time_less = prev_time_ - start_time_;
    
        if(current_time - prev_time_ > 0)
        {
            prev_time_ = time(nullptr);
    
            // cout << "Play time: " << current_time - start_time_ << "(s)" << endl;

            time_less = prev_time_ - start_time_;
        }

        return time_less;
    }

private:
    time_t prev_time_ = 0;
    time_t start_time_ = time(nullptr);
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

enum ability_type
{
    none,
    big_platform,
    small_platform,
    fast_platform,
    slow_platform
};
