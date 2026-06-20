#pragma once
// ── Vanilux on-screen keyboard ──────────────────────────────────────────────
// A Cairo-drawn MacBook-Air-style chiclet keyboard used by the hotkey picker.
// Two modes:
//   • F_KEY  — pick one of F1..F12 (those keys are backlit; the rest are dim).
//   • SUPER  — Super+<key>: both Super keys glow in the accent color, every other
//              key is backlit, and you click one to be the combo partner.
// Emits signal_changed() whenever the selection changes; get_binding() returns a
// gsettings-style string ("F4", "<Super>space", …) or "" when nothing is chosen.
#include <gtkmm.h>
#include <string>
#include <vector>

class KeyboardWidget : public Gtk::DrawingArea {
public:
    enum class Mode { F_KEY, SUPER };

    KeyboardWidget();

    void        set_mode(Mode m);
    Mode        get_mode() const { return m_mode; }

    // Switch the alphabetic layout to match a UI language (e.g. FR → AZERTY,
    // ES adds ñ, PT adds ç); everything else stays QWERTY. Keeps the selection.
    void        set_layout(const std::string& lang_code);

    void        set_binding(const std::string& binding);   // parse "F4" / "<Super>x"
    std::string get_binding() const;                        // "" if incomplete
    std::string get_display() const;                        // human label e.g. "Super + A"

    // Accent color for backlit/selected keys (0..1). Call on theme change.
    void set_accent(double r, double g, double b);

    sigc::signal<void>& signal_changed() { return m_signal_changed; }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_button_press_event(GdkEventButton* e) override;

private:
    enum class Kind { NORMAL, FUNC, SUPER_L, SUPER_R, MOD, SPACE, ARROW };

    struct Key {
        std::string label;    // glyph drawn on the cap
        std::string name;     // gdk/gsettings key name (binding partner)
        double      units;    // relative width within its row
        Kind        kind;
        int         fnum = 0; // 1..12 for FUNC keys
    };

    // Geometry of a key as laid out for the current allocation.
    struct Rect { const Key* key; double x, y, w, h; };

    void build_layout();                 // fills m_rows (static key data) once
    std::vector<Rect> compute_rects() const;  // pixel rects for current size
    bool selectable(const Key& k) const; // can this key be picked in current mode?

    std::vector<std::vector<Key>> m_rows;
    std::string m_layout = "qwerty";   // qwerty | azerty
    std::string m_special;             // extra home-row letter: "n~"/"c," → ñ/ç
    Mode m_mode = Mode::F_KEY;
    int  m_sel_f = 0;            // selected F number (1..12), 0 = none
    std::string m_sel_super;     // selected partner key name in SUPER mode

    double m_ar = 224.0/255.0, m_ag = 153.0/255.0, m_ab = 36.0/255.0;
    sigc::signal<void> m_signal_changed;
};
