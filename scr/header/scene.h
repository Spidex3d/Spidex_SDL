#pragma once
#include <SDL3/SDL.h>
#include <vector>

// This is my  scene.h for working with Backgrounds & Game textures

const int BACKGROUND_WIDTH = 3600;
const int BACKGROUND_HEIGHT = 720;

struct BackPosition {
	float x, y;
};
BackPosition backpos = { 0, 0 };

struct PlatformInfo {
    float x, y; 
    float width, height;
    
};

std::vector<PlatformInfo> platforminfo = {
    {10, 580, 64, 64},  // Example platform
    {78, 580, 64, 64},
    {160, 580, 64, 64},
    {224, 516, 64, 64}
};

struct ObjectInfo {
    float x, y;
    float width, height;
};
std::vector<ObjectInfo> objectinfo = {
    {10, 516, 64, 64},  // Example Object
    {224, 452, 64, 64}
};

//class GameTexture {
class GameTexture {
public:
    static SDL_Texture* BackgrounTex;
    static SDL_Texture* MidlegroundTex;
    static SDL_Texture* GroundTex;
    static SDL_Texture* ObjectTexRock;
   // static SDL_Texture* PlatformTex;
    
    static bool InitializeTextures(SDL_Renderer* renderer) {
       
        BackgrounTex = IMG_LoadTexture(renderer, "assets/Pixel_Valley/background_long.png"); //3600 x 300
        if (!BackgrounTex) {
            SDL_Log("Failed to load background texture: %s", SDL_GetError());
            return false;
        }
        MidlegroundTex = IMG_LoadTexture(renderer, "assets/Pixel_Valley/Midleground_long.png");
        if (!MidlegroundTex) {
            SDL_Log("Failed to load midground texture: %s", SDL_GetError());
            return false;
        }
       
        GroundTex = IMG_LoadTexture(renderer, "assets/New_Pack/tile/2.png");
        if (!GroundTex) {
            SDL_Log("Failed to load Ground texture: %s", SDL_GetError());
            return false;
        }
        ObjectTexRock = IMG_LoadTexture(renderer, "assets/New_Pack/tile/11.png");
        if (!ObjectTexRock) {
            SDL_Log("Failed to load Ground texture: %s", SDL_GetError());
            return false;
        }
        
        return true;
                
    }

    static void CleanupTextures() {
        if (BackgrounTex) {
            SDL_DestroyTexture(BackgrounTex);
            BackgrounTex = nullptr;
        }
        if (MidlegroundTex) {
            SDL_DestroyTexture(MidlegroundTex);
            MidlegroundTex = nullptr;
        }
        if (GroundTex) {
            SDL_DestroyTexture(GroundTex);
            GroundTex = nullptr;
        }
        if (ObjectTexRock) {
            SDL_DestroyTexture(ObjectTexRock);
            ObjectTexRock = nullptr;
        }
    }
private:
   
};
SDL_Texture* GameTexture::BackgrounTex = nullptr;
SDL_Texture* GameTexture::MidlegroundTex = nullptr;
SDL_Texture* GameTexture::GroundTex = nullptr;
SDL_Texture* GameTexture::ObjectTexRock = nullptr;
//SDL_Texture* GameTexture::PlatformTex = nullptr;


