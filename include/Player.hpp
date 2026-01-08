#pragma once

// ============================================================================
// Player.hpp
// Player-related functions: movement, combat, leveling, healing
// ============================================================================

// Forward declarations to avoid circular includes
struct Player;
struct Enemy;
struct GameState;

// ----------------------------------------------------------------------------
// Movement Functions
// ----------------------------------------------------------------------------

// Move the player in the specified direction
// Parameters:
//   - player: Reference to player to move
//   - direction: 'w' (up), 'a' (left), 's' (down), 'd' (right)
// Includes boundary checking to prevent walking through walls
void movePlayer(Player& player, char direction);

// Check if the player can move to a specific position
// Parameters:
//   - row, col: Target position to check
// Returns: true if position is within valid game bounds
bool canMoveTo(int row, int col);

// ----------------------------------------------------------------------------
// Combat Functions
// ----------------------------------------------------------------------------

// Player attacks an enemy, dealing damage
// Parameters:
//   - player: The attacking player
//   - enemy: The enemy being attacked
// Side effects: Reduces enemy health, may trigger enemy death
void attackEnemy(Player& player, Enemy& enemy);

// Check if player is alive
// Returns: true if player health > 0
bool isPlayerAlive(const Player& player);

// Handle enemy attacking the player (counter-attack)
// Parameters:
//   - enemy: The attacking enemy
//   - player: The player being attacked
// Side effects: Reduces player health
void enemyAttacksPlayer(const Enemy& enemy, Player& player);

// ----------------------------------------------------------------------------
// Progression Functions
// ----------------------------------------------------------------------------

// Increase player's level and improve stats
// Called when player gains enough experience
// Side effects: Increments level, increases max health and attack
void levelUpPlayer(Player& player);

// Restore some or all of the player's health
// Parameters:
//   - player: Player to heal
//   - amount: Health points to restore (capped at maxHealth)
void healPlayer(Player& player, int amount);

// Grant experience points to the player
// Parameters:
//   - player: Player receiving experience
//   - amount: Experience points to grant
// Side effects: May trigger level up if threshold reached
void grantExperience(Player& player, int amount);

// Calculate experience needed for next level
// Returns: Experience points required (increases with level)
int experienceForNextLevel(const Player& player);
