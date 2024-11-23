#include "Game.hpp"
#include <iostream>
#include <fstream>

Game::Game()
{
    music = LoadMusicStream("Sound/music.wav");
    explosionSound = LoadSound("Sound/explosion.wav");
    PlayMusicStream(music);
    InitGame();
}

void Game::InitGame()
{ 

    obstacles = CreateObstacles();
    aliens = CreateAliens();
    Alien::direction = 1;
    Alien::lastFireTime = 0.0;
    score = 0;
    highscore = loadHighscoreFromFile();
    run = true;
}


void Game::Reset() {
    spaceship.Reset();
    aliens.clear();
    obstacles.clear();
}


void Game::Update() {
    if (run) {

        
       spaceship.Update_Lasers();

       if (aliens.empty()) Win();

       AlienShootLaser();

       for (auto& alien : aliens)
       {
           alien.Upadate_Lasers();
       }

        CheckForCollisions();

        MoveAliens();
    }
   
}

void Game::Draw() {

    spaceship.Draw();
    spaceship.DrawLasers();

    for (auto& obstacle : obstacles) {
        obstacle.Draw();
    }

    for (auto& alien : aliens) {
        alien.Draw();
        alien.DrawLasers();
    }


  
}

void Game::HandleInput() {
    if (run) {
        if (IsKeyDown(KEY_LEFT)) {
            spaceship.MoveLeft();
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            spaceship.MoveRight();
        }
        else if (IsKeyDown(KEY_SPACE)) {
            spaceship.FireLaser();
        }
    }
    else {
		if (IsKeyPressed(KEY_ENTER)) {
			Reset();
			InitGame();
		}
	}
}


std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - 4 * obstacleWidth) / 5;

    for (int i = 0; i < 4; i++) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({ offsetX, float(GetScreenHeight() - 200) }));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 11; column++) {

            int alienType;
            if (row == 0) {
                alienType = 3;
            }
            else if (row == 1 || row == 2) {
                alienType = 2;
            }
            else {
                alienType = 1;
            }

            float x = 75 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, { x, y }));
            
        }
    }
    return aliens;
}

void Game::MoveAliens() {
    int dir=Alien::direction;
    for (auto& alien : aliens) {
        alien.SwitchDirection();

        if (dir != Alien::direction)
        {
            for (auto& alien : aliens) {
                alien.MoveDown();
            }
        }
        
        alien.Update();
    }
}


void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if (currentTime - Alien::lastFireTime >= 0.2 && !aliens.empty()) {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alien.Fire(); 
        Alien::lastFireTime = GetTime();
    }
}

void Game::CheckForCollisions()
{
    
    spaceship.HandleLaserCollisionsWithAliens(aliens, score, explosionSound);
    spaceship.HandleLasersCollisionsWithObstacles(obstacles);

    for (auto& alien : aliens)
    {
		alien.HnadleLaserCollisionsWithObstacles(obstacles);
        for (auto& obstacle : obstacles) {
            obstacle.HandleBlockCollisionsWithAlien(alien);
        }
        
        
    }

    for(auto& alien : aliens)
	{
        if ( alien.HandleLaserCollisionsWithShip(spaceship)) 
            GameOver();
      
        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
	     	GameOver();
		
	}
    


}

void Game::GameOver() {
    run = false;

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0); 

    while (!run && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({ 29, 29, 27, 255 });

        
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

        // Modal box
        int modalWidth = 400;
        int modalHeight = 300;
        int modalX = (GetScreenWidth() - modalWidth) / 2;
        int modalY = (GetScreenHeight() - modalHeight) / 2;
        DrawRectangle(modalX, modalY, modalWidth, modalHeight, YELLOW);

        
        DrawTextEx(font, "GAME OVER", { (float)(modalX + 80), (float)(modalY + 50) }, 34, 2, BLACK);

       
        Rectangle replayButton = { (float)(modalX + 50), (float)(modalY + 200), 120, 40 };
        Rectangle quitButton = { (float)(modalX + 230), (float)(modalY + 200), 120, 40 };

        
        DrawRectangleRec(replayButton, WHITE);
        DrawTextEx(font, "REPLAY", { replayButton.x + 20, replayButton.y + 10 }, 24, 2, BLACK);

        
        DrawRectangleRec(quitButton, WHITE);
        DrawTextEx(font, "QUIT", { quitButton.x + 35, quitButton.y + 10 }, 24, 2, BLACK);

       
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePosition = GetMousePosition();

           
            if (CheckCollisionPointRec(mousePosition, replayButton)) {
                
                Reset();           
                InitGame();      
                return;            
            }

           
            if (CheckCollisionPointRec(mousePosition, quitButton)) {
                UnloadFont(font); 
                CloseWindow();    
                exit(0);          
            }
        }

        EndDrawing();
    }

    UnloadFont(font); 
}




bool Game::isStopped() const
{
    return run;
}

int Game::checkForHighscore()
{
    if (score > highscore) {
        highscore = score;
        saveHighscoreToFile(highscore);
    }
    return highscore;
}

int Game::getScore() const
{
    return score;
}

void Game::Win() {

 
      
    run = false;
     /*Rectangle{ 300, 300, 200, 50 }
    Rectangle{ 300, 370, 200, 50 }*/
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    while (!run && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
       // ClearBackground({ 29, 29, 27, 255 });


        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

        
        int modalWidth = 400;
        int modalHeight = 300;
        int modalX = (GetScreenWidth() - modalWidth) / 2;
        int modalY = (GetScreenHeight() - modalHeight) / 2;
        DrawRectangle(modalX, modalY, modalWidth, modalHeight, YELLOW);



        DrawTextEx(font, "YOU WIN!!", { (float)(modalX + 80), (float)(modalY + 50) }, 40, 2, GREEN);
        DrawTextEx(font,TextFormat("SCORE: %d", score), { (float)(modalX + 80), (float)(modalY + 100) }, 24, 2, GREEN);


        Rectangle replayButton = { (float)(modalX + 50), (float)(modalY + 200), 120, 40 };
        Rectangle quitButton = { (float)(modalX + 230), (float)(modalY + 200), 120, 40 };


        DrawRectangleRec(replayButton, WHITE);
        DrawTextEx(font, "REPLAY", { replayButton.x + 20, replayButton.y + 10 }, 24, 2, BLACK);


        DrawRectangleRec(quitButton, WHITE);
        DrawTextEx(font, "QUIT", { quitButton.x + 35, quitButton.y + 10 }, 24, 2, BLACK);


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePosition = GetMousePosition();


            if (CheckCollisionPointRec(mousePosition, replayButton)) {

                Reset();
                InitGame();
                return;
            }


            if (CheckCollisionPointRec(mousePosition, quitButton)) {
                UnloadFont(font);
                CloseWindow();
                exit(0);
            }
        }

        EndDrawing();
    }

    UnloadFont(font);
}


void Game::saveHighscoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    }
    else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::loadHighscoreFromFile() {
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open()) {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    }
    else {
        std::cerr << "Failed to load highscore from file." << std::endl;
    }
    return loadedHighscore;
}



Game::~Game() {
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}