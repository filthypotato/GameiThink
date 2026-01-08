#pragma once

// GameLoop.hpp
// Main game loop and game logic update functions

// Forward declarations
struct GameState;

// Main Game Loop

// Run the main game loop with real-time input and rendering
// This is the core game loop that:
//   1. Polls for keyboard input
//   2. Updates game state
//   3. Renders the current frame
//   4. Repeats until game ends
//
// Parameters:
//   - state: Reference to game state (modified during gameplay)
//
// Loop exits when:
//   - Player presses 'q' to quit
//   - Player health reaches 0 (death)
void runGame(GameState& state);

// Game Logic Updates

// Update all game logic for current frame
// Handles:
//   - Collision detection between player and enemy
//   - Combat triggers
//   - Enemy respawning when defeated
//   - Any other per-frame game logic
//
// Parameters:
//   - state: Current game state to update
void updateGame(GameState& state);
