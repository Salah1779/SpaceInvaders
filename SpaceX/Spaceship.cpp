#include "Spaceship.hpp"

Spaceship::Spaceship() {
    image = LoadTexture("Graphics/spaceship.png");
    position.x = (GetScreenWidth() - image.width) / 2.0;
    position.y = GetScreenHeight() - image.height - 100;
    lastFireTime = 0.0;
    laserSound = LoadSound("Sound/laser.wav");
    life = 100;
}

Spaceship::~Spaceship() {
    UnloadTexture(image);
    UnloadSound(laserSound);
    lasers.clear(); 
}

void Spaceship::Draw() const {
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft() {
    position.x -= 7;
    if (position.x < 25) {
        position.x = 25;
    }
}

void Spaceship::MoveRight() {
    position.x += 7;
    if (position.x > GetScreenWidth() - image.width - 25) {
        position.x = GetScreenWidth() - image.width - 25;
    }
}

void Spaceship::FireLaser() {
    if (GetTime() - lastFireTime >= 0.35) {
        lasers.push_back(std::make_shared<Laser>(
            Vector2{ position.x + image.width / 2 - 2, position.y }, -6));
        lastFireTime = GetTime();
        PlaySound(laserSound);
    }
}

Rectangle Spaceship::getRect() const {

    return { position.x, position.y, float(image.width), float(image.height) };
}

void Spaceship::Reset() {
    life = 100;
    position.x = (GetScreenWidth() - image.width) / 2.0;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear(); 
}

void Spaceship::apply_damage(float dm) {
    life -= life * dm;
}

void Spaceship::Update_Lasers() {
    for (auto& laser : lasers) {
        laser->Update();
    }
    DeleteInactiveLasers();
}

void Spaceship::DrawLasers() {
    for (auto& laser : lasers) {
        laser->Draw();
    }
}

void Spaceship::DeleteInactiveLasers() {
    for (auto it = lasers.begin(); it != lasers.end();) {
        if (!(*it)->isActive()) {
            it = lasers.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Spaceship::HandleLasersCollisionsWithObstacles(std::vector<Obstacle>& obstacles) {
    for (auto& laser : lasers) {
        for (auto& obstacle : obstacles) {
            obstacle.HandleBlockCollisionsWithLaser(*laser);
        }
    }
}

void Spaceship::HandleLaserCollisionsWithAliens(std::vector<Alien>& aliens, int& score, Sound explosionSound) {
    for (auto& laser : lasers) {
        auto it = aliens.begin();
        while (it != aliens.end()) {
            if (laser->CollidesWith(it->getRect())) {
                PlaySound(explosionSound);
                score += it->DefineScore();
                laser->Desactivate();
                it = aliens.erase(it); 
            }
            else {
                ++it;
            }
        }
    }
}

bool Spaceship::isDestroyed() const {
    return life <= 0;
}
