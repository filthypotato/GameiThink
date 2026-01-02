#include "Renderer.hpp"
#include "GameState.hpp"
#include "GameLoop.hpp"

int main() {
    GameState state{100, 2}; // player health and attack
    
    runGame(state);
    // Initial render
    printMap(state);
   
    
    // TODO: Display game over message

    return 0;
}
