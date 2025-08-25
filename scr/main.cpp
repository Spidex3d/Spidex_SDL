#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image\SDL_image.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include "imgui/imgui_impl_sdlrenderer3.h"

#include "header\config.h"
#include "header\scene.h"
#include "header\window.h"
#include "header\utils.h"
#include "header\animation.h"
#include "header\enemieAnimation.h"
#include "header\player.h"
#include "header\enemies.h"
#include <iostream>

// ##################################### Start here Platformer Game ##########################################
//static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

static bool show_demo_window = true;


// Sound
static SDL_AudioStream* stream = NULL;
static Uint8* wav_data = NULL;
static Uint32 wav_data_len = 0;
// Animation    
Animation animation;
EnemiesAnimation enemies_animation;
SDL_FRect camera = { 0, 0, SCR_WIDTH, SCR_HEIGHT };
SDL_FRect camera1 = { 0, 0, SCR_WIDTH, SCR_HEIGHT };

// ################################## Setup Dear ImGui context ##########################################
auto init_imgui(SDL_Window* window, SDL_Renderer* renderer) -> auto {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

}
  
// This function runs once at startup. 
    SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
    {
        SDL_AudioSpec spec; // sound
        char* wav_path = NULL; // sound
       
                                    // need this for sound
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            SDL_Log("Couldent initilize sdl: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        if (!SDL_CreateWindowAndRenderer("Spidex Platform", SCR_WIDTH, SCR_HEIGHT, 0, &window, &renderer)) {
            SDL_Log("Coulden't make a window: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

		// Initialize PNG loading
        setWindowIcon(window, "icon.bmp"); // set the window icon must be a .bmp

       // Load the .wav file from wherever the app is being run from. 
        SDL_asprintf(&wav_path, "assets/sounds/helicopter_01.wav");  /* allocate a string of the full file path */
         
        if (!SDL_LoadWAV(wav_path, &spec, &wav_data, &wav_data_len)) {
            SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
            //return SDL_APP_FAILURE;
        }
        SDL_free(wav_path);  // done with this string.

        // Initialize textures NEW GameTexture
        if (!GameTexture::InitializeTextures(renderer)) {
            SDL_Log("Failed to initialize textures");
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        // Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. 
        stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
        if (!stream) {
            SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        // SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! 
        SDL_ResumeAudioStreamDevice(stream);

        init_imgui(window, renderer);

        return SDL_APP_CONTINUE;
    }

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    
    ImGui_ImplSDL3_ProcessEvent(event);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }    
    if (Keyboard_state[SDL_SCANCODE_ESCAPE]) {  // Esc key to close window
        return SDL_APP_SUCCESS;
    }
    // Set Idle animation as defaul
        appState.animation_State = 1;
    // Check for specific key presses
    if (Keyboard_state[SDL_SCANCODE_RIGHT]) {
        if (player.x < SCR_WIDTH / 2) {
            player.x += 10.5f;
            camera.x += 5.0f;
            camera1.x += 4.0f;  
        }
        else {
           // position.x += 0.0f;
            camera.x += 5.0f;
            camera1.x += 4.0f;
        }
        if (player.isOnGround) {
            appState.animation_State = 2; // Set to Run only if on the ground
        }
        flip = SDL_FLIP_NONE; // No flipping when moving right
        std::cout << "Running Right " << appState.animation_State << std::endl;
    }
    if (Keyboard_state[SDL_SCANCODE_LEFT]) {
        if (player.x > 0) {
        player.x -= 10.5f;
        camera.x += -5.0f;
        camera1.x += -4.0f;
        }
        else {
        player.x = 0.0f;
        }
        if (player.isOnGround) {
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
/* This function runs once per frame, and should contain your main loop code. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    Uint32 startTime = SDL_GetTicks();
    static int previous_State; // Keep track of the previous state
    // Sound
    //if (SDL_GetAudioStreamQueued(stream) < (int)wav_data_len) {
    //    /* feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
    //    SDL_PutAudioStreamData(stream, wav_data, wav_data_len);
    //}
   
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
    // Move the camera to the right for scrolling
    // Ensure camera doesn't go out of bounds
    if (camera.x >= BACKGROUND_WIDTH - SCR_WIDTH) {
        camera.x = 0.0f;  // Loop the background
    }
   
    // Ensure camera doesn't go out of bounds
    if (camera1.x >= BACKGROUND_WIDTH - SCR_WIDTH) {
        camera1.x = 0.0f;  // Loop the background
    }
        
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
        // Render the camera & background
    SDL_RenderTexture(renderer, GameTexture::BackgrounTex, &camera, nullptr);
    SDL_RenderTexture(renderer, GameTexture::MidlegroundTex, &camera1, nullptr);

        // Player
    SDL_FRect  playerSprite = { player.x, player.y, player.width, player.height };
    animation.animatePlayer(renderer, playerSprite, flip);

    // Floor
    for (const auto& platform : platforminfo) {
        SDL_FRect rect = { static_cast<int>(platform.x), static_cast<int>(platform.y),
                         static_cast<int>(platform.width), static_cast<int>(platform.height) };
        SDL_RenderTexture(renderer, GameTexture::GroundTex, NULL, &rect);
    }
    // objects Rocks
    for (const auto& object : objectinfo) {
        SDL_FRect rect = { static_cast<int>(object.x), static_cast<int>(object.y),
                         static_cast<int>(object.width), static_cast<int>(object.height) };
        SDL_RenderTexture(renderer, GameTexture::ObjectTexRock, NULL, &rect);
    }
        // Collition     
    for (const auto& platform : platforminfo) {
        if (player.x + player.width > platform.x &&
            player.x < platform.x + platform.width &&
            player.y + player.height > platform.y &&
            player.y < platform.y + platform.height) {
            // Handle collision
            player.y = platform.y - player.height;  // Place player on top of the platform
            player.isOnGround = true;
            std::cout << "Contact with floor  " << player.isOnGround << std::endl;
            break;
        }
        else {
            player.isOnGround = false;
            std::cout << "No Contact with floor  " << player.isOnGround << std::endl;
        }
    }
    
        //################################################ Enemie Helicopter ################################################
    std::string enemies_Type = "black"; // select the blck Helicopter
    std::string folderPath = "assets/helicopter/" + enemies_Type;   
    enemies_animation.loadEnemieAnimationFrames(folderPath);
        
    SDL_FRect enemie_sprite_position = { enemiePosition.x, enemiePosition.y, 155, 105 };
    SDL_SetTextureScaleMode(enemieTex, SDL_SCALEMODE_NEAREST);
           
    static SDL_FlipMode enemieflip = SDL_FLIP_HORIZONTAL; // To track the direction of the helicopter
    enemiePosition.x += velocity;

    // Update logic in the game loop enemie_sprite_position.w = sprite width 155
    if (enemiePosition.x >= SCR_WIDTH - enemie_sprite_position.w) {
    //if (enemiePosition.x >= BACKGROUND_WIDTH - enemie_sprite_position.w) {
        enemiePosition.x = SCR_WIDTH - enemie_sprite_position.w;
       // enemiePosition.x = BACKGROUND_WIDTH - enemie_sprite_position.w;
        std::cout << "Right edge reached. Reversing direction." << std::endl;
        velocity = -5.0f; // Reverse direction when reaching the right edge
        enemieflip = SDL_FLIP_NONE; // Flip horizontally
        
    }
    else if (enemiePosition.x <= 0) {
        enemiePosition.x = 0;
        std::cout << "Left edge reached. Reversing direction." << std::endl;
        velocity = 5.0f;  // Reverse direction when reaching the left edge
        enemieflip = SDL_FLIP_HORIZONTAL; // Unflip
       
    }

    // Update position
    enemie_sprite_position.x = enemiePosition.x;
    // ##########################################  ImGui window ######################################
    // Start the ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ImGui demo window (show your own widgets here!)
    ImGui::ShowDemoWindow(&show_demo_window);

    // Example custom window
    ImGui::Begin("Game Controls");
    ImGui::Text("Player X: %.2f", player.x);
    ImGui::Text("Player Y: %.2f", player.y);
    if (ImGui::Button("Jump")) {
        // Trigger your jump logic
    }
    ImGui::End();

	// ########################################## End ImGui window ######################################

    // Render with the updated position and flip state
    SDL_RenderTextureRotated(renderer, enemieTex, NULL, &enemie_sprite_position, 0, NULL, enemieflip);
    enemies_animation.animateEnemies(renderer, enemie_sprite_position, enemieflip);
    
    // ImGui Render
    ImGui::Render();

    //SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
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
	ImGui_ImplSDLRenderer3_Shutdown();
    SDL_free(wav_data); // sound
    GameTexture::CleanupTextures();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    SDL_free(appstate); // this just fees any memory we have used 
    /* SDL will clean up the window/renderer for us, and then we can go to bed */
 
}

