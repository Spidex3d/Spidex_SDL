#pragma once
#include <SDL3/SDL.h>
#include <glm.hpp>

#include "config.h"

//static SDL_Texture* playerTex;
std::string player_Animation_Type;

struct Player {
    float x, y;
    float width, height;
    bool isOnGround;
};
Player player = { 15, 500, 37, 87, false }; // Start position on top of the floor
//struct Position {
//	float x, y;
//};
//Position position = { 15, 500 }; // Start position on top of the floor
//SDL_FRect sprite_portion = { 0, 0, 27, 87 };

// Gravity and jump setup
glm::vec2 playerVelocity = { 0.0f, 0.0f };  // Current velocity (x, y)
const float jumpForce = -15.0f;           // Negative to apply upward velocity (adjust as needed)
const float forceForward = 10.0f;          // Forward force (for horizontal movement during jump)
const float gravity = 0.9f;               // Constant gravity acceleration
bool isOnGround = true;                   // Tracks if player is grounded

// Handle jump logic (call in your game loop)
void handleJump(bool Keyboard_State) {
    if (isOnGround && Keyboard_state[SDL_SCANCODE_SPACE]) {

        // Trigger jump
        playerVelocity.y = jumpForce;     // Apply upward velocity
        player.x += forceForward;      // Apply forward movement if moving
        isOnGround = false;              // Set player as airborne
        std::cout << "Jump initiated. Animation state: 4" << std::endl;
        // Set animation state
        appState.animation_State = 4; // Example (define appState elsewhere)
    }

    // Apply gravity while player is airborne
    if (!isOnGround) {
        playerVelocity.y += gravity;     // Apply downward force (gravity)
        player.y += playerVelocity.y;  // Update player's vertical position

        // Check if player has landed (ground collision logic)
        if (player.y >= 500) {         // Assuming 500 is the ground level
            player.y = 600;            // Reset to ground level
            playerVelocity.y = 0;        // Stop vertical movement
            isOnGround = true;           // Mark as grounded
            std::cout << "Player has landed." << std::endl;
        }
    }
}


