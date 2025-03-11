#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <SDL3_image\SDL_image.h>

#include "header\config.h"
#include "header\scene.h"
#include "header\window.h"
#include "header\utils.h"
#include "header\animation.h"
#include "header\enemieAnimation.h"
#include "header\player.h"
#include "header\enemies.h"

// ##################################### Start here Platform Game ##########################################
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
    
    Animation animation;
    EnemiesAnimation enemies_animation;
   // float velocity = 5.0f; // Speed of the helicopter
/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldent initilize sdl: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Spidex Platform", SCR_WIDTH, SCR_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Coulden't make a window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    setWindowIcon(window, "icon.bmp"); // set the window icon must be a .bmp

    // Initialize textures NEW BackGroundTexture
    if (!BackGroundTexture::InitializeTextures(renderer)) {
        SDL_Log("Failed to initialize textures");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
    }

    
   /* if (!Platforms::InitializeTextures(renderer)) {
        SDL_Log("Failed to initialize textures");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    }*/
   
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }    
    // Set Idle animation as defaul
        appState.animation_State = 1;
    // Check for specific key presses
    if (Keyboard_state[SDL_SCANCODE_RIGHT]) {
        position.x += 10.5f;
        if (isOnGround) {
            appState.animation_State = 2; // Set to Run only if on the ground
        }
        flip = SDL_FLIP_NONE; // No flipping when moving right
        std::cout << "Running Right " << appState.animation_State << std::endl;
    }
    if (Keyboard_state[SDL_SCANCODE_LEFT]) {
        position.x -= 10.5f;
        if (isOnGround) {
            appState.animation_State = 2; // Set to Run only if on the ground
        }
        flip = SDL_FLIP_HORIZONTAL; // Flip for left movement
        std::cout << "Running Left " << appState.animation_State << std::endl;
    }
    // only here for testing
    if (Keyboard_state[SDL_SCANCODE_DOWN]) {
        //position.y += 10.5f; // Adjust y-position for a jump will need gravaty
        appState.animation_State = 3; // Set to Death 
        std::cout << "Little green man dying " << appState.animation_State << std::endl;
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    Uint32 startTime = SDL_GetTicks();
    static int previous_State; // Keep track of the previous state
   
    handleJump(Keyboard_state);
 
    if (appState.animation_State != previous_State) {
        // Reload animation frames for the new state
        std::string player_Animation_Type;
        switch (appState.animation_State) {
        case 1: player_Animation_Type = "Idle"; break;
        case 2: player_Animation_Type = "Run"; break;
        case 3: player_Animation_Type = "Death"; break;
        case 4: player_Animation_Type = "Jump"; break;        
        }
        std::string folderPath = "assets/New_Pack/player/" + player_Animation_Type;
        animation.loadAnimationFrames(folderPath);
        previous_State = appState.animation_State; // Update the previous state
                      
    }
        
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
       // Background 
    SDL_FRect background_position = { backpos.x, backpos.y, SCR_WIDTH, SCR_HEIGHT };
    SDL_RenderTexture(renderer, BackGroundTexture::BackgrounTex, NULL, &background_position);
    SDL_RenderTexture(renderer, BackGroundTexture::MidlegroundTex, NULL, &background_position);
        // Floor
    SDL_FRect const platformFloor = { platforminfo.x, platforminfo.y, 64.0f, 64.0f };
    SDL_RenderTexture(renderer, BackGroundTexture::GroundTex, NULL, &platformFloor);
        // Platforms
        
        // Player
    SDL_FRect  playerSprite = { position.x, position.y, 37, 87 };
    //SDL_SetTextureScaleMode(playerTex, SDL_SCALEMODE_NEAREST); // dose not seem to have any impact
    animation.animatePlayer(renderer, playerSprite, flip);
        // Collition     
    if (SDL_HasRectIntersectionFloat(&playerSprite, &platformFloor)) {
        if (playerSprite.y + 79 <= platformFloor.y) {
            // The player is landing on top of the platform
            playerVelocity.y = platformFloor.y - playerSprite.h; // Adjust player position
            playerVelocity.y = 0;
            isOnGround = true; // player is on the ground
        }
        else {
            isOnGround = false; //player is airborne 
        }
        std::cout << "Contact is " << isOnGround << std::endl;
    }

        //################################################ Sound ################################################
        // Can't get Sound to work at the moment
        //################################################ Enemies ################################################
    std::string enemies_Type = "black"; // select the blck Helicopter
    std::string folderPath = "assets/helicopter/" + enemies_Type;   
    enemies_animation.loadEnemieAnimationFrames(folderPath);
        
    SDL_FRect enemie_sprite_position = { enemiePosition.x, enemiePosition.y, 155, 105 };
    SDL_SetTextureScaleMode(enemieTex, SDL_SCALEMODE_NEAREST);
           
    static SDL_FlipMode enemieflip = SDL_FLIP_HORIZONTAL; // To track the direction of the helicopter
    enemiePosition.x += velocity;

    // Update logic in the game loop enemie_sprite_position.w = sprite width 155
    if (enemiePosition.x >= SCR_WIDTH - enemie_sprite_position.w) {
        enemiePosition.x = SCR_WIDTH - enemie_sprite_position.w;
        std::cout << "Right edge reached. Reversing direction." << std::endl;
        velocity = -5.0f; // Reverse direction when reaching the right edge
        enemieflip = SDL_FLIP_NONE; // Flip horizontally
        
       /* std::cout << "SCR_WIDTH: " << SCR_WIDTH << ", EnemiePosition + Width: "
            << enemiePosition.x + enemie_sprite_position.w << std::endl;*/

    }
    else if (enemiePosition.x <= 0) {
        enemiePosition.x = 0;
        std::cout << "Left edge reached. Reversing direction." << std::endl;
        velocity = 5.0f;  // Reverse direction when reaching the left edge
        enemieflip = SDL_FLIP_HORIZONTAL; // Unflip
       // std::cout << "Enemie pos x " << enemiePosition.x << std::endl;
    
    }

    // Update position
    enemie_sprite_position.x = enemiePosition.x;
   /* std::cout << "Position: " << enemiePosition.x
        << ", Velocity: " << velocity << std::endl;*/

    // Render with the updated position and flip state
    SDL_RenderTextureRotated(renderer, enemieTex, NULL, &enemie_sprite_position, 0, NULL, enemieflip);
    enemies_animation.animateEnemies(renderer, enemie_sprite_position, enemieflip);
 
    SDL_RenderPresent(renderer);
    
    Uint32 endTime = SDL_GetTicks();
    Uint32 frameTime = endTime - startTime;
    if (frameTime < 80) {
        SDL_Delay(80 - frameTime);
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    BackGroundTexture::CleanupTextures();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    SDL_free(appstate); // this just fees any memory we have used 
    /* SDL will clean up the window/renderer for us, and then we can go to bed */
}

// ###################################### End ##########################################


