#include "../src/app_discovery.hpp"
#include <cassert>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void setup_mock_desktop_files() {
    // Explicitly clean up stale test state first
    fs::remove_all("mock_apps_priority");
    fs::remove_all("mock_apps_fallback");
    
    fs::create_directories("mock_apps_priority");
    fs::create_directories("mock_apps_fallback");
    
    std::ofstream app1("mock_apps_priority/app1.desktop");
    app1 << "[Desktop Entry]\n"
         << "Name = Tést Application Öne\n"
         << "Exec = test-app-one \"%F\" %u --verbose --space \"  \"\n"
         << "Icon = test-icon-one\n"
         << "NoDisplay = false\n"
         << "\n"
         << "[Desktop Action Gallery]\n"
         << "Name=Show Gallery\n";
    app1.close();
    
    std::ofstream app4("mock_apps_fallback/app4.desktop");
    app4 << "[Desktop Entry]\n"
         << "Name=tést application öne\n"
         << "Exec=test-app-dup\n"
         << "Icon=test-icon-dup\n";
    app4.close();
    
    std::ofstream app2("mock_apps_priority/app2.desktop");
    app2 << "[Desktop Entry]\n"
         << "Name=Hidden Application\n"
         << "Exec=hidden-exec\n"
         << "NoDisplay=true\n";
    app2.close();
    
    std::ofstream app3("mock_apps_priority/app3.desktop");
    app3 << "[Desktop Entry]\n"
         << "Name=Another Hidden\n"
         << "Exec=another-exec\n"
         << "Hidden=true\n";
    app3.close();
    
    std::ofstream app5("mock_apps_priority/app5.desktop");
    app5 << "[Desktop Entry]\n"
         << "Name=Format App\n"
         << "Exec=date +\"%Y-%m-%d\"\n";
    app5.close();
    
    std::ofstream app6("mock_apps_priority/app6.desktop");
    app6 << "[Desktop Entry]\n"
         << "Name=Missing App\n"
         << "Exec=missing-app\n"
         << "TryExec=non-existent-binary-xyz\n";
    app6.close();
    
    std::ofstream app7("mock_apps_priority/app7.desktop");
    app7 << "[Desktop Entry]\n"
         << "Name=System Shell App\n"
         << "Exec=sh\n"
         << "TryExec=sh\n";
    app7.close();
}

void cleanup_mock_desktop_files() {
    fs::remove_all("mock_apps_priority");
    fs::remove_all("mock_apps_fallback");
}

int main() {
    setup_mock_desktop_files();
    
    std::vector<std::string> dirs = {"mock_apps_priority", "mock_apps_fallback"};
    auto apps = AppDiscovery::scan_applications(dirs);
    
    assert(apps.size() == 3);
    
    assert(apps[0].name == "Format App");
    assert(apps[0].exec == "date +\"%Y-%m-%d\"");
    
    assert(apps[1].name == "System Shell App");
    assert(apps[1].exec == "sh");
    
    assert(apps[2].name == "Tést Application Öne");
    assert(apps[2].exec == "test-app-one --verbose --space \"  \"");
    assert(apps[2].icon == "test-icon-one");
    
    cleanup_mock_desktop_files();
    std::cout << "All Discovery Unit Tests Passed Successfully!" << std::endl;
    return 0;
}
