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

fs::path local_path(const std::string& filename) {
    std::error_code ec;
    fs::path dir = fs::current_path(ec);
    if (ec) dir = ".";
    return dir / ".minicode" / filename;
}

// Resolve the path to a named store file ("config", "commands", ...) for a scope.
// Local is always the current directory's ".minicode/<file>" (no upward search).
fs::path store_path(Level level, const std::string& filename) {
    switch (level) {
        case Level::System: return system_store_path(filename);
        case Level::Global: return global_store_path(filename);
        case Level::Local:  return local_path(filename);
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

fs::path config_path(Level level) {
    return store_path(level, "config");
}

fs::path commands_path(Level level) {
    return store_path(level, "commands");
}

} // namespace minicode
