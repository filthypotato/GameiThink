#include "GameState.hpp"
#include "Player.hpp"
#include <iostream>

void processInput(GameState& state) {
    char input;
    std::cout << "\nEnter move (w/a/s/d) or 'q' to quit: ";
    std::cin >> input;
    
    if (input == 'q') {
        // TODO: Handle quit game
    } else {
        movePlayer(state.player, input);
    }
}

void updateGame(GameState& state) {
    // TODO: Check if player and enemy are on same tile
    // TODO: If so, trigger combat
    // TODO: Check if enemy is dead
}

// TODO: Implement checkCollision function
// TODO: Implement spawnEnemy function
