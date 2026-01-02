#pragma once

// Represents the player character
struct Player {
    int health;
    int attack;
    int row;
    int col;

    Player(int h, int a, int r, int c) 
        : health{h}, attack{a}, row{r}, col{c} {}
};

// Represents an enemy
struct Enemy {
    int health;
    int attack;
    int row;
    int col;

    Enemy(int h, int a, int r, int c)
        : health{h}, attack{a}, row{r}, col{c} {}
};

// Main game state - holds all game objects
struct GameState {
    Player player;
    Enemy enemy;
    bool isGameRunning;

    GameState(int playerHealth, int playerAttack)
        : player{playerHealth, playerAttack, 17, 16},
          enemy{50, 1, 5, 30} {}
};

// Game loop functions
void updateGame(GameState& state);

// TODO: Add function to check collision between player and enemy
// TODO: Add function to spawn new enemies
