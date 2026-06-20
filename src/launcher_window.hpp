#pragma once
#include <gtkmm.h>
#include "app_discovery.hpp"
#include "i18n.hpp"
#include "keyboard_widget.hpp"
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <tuple>
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
    // Re-tint the favorite star (and list-mode ⚙) for the current theme version,
    // so live color-picker previews recolor them in step with everything else.
    void refresh_theme();
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
    void reload_control_icons();
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
    // Re-scan installed apps and rebuild only if the set actually changed. Run
    // (deferred) on every open so newly installed apps appear silently without
    // slowing the launcher's open or flickering the grid when nothing changed.
    void sync_apps();
    void build_settings_panel();   // builds the in-window modal once, lazily
    void open_settings();
    void close_settings();
    void update_combo_label();     // refresh the chosen-hotkey display in the modal

    // i18n: register a label for retranslation, switch language, re-apply strings.
    void register_tr(Gtk::Label* lbl, const std::string& key, bool markup = false);
    void retranslate();
    void apply_language(Lang l);
    bool set_theme_color(const std::string& hex_color);   // updates globals only
    void recolor_theme_live(const std::string& hex_color); // cheap live preview
    void apply_theme(const std::string& hex_color);        // full commit + persist

    static std::string detect_category(const std::string& categories);

    static constexpr int COLS = 9; // Matching mockup columns
    // On the "Todas" home, the Favoritos/Recientes sections show at most this
    // many icons (newest first); older ones drop off the view.
    static constexpr int HOME_SECTION_MAX = 9;

    // Widgets
    // Root overlay: the launcher content sits in the base layer; the settings
    // modal is an overlay child shown in-place (no separate top-level window).
    Gtk::Overlay m_root_overlay;
    Gtk::Box  m_outer_box{Gtk::ORIENTATION_VERTICAL};

    // In-window settings modal
    Gtk::EventBox m_settings_backdrop;                       // dimmed full-area scrim
    Gtk::EventBox m_settings_card_evt;                       // captures clicks on the card
    Gtk::Box      m_settings_card{Gtk::ORIENTATION_VERTICAL}; // centered panel content
    Gtk::ColorSelection* m_color_picker = nullptr;
    KeyboardWidget*      m_keyboard     = nullptr;
    Gtk::RadioButton*    m_mode_f       = nullptr;
    Gtk::RadioButton*    m_mode_super   = nullptr;
    Gtk::Label*          m_combo_value  = nullptr;
    std::vector<std::pair<Gtk::Button*, Lang>> m_lang_buttons;
    bool m_settings_built = false;
    bool m_settings_open  = false;
    std::string m_settings_orig_color;   // color when the modal opened (for cancel)

    // Live preview driven by the frame clock: the picker's change events just
    // stash the color and set a dirty flag; a per-frame tick callback (installed
    // only while the modal is open) does the actual recolor. Running it in the
    // frame clock's update phase means the CSS text, icons and stars all repaint
    // together in the SAME frame — and at most once per frame — so the preview is
    // both perfectly in step and cheap, even during the picker's pointer grab.
    std::string m_pending_color;
    bool  m_preview_dirty = false;
    guint m_preview_tick  = 0;

    // Raw CSS template cached in memory (read from disk once).
    std::string m_css_template;

    // Search Box
    Gtk::Frame m_search_frame;
    Gtk::Box m_search_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label m_search_prompt;
    Gtk::Entry m_search_entry;
    Gtk::Box m_view_toggle_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Button m_btn_grid;
    Gtk::Button m_btn_list;
    Gtk::Image* m_img_grid_view = nullptr;
    Gtk::Image* m_img_list_view = nullptr;
    Gtk::Image* m_img_settings  = nullptr;   // wrench glyph in the top toggle row
    Gtk::Image* m_img_fav_header = nullptr;
    Gtk::Image* m_img_rec_header = nullptr;
    Gtk::Image* m_img_all_header = nullptr;

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

    // i18n retranslation registry: labels + their string keys (markup flag).
    std::vector<std::tuple<Gtk::Label*, std::string, bool>> m_tr_labels;
    std::map<std::string, Gtk::Label*> m_cat_labels;   // sidebar tag id → its label

    std::chrono::steady_clock::time_point m_created_at;
    bool m_close_inhibited = false;
};

