#pragma once

// GameState.hpp
// Defines all core game data structures and entities

// Player Structure
// Represents the player character with position, stats, and level progression
struct Player {
    // Stats
    int health;        // Current health points
    int maxHealth;     // Maximum health capacity
    int attack;        // Damage dealt per attack
    int level;         // Current level (affects stats)
    int experience;    // Experience points toward next level

    // Position on the game map
    int row;           // Vertical position (Y coordinate)
    int col;           // Horizontal position (X coordinate)

    // Constructor: Initialize a new player at starting position
    Player(int h, int a, int r, int c)
        : health{h}, maxHealth{h}, attack{a}, level{1}, experience{0},
          row{r}, col{c} {}
};

// Enemy Structure
// Represents hostile NPCs with position, stats, and alive status
struct Enemy {
    // Stats
    int health;        // Current health points
    int maxHealth;     // Maximum health capacity
    int attack;        // Damage dealt per attack

    // Position on the game map
    int row;           // Vertical position (Y coordinate)
    int col;           // Horizontal position (X coordinate)

    // State
    bool isAlive;      // Whether this enemy is currently active

    // Constructor: Initialize a new enemy at specified position
    Enemy(int h, int a, int r, int c)
        : health{h}, maxHealth{h}, attack{a},
          row{r}, col{c}, isAlive{true} {}
};

// GameState Structure
// Main container for all game state - this is the single source of truth
// for the entire game world
struct GameState {
    Player player;           // The player character
    Enemy enemy;             // The enemy (in future, could be a vector)
    bool isGameRunning;      // Whether the game loop should continue
    int enemiesDefeated;     // Score tracking

    // Map dimensions (const - these don't change during gameplay)
    static constexpr int MAP_ROWS = 20;
    static constexpr int MAP_COLS = 40;

    // Constructor: Initialize game state with starting values
    // Parameters: player starting health and attack damage
    GameState(int playerHealth, int playerAttack)
        : player{playerHealth, playerAttack, 17, 16},  // Start near bottom-center
          enemy{50, 1, 5, 30},                          // Spawn near top-right
          isGameRunning{true},
          enemiesDefeated{0} {}
};
