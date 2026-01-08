#include "Enemy.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include <random>
#include <cmath>

// Enemy Management Implementation

// Generate a random number within a range
// Helper function for spawning enemies at random locations
static int randomInRange(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

// Calculate distance between two points (used to ensure enemies spawn away from player)
static double distance(int row1, int col1, int row2, int col2) {
    int dRow = row2 - row1;
    int dCol = col2 - col1;
    return std::sqrt(dRow * dRow + dCol * dCol);
}

// Spawn enemy at a random location, ensuring it's not too close to the player
void spawnEnemy(Enemy& enemy, const Player& player) {
    // Minimum distance from player when spawning (prevents unfair spawns)
    const double MIN_SPAWN_DISTANCE = 8.0;

    // Try to find a valid spawn position
    // Maximum attempts to prevent infinite loop
    const int MAX_ATTEMPTS = 100;
    int attempts = 0;

    int spawnRow, spawnCol;

    do {
        // Generate random position within playable area
        // Playable area: rows [1, MAP_ROWS-2], cols [1, MAP_COLS-2]
        spawnRow = randomInRange(1, GameState::MAP_ROWS - 2);
        spawnCol = randomInRange(1, GameState::MAP_COLS - 2);

        attempts++;

        // If we've tried too many times, just spawn anywhere valid
        if (attempts >= MAX_ATTEMPTS) {
            break;
        }

    } while (distance(spawnRow, spawnCol, player.row, player.col) < MIN_SPAWN_DISTANCE);

    // Set enemy position
    enemy.row = spawnRow;
    enemy.col = spawnCol;

    // Reset enemy state
    enemy.health = enemy.maxHealth;
    enemy.isAlive = true;
}

// Check if enemy is currently alive
bool isEnemyAlive(const Enemy& enemy) {
    return enemy.isAlive && enemy.health > 0;
}

// Collision Detection Implementation

// Check if player and enemy occupy the same tile
bool checkCollision(const Player& player, const Enemy& enemy) {
    // Only consider collision if enemy is alive
    if (!enemy.isAlive) {
        return false;
    }

    // Check if positions match
    return (player.row == enemy.row) && (player.col == enemy.col);
}

// Enemy AI Implementation (Placeholder)

// Simple AI to move enemy toward player
// Currently unimplemented - enemies are stationary
// Future enhancement: Add pathfinding or simple chase behavior
void updateEnemyAI(Enemy& enemy, const Player& player) {
    // TODO: Implement basic AI
    // Possible behaviors:
    //   - Move toward player (naive chase)
    //   - Random movement
    //   - Patrol behavior
    //   - Only move when player is within detection range

    // For now, enemies don't move
    (void)enemy;   // Suppress unused parameter warning
    (void)player;  // Suppress unused parameter warning
}
