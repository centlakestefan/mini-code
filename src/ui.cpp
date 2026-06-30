// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 Centlake Software AB

#include "tapto/ui.h"

#include <iostream>
#include <string>
#include <vector>

namespace tapto::ui {

// ---------------------------------------------------------------------------
// Internal state
// ---------------------------------------------------------------------------

namespace {

// True while a status line is visible on the terminal (cursor is on that row).
// Used to decide whether an erase sequence must be emitted before any
// permanent output, and to guard the cursor-hide/show escape pair.
bool g_status_visible = false;

// The last string written to the status line, kept so commit_status() can
// re-emit it as a permanent scroll line without the caller having to repeat it.
std::string g_status_text;

// Erase the current status line content without moving to a new line, so
// the next write starts at column 0 on the same row.
void erase_status_line() {
    std::cout << "\r\x1b[K";
}

// Build the prefix that goes before the action text on the status line.
// Returns "" for the initial thinking phase (iteration == 0).
std::string iteration_prefix(int iteration, int max_iterations) {
    if (iteration <= 0) return "";
    return "[" + std::to_string(iteration) + "/" + std::to_string(max_iterations) + "] ";
}

} // namespace

// ---------------------------------------------------------------------------
// Progress / status line
// ---------------------------------------------------------------------------

void set_status(const std::string& text, int iteration, int max_iterations) {
    constexpr size_t kMaxWidth = 78;

    g_status_text = iteration_prefix(iteration, max_iterations) + text;
    if (g_status_text.size() > kMaxWidth)
        g_status_text = g_status_text.substr(0, kMaxWidth - 3) + "...";

    if (!g_status_visible)
        std::cout << "\x1b[?25l"; // hide cursor while the status line is live

    std::cout << "\r" << g_status_text << "\x1b[K" << std::flush;
    g_status_visible = true;
}

void commit_status() {
    if (!g_status_visible) return;
    // Print the saved line as a permanent scroll entry, then move to a fresh line.
    std::cout << "\r" << g_status_text << "\x1b[K\n" << std::flush;
    g_status_visible = false;
    g_status_text.clear();
}

void end_status() {
    if (!g_status_visible) return;
    std::cout << "\r\x1b[K\x1b[?25h" << std::flush; // erase line + restore cursor
    g_status_visible = false;
    g_status_text.clear();
}

// ---------------------------------------------------------------------------
// Permanent output
// ---------------------------------------------------------------------------

void emit_intermediate(const std::string& text, bool is_reasoning, bool print_cot) {
    if (text.empty() || !print_cot) return;
    if (g_status_visible) erase_status_line();
    if (is_reasoning)
        std::cout << "\x1b[90m" << text << "\x1b[0m\n" << std::flush; // dimmed
    else
        std::cout << text << "\n" << std::flush;
}

void print_reply(const std::string& text) {
    std::cout << text << "\n";
}

void print_line(const std::string& text) {
    std::cout << text << "\n";
}

void print_error(const std::string& text) {
    std::cerr << "error: " << text << "\n";
}

void print_warning(const std::string& text) {
    std::cerr << "warning: " << text << "\n";
}

void print_usage(const std::string& text) {
    std::cerr << text;
}

// ---------------------------------------------------------------------------
// Chat session header
// ---------------------------------------------------------------------------

void print_chat_header(const std::string& provider,
                       const std::string& model,
                       const std::vector<std::string>& tool_names) {
    std::cout << "tapto-code chat - provider: " << provider
              << ", model: " << model << "\n"
              << "Tools: ";
    for (size_t i = 0; i < tool_names.size(); ++i) {
        std::cout << (i ? ", " : "") << tool_names[i];
    }
    std::cout << "\n";
}

void print_chat_hints() {
    std::cout << "Slash commands: /clear, /list-commands, "
                 "/add-command <name> <command...>, /exit\n";
}

// ---------------------------------------------------------------------------
// First-run / interactive prompts
// ---------------------------------------------------------------------------

void print_setup_welcome() {
    std::cout << "Welcome to tapto-code. Let's set up your AI provider.\n";
}

void print_setup_provider_prompt() {
    std::cout << "Provider (claude / openai / gemini): " << std::flush;
}

void print_setup_apikey_prompt(const char* env_var_name) {
    std::cout << "API key";
    if (env_var_name && *env_var_name)
        std::cout << " (or leave blank to use $" << env_var_name << ")";
    std::cout << ": " << std::flush;
}

void print_setup_saved(const std::string& path) {
    std::cout << "Saved to " << path << "\n\n";
}

void print_prompt(const std::string& text) {
    std::cout << text << std::flush;
}

// ---------------------------------------------------------------------------
// Config / command listing
// ---------------------------------------------------------------------------

void print_config_entry(const std::string& scope,
                        const std::string& key,
                        const std::string& value) {
    if (!scope.empty()) std::cout << scope << "\t";
    std::cout << key << "=" << value << "\n";
}

void print_command_entry(const std::string& scope,
                         const std::string& name,
                         const std::string& command) {
    if (!scope.empty()) std::cout << scope << "\t";
    std::cout << name << " = " << command << "\n";
}

void print_command_added(const std::string& name,
                         const std::string& scope,
                         const std::string& command) {
    std::cout << "Added '" << name << "' (" << scope << "): " << command << "\n";
}

void print_command_removed(const std::string& name, const std::string& scope) {
    std::cout << "Removed '" << name << "' from " << scope << "\n";
}

void print_no_commands() {
    std::cout << "(no commands configured)\n";
}

} // namespace tapto::ui
