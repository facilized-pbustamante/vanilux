#include "app_discovery.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <cstdlib>
#include <glibmm/ustring.h>
#include <unistd.h>

namespace fs = std::filesystem;

bool AppDiscovery::is_executable_in_path(const std::string& binary) {
    if (binary.empty()) return false;
    
    // Check if absolute or relative path with slashes
    if (binary.find('/') != std::string::npos) {
        std::error_code ec;
        return fs::exists(binary, ec) && !fs::is_directory(binary, ec) && ::access(binary.c_str(), X_OK) == 0;
    }
    
    // Lookup in PATH
    const char* path_env = std::getenv("PATH");
    if (!path_env) return false;
    
    std::string path_str(path_env);
    std::stringstream ss(path_str);
    std::string dir;
    std::error_code ec;
    while (std::getline(ss, dir, ':')) {
        if (dir.empty()) continue;
        fs::path p = fs::path(dir) / binary;
        if (fs::exists(p, ec) && !fs::is_directory(p, ec) && ::access(p.c_str(), X_OK) == 0) {
            return true;
        }
    }
    return false;
}

std::optional<AppEntry> AppDiscovery::parse_desktop_file(const std::filesystem::path& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return std::nullopt;
    
    std::string line;
    bool is_desktop_entry = false;
    bool has_desktop_entry = false;
    bool nodisplay = false;
    bool hidden = false;
    std::string name, exec, icon, try_exec, categories, comment, generic_name;
    
    while (std::getline(file, line)) {
        // Safe trim leading/trailing
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        auto last_non_ws = line.find_last_not_of(" \t\r\n");
        if (last_non_ws != std::string::npos) {
            line.erase(last_non_ws + 1);
        } else {
            line.clear();
        }
        
        if (line.empty() || line[0] == '#') continue;
        
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            is_desktop_entry = (line == "[Desktop Entry]");
            if (is_desktop_entry) {
                has_desktop_entry = true;
            }
            continue;
        }
        
        if (is_desktop_entry) {
            auto eq = line.find('=');
            if (eq != std::string::npos) {
                std::string key = line.substr(0, eq);
                std::string val = line.substr(eq + 1);
                
                // Safe trim key
                key.erase(0, key.find_first_not_of(" \t"));
                auto last_key = key.find_last_not_of(" \t");
                if (last_key != std::string::npos) {
                    key.erase(last_key + 1);
                } else {
                    key.clear();
                }
                
                // Safe trim val
                val.erase(0, val.find_first_not_of(" \t"));
                auto last_val = val.find_last_not_of(" \t");
                if (last_val != std::string::npos) {
                    val.erase(last_val + 1);
                } else {
                    val.clear();
                }
                
                if (key == "Name") {
                    name = val;
                } else if (key == "Exec") {
                    static const std::regex field_code_regex(R"((?:^|\s)(?:%[fFuUdDnNiIckvm]|"%[fFuUdDnNiIckvm]"|'%[fFuUdDnNiIckvm]')(?=\s|$))");
                    std::string cleaned = std::regex_replace(val, field_code_regex, "");
                    
                    cleaned.erase(0, cleaned.find_first_not_of(" \t"));
                    auto last_clean = cleaned.find_last_not_of(" \t");
                    if (last_clean != std::string::npos) {
                        cleaned.erase(last_clean + 1);
                    } else {
                        cleaned.clear();
                    }
                    exec = cleaned;
                } else if (key == "Icon") {
                    icon = val;
                } else if (key == "NoDisplay" && val == "true") {
                    nodisplay = true;
                } else if (key == "Hidden" && val == "true") {
                    hidden = true;
                } else if (key == "TryExec") {
                    try_exec = val;
                } else if (key == "Categories") {
                    categories = val;
                } else if (key == "Comment") {
                    comment = val;
                } else if (key == "GenericName") {
                    generic_name = val;
                }
            }
        }
    }
    
    if (!has_desktop_entry || nodisplay || hidden || name.empty() || exec.empty()) {
        return std::nullopt;
    }
    
    if (!try_exec.empty() && !is_executable_in_path(try_exec)) {
        return std::nullopt;
    }
    
    AppEntry entry;
    entry.name = name;
    entry.exec = exec;
    entry.icon = icon.empty() ? "application-x-executable" : icon;
    entry.categories = categories;
    entry.comment = comment.empty() ? generic_name : comment;
    return entry;
}

std::vector<AppEntry> AppDiscovery::scan_applications(const std::vector<std::string>& dirs) {
    struct SortEntry {
        AppEntry app;
        std::string lower_name;
    };
    
    std::vector<SortEntry> sort_apps;
    std::unordered_set<std::string> seen_names;
    std::error_code ec;
    
    for (const auto& dir_path : dirs) {
        ec.clear();
        if (!fs::exists(dir_path, ec)) continue;
        
        fs::directory_iterator it(dir_path, ec);
        fs::directory_iterator end;
        while (it != end && !ec) {
            const auto& entry = *it;
            std::error_code status_ec;
            auto status = entry.status(status_ec);
            if (!status_ec && fs::is_regular_file(status) && entry.path().extension() == ".desktop") {
                auto app_opt = parse_desktop_file(entry.path());
                if (app_opt) {
                    // Standard Unicode case folding via casefold()
                    std::string lower_name = Glib::ustring(app_opt->name).casefold();
                    if (seen_names.insert(lower_name).second) {
                        sort_apps.push_back({*app_opt, lower_name});
                    }
                }
            }
            it.increment(ec);
        }
    }
    
    std::sort(sort_apps.begin(), sort_apps.end(), [](const SortEntry& a, const SortEntry& b) {
        return a.lower_name < b.lower_name;
    });
    
    std::vector<AppEntry> apps;
    apps.reserve(sort_apps.size());
    for (auto& entry : sort_apps) {
        apps.push_back(std::move(entry.app));
    }
    return apps;
}
