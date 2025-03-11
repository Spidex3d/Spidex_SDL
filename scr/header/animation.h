#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image\SDL_image.h>

#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

struct AppState {
    int animation_State;
};
AppState appState = { 1 }; // Start with Idle

class Animation {
private:
    std::vector<std::string> framePaths; // Stores the paths of animation frames
    size_t currentFrame = 0;             // Current frame index

public:
    // Load animation frames from the specified folder
    void loadAnimationFrames(const std::string& folderPath) {
        framePaths.clear();
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                framePaths.push_back(entry.path().string());
            }
        }
        std::sort(framePaths.begin(), framePaths.end()); // Ensure sequential order
        currentFrame = 0;
    }

     //Animate the player by rendering the current frame
    void animatePlayer(SDL_Renderer* renderer, SDL_FRect& spritePosition, SDL_FlipMode flip) {
        if (framePaths.empty()) {
            std::cerr << "No frames available for animation!" << std::endl;
            return;
        }

        // Load the current frame texture
        SDL_Texture* playerTex = IMG_LoadTexture(renderer, framePaths[currentFrame].c_str());
        if (playerTex) {
            // Render the current frame
            SDL_RenderTextureRotated(renderer, playerTex, nullptr, &spritePosition, 0.0, nullptr, flip);
            SDL_DestroyTexture(playerTex); // Clean up the texture
        }

        // Update frame index for the next iteration
        currentFrame = (currentFrame + 1) % framePaths.size();
    }

};

