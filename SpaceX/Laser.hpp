#pragma once
#include <raylib.h>

class Laser {
public:
    Laser(Vector2 position, int speed);
    void Update();
    void Draw() const;
    Rectangle getRect() const ;
	bool isActive() const;
    void Activate();
    void Desactivate();
    bool CollidesWith(const Rectangle& rect) const;
    
private:
    bool active;
    Vector2 position;
    int speed;
};