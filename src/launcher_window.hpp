#pragma once
#include <gtkmm.h>
#include "app_discovery.hpp"
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>

class AppIconButton : public Gtk::EventBox {
public:
    // Callback types for the list-mode category-tagging popover.
    using IsTaggedCb = std::function<bool(const std::string& app, const std::string& cat_id)>;
    using SetTagCb   = std::function<void(const std::string& app, const std::string& cat_id, bool on)>;

    AppIconButton(const AppEntry& entry, bool is_favorite = false, bool list_mode = false,
                  std::function<void(const std::string&)> toggle_fav_cb = nullptr,
                  const std::string& detected_cat = "other",
                  IsTaggedCb is_tagged_cb = nullptr,
                  SetTagCb set_tag_cb = nullptr);
    const AppEntry& get_entry() const { return m_entry; }
    sigc::signal<void>& signal_clicked() { return m_signal_clicked; }
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
    void build_tags_popover();   // lazily builds the list-mode ⚙ category popover

    AppEntry m_entry;
    bool m_is_favorite;
    bool m_list_mode = false;
    std::string m_detected_cat;
    std::function<void(const std::string&)> m_toggle_fav_cb;
    IsTaggedCb m_is_tagged_cb;
    SetTagCb m_set_tag_cb;
    sigc::signal<void> m_signal_clicked;
    Gtk::Box m_box{Gtk::ORIENTATION_VERTICAL};
    Gtk::Overlay m_overlay;
    Gtk::Box m_icon_wrapper{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Image m_image;
    Gtk::Label m_label;
    Gtk::Button m_fav_toggle_btn;
    Gtk::Image m_fav_icon_img;

    // List-mode extras (unused in grid mode)
    Gtk::Box m_text_box{Gtk::ORIENTATION_VERTICAL};
    Gtk::Label m_sublabel;
    Gtk::Label m_cat_badge;
    // ⚙ is a window-less Gtk::Image (NOT a Button): clicks pass through to this
    // EventBox, which decides ⚙-vs-launch purely by pointer position — the same
    // reliable scheme the favorite star uses.
    Gtk::Image m_config_icon_img;
    Gtk::Popover m_tags_popover;
    bool m_popover_built = false;
};

class LauncherWindow : public Gtk::Window {
public:
    LauncherWindow();
    virtual ~LauncherWindow();

    // Resident-process control: show/hide instead of spawning/killing, so the
    // (one-time) icon-theme load is paid once and later opens are instant.
    void toggle_visibility();

protected:
    void show_launcher();
    void hide_launcher();
    bool on_key_press_event(GdkEventKey* event) override;
    bool on_button_release_event(GdkEventButton* event) override;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void load_css();
    void setup_ui();
    
    // Persistence and Core Logic
    void load_persisted_data();
    void save_persisted_data();
    void toggle_favorite(const std::string& name);
    void add_to_recents(const std::string& name);

    // Manual category tags: let the user classify (esp. custom) apps into the
    // existing categories from the list-mode ⚙ popover. Additive — never removes
    // an app from its auto-detected category, so nothing that already works breaks.
    bool app_has_tag(const std::string& name, const std::string& cat_id) const;
    void set_app_tag(const std::string& name, const std::string& cat_id, bool on);
    
    // Layout and UI builders
    void rebuild_all();
    void populate_grid(Gtk::Grid& grid, const std::vector<AppEntry>& list);
    void set_category(const std::string& cat_id);
    void update_status();
    void on_search_changed();
    void on_app_clicked(const AppEntry& entry);
    void toggle_view_mode(const std::string& mode);

    // Keyboard navigation helper
    bool handle_custom_navigation(unsigned int keyval);

    // Refresh, settings, theming
    void refresh_apps();
    void show_settings_dialog();
    void apply_theme(const std::string& hex_color);

    static std::string detect_category(const std::string& categories);

    static constexpr int COLS = 9; // Matching mockup columns
    // On the "Todas" home, the Favoritos/Recientes sections show at most this
    // many icons (newest first); older ones drop off the view.
    static constexpr int HOME_SECTION_MAX = 9;

    // Widgets
    Gtk::Box  m_outer_box{Gtk::ORIENTATION_VERTICAL};

    // Search Box
    Gtk::Frame m_search_frame;
    Gtk::Box m_search_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label m_search_prompt;
    Gtk::Entry m_search_entry;
    Gtk::Box m_view_toggle_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Button m_btn_grid;
    Gtk::Button m_btn_list;

    // Middle Pane
    Gtk::Box m_middle_box{Gtk::ORIENTATION_HORIZONTAL};

    // Sidebar
    Gtk::Frame m_sidebar_frame;
    Gtk::Box m_sidebar_box{Gtk::ORIENTATION_VERTICAL};

    // Main Apps Panel
    Gtk::Frame m_apps_frame;
    Gtk::ScrolledWindow m_apps_scrolled;
    Gtk::Box m_apps_scroll_content{Gtk::ORIENTATION_VERTICAL};

    // Favorites Grid
    Gtk::Box m_favorites_container{Gtk::ORIENTATION_VERTICAL};
    Gtk::Box m_favorites_header{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label m_favorites_label;
    Gtk::Separator m_favorites_sep{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Grid m_favorites_grid;

    // Recents Grid
    Gtk::Box m_recents_container{Gtk::ORIENTATION_VERTICAL};
    Gtk::Box m_recents_header{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label m_recents_label;
    Gtk::Separator m_recents_sep{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Grid m_recents_grid;

    // All Apps Grid
    Gtk::Box m_all_container{Gtk::ORIENTATION_VERTICAL};
    Gtk::Box m_all_header{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label m_all_label;
    Gtk::Separator m_all_sep{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Grid m_all_grid;

    // Status Panel
    Gtk::Frame m_status_frame;
    Gtk::Box m_status_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label m_ws_label;
    Gtk::Label m_app_count_label;
    Gtk::Label m_cat_name_label;
    Gtk::Box m_shortcuts_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Button m_btn_refresh;
    Gtk::Button m_btn_settings;

    // Data lists
    std::vector<AppEntry> m_all_apps;
    std::vector<AppEntry> m_apps; // Currently filtered / displayed in "All"
    std::vector<std::string> m_favorites;
    std::vector<std::string> m_recents;
    std::map<std::string, std::vector<std::string>> m_custom_tags; // app name -> manual category ids
    std::string m_current_category = "all";
    std::string m_view_mode = "grid";
    std::vector<std::pair<std::string, Gtk::Button*>> m_cat_buttons;

    Glib::RefPtr<Gtk::CssProvider> m_theme_provider;

    std::chrono::steady_clock::time_point m_created_at;
    bool m_close_inhibited = false;
};

