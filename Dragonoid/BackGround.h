#pragma once
#include "Framework.h"
#include "Helpers.h"

class BackGround
{
public:
    BackGround();
    
    void draw() const;
    
private:
    Sprite* field_sprite = nullptr;

    vector2_int size = {0,0};
};
