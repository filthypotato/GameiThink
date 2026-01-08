#include "Renderer.hpp"
#include "GameState.hpp"
#include "Enemy.hpp"
#include <iostream>

// Screen Management

// Clear terminal screen using ANSI escape codes
// Works on Unix/Linux/Mac terminals
void clearScreen() {
    // ANSI escape sequence: ESC[2J clears screen, ESC[H moves cursor to home
    std::cout << "\033[2J\033[H" << std::flush;
}

// Map Rendering

// Render the complete game state to terminal
void printMap(const GameState& state) {
    // Clear previous frame for smooth animation
    clearScreen();

    // Create map array - 0 = empty, 1 = wall, 2 = player, 3 = enemy
    int map[GameState::MAP_ROWS][GameState::MAP_COLS]{};

    // Draw border walls around the entire map
    // Top and bottom walls
    for (int c = 0; c < GameState::MAP_COLS; ++c) {
        map[0][c] = 1;                              // Top wall
        map[GameState::MAP_ROWS - 1][c] = 1;       // Bottom wall
    }
    // Left and right walls
    for (int r = 0; r < GameState::MAP_ROWS; ++r) {
        map[r][0] = 1;                              // Left wall
        map[r][GameState::MAP_COLS - 1] = 1;       // Right wall
    }

    // Place player on map
    if (state.player.row >= 0 && state.player.row < GameState::MAP_ROWS &&
        state.player.col >= 0 && state.player.col < GameState::MAP_COLS) {
        map[state.player.row][state.player.col] = 2;
    }

    // Place enemy on map (only if alive)
    if (isEnemyAlive(state.enemy) &&
        state.enemy.row >= 0 && state.enemy.row < GameState::MAP_ROWS &&
        state.enemy.col >= 0 && state.enemy.col < GameState::MAP_COLS) {

        // If enemy and player are on same tile, show player (they're fighting)
        if (map[state.enemy.row][state.enemy.col] != 2) {
            map[state.enemy.row][state.enemy.col] = 3;
        }
    }

    // Render the map character by character
    for (int r = 0; r < GameState::MAP_ROWS; ++r) {
        for (int c = 0; c < GameState::MAP_COLS; ++c) {
            char tile;

            // Convert tile type to display character
            switch (map[r][c]) {
                case 0:  // Empty floor
                    tile = '.';
                    break;
                case 1:  // Wall
                    tile = '#';
                    break;
                case 2:  // Player
                    tile = '@';
                    break;
                case 3:  // Enemy
                    tile = 'E';
                    break;
                default: // Unknown (shouldn't happen)
                    tile = '?';
                    break;
            }

            std::cout << tile;
        }
        std::cout << '\n';  // New line after each row
    }

    // Display player statistics below the map
    std::cout << "\n========================================\n";
    std::cout << "Level: " << state.player.level
              << " | Health: " << state.player.health
              << "/" << state.player.maxHealth
              << " | Attack: " << state.player.attack << "\n";
    std::cout << "XP: " << state.player.experience
              << "/" << (100 * state.player.level)
              << " | Enemies Defeated: " << state.enemiesDefeated << "\n";
    std::cout << "========================================\n";

    // Display enemy status if alive
    if (isEnemyAlive(state.enemy)) {
        std::cout << "Enemy Health: " << state.enemy.health
                  << "/" << state.enemy.maxHealth << "\n";
    }

    // Display controls
    std::cout << "\nControls: W/A/S/D to move, Q to quit\n";
}

// Game Over Screen

// Display game over message with final statistics
void displayGameOver(const GameState& state) {
    clearScreen();

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "           GAME OVER!                   \n";
    std::cout << "========================================\n";
    std::cout << "\n";
    std::cout << "Final Statistics:\n";
    std::cout << "  Level Reached: " << state.player.level << "\n";
    std::cout << "  Enemies Defeated: " << state.enemiesDefeated << "\n";
    std::cout << "  Final Attack: " << state.player.attack << "\n";
    std::cout << "\n";
    std::cout << "Thank you for playing!\n";
    std::cout << "========================================\n";
}

// Display victory message when enemy is defeated
void displayVictory() {
    std::cout << "\n";
    std::cout << "**************************************\n";
    std::cout << "          VICTORY!                    \n";
    std::cout << "     The enemy has been defeated!     \n";
    std::cout << "**************************************\n";
    std::cout << "\n";
}
