#include "Player.hpp"
#include "GameState.hpp"
#include <iostream>

// Movement Implementation

// Check if a position is within valid map boundaries
// Map has walls at edges: row [1, 18] and col [1, 38] are playable
bool canMoveTo(int row, int col) {
    // Check against map boundaries (accounting for walls at edges)
    return row >= 1 && row < GameState::MAP_ROWS - 1 &&
           col >= 1 && col < GameState::MAP_COLS - 1;
}

// Move player based on directional input
// Uses WASD controls: W=up, A=left, S=down, D=right
void movePlayer(Player& player, char direction) {
    // Store current position in case we need to revert
    int oldRow = player.row;
    int oldCol = player.col;

    // Calculate new position based on direction
    // Note: Row 0 is top of screen, so moving "up" decreases row
    switch(direction) {
        case 'w': // Move up (decrease row)
            player.row--;
            break;
        case 's': // Move down (increase row)
            player.row++;
            break;
        case 'a': // Move left (decrease column)
            player.col--;
            break;
        case 'd': // Move right (increase column)
            player.col++;
            break;
        default:
            // Invalid direction - do nothing
            return;
    }

    // Validate the new position - if invalid, revert to old position
    if (!canMoveTo(player.row, player.col)) {
        player.row = oldRow;
        player.col = oldCol;
    }
}

// Combat Implementation

// Player initiates attack on an enemy
void attackEnemy(Player& player, Enemy& enemy) {
    // Only attack if enemy is alive
    if (!enemy.isAlive) {
        return;
    }

    // Display attack message
    std::cout << "\n[COMBAT] Player attacks enemy for "
              << player.attack << " damage!\n";

    // Apply damage to enemy
    enemy.health -= player.attack;

    // Check if enemy died from the attack
    if (enemy.health <= 0) {
        enemy.health = 0;
        enemy.isAlive = false;
        std::cout << "[COMBAT] Enemy defeated!\n";

        // Grant experience for the kill
        const int EXPERIENCE_REWARD = 25;
        grantExperience(player, EXPERIENCE_REWARD);

        return;  // Enemy is dead, no counter-attack
    }

    // Enemy survived - counter-attack the player
    enemyAttacksPlayer(enemy, player);
}

// Enemy counter-attacks the player
void enemyAttacksPlayer(const Enemy& enemy, Player& player) {
    // Only attack if enemy is alive
    if (!enemy.isAlive) {
        return;
    }

    // Display counter-attack message
    std::cout << "[COMBAT] Enemy attacks back for "
              << enemy.attack << " damage!\n";

    // Apply damage to player
    player.health -= enemy.attack;

    // Ensure health doesn't go below zero
    if (player.health < 0) {
        player.health = 0;
    }
}

// Check if player is still alive
bool isPlayerAlive(const Player& player) {
    return player.health > 0;
}

// Progression Implementation

// Calculate experience points needed for next level
// Uses exponential scaling: each level requires more XP
int experienceForNextLevel(const Player& player) {
    // Formula: 100 * level (so level 1->2 needs 100, 2->3 needs 200, etc.)
    return 100 * player.level;
}

// Grant experience points to player and check for level up
void grantExperience(Player& player, int amount) {
    player.experience += amount;
    std::cout << "[XP] Gained " << amount << " experience! ("
              << player.experience << "/"
              << experienceForNextLevel(player) << ")\n";

    // Check if player has enough XP to level up
    while (player.experience >= experienceForNextLevel(player)) {
        player.experience -= experienceForNextLevel(player);
        levelUpPlayer(player);
    }
}

// Level up the player, improving their stats
void levelUpPlayer(Player& player) {
    player.level++;

    // Stat increases per level
    const int HEALTH_INCREASE = 20;
    const int ATTACK_INCREASE = 1;

    // Increase max health and fully heal player
    player.maxHealth += HEALTH_INCREASE;
    player.health = player.maxHealth;

    // Increase attack power
    player.attack += ATTACK_INCREASE;

    // Notify player of level up
    std::cout << "\n*** LEVEL UP! ***\n";
    std::cout << "Level: " << player.level << "\n";
    std::cout << "Max Health: " << player.maxHealth << " (+" << HEALTH_INCREASE << ")\n";
    std::cout << "Attack: " << player.attack << " (+" << ATTACK_INCREASE << ")\n";
    std::cout << "****************\n\n";
}

// Restore player health by specified amount
void healPlayer(Player& player, int amount) {
    // Store old health to show how much was healed
    int oldHealth = player.health;

    // Add healing amount
    player.health += amount;

    // Cap at max health (can't overheal)
    if (player.health > player.maxHealth) {
        player.health = player.maxHealth;
    }

    // Calculate actual amount healed
    int actualHealing = player.health - oldHealth;

    // Display healing message
    if (actualHealing > 0) {
        std::cout << "[HEAL] Restored " << actualHealing << " health points!\n";
    }
}
