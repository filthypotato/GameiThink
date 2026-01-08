# Terminal Dungeon Game — Fixed WASD Movement

A small terminal-based dungeon game (C++) with persistent WASD movement and simple collision handling.

## Summary

This repository contains a minimal terminal dungeon game. Recent fixes re-enabled full WASD movement, added bounds checking so the player cannot walk through the map borders, and corrected a typo in the game loop header.

## Changes in this branch

- Fixed typo in `GameLoop.hpp`: `GameSate` → `GameState`
- Restored up/down movement in `Player.cpp`
- Added bounds checking to prevent walking through walls
- Movement is persistent (hold a key to keep moving) with an 80ms step delay (configurable)

## How it works

- Persistent movement: hold W/A/S/D to move continuously
- Movement step delay configured in `GameLoop.cpp` (default ~80ms)
- Press Q to quit
- `RawInput` handles non-blocking keyboard input

## Build

```bash
# Compile with g++ (requires C++17)
g++ -std=c++17 -Iinclude -o game src/*.cpp

# Or run the provided build script
./build.sh
```

Note: `build.sh` is a generic build helper included in the repository.

## Run

```bash
./game
```

## Controls

- W — Move up
- S — Move down
- A — Move left
- D — Move right
- Q — Quit the game

## TODO / Next features

- Collision detection between player and enemies
- Combat system when player meets an enemy
- Enemy AI / movement
- Level-up and progression system
- Healing and item mechanics
- Enemy spawning system

## Contributing

Pull requests welcome. If you'd like to contribute, open an issue first to discuss larger changes.

## License

This project does not currently include a license file. Add one if you intend to share or accept contributions under a specific license.
