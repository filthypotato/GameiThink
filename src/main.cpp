#include "GameState.hpp"
#include "GameLoop.hpp"
#include "Renderer.hpp"
#include <iostream>

// ============================================================================
// main.cpp
// Entry point for the dungeon crawler game
// ============================================================================

int main() {
    // Display welcome message
    std::cout << "========================================\n";
    std::cout << "     DUNGEON CRAWLER v1.0               \n";
    std::cout << "========================================\n";
    std::cout << "\n";
    std::cout << "Controls:\n";
    std::cout << "  W/A/S/D - Move up/left/down/right\n";
    std::cout << "  Q - Quit game\n";
    std::cout << "\n";
    std::cout << "Objective:\n";
    std::cout << "  Defeat enemies to gain experience\n";
    std::cout << "  Level up to become stronger!\n";
    std::cout << "\n";
    std::cout << "Press any key to start...\n";
    std::cout << "========================================\n";

    // Wait for user to press a key
    std::cin.get();

    // GAME INITIALIZATION
    // Create game state with starting player stats
    // Parameters: player health, player attack damage
    GameState state{100, 2};

    // MAIN GAME LOOP
    // Run the game loop - this handles all gameplay until exit
    // Loop ends when player quits or dies
    runGame(state);

    // GAME OVER
    // Display final statistics and game over message
    displayGameOver(state);

    return 0;
}
