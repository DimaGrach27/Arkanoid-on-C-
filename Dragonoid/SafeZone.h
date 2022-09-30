#pragma once
#include "Framework.h"
#include "Helpers.h"

class SafeZone
{
public:
    void init();
    void restart();
    void death();
    void draw() const;
    bool isAlive() const;
private:
    bool is_alive = true;
    
    int position = 0;
    
    vector2_int size = {0,0};
    
    Sprite* save_zone_sprite = nullptr;
};
