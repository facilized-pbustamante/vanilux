#include "launcher_window.hpp"
#include <gtkmm/application.h>
#include <giomm/application.h>
#include <fstream>
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>
#include <glib-unix.h>

#ifndef VLX_VERSION
#define VLX_VERSION "0.1"
#endif

static const char* PID_FILE = "/tmp/vanilux.pid";

static pid_t read_pid() {
    std::ifstream f(PID_FILE);
    if (!f) return 0;
    pid_t pid = 0;
    f >> pid;
    return pid;
}

static LauncherWindow* g_window = nullptr;

// SIGUSR1 => toggle the resident window (another invocation asked us to).
static gboolean on_toggle_signal(gpointer) {
    if (g_window) g_window->toggle_visibility();
    return G_SOURCE_CONTINUE;
}

// SIGTERM => quit cleanly so the PID file is removed.
static gboolean on_term_signal(gpointer data) {
    auto* app = static_cast<Gtk::Application*>(data);
    if (app) app->quit();
    return G_SOURCE_REMOVE;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--version" || arg == "-v") {
            std::cout << "Vanilux " << VLX_VERSION << std::endl;
            return 0;
        }
    }

    pid_t existing = read_pid();
    if (existing > 0 && kill(existing, 0) == 0) {
        // A resident instance is already running -> tell it to toggle, then exit.
        kill(existing, SIGUSR1);
        return 0;
    }

    std::ofstream(PID_FILE) << getpid();

    auto app = Gtk::Application::create(argc, argv, "org.vanilux.launcher",
                                        Gio::APPLICATION_NON_UNIQUE);

    // Keep the application alive even while the window is hidden (resident).
    app->hold();

    LauncherWindow window;
    g_window = &window;

    g_unix_signal_add(SIGUSR1, on_toggle_signal, nullptr);
    g_unix_signal_add(SIGTERM, on_term_signal, app.get());

    int result = app->run(window);

    unlink(PID_FILE);
    return result;
}
