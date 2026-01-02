# Terminal Dungeon Game - Fixed WASD Movement

## Changes Made

### 1. Fixed `GameLoop.hpp`
- Corrected typo: `GameSate` → `GameState`

### 2. Fixed `Player.cpp`
- **Uncommented W/S movement** - You can now move up and down
- **Added bounds checking** - Player can't walk through walls (borders)
- Movement is now fully functional for all directions

## What Was Wrong

The persistent WASD movement system was already correctly implemented in `GameLoop.cpp`. The issue was that the actual movement logic in `Player.cpp` had the up/down cases commented out, so pressing W and S did nothing.

## How It Works

The game uses **persistent movement** (not turn-based):
- Hold W/A/S/D to continuously move in that direction
- Movement has an 80ms delay between steps (configurable in GameLoop.cpp)
- Press Q to quit
- The RawInput class handles non-blocking keyboard input

## Compilation

```bash
g++ -std=c++17 -o game main.cpp GameLoop.cpp GameState.cpp Player.cpp Renderer.cpp
```

## Running

```bash
./game
```

## Controls

- **W** - Move up
- **S** - Move down  
- **A** - Move left
- **D** - Move right
- **Q** - Quit game

## Next Steps (TODOs in code)

- Implement collision detection between player and enemy
- Add combat system when player meets enemy
- Add enemy AI/movement
- Implement level up system
- Add healing mechanics
- Add spawning new enemies
