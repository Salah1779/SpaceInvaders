#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include "Laser.hpp"
#include "Alien.hpp"
#include "Obstacle.hpp"


class Spaceship {
public:
    Spaceship();
    ~Spaceship();
    void Draw() const;
    void MoveLeft();
    void MoveRight();
    void FireLaser();
    Rectangle getRect() const;
    void Reset();
    void apply_damage(float dm);
    void Update_Lasers();
    void DrawLasers();
    void DeleteInactiveLasers();
    void HandleLaserCollisionsWithAliens(std::vector<Alien>& aliens, int& score, Sound explosionSound);
    void HandleLasersCollisionsWithObstacles(std::vector<Obstacle>& obstacles);
    bool isDestroyed() const;
   

private:
    std::vector<std::shared_ptr<Laser>> lasers;
    Texture2D image;
    Vector2 position;
    double lastFireTime;
    Sound laserSound;
    int life;
};