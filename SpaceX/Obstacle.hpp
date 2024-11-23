#pragma once
#include <vector>
#include "Block.hpp"
#include "Laser.hpp"

class Alien;

class Obstacle {
public:
    Obstacle(Vector2 position);
    void Draw();
    void HandleBlockCollisionsWithLaser(Laser& laser);
    void HandleBlockCollisionsWithAlien(Alien& alien);

   
    static std::vector<std::vector<int>> grid;
private:
    Vector2 position;
    std::vector<Block> blocks;
};