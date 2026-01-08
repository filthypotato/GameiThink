#pragma once

// Renderer.hpp
// Screen rendering and display functions

// Forward declarations
struct GameState;

// Display Functions

// Render the complete game map with all entities
// Displays:
//   - Map boundaries (walls)
//   - Player position
//   - Enemy position (if alive)
//   - Player stats (health, attack, level, etc.)
// Parameters:
//   - state: Current game state to render
void printMap(const GameState& state);

// Clear the terminal screen (platform-specific)
// Improves visual feedback by preventing screen scrolling
void clearScreen();

// Display the game over message
// Shows final stats and score
// Parameters:
//   - state: Final game state
void displayGameOver(const GameState& state);

// Display a victory message when enemy is defeated
void displayVictory();
