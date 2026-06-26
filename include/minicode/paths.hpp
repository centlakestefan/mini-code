#pragma once

#include <filesystem>

namespace minicode {

// Config scopes, ordered lowest to highest precedence.
enum class Level { System, Global, Local };

// Human-readable name ("system" / "global" / "local").
const char* level_name(Level level);

// Resolve the config file path for a scope.
//
// For System and Global the path is fixed per-user / per-machine.
// For Local:
//   - for_writing == false: the nearest ".minicode/config" found by walking
//     up from the current directory; falls back to "./.minicode/config".
//   - for_writing == true:  always "./.minicode/config" in the current dir.
std::filesystem::path config_path(Level level, bool for_writing);

// Same resolution as config_path, but for the allow-listed commands store
// (".minicode/commands").
std::filesystem::path commands_path(Level level, bool for_writing);

} // namespace minicode
