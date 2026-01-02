#pragma once

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Puts terminal into non-canonical, no-echo, non-blocking mode.
// Restores everything automatically when the object goes out of scope.
class RawInput {
public:
    RawInput() {
        // Save current terminal settings
        tcgetattr(STDIN_FILENO, &old_);

        // Make a modified copy
        termios t = old_;
        t.c_lflag &= ~(ICANON | ECHO); // no line buffering, no echo
        t.c_cc[VMIN]  = 0;            // return immediately
        t.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);

        // Make stdin non-blocking
        oldFlags_ = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldFlags_ | O_NONBLOCK);
    }

    ~RawInput() {
        // Restore terminal + flags
        tcsetattr(STDIN_FILENO, TCSANOW, &old_);
        fcntl(STDIN_FILENO, F_SETFL, oldFlags_);
    }

    // Returns -1 if no key is available, otherwise returns the char code.
    int pollKey() const {
        unsigned char ch;
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        return (n == 1) ? ch : -1;
    }

    RawInput(const RawInput&) = delete;
    RawInput& operator=(const RawInput&) = delete;

private:
    termios old_{};
    int oldFlags_{};
};

