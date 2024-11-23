#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include "Laser.hpp"
#include "Obstacle.hpp"
class Spaceship;

class Alien {
public:
    static Texture2D alienImages[3];
    static float damage[3];
    static int direction;
    static double lastFireTime;

    Alien(int type, Vector2 position );
    void Update();
    void Draw() const;
    static void UnloadImages();
    void SwitchDirection() const;
    Rectangle getRect() const;
    void Upadate_Lasers();
    void DrawLasers();
    void Fire();
    void MoveDown();
    void DeleteInactivseLasers();
    int DefineScore() const;
    bool HandleLaserCollisionsWithShip(Spaceship& spaceship);
    void HnadleLaserCollisionsWithObstacles(std::vector<Obstacle>& obstacles);


    
    
private:
  
    int type;
    Vector2 position;
    std::vector<std::shared_ptr<Laser>> lasers;
   

};