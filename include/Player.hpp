#pragma once

struct Player;
struct Enemy;

// Player movement functions
void movePlayer(Player& player, char direction);

// Combat functions
void attackEnemy(Player& player, Enemy& enemy);
bool isPlayerAlive(const Player& player);

// TODO: Add function to level up player (increase health/attack)
// TODO: Add function to heal player
