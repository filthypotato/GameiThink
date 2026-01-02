#include "Player.hpp"
#include "GameState.hpp"
#include <iostream>

void movePlayer(Player& player, char direction) {
    // Save old position in case we need to revert
    int oldRow = player.row;
    int oldCol = player.col;
    
    // Move based on direction
    switch(direction) {
        case 'w': // up
            player.row--;
            break;
        case 's': // down
            player.row++;
            break;
        case 'a': // left
            player.col--;
            break;
        case 'd': // right
            player.col++;
            break;
    }
    
    // Bounds checking - don't let player walk through walls
    // Map is 20 rows x 40 cols, with borders at edges
    if (player.row < 1 || player.row >= 19 || 
        player.col < 1 || player.col >= 39) {
        // Revert to old position if out of bounds
        player.row = oldRow;
        player.col = oldCol;
    }
}

void attackEnemy(Player& player, Enemy& enemy) {
    std::cout << "Player attacks enemy for " << player.attack << " damage!\n";
    enemy.health -= player.attack;
    
    // TODO: Check if enemy is dead, if not, enemy attacks back
}

bool isPlayerAlive(const Player& player) {
    return player.health > 0;
}

// TODO: Implement levelUp function
// TODO: Implement healPlayer function
