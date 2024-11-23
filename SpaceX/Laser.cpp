#include "Laser.hpp"
#include <iostream>

Laser::Laser(Vector2 position, int speed)
{
    this->position = position;
    this->speed = speed;
    active = true;
}

void Laser::Draw() const {
    if (active)
        DrawRectangle(position.x, position.y, 4, 15, { 243, 216, 63, 255 });
}

Rectangle Laser::getRect() const
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 4;
    rect.height = 15;       
    return rect;
}

bool Laser::isActive() const
{
    return active;
}

void Laser::Activate() 
{
    if(!active)
		active = true;
}

void Laser::Desactivate()
{
	if (active)
		active = false;
}
bool Laser::CollidesWith(const Rectangle& rect) const 
{
    return CheckCollisionRecs(getRect(), rect);
}



void Laser::Update() {
    position.y += speed;
    if (active) {
        if (position.y > GetScreenHeight() - 100 || position.y < 25) {
            active = false;
        }
    }
}