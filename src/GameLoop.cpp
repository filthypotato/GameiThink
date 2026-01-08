#include "GameLoop.hpp"
#include "GameState.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Renderer.hpp"

#include <chrono>
#include <thread>

// Game Loop Implementation
// Main game loop with smooth, persistent movement
void runGame(GameState& state) {
    // Initialize raw input handler (automatically restores terminal on exit)
    RawInput input;

    // Timing system for consistent movement speed
    using clock = std::chrono::steady_clock;
    auto lastMove = clock::now();

    // Movement delay: controls how fast player moves (80ms = ~12 moves/second)
    // Lower = faster movement, Higher = slower movement
    const auto moveDelay = std::chrono::milliseconds(150);

    // Game loop state
    bool running = true;
    char lastDir = 0;  // Stores last pressed direction for persistent movement

    // Main game loop - runs every frame
    while (running && isPlayerAlive(state.player)) {
        // INPUT PHASE: Check for keyboard input
        int k = input.pollKey();  // Non-blocking input check

        if (k != -1) {
            // A key was pressed this frame
            char ch = static_cast<char>(k);

            // Check for quit command
            if (ch == 'q' || ch == 'Q') {
                running = false;
                continue;
            }

            // Check for movement keys (WASD)
            if (ch == 'w' || ch == 'W' ||
                ch == 'a' || ch == 'A' ||
                ch == 's' || ch == 'S' ||
                ch == 'd' || ch == 'D') {

                // Store direction for persistent movement
                // Convert to lowercase for consistency
                lastDir = (ch >= 'A' && ch <= 'Z') ? (ch + 32) : ch;
            }
        }

        // MOVEMENT PHASE: Apply persistent movement with rate limiting
        auto now = clock::now();

        // Only move if:
        //   1. A direction is held (lastDir != 0)
        //   2. Enough time has passed since last move (rate limiting)
        if (lastDir && (now - lastMove >= moveDelay)) {
            movePlayer(state.player, lastDir);
            lastMove = now;  // Reset movement timer
        }

        // UPDATE PHASE: Update game logic
        updateGame(state);

        // RENDER PHASE: Draw current frame
        printMap(state);

        // FRAME RATE: Small sleep to prevent excessive CPU usage
        // Sleep for 8ms (~120 FPS) - balances responsiveness and CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

// Game Logic Updates

// Update game state each frame
void updateGame(GameState& state) {
    // Check if player and enemy are on the same tile
    if (checkCollision(state.player, state.enemy)) {
        // Collision detected - trigger combat
        attackEnemy(state.player, state.enemy);

        // Check if enemy was defeated
        if (!isEnemyAlive(state.enemy)) {
            // Enemy defeated - show victory and respawn
            displayVictory();
            state.enemiesDefeated++;

            // Wait a moment so player can see victory message
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));

            // Spawn new enemy
            spawnEnemy(state.enemy, state.player);
        }
    }


    //   - Update enemy AI
    //   - Check for pickups/items
    //   - Update timers or cooldowns
    //   - Spawn additional enemies
}
