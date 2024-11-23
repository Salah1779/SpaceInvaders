#pragma once
#include "Spaceship.hpp"
#include "Obstacle.hpp"
#include "Alien.hpp"


class Game {
public:
    Game();
    ~Game();
    void Draw();
    void Update();
    void HandleInput();
    int checkForHighscore();
    int getScore() const ;
    bool isStopped() const;


    Music music;

 private:
     int score;
     bool run;
     int highscore;
     Spaceship spaceship;
     std::vector<Obstacle> obstacles;
     std::vector<Alien> aliens;
     Sound explosionSound;


    std::vector<Obstacle> CreateObstacles();
    std::vector<Alien> CreateAliens();
    void MoveAliens();
    void AlienShootLaser();
    void CheckForCollisions();
    void GameOver();
    void Reset();
    void InitGame();
   
    void Win();
    void saveHighscoreToFile(int highscore);
    int loadHighscoreFromFile();
    
};