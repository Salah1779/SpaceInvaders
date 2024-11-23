#include "Alien.hpp"
#include"Spaceship.hpp"
#include <iostream>

// Static variables initialization
float Alien::damage[3] = { 0.1, 0.25, 0.35 };
int Alien::direction = 1;
double Alien::lastFireTime = 0.0;

Texture2D Alien::alienImages[3] = {};

Alien::Alien(int type, Vector2 position){

    if (type < 1 || type > 3) {
        std::cout << "Invalid alien type" << std::endl;
    }
    this->type = type;
    this->position = position;


    if (alienImages[type - 1].id == 0) {
        switch (type) {
        case 1:
            alienImages[0] = LoadTexture("Graphics/alien_1.png");
            break;
        case 2:
            alienImages[1] = LoadTexture("Graphics/alien_2.png");
            break;
        case 3:
            alienImages[2] = LoadTexture("Graphics/alien_3.png");
            break;
        default:
            alienImages[0] = LoadTexture("Graphics/alien_1.png");
            break;
        }
    }
}




void Alien::Draw() const {
    DrawTextureV(alienImages[type - 1], position, WHITE);
}

void Alien::UnloadImages() {
    for (int i = 0; i < 3; ++i) {
        if (alienImages[i].id != 0) {
            UnloadTexture(alienImages[i]);
            alienImages[i] = {}; 
        }
    }
}

void Alien::SwitchDirection() const {
    if (position.x + alienImages[type - 1].width > GetScreenWidth() - 25) {
        direction = -1;
    }
    else if (position.x < 25) {
        direction = 1;
    }
}

Rectangle Alien::getRect() const {
    if (type < 1 || type > 3 || alienImages[type - 1].id == 0) {
        return { 0, 0, 0, 0 }; 
    }
    return { position.x, position.y,
             float(alienImages[type - 1].width),
             float(alienImages[type - 1].height) 
       };
}
void Alien::Upadate_Lasers()
{
	for (auto& laser : lasers) {
		laser->Update();
	}
	DeleteInactivseLasers();
}

void Alien::Update() {
    position.x += direction;
}


void Alien::DrawLasers() {
    for (auto& laser : lasers) {
        laser->Draw();
    }
}

void Alien::Fire() {
    lasers.push_back(std::make_shared<Laser>(
        Vector2{ position.x + alienImages[type - 1].width / 2, position.y + alienImages[type - 1].height }, 6));
}

void Alien::MoveDown() {
    position.y += 0.2;
}

void Alien::DeleteInactivseLasers()
{
    for (auto it = lasers.begin(); it != lasers.end();) {
        if (!(*it)->isActive()) {
            it = lasers.erase(it);
        }
        else {
            ++it;
        }
    }
}


int Alien::DefineScore() const {
    return type * 100;
}

bool Alien::HandleLaserCollisionsWithShip(Spaceship& sp) 
{
	for (auto& laser : lasers) {
		if (laser->CollidesWith(sp.getRect())) {
			laser->Desactivate();
			sp.apply_damage(damage[type]);
			if (sp.isDestroyed()) {
				return true;
			}
		}
	}
	return false;
}

void Alien::HnadleLaserCollisionsWithObstacles(std::vector<Obstacle>& obstacles)
{
	for (auto& laser : lasers) {
		for (auto& obstacle : obstacles) {
			obstacle.HandleBlockCollisionsWithLaser(*laser);
		}
	}

}

