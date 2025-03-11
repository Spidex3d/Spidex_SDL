#pragma once
#include <SDL3/SDL.h>
//#include <glm.hpp>


// This is my  scene.h for working with Backgrounds

struct BackPosition {
	float x, y;
};
BackPosition backpos = { 0, 0 };

struct PlatformInfo {
    float x, y; 
};
PlatformInfo platforminfo = {10.0f, 580.0f};

class BackGroundTexture {
public:
    static SDL_Texture* BackgrounTex;
    static SDL_Texture* MidlegroundTex;
    static SDL_Texture* GroundTex;
    static SDL_Texture* PlatformTex;

    static bool InitializeTextures(SDL_Renderer* renderer) {
        BackgrounTex = IMG_LoadTexture(renderer, "assets/Pixel_Valley/Background.png");
        if (!BackgrounTex) {
            SDL_Log("Failed to load background texture: %s", SDL_GetError());
            return false;
        }
        MidlegroundTex = IMG_LoadTexture(renderer, "assets/Pixel_Valley/Midleground.png");
        if (!MidlegroundTex) {
            SDL_Log("Failed to load midground texture: %s", SDL_GetError());
            return false;
        }
       
        GroundTex = IMG_LoadTexture(renderer, "assets/New_Pack/tile/2.png");
        if (!GroundTex) {
            SDL_Log("Failed to load Ground texture: %s", SDL_GetError());
            return false;
        }
        /*PlatformTex = IMG_LoadTexture(renderer, "assets/New_Pack/tile/2.png");
        if (!PlatformTex) {
            SDL_Log("Failed to load Platform texture: %s", SDL_GetError());
            return false;
        }*/
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
    }
private:
   
};
SDL_Texture* BackGroundTexture::BackgrounTex = nullptr;
SDL_Texture* BackGroundTexture::MidlegroundTex = nullptr;
SDL_Texture* BackGroundTexture::GroundTex = nullptr;
//SDL_Texture* BackGroundTexture::PlatformTex = nullptr;

//class Platforms {
//public:
//    static SDL_Texture* GroundTex;
//    static SDL_Texture* PlatformTex;
//
//    static bool InitializeTextures(SDL_Renderer* renderer) {
//        GroundTex = IMG_LoadTexture(renderer, "assets/New_Pack/background/pine1.png");
//        if (!GroundTex) {
//            SDL_Log("Failed to load Ground texture: %s", SDL_GetError());
//            return false;
//        }
//        /*PlatformTex = IMG_LoadTexture(renderer, "assets/New_Pack/tile/2.png");
//        if (!PlatformTex) {
//            SDL_Log("Failed to load Platform texture: %s", SDL_GetError());
//            return false;
//        }*/
//        return true;
//    }
//};

