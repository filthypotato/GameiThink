#pragma once

// Enemy.hpp
// Enemy-related functions: spawning, AI, collision detection

// Forward declarations
struct Enemy;
struct Player;
struct GameState;

// Enemy Management Functions

// Spawn a new enemy at a random location away from the player
// Parameters:
//   - enemy: Reference to enemy to respawn
//   - player: Player reference to avoid spawning on top of them
// Side effects: Sets enemy position, resets health, marks as alive
void spawnEnemy(Enemy& enemy, const Player& player);

// Check if enemy is alive
// Returns: true if enemy health > 0 and isAlive flag is true
bool isEnemyAlive(const Enemy& enemy);

// Collision Detection

// Check if player and enemy are on the same tile
// Parameters:
//   - player: Player to check
//   - enemy: Enemy to check
// Returns: true if both entities occupy the same position
bool checkCollision(const Player& player, const Enemy& enemy);

// Enemy AI (Future Enhancement)

// Simple AI: Move enemy toward player
// Parameters:
//   - enemy: Enemy to move
//   - player: Player to move toward
// Note: Currently unimplemented - enemy stays stationary
void updateEnemyAI(Enemy& enemy, const Player& player);
