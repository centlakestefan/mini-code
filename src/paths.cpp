#include "minicode/paths.hpp"

#include <cstdlib>
#include <system_error>

namespace fs = std::filesystem;

namespace minicode {

namespace {

fs::path env_path(const char* name) {
    const char* value = std::getenv(name);
    return value ? fs::path(value) : fs::path();
}

fs::path system_store_path(const std::string& filename) {
#ifdef _WIN32
    fs::path base = env_path("PROGRAMDATA");
    if (base.empty()) base = "C:\\ProgramData";
    return base / "minicode" / filename;
#else
    return fs::path("/etc/minicode") / filename;
#endif
}

fs::path global_store_path(const std::string& filename) {
#ifdef _WIN32
    fs::path home = env_path("USERPROFILE");
    if (home.empty()) home = env_path("HOMEDRIVE") / env_path("HOMEPATH");
#else
    fs::path home = env_path("HOME");
#endif
    return home / ".minicode" / filename;
}

fs::path local_write_path(const std::string& filename) {
    std::error_code ec;
    fs::path dir = fs::current_path(ec);
    if (ec) dir = ".";
    return dir / ".minicode" / filename;
}

// Walk up from the current directory looking for an existing ".minicode/<file>".
fs::path local_read_path(const std::string& filename) {
    std::error_code ec;
    fs::path dir = fs::current_path(ec);
    if (ec) return fs::path(".minicode") / filename;

    for (;;) {
        fs::path candidate = dir / ".minicode" / filename;
        if (fs::exists(candidate, ec)) return candidate;
        fs::path parent = dir.parent_path();
        if (parent == dir) break; // reached filesystem root
        dir = parent;
    }
    return local_write_path(filename);
}

// Resolve the path to a named store file ("config", "commands", ...) for a scope.
fs::path store_path(Level level, const std::string& filename, bool for_writing) {
    switch (level) {
        case Level::System: return system_store_path(filename);
        case Level::Global: return global_store_path(filename);
        case Level::Local:  return for_writing ? local_write_path(filename)
                                               : local_read_path(filename);
    }
    return fs::path();
}

} // namespace

const char* level_name(Level level) {
    switch (level) {
        case Level::System: return "system";
        case Level::Global: return "global";
        case Level::Local:  return "local";
    }
    return "unknown";
}

fs::path config_path(Level level, bool for_writing) {
    return store_path(level, "config", for_writing);
}

fs::path commands_path(Level level, bool for_writing) {
    return store_path(level, "commands", for_writing);
}

} // namespace minicode
