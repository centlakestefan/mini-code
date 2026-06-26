#include "minicode/config.hpp"

#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace minicode {

namespace {

std::string trim(const std::string& s) {
    static const char* ws = " \t\r\n";
    size_t begin = s.find_first_not_of(ws);
    if (begin == std::string::npos) return std::string();
    size_t end = s.find_last_not_of(ws);
    return s.substr(begin, end - begin + 1);
}

} // namespace

Config Config::load(const fs::path& path) {
    Config cfg;
    std::ifstream in(path);
    if (!in) return cfg; // missing file -> empty config

    std::string line;
    while (std::getline(in, line)) {
        std::string text = trim(line);
        if (text.empty() || text[0] == '#' || text[0] == ';') continue;

        size_t eq = text.find('=');
        if (eq == std::string::npos) continue; // ignore malformed lines

        std::string key = trim(text.substr(0, eq));
        std::string value = trim(text.substr(eq + 1));
        if (!key.empty()) cfg.set(key, value);
    }
    return cfg;
}

std::optional<std::string> Config::get(const std::string& key) const {
    for (const auto& entry : entries_) {
        if (entry.first == key) return entry.second;
    }
    return std::nullopt;
}

void Config::set(const std::string& key, const std::string& value) {
    for (auto& entry : entries_) {
        if (entry.first == key) {
            entry.second = value;
            return;
        }
    }
    entries_.emplace_back(key, value);
}

bool Config::unset(const std::string& key) {
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if (it->first == key) {
            entries_.erase(it);
            return true;
        }
    }
    return false;
}

void Config::save(const fs::path& path) const {
    std::error_code ec;
    if (path.has_parent_path()) {
        fs::create_directories(path.parent_path(), ec);
    }

    std::ofstream out(path, std::ios::trunc);
    if (!out) {
        throw std::runtime_error("cannot write config file: " + path.string());
    }

    out << "# mini-code\n";
    for (const auto& entry : entries_) {
        out << entry.first << " = " << entry.second << "\n";
    }
}

} // namespace minicode
