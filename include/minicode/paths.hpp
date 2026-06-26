#pragma once

#include <filesystem>

namespace minicode {

// Config scopes, ordered lowest to highest precedence.
enum class Level { System, Global, Local };

// Human-readable name ("system" / "global" / "local").
const char* level_name(Level level);

// Resolve the config file path for a scope.
//
// System and Global are fixed per-machine / per-user. Local is always
// "./.minicode/config" in the current directory — there is no upward search:
// the working directory is the sandbox, and (unlike git, which has `git init`
// to mark a root) mini-code has no project boundary to walk up to.
std::filesystem::path config_path(Level level);

// Same resolution as config_path, but for the allow-listed commands store
// (".minicode/commands").
std::filesystem::path commands_path(Level level);

} // namespace minicode
