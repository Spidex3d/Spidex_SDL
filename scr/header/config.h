#pragma once
#include <SDL3\SDL.h>

#include <iostream>
#include <filesystem>
#include <vector>

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

static SDL_FlipMode flip = SDL_FLIP_NONE; // Keep track of the current flip state

const bool* Keyboard_state = SDL_GetKeyboardState(NULL); // keyboard input




