#include "launcher_window.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include <cstring>

// ── Theme globals (shared by LauncherWindow & AppIconButton Cairo glow) ─────
static double g_theme_r = 224.0 / 255.0;
static double g_theme_g = 153.0 / 255.0;
static double g_theme_b = 36.0 / 255.0;
static int    g_theme_ri = 224;
static int    g_theme_gi = 153;
static int    g_theme_bi = 36;
static std::string g_theme_color = "#e09924";
static std::string g_theme_hotkey = "F4";

static std::string config_path() {
    const char* home = std::getenv("HOME");
    if (!home) return "";
    std::string dir = std::string(home) + "/.config/vanilux";
    std::filesystem::create_directories(dir);
    return dir + "/config.txt";
}

static void load_theme_config() {
    std::string path = config_path();
    if (path.empty()) return;
    std::ifstream f(path);
    if (!f) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.rfind("color=", 0) == 0 && line.size() >= 8) {
            std::string c = line.substr(6);
            if (c.size() == 7 && c[0] == '#') {
                try {
                    unsigned int ri, gi, bi;
                    std::istringstream(c.substr(1,2)) >> std::hex >> ri;
                    std::istringstream(c.substr(3,2)) >> std::hex >> gi;
                    std::istringstream(c.substr(5,2)) >> std::hex >> bi;
                    g_theme_ri = ri; g_theme_gi = gi; g_theme_bi = bi;
                    g_theme_r = ri / 255.0; g_theme_g = gi / 255.0; g_theme_b = bi / 255.0;
                    g_theme_color = c;
                } catch (...) {}
            }
        } else if (line.rfind("hotkey=", 0) == 0 && line.size() > 7) {
            g_theme_hotkey = line.substr(7);
        }
    }
}

static void save_theme_config() {
    std::string path = config_path();
    if (path.empty()) return;
    std::ofstream f(path);
    if (!f) return;
    f << "color=" << g_theme_color << "\n";
    f << "hotkey=" << g_theme_hotkey << "\n";
}

static std::string replace_theme_color(const std::string& css) {
    std::string result = css;
    // Replace hardcoded amber hex #e09924 with theme color (case-sensitive)
    size_t pos = 0;
    while ((pos = result.find("#e09924", pos)) != std::string::npos) {
        result.replace(pos, 7, g_theme_color);
        pos += g_theme_color.size();
    }
    // Replace #ffb000 (favorite star hover) with a brightened version
    pos = 0;
    int hr = std::min(255, (int)(g_theme_ri * 1.3 + 30));
    int hg = std::min(255, (int)(g_theme_gi * 1.3 + 30));
    int hb = std::min(255, (int)(g_theme_bi * 1.3 + 30));
    std::ostringstream bright_hex;
    bright_hex << "#" << std::hex << std::setfill('0') << std::setw(2) << hr
               << std::setw(2) << hg << std::setw(2) << hb;
    std::string bright = bright_hex.str();
    pos = 0;
    while ((pos = result.find("#ffb000", pos)) != std::string::npos) {
        result.replace(pos, 7, bright);
        pos += bright.size();
    }
    pos = 0;
    while ((pos = result.find("rgba(255, 176, 0,", pos)) != std::string::npos) {
        std::ostringstream rep;
        rep << "rgba(" << hr << ", " << hg << ", " << hb << ",";
        result.replace(pos, 17, rep.str());
        pos += rep.str().size();
    }
    // Replace rgba(224, 153, 36, with rgba(THEME_R, THEME_G, THEME_B,
    std::string old_rgba = "rgba(224, 153, 36,";
    std::ostringstream new_rgba;
    new_rgba << "rgba(" << g_theme_ri << ", " << g_theme_gi << ", " << g_theme_bi << ",";
    std::string nr = new_rgba.str();
    pos = 0;
    while ((pos = result.find(old_rgba, pos)) != std::string::npos) {
        result.replace(pos, old_rgba.size(), nr);
        pos += nr.size();
    }

    return result;
}

struct CategoryMeta {
    std::string id;
    std::string name;
    std::string icon;
};

static const std::vector<CategoryMeta> CATEGORIES = {
    {"all",      "Todas",        "⊞"},
    {"internet", "Internet",     "🌐"},
    {"dev",      "Programación",  "</>"},
    {"media",    "Multimedia",   "🎵"},
    {"system",   "Sistema",      "⚙"},
    {"android",  "Android",      "🤖"},
    {"office",   "Oficina",      "📄"},
    {"games",    "Juegos",       "🎮"}
};

std::string LauncherWindow::detect_category(const std::string& cats) {
    if (cats.find("X-WayDroid-App") != std::string::npos) return "android";
    if (cats.find("Game")           != std::string::npos) return "games";
    if (cats.find("Development")    != std::string::npos ||
        cats.find("IDE")            != std::string::npos ||
        cats.find("Debugger")       != std::string::npos) return "dev";
    if (cats.find("WebBrowser")     != std::string::npos ||
        cats.find("Email")          != std::string::npos ||
        cats.find("Network")        != std::string::npos) return "internet";
    if (cats.find("Audio")          != std::string::npos ||
        cats.find("Video")          != std::string::npos ||
        cats.find("Music")          != std::string::npos ||
        cats.find("Player")         != std::string::npos) return "media";
    if (cats.find("Office")         != std::string::npos ||
        cats.find("Productivity")   != std::string::npos ||
        cats.find("WordProcessor")  != std::string::npos) return "office";
    if (cats.find("System")         != std::string::npos ||
        cats.find("Settings")       != std::string::npos ||
        cats.find("Monitor")        != std::string::npos ||
        cats.find("Security")       != std::string::npos) return "system";
    return "other";
}

static std::string get_config_dir() {
    const char* home = std::getenv("HOME");
    if (!home) return "";
    std::string path = std::string(home) + "/.config/vanilux";
    std::filesystem::create_directories(path);
    return path;
}

static std::string find_installed(const std::string& rel_under_share) {
    std::vector<std::string> prefixes = {"/usr/share/vanilux", "/usr/local/share/vanilux"};
    for (const auto& prefix : prefixes) {
        std::string path = prefix + "/" + rel_under_share;
        if (std::filesystem::exists(path)) return path;
    }
    return "";
}

static std::string resolve_icon_path(const std::string& rel_path) {
    if (rel_path.empty()) return "";
    if (rel_path.rfind("src/icons/", 0) == 0) {
        std::string filename = rel_path.substr(10);
        std::string installed = find_installed("icons/" + filename);
        if (!installed.empty()) return installed;
    }
    return rel_path;
}

// ── AppIconButton ────────────────────────────────────────────────────────────

static std::string get_custom_icon(const std::string& icon_name) {
    (void)icon_name;
    return "";
}

AppIconButton::AppIconButton(const AppEntry& entry, bool is_favorite, bool list_mode,
                             std::function<void(const std::string&)> toggle_fav_cb,
                             const std::string& detected_cat,
                             IsTaggedCb is_tagged_cb, SetTagCb set_tag_cb)
    : m_entry(entry), m_is_favorite(is_favorite), m_list_mode(list_mode),
      m_detected_cat(detected_cat), m_toggle_fav_cb(toggle_fav_cb),
      m_is_tagged_cb(is_tagged_cb), m_set_tag_cb(set_tag_cb) {
    get_style_context()->add_class("app-button");
    set_can_focus(true);   // allow keyboard (arrow) navigation to land here

    // List mode uses a smaller icon + a denser row; grid mode keeps the big tile.
    const int icon_px  = list_mode ? 30 : 48;
    const int wrap_sz  = list_mode ? 40 : 54;
    const int ov_w     = list_mode ? 40 : 96;
    const int ov_h     = list_mode ? 40 : 72;
    const int wrap_top = list_mode ? 0  : 17;

    if (list_mode) {
        m_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_box.set_spacing(10);
        set_size_request(-1, 50);
        set_hexpand(true);
    } else {
        m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
        m_box.set_spacing(1);
        set_size_request(96, 96);
    }

    std::string custom_icon_path = resolve_icon_path(get_custom_icon(entry.icon));
    if (custom_icon_path.empty()) {
        custom_icon_path = entry.icon;
    }

    m_image.set_pixel_size(icon_px);
    if (!custom_icon_path.empty() && (custom_icon_path[0] == '/' || custom_icon_path.rfind("src/icons/", 0) == 0)) {
        // Absolute / project file path: decode once and cache (repeats across
        // the favorites / recents / all grids and across rebuilds).
        static std::unordered_map<std::string, Glib::RefPtr<Gdk::Pixbuf>> s_icon_cache;
        Glib::RefPtr<Gdk::Pixbuf> pb;
        // Key by path + size: grid (48) and list (30) need different rasterizations.
        std::string cache_key = custom_icon_path + "@" + std::to_string(icon_px);
        auto cit = s_icon_cache.find(cache_key);
        if (cit != s_icon_cache.end()) {
            pb = cit->second;
        } else {
            try { pb = Gdk::Pixbuf::create_from_file(custom_icon_path, icon_px, icon_px, true); } catch (...) {}
            s_icon_cache[cache_key] = pb;
        }
        if (pb) m_image.set(pb);
        else    m_image.set_from_icon_name("application-x-executable", Gtk::ICON_SIZE_DIALOG);
    } else {
        // Themed icon name: hand it to GtkImage and let GTK rasterize it lazily
        // at draw time (only on-screen icons are ever rendered) with its own
        // internal cache — far cheaper than eagerly decoding all ~130 SVGs.
        auto theme = Gtk::IconTheme::get_default();
        std::string name = custom_icon_path;
        if (name.empty() || !(theme && theme->has_icon(name)))
            name = "application-x-executable";
        m_image.set_from_icon_name(name, Gtk::ICON_SIZE_DIALOG);
    }

    m_image.set_halign(Gtk::ALIGN_CENTER);
    m_image.set_valign(Gtk::ALIGN_CENTER);

    m_icon_wrapper.get_style_context()->add_class("icon-wrapper");
    m_icon_wrapper.set_size_request(wrap_sz, wrap_sz);
    m_icon_wrapper.set_halign(Gtk::ALIGN_CENTER);
    m_icon_wrapper.set_valign(Gtk::ALIGN_CENTER);
    m_icon_wrapper.pack_start(m_image, Gtk::PACK_SHRINK);

    // Star glyphs are identical for every icon — load them once and reuse.
    static Glib::RefPtr<Gdk::Pixbuf> s_pb_empty;
    static Glib::RefPtr<Gdk::Pixbuf> s_pb_filled;
    static bool s_stars_loaded = false;
    if (!s_stars_loaded) {
        s_stars_loaded = true;
        std::string path_empty = find_installed("icons/star_empty_rounded.svg");
        if (path_empty.empty()) path_empty = "src/icons/star_empty_rounded.svg";
        std::string path_filled = find_installed("icons/star_filled_rounded.svg");
        if (path_filled.empty()) path_filled = "src/icons/star_filled_rounded.svg";
        try {
            s_pb_empty = Gdk::Pixbuf::create_from_file(path_empty, 14, 14, true);
            s_pb_filled = Gdk::Pixbuf::create_from_file(path_filled, 14, 14, true);
        } catch (...) {}
    }
    Glib::RefPtr<Gdk::Pixbuf> pb_empty = s_pb_empty, pb_filled = s_pb_filled;

    // Create an intermediate main container sized 68x68 to align the overlay children relative to it
    auto* overlay_main = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    overlay_main->set_size_request(ov_w, ov_h);
    overlay_main->set_halign(Gtk::ALIGN_CENTER);
    overlay_main->set_valign(Gtk::ALIGN_CENTER);
    m_icon_wrapper.set_halign(Gtk::ALIGN_CENTER);
    m_icon_wrapper.set_valign(list_mode ? Gtk::ALIGN_CENTER : Gtk::ALIGN_START);
    // Reserve glow room ABOVE the icon (valign has no effect for a shrink child
    // in a vertical box). The label then sits right below the icon, not far down.
    m_icon_wrapper.set_margin_top(wrap_top);
    overlay_main->pack_start(m_icon_wrapper, Gtk::PACK_SHRINK);

    m_overlay.set_size_request(ov_w, ov_h);
    m_overlay.set_halign(Gtk::ALIGN_CENTER);
    m_overlay.set_valign(Gtk::ALIGN_CENTER);
    m_overlay.add(*overlay_main);

    // Favorite toggle button overlay
    m_fav_toggle_btn.set_no_show_all(true);
    m_fav_toggle_btn.get_style_context()->add_class("fav-toggle-btn");
    if (m_is_favorite) {
        if (pb_filled) m_fav_icon_img.set(pb_filled);
        m_fav_toggle_btn.get_style_context()->add_class("active");
        m_fav_toggle_btn.show();
    } else {
        if (pb_empty) m_fav_icon_img.set(pb_empty);
        m_fav_toggle_btn.hide();
    }
    m_fav_toggle_btn.set_image(m_fav_icon_img);
    m_fav_toggle_btn.set_always_show_image(true);
    // Star badge at the top-right corner, poking out a bit up and to the right
    // of the icon. The glyph is 14px; the click hit-area is enlarged in code
    // (see button-release) so it's forgiving without launching the app.
    m_fav_toggle_btn.set_size_request(24, 24);
    m_fav_toggle_btn.set_halign(Gtk::ALIGN_END);
    m_fav_toggle_btn.set_valign(Gtk::ALIGN_START);
    m_fav_toggle_btn.set_margin_end(0);
    m_fav_toggle_btn.set_margin_top(0);
    m_fav_toggle_btn.set_margin_bottom(0);
    m_overlay.add_overlay(m_fav_toggle_btn);

    // The star is drawn on top of the icon but must NEVER capture pointer
    // events itself. A Gtk::Button delivers its clicks on a private input-only
    // event window (neither this EventBox's window nor the one get_window()
    // returns), and for a star that is only mapped on hover its "clicked"
    // signal fires unreliably — both of which broke the old window/flag-based
    // detection. Making it pass-through lets every click fall through to this
    // EventBox, where we decide star-vs-launch purely by pointer position.
    m_overlay.set_overlay_pass_through(m_fav_toggle_btn, true);

    // Pointer position relative to this icon, derived from absolute root
    // coordinates so it's correct no matter which GdkWindow delivered the event.
    auto pointer_in_icon = [this](double x_root, double y_root, double& px, double& py) -> bool {
        auto win = get_window();
        if (!win) return false;
        int ox = 0, oy = 0;
        win->get_origin(ox, oy);
        px = x_root - ox;
        py = y_root - oy;
        return true;
    };

    // True when (px,py) — in this icon's coordinate space — lands on the star
    // badge's (forgiving) hit area in the top-right corner.
    auto over_star = [this](double px, double py) -> bool {
        int sx = 0, sy = 0;
        if (!m_fav_toggle_btn.translate_coordinates(*this, 0, 0, sx, sy)) {
            Gtk::Allocation self = get_allocation();
            sx = self.get_width() - 24;
            sy = 0;
        }
        Gtk::Allocation a = m_fav_toggle_btn.get_allocation();
        int w = a.get_width()  > 0 ? a.get_width()  : 24;
        int h = a.get_height() > 0 ? a.get_height() : 24;
        const int pad = 6;   // forgiving hit area around the 14px glyph
        return px >= sx - pad && px < sx + w + pad &&
               py >= sy - pad && py < sy + h + pad;
    };

    // True when (px,py) lands on the list-mode ⚙ icon (forgiving hit area).
    auto over_config = [this](double px, double py) -> bool {
        if (!m_list_mode) return false;
        int cx = 0, cy = 0;
        if (!m_config_icon_img.translate_coordinates(*this, 0, 0, cx, cy)) return false;
        Gtk::Allocation a = m_config_icon_img.get_allocation();
        int w = a.get_width()  > 0 ? a.get_width()  : 18;
        int h = a.get_height() > 0 ? a.get_height() : 18;
        const int pad = 10;
        return px >= cx - pad && px < cx + w + pad &&
               py >= cy - pad && py < cy + h + pad;
    };

    // Support hover visibility for favorite button
    add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK |
               Gdk::BUTTON_RELEASE_MASK);
    signal_enter_notify_event().connect([this](GdkEventCrossing*) {
        grab_focus(); // focus-follows-mouse: a single highlight shared with keyboard nav
        m_fav_toggle_btn.show();
        return false;
    });
    signal_leave_notify_event().connect([this](GdkEventCrossing* event) {
        if (event->detail == GDK_NOTIFY_INFERIOR) {
            return false; // Pointer entered the star button (child widget), ignore to prevent flicker
        }
        get_style_context()->remove_class("hover");
        // The glow + star follow keyboard focus now (focus-out handles hiding),
        // so leaving to empty space keeps the current selection highlighted.
        return false;
    });

    // Keyboard focus (arrow navigation) lights up the same glow as hover.
    signal_focus_in_event().connect([this](GdkEventFocus*) {
        queue_draw();
        m_fav_toggle_btn.show();
        return false;
    });
    signal_focus_out_event().connect([this](GdkEventFocus*) {
        queue_draw();
        if (!m_is_favorite) m_fav_toggle_btn.hide();
        return false;
    });

    // The star glyph reflects the REAL favorite state and nothing else: filled
    // (active) when this app is a favorite, empty otherwise. It is set once at
    // construction (above) and never flipped on hover — previewing the opposite
    // state while the pointer sat on the star made a just-added favorite look
    // unmarked, since the cursor lingers on the star after the click.

    // If the pointer is already inside when this button maps (e.g. after a
    // rebuild triggered by toggling), reveal the star so it stays visible under
    // the cursor — without altering its glyph.
    signal_map().connect([this]() {
        auto win = get_window();
        if (!win) return;
        auto display = win->get_display();
        auto seat = display ? display->get_default_seat() : Glib::RefPtr<Gdk::Seat>();
        auto pointer = seat ? seat->get_pointer() : Glib::RefPtr<Gdk::Device>();
        if (!pointer) return;
        int x = 0, y = 0;
        Gdk::ModifierType mask;
        win->get_device_position(pointer, x, y, mask);
        Gtk::Allocation a = get_allocation();
        if (x >= 0 && x < a.get_width() && y >= 0 && y < a.get_height()) {
            get_style_context()->add_class("hover");
            m_fav_toggle_btn.show();
        }
    });

    // A left-release on the star's corner toggles the favorite; anywhere else
    // on the icon launches the app. The decision is made purely from pointer
    // position (helpers above), which is reliable regardless of which window
    // delivered the event — unlike the old "clicked"/window-comparison scheme.
    signal_button_release_event().connect([this, pointer_in_icon, over_star, over_config](GdkEventButton* event) {
        if (event->button != 1) return false;
        double px = 0, py = 0;
        if (pointer_in_icon(event->x_root, event->y_root, px, py)) {
            if (over_star(px, py)) {
                if (m_toggle_fav_cb) m_toggle_fav_cb(m_entry.name);
                return true;
            }
            if (over_config(px, py)) {
                // ⚙ opens the category popover ONLY — never launches the app.
                build_tags_popover();
                m_tags_popover.popup();
                return true;
            }
        }
        m_signal_clicked.emit();
        return true;
    });

    m_label.set_text(entry.name);
    m_label.get_style_context()->add_class("app-label");

    if (list_mode) {
        // ── Compact list row: [icon] [name / comment] [category badge] [⚙] ──
        m_label.set_line_wrap(false);
        m_label.set_ellipsize(Pango::ELLIPSIZE_END);
        m_label.set_justify(Gtk::JUSTIFY_LEFT);
        m_label.set_halign(Gtk::ALIGN_START);
        m_label.set_valign(Gtk::ALIGN_END);
        m_label.set_xalign(0.0f);

        m_sublabel.set_text(entry.comment);
        m_sublabel.get_style_context()->add_class("app-sublabel");
        m_sublabel.set_line_wrap(false);
        m_sublabel.set_ellipsize(Pango::ELLIPSIZE_END);
        m_sublabel.set_halign(Gtk::ALIGN_START);
        m_sublabel.set_valign(Gtk::ALIGN_START);
        m_sublabel.set_xalign(0.0f);

        m_text_box.set_valign(Gtk::ALIGN_CENTER);
        m_text_box.set_halign(Gtk::ALIGN_START);
        m_text_box.set_hexpand(true);
        m_text_box.pack_start(m_label, Gtk::PACK_SHRINK);
        if (!entry.comment.empty())
            m_text_box.pack_start(m_sublabel, Gtk::PACK_SHRINK);

        // Category badge: display name of the auto-detected category.
        std::string badge_text;
        for (const auto& c : CATEGORIES)
            if (c.id == m_detected_cat) { badge_text = c.name; break; }
        if (badge_text.empty()) badge_text = "Otros";
        m_cat_badge.set_text(badge_text);
        m_cat_badge.get_style_context()->add_class("cat-badge");
        m_cat_badge.set_valign(Gtk::ALIGN_CENTER);

        // ⚙ icon (window-less image): clicks fall through to this EventBox, whose
        // release handler opens the popover by pointer position (see over_config).
        {
            static Glib::RefPtr<Gdk::Pixbuf> s_cfg_pb;
            static bool s_cfg_loaded = false;
            if (!s_cfg_loaded) {
                s_cfg_loaded = true;
                std::string p = find_installed("icons/settings_amber.svg");
                if (p.empty()) p = "src/icons/settings_amber.svg";
                try { s_cfg_pb = Gdk::Pixbuf::create_from_file(p, 18, 18, true); } catch (...) {}
            }
            if (s_cfg_pb) m_config_icon_img.set(s_cfg_pb);
            else          m_config_icon_img.set_from_icon_name("emblem-system-symbolic", Gtk::ICON_SIZE_MENU);
        }
        m_config_icon_img.get_style_context()->add_class("config-icon");
        m_config_icon_img.set_valign(Gtk::ALIGN_CENTER);
        m_config_icon_img.set_margin_start(6);
        m_config_icon_img.set_margin_end(8);
        m_config_icon_img.set_tooltip_text("Clasificar en categorías");

        m_box.pack_start(m_overlay, Gtk::PACK_SHRINK);
        m_box.pack_start(m_text_box, Gtk::PACK_EXPAND_WIDGET);
        m_box.pack_start(m_cat_badge, Gtk::PACK_SHRINK);
        m_box.pack_start(m_config_icon_img, Gtk::PACK_SHRINK);

        m_box.set_halign(Gtk::ALIGN_FILL);
        m_box.set_valign(Gtk::ALIGN_CENTER);
        set_halign(Gtk::ALIGN_FILL);
        set_valign(Gtk::ALIGN_CENTER);
    } else {
        m_label.set_line_wrap(true);
        m_label.set_line_wrap_mode(Pango::WRAP_WORD_CHAR);
        m_label.set_max_width_chars(12);
        m_label.set_lines(2);
        m_label.set_ellipsize(Pango::ELLIPSIZE_END);
        m_label.set_justify(Gtk::JUSTIFY_CENTER);
        m_label.set_halign(Gtk::ALIGN_CENTER);
        m_label.set_valign(Gtk::ALIGN_START);
        // Fixed height request for the label so the icon never moves!
        m_label.set_size_request(-1, 32);

        m_box.pack_start(m_overlay, Gtk::PACK_SHRINK);
        m_box.pack_start(m_label, Gtk::PACK_SHRINK);

        m_box.set_halign(Gtk::ALIGN_CENTER);
        m_box.set_valign(Gtk::ALIGN_CENTER);
        set_halign(Gtk::ALIGN_CENTER);
        set_valign(Gtk::ALIGN_CENTER);
    }

    add(m_box);
    m_box.show_all();
}

// Lazily build the ⚙ popover that lets the user tag this app into the existing
// categories. The auto-detected category is shown checked + locked (it's
// automatic); the rest are manual tags persisted by LauncherWindow.
void AppIconButton::build_tags_popover() {
    if (m_popover_built) return;
    m_popover_built = true;

    m_tags_popover.set_relative_to(m_config_icon_img);
    m_tags_popover.set_position(Gtk::POS_TOP);
    m_tags_popover.get_style_context()->add_class("tags-popover");

    auto* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    box->set_spacing(2);
    box->set_margin_top(10);
    box->set_margin_bottom(10);
    box->set_margin_start(12);
    box->set_margin_end(12);

    auto* title = Gtk::manage(new Gtk::Label());
    title->set_text("Categorías");
    title->get_style_context()->add_class("section-header-label");
    title->set_halign(Gtk::ALIGN_START);
    title->set_margin_bottom(6);
    box->pack_start(*title, Gtk::PACK_SHRINK);

    for (const auto& c : CATEGORIES) {
        if (c.id == "all") continue;
        auto* chk = Gtk::manage(new Gtk::CheckButton(c.name));
        chk->get_style_context()->add_class("tag-check");
        chk->set_can_focus(false);

        if (c.id == m_detected_cat) {
            // Automatic membership — always on, can't be removed here.
            chk->set_active(true);
            chk->set_sensitive(false);
            chk->set_label(c.name + "  (auto)");
        } else {
            chk->set_active(m_is_tagged_cb ? m_is_tagged_cb(m_entry.name, c.id) : false);
            std::string cat_id = c.id;
            chk->signal_toggled().connect([this, chk, cat_id]() {
                if (m_set_tag_cb) m_set_tag_cb(m_entry.name, cat_id, chk->get_active());
            });
        }
        box->pack_start(*chk, Gtk::PACK_SHRINK);
    }

    // Commit (rebuild the views) once, when the popover closes — keeps it open
    // for several toggles without the row rebuilding underneath the pointer.
    m_tags_popover.signal_closed().connect([this]() {
        if (m_set_tag_cb) m_set_tag_cb(m_entry.name, "", false);
    });

    box->show_all();
    m_tags_popover.add(*box);
}

// Separable box blur over an ARGB32 surface (a few passes approximate a
// Gaussian). Used to turn the crisp frame edge into a single uniform glow
// gradient with no visible stacked layers.
static void blur_argb_surface(const Cairo::RefPtr<Cairo::ImageSurface>& surf, int radius, int passes) {
    if (radius < 1) return;
    const int w = surf->get_width();
    const int h = surf->get_height();
    const int stride = surf->get_stride();
    surf->flush();
    unsigned char* data = surf->get_data();
    if (!data) return;
    std::vector<unsigned char> tmp(stride * h);
    for (int p = 0; p < passes; ++p) {
        // Horizontal pass: data -> tmp
        for (int y = 0; y < h; ++y) {
            unsigned char* src = data + y * stride;
            unsigned char* dst = tmp.data() + y * stride;
            for (int x = 0; x < w; ++x) {
                int b = 0, g = 0, rr = 0, a = 0, cnt = 0;
                int x0 = x - radius < 0 ? 0 : x - radius;
                int x1 = x + radius >= w ? w - 1 : x + radius;
                for (int xi = x0; xi <= x1; ++xi) {
                    unsigned char* px = src + xi * 4;
                    b += px[0]; g += px[1]; rr += px[2]; a += px[3]; ++cnt;
                }
                unsigned char* d = dst + x * 4;
                d[0] = b / cnt; d[1] = g / cnt; d[2] = rr / cnt; d[3] = a / cnt;
            }
        }
        // Vertical pass: tmp -> data
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                int b = 0, g = 0, rr = 0, a = 0, cnt = 0;
                int y0 = y - radius < 0 ? 0 : y - radius;
                int y1 = y + radius >= h ? h - 1 : y + radius;
                for (int yi = y0; yi <= y1; ++yi) {
                    unsigned char* px = tmp.data() + yi * stride + x * 4;
                    b += px[0]; g += px[1]; rr += px[2]; a += px[3]; ++cnt;
                }
                unsigned char* d = data + y * stride + x * 4;
                d[0] = b / cnt; d[1] = g / cnt; d[2] = rr / cnt; d[3] = a / cnt;
            }
        }
    }
    surf->mark_dirty();
}

// Draw the amber border + soft glow ourselves with Cairo. Relying on a CSS
// box-shadow toggled by a state class is unreliable in GTK3 (it renders once
// then fails to re-invalidate the shadow region on subsequent hovers). Doing
// it here, combined with queue_draw() on enter/leave, guarantees the glow
// repaints cleanly every single time.
bool AppIconButton::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    bool ret = Gtk::EventBox::on_draw(cr);

    // Exactly one icon is highlighted at a time: the focused one. The mouse
    // grabs focus on enter (focus-follows-mouse), so hover and keyboard
    // selection share a single source of truth and never glow two at once.
    if (!has_focus()) return ret;

    // Anchor the frame to the actual icon image (fixed size, centered on it)
    // rather than to the icon wrapper, whose allocation can stretch in the
    // grid and would make the frame taller than the icon.
    int ix = 0, iy = 0;
    if (!m_image.translate_coordinates(*this, 0, 0, ix, iy)) return ret;
    Gtk::Allocation ia = m_image.get_allocation();
    const double cx = ix + ia.get_width()  / 2.0;
    const double cy = iy + ia.get_height() / 2.0;
    const double half = m_list_mode ? 20.0 : 28.0;  // frame half-size around icon
    const double x = cx - half, y = cy - half;
    const double w = 2.0 * half, h = 2.0 * half;
    const double r = 12.0;
    const double R = g_theme_r, G = g_theme_g, B = g_theme_b;

    auto rounded = [&](double rx, double ry, double rw, double rh, double rr) {
        if (rr > rw / 2.0) rr = rw / 2.0;
        if (rr > rh / 2.0) rr = rh / 2.0;
        cr->begin_new_sub_path();
        cr->arc(rx + rw - rr, ry + rr,      rr, -M_PI / 2.0, 0.0);
        cr->arc(rx + rw - rr, ry + rh - rr, rr, 0.0,         M_PI / 2.0);
        cr->arc(rx + rr,      ry + rh - rr, rr, M_PI / 2.0,  M_PI);
        cr->arc(rx + rr,      ry + rr,      rr, M_PI,        M_PI * 1.5);
        cr->close_path();
    };

    // Smooth glow: stroke the frame edge onto an offscreen surface and blur it,
    // so the halo is one uniform gradient (no visible concentric layers). It is
    // kept very faint — just the border barely bleeding light in and out.
    const int M = 26;
    const int sw = (int)w + 2 * M;
    const int sh = (int)h + 2 * M;
    auto glow_surf = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, sw, sh);
    {
        auto gc = Cairo::Context::create(glow_surf);
        gc->set_line_join(Cairo::LINE_JOIN_ROUND);
        double rr = r, rw = w, rh = h, rx = M, ry = M;
        gc->begin_new_sub_path();
        gc->arc(rx + rw - rr, ry + rr,      rr, -M_PI / 2.0, 0.0);
        gc->arc(rx + rw - rr, ry + rh - rr, rr, 0.0,         M_PI / 2.0);
        gc->arc(rx + rr,      ry + rh - rr, rr, M_PI / 2.0,  M_PI);
        gc->arc(rx + rr,      ry + rr,      rr, M_PI,        M_PI * 1.5);
        gc->close_path();
        gc->set_source_rgba(R, G, B, 0.55);
        gc->set_line_width(4.0);
        gc->stroke();
    }
    blur_argb_surface(glow_surf, 9, 3);
    cr->save();
    cr->set_source(glow_surf, x - M, y - M);
    cr->paint();
    cr->restore();

    // Crisp amber border at the icon edge (unchanged)
    cr->save();
    cr->set_line_join(Cairo::LINE_JOIN_ROUND);
    cr->set_source_rgba(R, G, B, 0.95);
    cr->set_line_width(2.0);
    rounded(x + 1.0, y + 1.0, w - 2.0, h - 2.0, r);
    cr->stroke();
    cr->restore();

    return ret;
}

// ── LauncherWindow ───────────────────────────────────────────────────────────

LauncherWindow::LauncherWindow() {
    m_created_at = std::chrono::steady_clock::now();

    set_name("launcher-window");
    set_decorated(false);
    fullscreen();
    set_app_paintable(true);

    auto gtk_settings = Gtk::Settings::get_default();
    if (gtk_settings) gtk_settings->property_gtk_icon_theme_name().set_value("Papirus");

    auto screen = get_screen();
    if (screen) {
        auto visual = screen->get_rgba_visual();
        if (visual && screen->is_composited())
            gtk_widget_set_visual(GTK_WIDGET(gobj()), const_cast<GdkVisual*>(visual->gobj()));
    }

    load_theme_config();
    load_css();
    load_persisted_data();
    setup_ui();
}

LauncherWindow::~LauncherWindow() {}

void LauncherWindow::toggle_visibility() {
    if (get_visible()) hide_launcher();
    else               show_launcher();
}

void LauncherWindow::show_launcher() {
    // Everything was already reset on hide, so opening is just a show() — instant.
    m_created_at = std::chrono::steady_clock::now();
    show();
    fullscreen();
    present();
    m_search_entry.grab_focus();
}

void LauncherWindow::hide_launcher() {
    // Stay resident (just hide) so the next open is instant. Reset the view now
    // (while hidden, off the critical path) so it reopens fresh.
    hide();
    if (m_current_category != "all" || !m_search_entry.get_text().empty())
        set_category("all");   // clears search + restores "Todas" + rebuilds
    else
        rebuild_all();         // already on "Todas": refresh so a just-opened
                               // app shows up under Recientes on the next open
    if (auto adj = m_apps_scrolled.get_vadjustment()) adj->set_value(0.0);
}

void LauncherWindow::load_css() {
    try {
        std::string css_data;
        std::ifstream file;
        std::string installed_css = find_installed("style.css");
        if (!installed_css.empty()) file.open(installed_css);
        if (!file) file.open("src/style.css");
        if (!file) file.open("style.css");
        if (file) {
            std::stringstream ss;
            ss << file.rdbuf();
            css_data = ss.str();
        } else {
            css_data = R"(
                #launcher-window, label, entry, button, textview { font-family: monospace; }
                #launcher-window { background-color: #0d0d12; }
                button,button:hover,button:active,button:focus,button:backdrop,
                button:backdrop:hover,button:backdrop:active,button:checked,
                button:checked:hover {
                    background-image: none; background-color: transparent;
                    border-style: none; border-width: 0; box-shadow: none; text-shadow: none;
                }
                frame { border: 1px solid rgba(224,153,36,0.35); border-radius: 4px; margin: 4px; padding: 6px; background-color: transparent; }
                frame > label { color: #e09924; font-size: 11px; font-weight: bold; background-color: #0d0d12; margin: 0 4px; padding: 0 6px; }
                entry { border: none; background: transparent; color: #f2e9e1; caret-color: #e09924; font-size: 14px; box-shadow: none; text-shadow: none; padding: 2px 4px; }
                entry:focus { box-shadow: none; border: none; }
                .search-prompt { color: #e09924; font-size: 16px; font-weight: bold; margin-right: 8px; }
                button.view-btn,button.view-btn:backdrop { background-color: transparent; border: 1px solid rgba(224,153,36,0.35); color: rgba(255,255,255,0.6); padding: 3px 8px; border-radius: 4px; margin: 0 2px; }
                button.view-btn:hover { background-color: rgba(224,153,36,0.1); color: #f2e9e1; }
                button.view-btn.active,button.view-btn.active:backdrop { background-color: rgba(224,153,36,0.25); border-color: #e09924; color: #e09924; box-shadow: 0 0 5px rgba(224,153,36,0.4); }
                button.category-btn,button.category-btn:backdrop { background-color: transparent; border-style: none; border-width: 0; box-shadow: none; color: rgba(255,255,255,0.6); padding: 8px 12px; margin: 2px 4px; font-size: 12px; font-weight: 500; border-radius: 4px; }
                button.category-btn:hover { background-color: rgba(224,153,36,0.08); color: #f2e9e1; }
                button.category-btn.active,button.category-btn.active:backdrop { background-color: rgba(224,153,36,0.15); border-left: 3px solid #e09924; border-radius: 0 4px 4px 0; color: #e09924; font-weight: bold; }
                .app-button,.app-button:backdrop { background-color: rgba(224,153,36,0.0); border-style: none; border-width: 0; box-shadow: none; padding: 4px; border-radius: 6px; }
                .app-button:focus,.app-button.hover { background-color: transparent; }
                .icon-wrapper { border: 2px solid rgba(224,153,36,0.0); border-radius: 8px; padding: 3px; margin-bottom: 0px; background-color: transparent; box-shadow: 0 0 0 rgba(224,153,36,0); }
                .app-label { color: rgba(255,255,255,0.85); font-size: 10px; font-weight: 400; margin-top: 2px; }
                .app-button.hover .app-label,.app-button:focus .app-label { color: #e09924; font-weight: bold; }
                .app-sublabel { color: rgba(255,255,255,0.45); font-size: 10px; font-weight: 400; }
                .app-button.hover .app-sublabel,.app-button:focus .app-sublabel { color: rgba(242,233,225,0.7); }
                .cat-badge { color: #e09924; background-color: rgba(224,153,36,0.12); border: 1px solid rgba(224,153,36,0.35); border-radius: 10px; padding: 1px 9px; font-size: 9px; font-weight: bold; margin-right: 4px; }
                .config-icon { opacity: 0.5; }
                .app-button.hover .config-icon,.app-button:focus .config-icon { opacity: 1.0; }
                popover.tags-popover,popover.tags-popover>box,popover.tags-popover contents { background-color: #14141c; border: 1px solid rgba(224,153,36,0.4); border-radius: 8px; }
                popover.tags-popover checkbutton { color: rgba(255,255,255,0.85); font-size: 12px; padding: 3px 2px; }
                popover.tags-popover checkbutton:hover { color: #e09924; }
                popover.tags-popover check { border: 1px solid rgba(224,153,36,0.5); border-radius: 3px; min-width: 14px; min-height: 14px; }
                popover.tags-popover check:checked { background-color: #e09924; border-color: #e09924; }
                button.fav-toggle-btn,button.fav-toggle-btn:backdrop { color: rgba(224,153,36,0.4); background-color: transparent; background-image: none; border-style: none; border-width: 0; padding: 1px; margin: 0; font-size: 14px; font-weight: bold; min-width: 18px; min-height: 18px; box-shadow: none; text-shadow: none; border-radius: 4px; }
                button.fav-toggle-btn:hover,button.fav-toggle-btn.active,button.fav-toggle-btn.active:backdrop { color: #ffb000; background-color: transparent; background-image: none; border-style: none; border-width: 0; box-shadow: none; text-shadow: 0 0 6px rgba(255,176,0,0.8); }
                .section-header-label { color: rgba(255,255,255,0.65); font-size: 11px; font-weight: bold; margin-right: 8px; }
                .section-separator { background-color: rgba(224,153,36,0.2); min-height: 1px; }
                .status-ws { color: #e09924; font-weight: bold; margin-right: 12px; }
                .status-text { color: rgba(255,255,255,0.6); font-size: 11px; margin-right: 12px; }
                .key-cap { border: 1px solid rgba(224,153,36,0.5); border-radius: 3px; padding: 0 3px; background-color: #1a1a24; color: #e09924; font-weight: bold; font-size: 8px; margin-right: 4px; }
                .key-cap-arrow { border: 1px solid rgba(224,153,36,0.5); border-radius: 2px; background-color: #1a1a24; color: #e09924; font-weight: bold; font-size: 4px; padding: 0; margin: 0; min-width: 9px; min-height: 7px; }
                .key-desc { color: rgba(255,255,255,0.5); font-size: 11px; margin-right: 16px; }
                scrollbar,scrollbar button,scrollbar slider { background-color: transparent; border: none; }
                scrollbar.vertical slider { background-color: rgba(224,153,36,0.2); min-width: 5px; border-radius: 3px; }
                scrollbar.vertical slider:hover { background-color: #e09924; }
            )";
        }

        // Apply theme color substitution to the entire CSS
        css_data = replace_theme_color(css_data);

        auto css_provider = Gtk::CssProvider::create();
        css_provider->load_from_data(css_data);
        Gtk::StyleContext::add_provider_for_screen(
            Gdk::Screen::get_default(), css_provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        // Store a reference so we can re-create on theme change
        m_theme_provider = css_provider;
    } catch (const std::exception& e) {
        std::cerr << "vanilux: CSS load error: " << e.what() << std::endl;
    }
}

void LauncherWindow::apply_theme(const std::string& hex_color) {
    // Parse hex to RGB
    try {
        std::string h = hex_color;
        if (h[0] == '#') h = h.substr(1);
        unsigned int ri, gi, bi;
        std::istringstream(h.substr(0,2)) >> std::hex >> ri;
        std::istringstream(h.substr(2,2)) >> std::hex >> gi;
        std::istringstream(h.substr(4,2)) >> std::hex >> bi;
        g_theme_ri = ri; g_theme_gi = gi; g_theme_bi = bi;
        g_theme_r = ri / 255.0; g_theme_g = gi / 255.0; g_theme_b = bi / 255.0;
        g_theme_color = "#" + h;
    } catch (...) { return; }

    // Reload CSS with new color substitution
    load_css();

    // Force queue redraw on all children so Cairo glow updates
    queue_draw();
    for (auto* w : get_children()) {
        w->queue_draw();
        auto box = dynamic_cast<Gtk::Container*>(w);
        if (box) box->foreach([](Gtk::Widget& c) { c.queue_draw(); });
    }

    save_theme_config();
}

void LauncherWindow::refresh_apps() {
    std::vector<std::string> dirs = {
        "/usr/share/applications",
        "/usr/local/share/applications",
    };
    const char* home = std::getenv("HOME");
    if (home) dirs.push_back(std::string(home) + "/.local/share/applications");

    m_all_apps = AppDiscovery::scan_applications(dirs);

    if (m_current_category == "all") {
        m_apps = m_all_apps;
    } else {
        m_apps.clear();
        for (const auto& app : m_all_apps) {
            if (detect_category(app.categories) == m_current_category) {
                m_apps.push_back(app);
            }
        }
    }

    // Rebuild sidebar first to recalc which categories exist
    // Then rebuild the main grid
    // We do this by clearing and re-setting up the sidebar buttons
    // then calling set_category to re-filter and rebuild
    set_category(m_current_category);
    update_status();
}

void LauncherWindow::show_settings_dialog() {
    auto dialog = Gtk::Dialog();
    dialog.set_title("Configuración de Vanilux");
    dialog.set_transient_for(*this);
    dialog.set_modal(true);
    dialog.set_size_request(500, 400);
    dialog.set_resizable(false);

    auto content = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 20));
    content->set_margin_top(24);
    content->set_margin_bottom(24);
    content->set_margin_start(24);
    content->set_margin_end(24);

    // ── Color picker section ──────────────────────────────────────────────
    auto color_label = Gtk::manage(new Gtk::Label());
    color_label->set_markup("<b>Color del tema</b>");
    color_label->set_halign(Gtk::ALIGN_START);

    auto color_desc = Gtk::manage(new Gtk::Label());
    color_desc->set_text("Elegí el color principal para bordes, brillos y destacados.");
    color_desc->set_halign(Gtk::ALIGN_START);
    color_desc->set_ellipsize(Pango::ELLIPSIZE_END);
    color_desc->set_opacity(0.7);

    auto color_picker = Gtk::manage(new Gtk::ColorSelection());
    Gdk::Color current;
    current.set_rgb(g_theme_ri * 256, g_theme_gi * 256, g_theme_bi * 256);
    color_picker->set_has_opacity_control(false);
    color_picker->set_has_palette(true);
    color_picker->set_current_color(current);
    color_picker->set_halign(Gtk::ALIGN_CENTER);
    color_picker->set_size_request(400, 240);

    // ── Keybinding section ────────────────────────────────────────────────
    auto key_label = Gtk::manage(new Gtk::Label());
    key_label->set_markup("<b>Tecla rápida</b>");
    key_label->set_halign(Gtk::ALIGN_START);

    auto key_desc = Gtk::manage(new Gtk::Label());
    key_desc->set_text("Hacé clic en el botón y presioná la combinación de teclas.");
    key_desc->set_halign(Gtk::ALIGN_START);
    key_desc->set_opacity(0.7);

    auto key_hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 12));
    key_hbox->set_halign(Gtk::ALIGN_CENTER);
    key_hbox->set_margin_top(8);

    auto key_entry = Gtk::manage(new Gtk::Entry());
    key_entry->set_text(g_theme_hotkey);
    key_entry->set_width_chars(20);
    key_entry->set_alignment(Gtk::ALIGN_CENTER);
    key_entry->set_editable(false);
    key_entry->set_can_focus(true);

    // Capture key presses on the entry
    auto key_handler = [key_entry](GdkEventKey* ev) -> bool {
        if (ev->type == GDK_KEY_PRESS) {
            std::string key_name;

            if (ev->state & GDK_CONTROL_MASK) key_name += "<Ctrl>";
            if (ev->state & GDK_MOD1_MASK)    key_name += "<Alt>";
            if (ev->state & GDK_SHIFT_MASK)   key_name += "<Shift>";
            if (ev->state & GDK_SUPER_MASK)   key_name += "<Super>";

            auto name = gdk_keyval_name(ev->keyval);
            if (name) {
                // Skip modifier-only presses
                std::string n(name);
                if (n == "Control_L" || n == "Control_R" || n == "Alt_L" ||
                    n == "Alt_R" || n == "Shift_L" || n == "Shift_R" ||
                    n == "Super_L" || n == "Super_R") return true;

                key_name += n;
                key_entry->set_text(key_name);
            }
            return true;
        }
        return false;
    };
    key_entry->signal_key_press_event().connect(key_handler, false);

    auto key_btn_clear = Gtk::manage(new Gtk::Button("Limpiar"));
    key_btn_clear->signal_clicked().connect([key_entry]() {
        key_entry->set_text("");
    });

    key_hbox->pack_start(*key_entry, Gtk::PACK_SHRINK);
    key_hbox->pack_start(*key_btn_clear, Gtk::PACK_SHRINK);

    // ── Assemble ──────────────────────────────────────────────────────────
    content->pack_start(*color_label, Gtk::PACK_SHRINK);
    content->pack_start(*color_desc, Gtk::PACK_SHRINK);
    content->pack_start(*color_picker, Gtk::PACK_SHRINK);
    content->pack_start(*Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL)), Gtk::PACK_SHRINK);
    content->pack_start(*key_label, Gtk::PACK_SHRINK);
    content->pack_start(*key_desc, Gtk::PACK_SHRINK);
    content->pack_start(*key_hbox, Gtk::PACK_SHRINK);

    dialog.add_button("Cancelar", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Aplicar",  Gtk::RESPONSE_APPLY);
    auto btn_save    = dialog.add_button("Guardar",  Gtk::RESPONSE_OK);
    dialog.set_default_response(Gtk::RESPONSE_OK);

    auto ca = dialog.get_content_area();
    ca->pack_start(*content, Gtk::PACK_EXPAND_WIDGET);
    ca->show_all();

    // Real-time preview: on color change, apply immediately
    auto color_conn = color_picker->signal_color_changed().connect([this, color_picker, btn_save]() {
        auto c = color_picker->get_current_color();
        std::ostringstream hex;
        hex << "#" << std::hex << std::setfill('0') << std::setw(2)
            << (c.get_red_p() * 255) << std::setw(2)
            << (c.get_green_p() * 255) << std::setw(2)
            << (c.get_blue_p() * 255);
        apply_theme(hex.str());
        btn_save->grab_focus();
    });

    // Apply keybinding via gsettings when saved
    int result = dialog.run();

    // Disconnect real-time preview to avoid double-apply
    color_conn.disconnect();

    if (result == Gtk::RESPONSE_OK || result == Gtk::RESPONSE_APPLY) {
        // Save color
        if (result == Gtk::RESPONSE_OK) {
            auto c = color_picker->get_current_color();
            std::ostringstream hex;
            hex << "#" << std::hex << std::setfill('0') << std::setw(2)
                << (unsigned int)(c.get_red_p() * 255) << std::setw(2)
                << (unsigned int)(c.get_green_p() * 255) << std::setw(2)
                << (unsigned int)(c.get_blue_p() * 255);
            apply_theme(hex.str());
        }

        // Save keybinding
        std::string new_key = key_entry->get_text();
        if (!new_key.empty()) {
            g_theme_hotkey = new_key;
            save_theme_config();
        }

        // Apply keybinding to the system
        if (get_visible() == false) {
            // Only run keybinding setup if we're not root
            std::string bin_path = "/usr/local/bin/vanilux";
            if (!std::filesystem::exists(bin_path))
                bin_path = "/usr/bin/vanilux";
            if (std::filesystem::exists(bin_path)) {
                std::string cmd = "gsettings set org.cinnamon.desktop.keybindings.custom-keybinding:/org/cinnamon/desktop/keybindings/custom-keybindings/vanilux/ binding \"['" + new_key + "']\" 2>/dev/null || "
                                  "gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/vanilux/ binding \"['" + new_key + "']\" 2>/dev/null || true";
                int rc = std::system(cmd.c_str());
                (void)rc;
            }
        }
    }
}

void LauncherWindow::load_persisted_data() {
    std::string dir = get_config_dir();
    if (dir.empty()) return;

    m_favorites.clear();
    std::ifstream fav_file(dir + "/favorites.txt");
    std::string line;
    while (std::getline(fav_file, line)) {
        if (!line.empty()) m_favorites.push_back(line);
    }

    m_recents.clear();
    std::ifstream rec_file(dir + "/recents.txt");
    while (std::getline(rec_file, line)) {
        if (!line.empty()) m_recents.push_back(line);
    }

    // Manual category tags: "App Name<TAB>cat1,cat2" per line.
    m_custom_tags.clear();
    std::ifstream tag_file(dir + "/tags.txt");
    while (std::getline(tag_file, line)) {
        if (line.empty()) continue;
        auto tab = line.find('\t');
        if (tab == std::string::npos) continue;
        std::string app = line.substr(0, tab);
        std::string cats = line.substr(tab + 1);
        std::vector<std::string> ids;
        std::stringstream ss(cats);
        std::string id;
        while (std::getline(ss, id, ',')) {
            if (!id.empty()) ids.push_back(id);
        }
        if (!app.empty() && !ids.empty()) m_custom_tags[app] = ids;
    }
}

void LauncherWindow::save_persisted_data() {
    std::string dir = get_config_dir();
    if (dir.empty()) return;

    std::ofstream fav_file(dir + "/favorites.txt");
    for (const auto& fav : m_favorites) {
        fav_file << fav << "\n";
    }

    std::ofstream rec_file(dir + "/recents.txt");
    for (const auto& rec : m_recents) {
        rec_file << rec << "\n";
    }

    std::ofstream tag_file(dir + "/tags.txt");
    for (const auto& [app, ids] : m_custom_tags) {
        if (ids.empty()) continue;
        tag_file << app << "\t";
        for (size_t i = 0; i < ids.size(); ++i) {
            tag_file << ids[i];
            if (i + 1 < ids.size()) tag_file << ",";
        }
        tag_file << "\n";
    }
}

void LauncherWindow::toggle_favorite(const std::string& name) {
    auto it = std::find(m_favorites.begin(), m_favorites.end(), name);
    if (it != m_favorites.end()) {
        m_favorites.erase(it);
    } else {
        m_favorites.push_back(name);
    }
    save_persisted_data();
    Glib::signal_idle().connect_once([this]() {
        // On the Favoritos/Recientes tags the main list is derived from these
        // lists, so re-resolve it; otherwise just refresh the sections/grids.
        if (m_current_category == "favorites" || m_current_category == "recents")
            set_category(m_current_category);
        else
            rebuild_all();
    });
}

bool LauncherWindow::app_has_tag(const std::string& name, const std::string& cat_id) const {
    auto it = m_custom_tags.find(name);
    if (it == m_custom_tags.end()) return false;
    return std::find(it->second.begin(), it->second.end(), cat_id) != it->second.end();
}

void LauncherWindow::set_app_tag(const std::string& name, const std::string& cat_id, bool on) {
    // Empty cat_id is the "commit" signal sent when the popover closes: persist
    // nothing new, just refresh the views so tag changes take effect at once.
    if (!cat_id.empty()) {
        auto& ids = m_custom_tags[name];
        auto it = std::find(ids.begin(), ids.end(), cat_id);
        if (on && it == ids.end())      ids.push_back(cat_id);
        else if (!on && it != ids.end()) ids.erase(it);
        if (ids.empty()) m_custom_tags.erase(name);
        save_persisted_data();
        return;  // keep the popover open for further toggles; rebuild on close
    }

    Glib::signal_idle().connect_once([this]() {
        if (m_current_category == "favorites" || m_current_category == "recents")
            set_category(m_current_category);
        else
            rebuild_all();
    });
}

void LauncherWindow::add_to_recents(const std::string& name) {
    auto it = std::find(m_recents.begin(), m_recents.end(), name);
    if (it != m_recents.end()) {
        m_recents.erase(it);
    }
    m_recents.insert(m_recents.begin(), name);
    if ((int)m_recents.size() > HOME_SECTION_MAX) {
        m_recents.resize(HOME_SECTION_MAX);
    }
    save_persisted_data();
}

void LauncherWindow::setup_ui() {
    std::vector<std::string> dirs = {
        "/usr/share/applications",
        "/usr/local/share/applications",
    };
    const char* home = std::getenv("HOME");
    if (home) dirs.push_back(std::string(home) + "/.local/share/applications");

    m_all_apps = AppDiscovery::scan_applications(dirs);
    m_apps     = m_all_apps;

    // ── Outer Box ────────────────────────────────────────────────────────────
    m_outer_box.set_spacing(6);
    m_outer_box.set_margin_start(16);
    m_outer_box.set_margin_end(16);
    m_outer_box.set_margin_top(16);
    m_outer_box.set_margin_bottom(16);
    add(m_outer_box);

    // ── Search Frame ──────────────────────────────────────────────────────────
    m_search_frame.set_label(" [ search ] ");
    m_search_box.set_spacing(8);
    m_search_box.set_margin_top(4);
    m_search_box.set_margin_bottom(4);
    m_search_box.set_margin_start(12);
    m_search_box.set_margin_end(12);

    m_search_prompt.set_text(">");
    m_search_prompt.get_style_context()->add_class("search-prompt");

    m_search_entry.set_placeholder_text("Escribe para filtrar");
    m_search_entry.set_hexpand(true);
    m_search_entry.set_can_focus(true);
    m_search_entry.signal_changed().connect([this]() { on_search_changed(); });

    m_view_toggle_box.set_spacing(0);
    auto grid_img = Gtk::manage(new Gtk::Image());
    try { grid_img->set(Gdk::Pixbuf::create_from_file(resolve_icon_path("src/icons/grid_amber.svg"), 16, 16, true)); } catch (...) {}
    m_btn_grid.set_image(*grid_img);
    m_btn_grid.set_always_show_image(true);
    m_btn_grid.get_style_context()->add_class("view-btn");
    m_btn_grid.get_style_context()->add_class("active");
    m_btn_grid.signal_clicked().connect([this]() { toggle_view_mode("grid"); });

    auto list_img = Gtk::manage(new Gtk::Image());
    try { list_img->set(Gdk::Pixbuf::create_from_file(resolve_icon_path("src/icons/list_amber.svg"), 16, 16, true)); } catch (...) {}
    m_btn_list.set_image(*list_img);
    m_btn_list.set_always_show_image(true);
    m_btn_list.get_style_context()->add_class("view-btn");
    m_btn_list.signal_clicked().connect([this]() { toggle_view_mode("list"); });

    m_view_toggle_box.pack_start(m_btn_grid, Gtk::PACK_SHRINK);
    m_view_toggle_box.pack_start(m_btn_list, Gtk::PACK_SHRINK);

    m_search_box.pack_start(m_search_prompt, Gtk::PACK_SHRINK);
    m_search_box.pack_start(m_search_entry, Gtk::PACK_EXPAND_WIDGET);
    m_search_box.pack_start(m_view_toggle_box, Gtk::PACK_SHRINK);

    m_search_frame.add(m_search_box);
    m_outer_box.pack_start(m_search_frame, Gtk::PACK_SHRINK);

    // ── Middle Layout ────────────────────────────────────────────────────────
    m_middle_box.set_spacing(8);
    m_outer_box.pack_start(m_middle_box, Gtk::PACK_EXPAND_WIDGET);

    // ── Sidebar Frame (Tags) ──────────────────────────────────────────────────
    m_sidebar_frame.set_label(" [ tags ] ");
    m_sidebar_box.set_spacing(2);
    m_sidebar_box.set_margin_start(4);
    m_sidebar_box.set_margin_end(4);
    m_sidebar_box.set_margin_top(6);
    m_sidebar_box.set_margin_bottom(6);
    m_sidebar_frame.add(m_sidebar_box);
    m_middle_box.pack_start(m_sidebar_frame, Gtk::PACK_SHRINK);
    m_sidebar_frame.set_size_request(180, -1);

    std::set<std::string> populated;
    for (const auto& app : m_all_apps)
        populated.insert(detect_category(app.categories));

    auto make_tag_btn = [&](const std::string& id, const std::string& name,
                            const std::string& svg_path, bool active) {
        auto btn_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 8));
        auto icon_img = Gtk::manage(new Gtk::Image());
        try {
            auto pb = Gdk::Pixbuf::create_from_file(resolve_icon_path(svg_path), 16, 16, true);
            icon_img->set(pb);
        } catch (...) {
            icon_img->set_from_icon_name("image-missing", Gtk::ICON_SIZE_MENU);
        }
        auto text_lbl = Gtk::manage(new Gtk::Label(name));
        text_lbl->set_halign(Gtk::ALIGN_START);
        btn_box->pack_start(*icon_img, Gtk::PACK_SHRINK);
        btn_box->pack_start(*text_lbl, Gtk::PACK_SHRINK);

        auto btn = Gtk::manage(new Gtk::Button());
        btn->add(*btn_box);
        btn->set_can_focus(true);
        btn->get_style_context()->add_class("category-btn");
        if (active) btn->get_style_context()->add_class("active");

        const std::string cid = id;
        btn->signal_clicked().connect([this, cid]() { set_category(cid); });
        m_sidebar_box.pack_start(*btn, Gtk::PACK_SHRINK);
        m_cat_buttons.push_back({id, btn});
    };

    // Todas + the two special tags (favorites / recents) always at the top.
    make_tag_btn("all",       "Todas",     "src/icons/grid_amber.svg",  true);
    make_tag_btn("favorites", "Favoritos", "src/icons/star_amber.svg",  false);
    make_tag_btn("recents",   "Recientes", "src/icons/clock_amber.svg", false);

    for (const auto& cat : CATEGORIES) {
        if (cat.id == "all") continue;
        if (populated.find(cat.id) == populated.end()) continue;

        std::string svg_path = "";
        if      (cat.id == "internet") svg_path = "src/icons/globe_amber.svg";
        else if (cat.id == "dev")      svg_path = "src/icons/code_amber.svg";
        else if (cat.id == "media")    svg_path = "src/icons/music_amber.svg";
        else if (cat.id == "system")   svg_path = "src/icons/settings_amber.svg";
        else if (cat.id == "android")  svg_path = "src/icons/android_amber.svg";
        else if (cat.id == "office")   svg_path = "src/icons/document_amber.svg";
        else if (cat.id == "games")    svg_path = "src/icons/gamepad_amber.svg";
        make_tag_btn(cat.id, cat.name, svg_path, false);
    }

    // ── Apps Frame ───────────────────────────────────────────────────────────
    m_apps_frame.set_label(" [ apps ] ");
    m_apps_scrolled.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    m_apps_scrolled.add(m_apps_scroll_content);
    m_apps_frame.add(m_apps_scrolled);
    m_middle_box.pack_start(m_apps_frame, Gtk::PACK_EXPAND_WIDGET);

    m_apps_scroll_content.set_spacing(16);
    m_apps_scroll_content.set_hexpand(true);
    m_apps_scroll_content.set_halign(Gtk::ALIGN_FILL);
    m_apps_scroll_content.set_margin_start(12);
    m_apps_scroll_content.set_margin_end(12);
    m_apps_scroll_content.set_margin_top(8);
    m_apps_scroll_content.set_margin_bottom(8);

    // Favorites Grid Setup
    m_favorites_container.set_spacing(8);
    m_favorites_header.set_spacing(8);
    auto fav_img = Gtk::manage(new Gtk::Image());
    try { fav_img->set(Gdk::Pixbuf::create_from_file(resolve_icon_path("src/icons/star_amber.svg"), 16, 16, true)); } catch(...) {}
    m_favorites_label.get_style_context()->add_class("section-header-label");
    m_favorites_label.set_text("FAVORITOS");
    m_favorites_sep.set_hexpand(true);
    m_favorites_sep.get_style_context()->add_class("section-separator");
    m_favorites_header.pack_start(*fav_img, Gtk::PACK_SHRINK);
    m_favorites_header.pack_start(m_favorites_label, Gtk::PACK_SHRINK);
    m_favorites_header.pack_start(m_favorites_sep, Gtk::PACK_EXPAND_WIDGET);
    m_favorites_grid.set_column_spacing(12);
    m_favorites_grid.set_row_spacing(12);
    m_favorites_grid.set_column_homogeneous(true);
    m_favorites_grid.set_hexpand(true);
    m_favorites_grid.set_halign(Gtk::ALIGN_FILL);
    m_favorites_container.set_hexpand(true);
    m_favorites_container.set_halign(Gtk::ALIGN_FILL);
    m_favorites_container.pack_start(m_favorites_header, Gtk::PACK_SHRINK);
    m_favorites_container.pack_start(m_favorites_grid, Gtk::PACK_SHRINK);
    m_apps_scroll_content.pack_start(m_favorites_container, Gtk::PACK_SHRINK);

    // Recents Grid Setup
    m_recents_container.set_spacing(8);
    m_recents_header.set_spacing(8);
    auto rec_img = Gtk::manage(new Gtk::Image());
    try { rec_img->set(Gdk::Pixbuf::create_from_file(resolve_icon_path("src/icons/clock_amber.svg"), 16, 16, true)); } catch(...) {}
    m_recents_label.get_style_context()->add_class("section-header-label");
    m_recents_label.set_text("RECIENTES");
    m_recents_sep.set_hexpand(true);
    m_recents_sep.get_style_context()->add_class("section-separator");
    m_recents_header.pack_start(*rec_img, Gtk::PACK_SHRINK);
    m_recents_header.pack_start(m_recents_label, Gtk::PACK_SHRINK);
    m_recents_header.pack_start(m_recents_sep, Gtk::PACK_EXPAND_WIDGET);
    m_recents_grid.set_column_spacing(12);
    m_recents_grid.set_row_spacing(12);
    m_recents_grid.set_column_homogeneous(true);
    m_recents_grid.set_hexpand(true);
    m_recents_grid.set_halign(Gtk::ALIGN_FILL);
    m_recents_container.set_hexpand(true);
    m_recents_container.set_halign(Gtk::ALIGN_FILL);
    m_recents_container.pack_start(m_recents_header, Gtk::PACK_SHRINK);
    m_recents_container.pack_start(m_recents_grid, Gtk::PACK_SHRINK);
    m_apps_scroll_content.pack_start(m_recents_container, Gtk::PACK_SHRINK);

    // All Apps Grid Setup
    m_all_container.set_spacing(8);
    m_all_header.set_spacing(8);
    auto all_img = Gtk::manage(new Gtk::Image());
    try { all_img->set(Gdk::Pixbuf::create_from_file(resolve_icon_path("src/icons/grid_amber.svg"), 16, 16, true)); } catch(...) {}
    m_all_label.get_style_context()->add_class("section-header-label");
    m_all_label.set_text("TODAS");
    m_all_sep.set_hexpand(true);
    m_all_sep.get_style_context()->add_class("section-separator");
    m_all_header.pack_start(*all_img, Gtk::PACK_SHRINK);
    m_all_header.pack_start(m_all_label, Gtk::PACK_SHRINK);
    m_all_header.pack_start(m_all_sep, Gtk::PACK_EXPAND_WIDGET);
    m_all_grid.set_column_spacing(12);
    m_all_grid.set_row_spacing(12);
    m_all_grid.set_column_homogeneous(true);
    m_all_grid.set_hexpand(true);
    m_all_grid.set_halign(Gtk::ALIGN_FILL);
    m_all_container.set_hexpand(true);
    m_all_container.set_halign(Gtk::ALIGN_FILL);
    m_all_container.pack_start(m_all_header, Gtk::PACK_SHRINK);
    m_all_container.pack_start(m_all_grid, Gtk::PACK_SHRINK);
    m_apps_scroll_content.pack_start(m_all_container, Gtk::PACK_SHRINK);

    // ── Status Frame ─────────────────────────────────────────────────────────
    m_status_frame.set_label(" [ status ] ");
    m_status_box.set_spacing(16);
    m_status_box.set_margin_start(12);
    m_status_box.set_margin_end(12);
    m_status_box.set_margin_top(4);
    m_status_box.set_margin_bottom(4);

    m_ws_label.set_text("> ~/apps");
    m_ws_label.get_style_context()->add_class("status-ws");

    m_app_count_label.get_style_context()->add_class("status-text");
    m_cat_name_label.get_style_context()->add_class("status-text");

    m_status_box.pack_start(m_ws_label, Gtk::PACK_SHRINK);
    m_status_box.pack_start(m_app_count_label, Gtk::PACK_SHRINK);
    m_status_box.pack_start(m_cat_name_label, Gtk::PACK_SHRINK);

    m_shortcuts_box.set_spacing(4);
    m_shortcuts_box.set_halign(Gtk::ALIGN_END);
    m_status_box.pack_end(m_shortcuts_box, Gtk::PACK_SHRINK);

    auto add_shortcut = [this](const std::string& key, const std::string& desc) {
        auto key_lbl = Gtk::manage(new Gtk::Label(key));
        key_lbl->get_style_context()->add_class("key-cap");
        auto desc_lbl = Gtk::manage(new Gtk::Label(desc));
        desc_lbl->get_style_context()->add_class("key-desc");
        m_shortcuts_box.pack_start(*key_lbl, Gtk::PACK_SHRINK);
        m_shortcuts_box.pack_start(*desc_lbl, Gtk::PACK_SHRINK);
    };

    // Navigation shortcut with inverted T arrow keys
    auto nav_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 6));
    nav_box->set_valign(Gtk::ALIGN_CENTER);

    auto arrow_grid = Gtk::manage(new Gtk::Grid());
    arrow_grid->set_row_spacing(1);
    arrow_grid->set_column_spacing(1);
    arrow_grid->set_valign(Gtk::ALIGN_CENTER);

    auto lbl_up = Gtk::manage(new Gtk::Label("▲"));
    lbl_up->get_style_context()->add_class("key-cap-arrow");
    lbl_up->set_size_request(9, 7);

    auto lbl_left = Gtk::manage(new Gtk::Label("◀"));
    lbl_left->get_style_context()->add_class("key-cap-arrow");
    lbl_left->set_size_request(9, 7);

    auto lbl_down = Gtk::manage(new Gtk::Label("▼"));
    lbl_down->get_style_context()->add_class("key-cap-arrow");
    lbl_down->set_size_request(9, 7);

    auto lbl_right = Gtk::manage(new Gtk::Label("▶"));
    lbl_right->get_style_context()->add_class("key-cap-arrow");
    lbl_right->set_size_request(9, 7);

    // Up arrow alone on top row, centered above down
    arrow_grid->attach(*lbl_up, 1, 0, 1, 1);
    // Left, Down, Right all on bottom row
    arrow_grid->attach(*lbl_left, 0, 1, 1, 1);
    arrow_grid->attach(*lbl_down, 1, 1, 1, 1);
    arrow_grid->attach(*lbl_right, 2, 1, 1, 1);

    auto nav_desc = Gtk::manage(new Gtk::Label("navegar"));
    nav_desc->get_style_context()->add_class("key-desc");
    nav_desc->set_valign(Gtk::ALIGN_CENTER);

    nav_box->pack_start(*arrow_grid, Gtk::PACK_SHRINK);
    nav_box->pack_start(*nav_desc, Gtk::PACK_SHRINK);

    m_shortcuts_box.pack_start(*nav_box, Gtk::PACK_SHRINK);

    add_shortcut("↵", "abrir");
    add_shortcut("f", "favorito");
    add_shortcut("esc", "cerrar");

    // ── Refresh & Settings buttons ────────────────────────────────────────
    m_btn_refresh.set_label("↻");
    m_btn_refresh.set_tooltip_text("Re-escanear aplicaciones instaladas");
    m_btn_refresh.get_style_context()->add_class("view-btn");
    m_btn_refresh.signal_clicked().connect([this]() { refresh_apps(); });
    m_status_box.pack_end(m_btn_refresh, Gtk::PACK_SHRINK);
    m_status_box.pack_end(*Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL)), Gtk::PACK_SHRINK);

    m_btn_settings.set_label("⚙");
    m_btn_settings.set_tooltip_text("Configurar tema y tecla rápida");
    m_btn_settings.get_style_context()->add_class("view-btn");
    m_btn_settings.signal_clicked().connect([this]() { show_settings_dialog(); });
    m_status_box.pack_end(m_btn_settings, Gtk::PACK_SHRINK);
    m_status_box.pack_end(*Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL)), Gtk::PACK_SHRINK);

    m_status_frame.add(m_status_box);
    m_outer_box.pack_start(m_status_frame, Gtk::PACK_SHRINK);

    m_outer_box.show_all();

    // Defer the icon-heavy first population so the window paints (opens) right
    // away and the grid fills in a moment later, instead of blocking the open
    // while ~130 icons decode. Lower-than-redraw priority => paint happens first.
    Glib::signal_idle().connect_once([this]() { rebuild_all(); }, Glib::PRIORITY_DEFAULT_IDLE);

    // Park initial focus on the search box so no icon starts "selected".
    m_search_entry.grab_focus();
    signal_show().connect([this]() { m_search_entry.grab_focus(); });
}

void LauncherWindow::rebuild_all() {
    // Favorites/Recents sections only show on the "Todas" tag with no active
    // search. Any other tag (including the Favoritos/Recientes tags themselves)
    // shows just its own list in the main grid.
    bool show_sections = (m_current_category == "all") && m_search_entry.get_text().empty();

    // 1. Rebuild Favorites
    for (auto* child : m_favorites_grid.get_children())
        m_favorites_grid.remove(*child);

    // Newest-added favorite on the left, capped to HOME_SECTION_MAX. m_favorites
    // is stored oldest→newest (append on add), so walk it in reverse.
    std::vector<AppEntry> fav_entries;
    for (auto it = m_favorites.rbegin();
         it != m_favorites.rend() && (int)fav_entries.size() < HOME_SECTION_MAX; ++it) {
        for (const auto& app : m_all_apps) {
            if (app.name == *it) {
                fav_entries.push_back(app);
                break;
            }
        }
    }
    if (fav_entries.empty() || !show_sections) {
        m_favorites_container.hide();
    } else {
        populate_grid(m_favorites_grid, fav_entries);
        m_favorites_container.show();
    }

    // 2. Rebuild Recents
    for (auto* child : m_recents_grid.get_children())
        m_recents_grid.remove(*child);

    // m_recents is already newest→oldest (insert at front on open); cap to
    // HOME_SECTION_MAX so the home shows only the latest few.
    std::vector<AppEntry> rec_entries;
    for (const auto& rec_name : m_recents) {
        if ((int)rec_entries.size() >= HOME_SECTION_MAX) break;
        for (const auto& app : m_all_apps) {
            if (app.name == rec_name) {
                rec_entries.push_back(app);
                break;
            }
        }
    }
    if (rec_entries.empty() || !show_sections) {
        m_recents_container.hide();
    } else {
        populate_grid(m_recents_grid, rec_entries);
        m_recents_container.show();
    }

    // 3. Rebuild All Apps
    for (auto* child : m_all_grid.get_children())
        m_all_grid.remove(*child);

    populate_grid(m_all_grid, m_apps);

    m_favorites_grid.show_all();
    m_recents_grid.show_all();
    m_all_grid.show_all();

    update_status();
}

void LauncherWindow::populate_grid(Gtk::Grid& grid, const std::vector<AppEntry>& list) {
    int cols = (m_view_mode == "list") ? 1 : COLS;
    for (size_t i = 0; i < list.size(); ++i) {
        const auto& entry = list[i];
        bool is_fav = std::find(m_favorites.begin(), m_favorites.end(), entry.name) != m_favorites.end();
        
        auto* btn = Gtk::manage(new AppIconButton(
            entry, is_fav, m_view_mode == "list",
            [this](const std::string& name) { toggle_favorite(name); },
            detect_category(entry.categories),
            [this](const std::string& n, const std::string& c) { return app_has_tag(n, c); },
            [this](const std::string& n, const std::string& c, bool on) { set_app_tag(n, c, on); }));
        btn->signal_clicked().connect([this, entry]() { on_app_clicked(entry); });
        
        if (m_view_mode == "list") {
            btn->set_hexpand(true);
            grid.attach(*btn, 0, i, 1, 1);
        } else {
            grid.attach(*btn, i % cols, i / cols, 1, 1);
        }
    }

    // Force all columns 0 to COLS-1 to exist in grid mode to align icons perfectly
    if (m_view_mode != "list" && !list.empty()) {
        for (int c = 0; c < COLS; ++c) {
            bool occupied = false;
            for (size_t i = 0; i < list.size(); ++i) {
                if ((int)(i % COLS) == c) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                auto* dummy = Gtk::manage(new Gtk::Box());
                dummy->set_size_request(96, 1);
                grid.attach(*dummy, c, 0, 1, 1);
                dummy->show();
            }
        }
    }
}

void LauncherWindow::set_category(const std::string& cat_id) {
    m_close_inhibited = true;
    m_current_category = cat_id;

    if (!m_search_entry.get_text().empty()) {
        m_search_entry.set_text(""); // Clear search to restore view
    }

    auto entries_for = [this](const std::vector<std::string>& names) {
        std::vector<AppEntry> out;
        for (const auto& name : names)
            for (const auto& app : m_all_apps)
                if (app.name == name) { out.push_back(app); break; }
        return out;
    };

    if (cat_id == "all") {
        m_apps = m_all_apps;
    } else if (cat_id == "favorites") {
        m_apps = entries_for(m_favorites);
    } else if (cat_id == "recents") {
        m_apps = entries_for(m_recents);
    } else {
        m_apps.clear();
        for (const auto& app : m_all_apps) {
            // Auto-detected category OR a manual tag the user assigned.
            if (detect_category(app.categories) == cat_id || app_has_tag(app.name, cat_id))
                m_apps.push_back(app);
        }
    }

    for (auto& [id, btn] : m_cat_buttons) {
        auto ctx = btn->get_style_context();
        if (id == cat_id) ctx->add_class("active");
        else              ctx->remove_class("active");
    }

    rebuild_all();
}

void LauncherWindow::update_status() {
    m_app_count_label.set_text(std::to_string(m_apps.size()) + " aplicaciones");

    std::string cat_label = "Todas";
    if (m_current_category == "favorites") {
        cat_label = "Favoritos";
    } else if (m_current_category == "recents") {
        cat_label = "Recientes";
    } else {
        for (const auto& cat : CATEGORIES) {
            if (cat.id == m_current_category) {
                cat_label = cat.name;
                break;
            }
        }
    }
    m_cat_name_label.set_text(cat_label);
}

void LauncherWindow::on_search_changed() {
    std::string query = m_search_entry.get_text();
    std::string lower_query = Glib::ustring(query).casefold();

    if (lower_query.empty()) {
        set_category(m_current_category);
        return;
    }

    m_apps.clear();
    for (const auto& app : m_all_apps) {
        std::string lower_name = Glib::ustring(app.name).casefold();
        if (lower_name.find(lower_query) != std::string::npos) {
            m_apps.push_back(app);
        }
    }
    rebuild_all();
}

void LauncherWindow::on_app_clicked(const AppEntry& entry) {
    add_to_recents(entry.name);
    try {
        auto argv = Glib::shell_parse_argv(entry.exec);
        if (!argv.empty())
            Glib::spawn_async("", argv, Glib::SPAWN_SEARCH_PATH);
        else
            std::cerr << "Empty command line for: " << entry.name << std::endl;
    } catch (const Glib::Error& ex) {
        std::cerr << "Failed to run: " << ex.what() << std::endl;
    }
    hide_launcher();
}

void LauncherWindow::toggle_view_mode(const std::string& mode) {
    if (m_view_mode == mode) return;
    m_view_mode = mode;

    if (mode == "grid") {
        m_btn_grid.get_style_context()->add_class("active");
        m_btn_list.get_style_context()->remove_class("active");
    } else {
        m_btn_grid.get_style_context()->remove_class("active");
        m_btn_list.get_style_context()->add_class("active");
    }

    rebuild_all();
}

bool LauncherWindow::on_key_press_event(GdkEventKey* event) {
    if (event->keyval == GDK_KEY_Escape) {
        hide_launcher();
        return true;
    }

    // Enter launches the keyboard-selected icon.
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        if (auto* app_btn = dynamic_cast<AppIconButton*>(get_focus())) {
            on_app_clicked(app_btn->get_entry());
            return true;
        }
    }

    // 'f' toggles favorite on the selected icon (ignored while typing in search,
    // since focus is then the entry, not an icon).
    if (event->keyval == GDK_KEY_f || event->keyval == GDK_KEY_F) {
        if (auto* app_btn = dynamic_cast<AppIconButton*>(get_focus())) {
            toggle_favorite(app_btn->get_entry().name);
            return true;
        }
    }

    if (event->keyval == GDK_KEY_Left || event->keyval == GDK_KEY_Right ||
        event->keyval == GDK_KEY_Up || event->keyval == GDK_KEY_Down) {
        if (handle_custom_navigation(event->keyval)) {
            return true;
        }
    }

    return Gtk::Window::on_key_press_event(event);
}

bool LauncherWindow::handle_custom_navigation(unsigned int keyval) {
    auto* focused = get_focus();
    if (!focused) {
        m_search_entry.grab_focus();
        return true;
    }

    if (focused == &m_search_entry) {
        if (keyval == GDK_KEY_Down) {
            if (m_favorites_container.get_visible() && !m_favorites_grid.get_children().empty()) {
                m_favorites_grid.get_children().back()->grab_focus();
                return true;
            }
            if (m_recents_container.get_visible() && !m_recents_grid.get_children().empty()) {
                m_recents_grid.get_children().back()->grab_focus();
                return true;
            }
            if (!m_all_grid.get_children().empty()) {
                m_all_grid.get_children().back()->grab_focus();
                return true;
            }
            if (!m_cat_buttons.empty()) {
                m_cat_buttons[0].second->grab_focus();
                return true;
            }
        }
        return false;
    }

    for (size_t i = 0; i < m_cat_buttons.size(); ++i) {
        if (focused == m_cat_buttons[i].second) {
            if (keyval == GDK_KEY_Up) {
                if (i > 0) {
                    m_cat_buttons[i-1].second->grab_focus();
                } else {
                    m_search_entry.grab_focus();
                }
                return true;
            } else if (keyval == GDK_KEY_Down) {
                if (i + 1 < m_cat_buttons.size()) {
                    m_cat_buttons[i+1].second->grab_focus();
                }
                return true;
            } else if (keyval == GDK_KEY_Right) {
                if (m_favorites_container.get_visible() && !m_favorites_grid.get_children().empty()) {
                    m_favorites_grid.get_children().back()->grab_focus();
                    return true;
                }
                if (m_recents_container.get_visible() && !m_recents_grid.get_children().empty()) {
                    m_recents_grid.get_children().back()->grab_focus();
                    return true;
                }
                if (!m_all_grid.get_children().empty()) {
                    m_all_grid.get_children().back()->grab_focus();
                    return true;
                }
            }
            return false;
        }
    }

    auto* app_btn = dynamic_cast<AppIconButton*>(focused);
    if (app_btn) {
        Gtk::Grid* grid = nullptr;
        Gtk::Widget* parent = app_btn->get_parent();
        while (parent) {
            if (parent == &m_favorites_grid) { grid = &m_favorites_grid; break; }
            if (parent == &m_recents_grid)   { grid = &m_recents_grid;   break; }
            if (parent == &m_all_grid)       { grid = &m_all_grid;       break; }
            parent = parent->get_parent();
        }

        if (!grid) return false;

        struct GridCell {
            AppIconButton* btn;
            int x, y;
        };
        std::vector<GridCell> cells;
        for (auto* child : grid->get_children()) {
            auto* b = dynamic_cast<AppIconButton*>(child);
            if (b) {
                int left = grid->child_property_left_attach(*b).get_value();
                int top = grid->child_property_top_attach(*b).get_value();
                cells.push_back({b, left, top});
            }
        }

        int cx = -1, cy = -1;
        for (const auto& cell : cells) {
            if (cell.btn == app_btn) {
                cx = cell.x;
                cy = cell.y;
                break;
            }
        }

        if (cx == -1 || cy == -1) return false;

        if (keyval == GDK_KEY_Left) {
            AppIconButton* next_btn = nullptr;
            int best_x = -1;
            for (const auto& cell : cells) {
                if (cell.y == cy && cell.x < cx && cell.x > best_x) {
                    best_x = cell.x;
                    next_btn = cell.btn;
                }
            }
            if (next_btn) {
                next_btn->grab_focus();
                return true;
            } else {
                if (!m_cat_buttons.empty()) {
                    for (auto& [id, btn] : m_cat_buttons) {
                        if (id == m_current_category) {
                            btn->grab_focus();
                            return true;
                        }
                    }
                    m_cat_buttons[0].second->grab_focus();
                    return true;
                }
            }
        } else if (keyval == GDK_KEY_Right) {
            AppIconButton* next_btn = nullptr;
            int best_x = 9999;
            for (const auto& cell : cells) {
                if (cell.y == cy && cell.x > cx && cell.x < best_x) {
                    best_x = cell.x;
                    next_btn = cell.btn;
                }
            }
            if (next_btn) {
                next_btn->grab_focus();
                return true;
            }
        } else if (keyval == GDK_KEY_Up) {
            AppIconButton* next_btn = nullptr;
            int best_y = -1;
            for (const auto& cell : cells) {
                if (cell.x == cx && cell.y < cy && cell.y > best_y) {
                    best_y = cell.y;
                    next_btn = cell.btn;
                }
            }
            if (next_btn) {
                next_btn->grab_focus();
                return true;
            } else {
                if (grid == &m_all_grid) {
                    if (m_recents_container.get_visible() && !m_recents_grid.get_children().empty()) {
                        m_recents_grid.get_children().back()->grab_focus();
                        return true;
                    }
                    if (m_favorites_container.get_visible() && !m_favorites_grid.get_children().empty()) {
                        m_favorites_grid.get_children().back()->grab_focus();
                        return true;
                    }
                } else if (grid == &m_recents_grid) {
                    if (m_favorites_container.get_visible() && !m_favorites_grid.get_children().empty()) {
                        m_favorites_grid.get_children().back()->grab_focus();
                        return true;
                    }
                }
                m_search_entry.grab_focus();
                return true;
            }
        } else if (keyval == GDK_KEY_Down) {
            AppIconButton* next_btn = nullptr;
            int best_y = 9999;
            for (const auto& cell : cells) {
                if (cell.x == cx && cell.y > cy && cell.y < best_y) {
                    best_y = cell.y;
                    next_btn = cell.btn;
                }
            }
            if (next_btn) {
                next_btn->grab_focus();
                return true;
            } else {
                if (grid == &m_favorites_grid) {
                    if (m_recents_container.get_visible() && !m_recents_grid.get_children().empty()) {
                        m_recents_grid.get_children().back()->grab_focus();
                        return true;
                    }
                    if (!m_all_grid.get_children().empty()) {
                        m_all_grid.get_children().back()->grab_focus();
                        return true;
                    }
                } else if (grid == &m_recents_grid) {
                    if (!m_all_grid.get_children().empty()) {
                        m_all_grid.get_children().back()->grab_focus();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool LauncherWindow::on_button_release_event(GdkEventButton* event) {
    if (event->window != get_window()->gobj()) {
        return Gtk::Window::on_button_release_event(event);
    }

    if (m_close_inhibited) {
        m_close_inhibited = false;
        return Gtk::Window::on_button_release_event(event);
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - m_created_at).count();
    if (elapsed < 400) return true;

    if (event->button == 1) {
        int ex = static_cast<int>(event->x);
        int ey = static_cast<int>(event->y);

        auto inside = [&](Gtk::Widget& w) -> bool {
            int wx = 0, wy = 0;
            w.translate_coordinates(*this, 0, 0, wx, wy);
            Gtk::Allocation a = w.get_allocation();
            return ex >= wx && ex < wx + a.get_width() &&
                   ey >= wy && ey < wy + a.get_height();
        };

        // Close only when clicking the empty background areas
        if (!inside(m_search_frame) && !inside(m_sidebar_frame) &&
            !inside(m_apps_frame) && !inside(m_status_frame)) {
            hide_launcher();
            return true;
        }
    }
    return Gtk::Window::on_button_release_event(event);
}

bool LauncherWindow::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->save();
    // Near-opaque dark backdrop. A previous frosted-glass experiment lowered the
    // alpha to 0.55 to let picom blur the desktop through the window, but that
    // left the launcher washed-out/illegible when the compositor isn't blurring,
    // so it's restored to a solid dark fill.
    cr->set_source_rgba(0.04, 0.04, 0.05, 0.96);
    cr->set_operator(Cairo::OPERATOR_SOURCE);
    cr->paint();
    cr->restore();
    return Gtk::Window::on_draw(cr);
}
