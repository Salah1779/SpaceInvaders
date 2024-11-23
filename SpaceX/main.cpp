#include <raylib.h>
#include "Game.hpp"
#include <string>

static std::string FormatWithLeadingZeros(int number, int width) {
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = std::string(leadingZeros, '0') + numberText;
}

int main() {
    Color grey = { 29, 29, 27, 255 };
    Color yellow = { 243, 216, 63, 255 };
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 630;

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Salah's Game");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(60);

    Game game;

    // Variable pour l'écran d'accueil
    bool showMenu = true;

    while (!WindowShouldClose()) {
        UpdateMusicStream(game.music);

        if (showMenu) {
            // Gestion de la page d'accueil
            BeginDrawing();
            ClearBackground(grey);

            // Texte de bienvenue
            DrawTextEx(font, "SPACE INVADERS", { 180, 200 }, 50, 2, yellow);
            DrawTextEx(font, "PRESS START", { 210, 300 }, 34, 2, yellow);

            // Bouton Start
            Rectangle startButton = { 280, 400, 200, 50 };
            DrawRectangleRec(startButton, yellow);
            DrawTextEx(font, "START", { 330, 410 }, 34, 2, grey);

            // Détection du clic
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePosition = GetMousePosition();
                if (CheckCollisionPointRec(mousePosition, startButton)) {
                    showMenu = false; // Passer à l'écran de jeu
                }
            }

            EndDrawing();
        }
        else {
            // Jeu principal
            game.HandleInput();
            game.Update();

            BeginDrawing();
            ClearBackground(grey);
            DrawRectangleRoundedLines(Rectangle{ 10, 10, 780, 780 }, 0.18f, 20, yellow);

           // DrawLineEx({ 25, 730 }, { 775, 730 }, 3, yellow);

           /*if(!game.isStopped())
            float x = 50.0;
            for (int i = 0; i < ; i++) {
                DrawTextureV(spaceshipImage, { x, 745 }, WHITE);
                x += 50;
            }*/

            DrawTextEx(font, "SCORE", { 50, 15 }, 34, 2, yellow);
            std::string scoreText = FormatWithLeadingZeros(game.getScore(), 5);
            DrawTextEx(font, scoreText.c_str(), { 50, 40 }, 34, 2, yellow);

            DrawTextEx(font, "HIGH-SCORE", { 570, 15 }, 34, 2, yellow);
            std::string highscoreText = FormatWithLeadingZeros(game.checkForHighscore(), 5);
            DrawTextEx(font, highscoreText.c_str(), { 655, 40 }, 34, 2, yellow);

            game.Draw();
            EndDrawing();
        }
    }

    CloseWindow();
    CloseAudioDevice();
}
