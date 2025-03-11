#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image\SDL_image.h>

#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class EnemiesAnimation {
private:
    std::vector<std::string> framePaths; // Stores the paths of animation frames
    size_t EcurrentFrame = 0;             // Current frame index

public:
    // Load animation frames from the specified folder
    void loadEnemieAnimationFrames(const std::string& folderPath) {
        framePaths.clear();
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                framePaths.push_back(entry.path().string());
            }
        }
        std::sort(framePaths.begin(), framePaths.end()); // Ensure sequential order
        //EcurrentFrame = 0;
    }

    // Animate the Enenies by rendering the current frame
    void animateEnemies(SDL_Renderer* renderer, SDL_FRect& spritePosition, SDL_FlipMode flip) {
        if (framePaths.empty()) {
            std::cerr << "No frames available for animation!" << std::endl;
            return;
        }

        // Load the current frame texture
        SDL_Texture* enemieTex = IMG_LoadTexture(renderer, framePaths[EcurrentFrame].c_str());
        if (enemieTex) {
            // Render the current frame
            SDL_RenderTextureRotated(renderer, enemieTex, nullptr, &spritePosition, 0.0, nullptr, flip);
            SDL_DestroyTexture(enemieTex); // Clean up the texture
        }

        // Update frame index for the next iteration
        EcurrentFrame = (EcurrentFrame + 1) % framePaths.size();
    }
};



