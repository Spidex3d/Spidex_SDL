#pragma once
#include <SDL3\SDL.h>

// Enemies a person or thing who is actively opposed or hostile to someone or something.

static SDL_Texture* enemieTex;
std::string Enemies_Animation_Type;
float velocity = 5.0f; // Speed of the helicopter

struct EnemiePosition {
	float x, y;
};
EnemiePosition enemiePosition = { 0, 100 }; // Start position on screen

//SDL_FRect enemie_sprite_portion = { 0, 0, 100, 100 };
//SDL_FRect enemie_sprite_portion = { 0, 0, 0, 0 };
