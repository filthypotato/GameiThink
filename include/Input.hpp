#pragma once

// Input.hpp
// Non-blocking keyboard input for smooth, real-time gameplay
// Uses POSIX terminal control (Unix/Linux/Mac)

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// RawInput Class
// RAII wrapper for terminal raw mode configuration
// Automatically restores terminal settings when destroyed
//
// Usage:
//   {
//       RawInput input;
//       int key = input.pollKey();  // Returns immediately
//   }  // Terminal restored here
//
class RawInput {
public:
    // Constructor: Configure terminal for non-blocking, raw input
    // Effects:
    //   - Disables line buffering (characters available immediately)
    //   - Disables echo (keypresses don't print to screen)
    //   - Makes stdin non-blocking (pollKey returns -1 if no input)
    RawInput() {
        // Save current terminal settings so we can restore them later
        tcgetattr(STDIN_FILENO, &old_);

        // Create modified terminal settings
        termios t = old_;

        // Disable canonical mode (line buffering) and echo
        t.c_lflag &= ~(ICANON | ECHO);

        // Set read to return immediately (non-blocking)
        t.c_cc[VMIN]  = 0;  // Minimum characters to read = 0
        t.c_cc[VTIME] = 0;  // Timeout = 0 (return immediately)

        // Apply new settings immediately
        tcsetattr(STDIN_FILENO, TCSANOW, &t);

        // Make stdin file descriptor non-blocking
        oldFlags_ = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldFlags_ | O_NONBLOCK);
    }

    // Destructor: Restore original terminal settings
    // Called automatically when object goes out of scope
    ~RawInput() {
        // Restore original terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old_);

        // Restore original file descriptor flags
        fcntl(STDIN_FILENO, F_SETFL, oldFlags_);
    }

    // Poll for keyboard input without blocking
    // Returns:
    //   - ASCII value of pressed key if available
    //   - -1 if no key is currently pressed
    // Note: This function returns immediately, doesn't wait for input
    int pollKey() const {
        unsigned char ch;
        ssize_t n = read(STDIN_FILENO, &ch, 1);

        // If we read 1 byte, return it; otherwise return -1 (no input)
        return (n == 1) ? static_cast<int>(ch) : -1;
    }

    // Disable copying (terminal state is unique)
    RawInput(const RawInput&) = delete;
    RawInput& operator=(const RawInput&) = delete;

private:
    termios old_{};     // Original terminal settings
    int oldFlags_{};    // Original file descriptor flags
};
