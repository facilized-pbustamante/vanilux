#pragma once
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

struct AppEntry {
    std::string name;
    std::string exec;
    std::string icon;
    std::string categories;
    std::string comment;   // .desktop Comment= (fallback: GenericName=), used as list-mode subtitle
};

class AppDiscovery {
public:
    static std::vector<AppEntry> scan_applications(const std::vector<std::string>& dirs);
    static std::optional<AppEntry> parse_desktop_file(const std::filesystem::path& filepath);
    static bool is_executable_in_path(const std::string& binary);
};
