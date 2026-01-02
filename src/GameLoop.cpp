#include "GameLoop.hpp"

#include "Input.hpp"
#include "Player.hpp"
#include "Renderer.hpp"

#include <chrono>
#include <thread>

void runGame(GameState& state) {
    RawInput input;

    using clock = std::chrono::steady_clock;
    auto lastMove = clock::now();
    const auto moveDelay = std::chrono::milliseconds(80);

    bool running = true;
    char lastDir = 0;

    while (running && isPlayerAlive(state.player)) {
        int k = input.pollKey();
        if (k != -1) {
            char ch = static_cast<char>(k);

            if (ch == 'q') running = false;

            if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') {
                lastDir = ch;
            }
        }

        auto now = clock::now();
        if (lastDir && (now - lastMove >= moveDelay)) {
            movePlayer(state.player, lastDir);
            lastMove = now;
        }

        updateGame(state);
        printMap(state);

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

