#include <iostream>
#include "Renderer.hpp"

constexpr size_t MAP_ROW = 20;
constexpr size_t MAP_COL = 40;

void printMap(const GameState& state) {
    std::cout << "RUNNING: " << __FILE__ << "\n\n";

    // Create map array
    int map[MAP_ROW][MAP_COL]{};

    // Draw borders
    for (size_t r = 0; r < MAP_ROW; ++r) {
        map[r][0] = 1;
        map[r][MAP_COL - 1] = 1;
    }
    for (size_t c = 0; c < MAP_COL; ++c) {
        map[0][c] = 1;
        map[MAP_ROW - 1][c] = 1;
    }

    // Place player
    map[state.player.row][state.player.col] = 2;
    
    // Place enemy
    map[state.enemy.row][state.enemy.col] = 3;

    // Render map
    for (size_t r = 0; r < MAP_ROW; ++r) {
        for (size_t c = 0; c < MAP_COL; ++c) {
            char tile = '?';
            switch (map[r][c]) {
                case 0: tile = '.'; break;  // empty floor
                case 1: tile = '#'; break;  // wall
                case 2: tile = '@'; break;  // player
                case 3: tile = 'E'; break;  // enemy
                default: tile = '?'; break;
            }
            std::cout << tile;
        }
        std::cout << '\n';
    }
    
    // Display player stats
    std::cout << "\nPlayer Health: " << state.player.health 
              << " | Attack: " << state.player.attack << '\n';
}
