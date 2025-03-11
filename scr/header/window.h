#pragma once
#include <SDL3/SDL.h>


// Set the window icon
void setWindowIcon(SDL_Window* window, const char* iconPath) {
    SDL_Surface* icon = SDL_LoadBMP("assets/win_icon/github.bmp"); // Load the icon (must be BMP format)
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_DestroySurface(icon); // Free the surface once it's set
    }
    else {
        SDL_Log("Failed to load icon: %s", SDL_GetError());
    }
}