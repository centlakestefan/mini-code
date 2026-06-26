#pragma once

#include <vector>

#include "ai/tool_registry.h"

namespace minicode {

// Returns the tools registered for chat: the str_replace text editor and a
// file-search tool, both operating on the local filesystem relative to the
// current working directory.
std::vector<ToolSpec> builtin_tools();

} // namespace minicode
