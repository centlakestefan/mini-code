#pragma once

#include <iostream>
#include <mutex>
#include <string>

// Minimal replacement for the original project's logfile. Off by default so the
// CLI stays quiet; flip on with Log::setEnabled(true) for debugging. Writes to
// stderr to keep stdout clean for command output.
class Log {
public:
    static Log& instance() {
        static Log log;
        return log;
    }

    void write(const std::string& message) {
        if (!s_enabled) return;
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cerr << message;
    }

    static void setEnabled(bool enabled) { s_enabled = enabled; }

private:
    static inline bool s_enabled = false;
    static inline std::mutex s_mutex;
};
