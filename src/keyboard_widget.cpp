#include "keyboard_widget.hpp"
#include <cmath>
#include <sstream>

KeyboardWidget::KeyboardWidget() {
    // Fixed-ish size — NOT hexpand: an expanding keyboard dragged the whole
    // settings card to full width and broke its centering.
    set_size_request(500, 170);
    add_events(Gdk::BUTTON_PRESS_MASK);
    build_layout();
}

void KeyboardWidget::build_layout() {
    using K = Key;
    auto N  = [](std::string l, std::string n)            { return K{l, n, 1.0, Kind::NORMAL}; };
    auto MOD= [](std::string l, std::string n, double u)  { return K{l, n, u, Kind::MOD}; };
    auto F  = [](int i)                                   { return K{"F"+std::to_string(i), "F"+std::to_string(i), 1.0, Kind::FUNC, i}; };
    auto ARR= [](std::string l, std::string n)            { return K{l, n, 1.0, Kind::ARROW}; };
    auto split = [](const std::string& s) {
        std::vector<std::string> v; for (char c : s) v.push_back(std::string(1, c)); return v;
    };

    // keynames = physical QWERTY positions used for the actual binding;
    // labels = what's drawn on the cap (latin / kana / hangul per language).
    std::string kn2 = "qwertyuiop", kn3 = "asdfghjkl", kn4 = "zxcvbnm";
    std::vector<std::string> l2, l3, l4;
    if (m_layout == "azerty") {
        kn2 = "azertyuiop"; kn3 = "qsdfghjkl"; kn4 = "wxcvbnm";
        l2 = split(kn2); l3 = split(kn3); l4 = split(kn4);
    } else if (m_layout == "jis") {           // Japanese kana
        l2 = {"た","て","い","す","か","ん","な","に","ら","せ"};
        l3 = {"ち","と","し","は","き","く","ま","の","り"};
        l4 = {"つ","さ","そ","ひ","こ","み","も"};
    } else if (m_layout == "hangul") {         // Korean dubeolsik
        l2 = {"ㅂ","ㅈ","ㄷ","ㄱ","ㅅ","ㅛ","ㅕ","ㅑ","ㅐ","ㅔ"};
        l3 = {"ㅁ","ㄴ","ㅇ","ㄹ","ㅎ","ㅗ","ㅓ","ㅏ","ㅣ"};
        l4 = {"ㅋ","ㅌ","ㅊ","ㅍ","ㅠ","ㅜ","ㅡ"};
    } else if (m_layout == "zhuyin") {          // Chinese Zhuyin (Bopomofo)
        l2 = {"ㄆ","ㄊ","ㄍ","ㄐ","ㄔ","ㄗ","ㄧ","ㄛ","ㄌ","ㄨ"};
        l3 = {"ㄇ","ㄋ","ㄎ","ㄑ","ㄓ","ㄕ","ㄖ","ㄘ","ㄙ"};
        l4 = {"ㄅ","ㄉ","ㄏ","ㄒ","ㄚ","ㄠ","ㄥ"};
    } else {                                   // qwerty (also ES/PT base)
        l2 = split(kn2); l3 = split(kn3); l4 = split(kn4);
    }

    // Append the alpha keys of a row: cap label from labels[i], binding from kn[i].
    auto alpha = [](std::vector<K>& row, const std::vector<std::string>& labels, const std::string& kn) {
        for (size_t i = 0; i < labels.size() && i < kn.size(); ++i)
            row.push_back(K{labels[i], std::string(1, kn[i]), 1.0, Kind::NORMAL});
    };

    // The key in the ';' slot becomes ñ (ES) or ç (PT) when requested.
    K extra = N(";", "semicolon");
    if      (m_special == "ntilde")   extra = N("ñ", "ntilde");
    else if (m_special == "ccedilla") extra = N("ç", "ccedilla");

    m_rows.clear();

    // Function row
    std::vector<K> row0 = { MOD("esc","Escape",1.5) };
    for (int i = 1; i <= 12; ++i) row0.push_back(F(i));
    m_rows.push_back(row0);

    // Number row
    m_rows.push_back({ N("`","grave"), N("1","1"), N("2","2"), N("3","3"), N("4","4"),
        N("5","5"), N("6","6"), N("7","7"), N("8","8"), N("9","9"), N("0","0"),
        N("-","minus"), N("=","equal"), MOD("⌫","BackSpace",1.7) });

    // Top letter row: tab + alpha + bracket/backslash keys
    std::vector<K> row2 = { MOD("⇥","Tab",1.5) };
    alpha(row2, l2, kn2);
    row2.push_back(N("[","bracketleft"));
    row2.push_back(N("]","bracketright"));
    row2.push_back(N("\\","backslash"));
    m_rows.push_back(row2);

    // Home row: caps + alpha + (;/ñ/ç) + ' + enter
    std::vector<K> row3 = { MOD("⇪","Caps_Lock",1.8) };
    alpha(row3, l3, kn3);
    row3.push_back(extra);
    row3.push_back(N("'","apostrophe"));
    row3.push_back(MOD("↵","Return",1.8));
    m_rows.push_back(row3);

    // Bottom letter row: shift + alpha + , . / + shift
    std::vector<K> row4 = { MOD("⇧","Shift_L",2.3) };
    alpha(row4, l4, kn4);
    row4.push_back(N(",","comma"));
    row4.push_back(N(".","period"));
    row4.push_back(N("/","slash"));
    row4.push_back(MOD("⇧","Shift_R",2.3));
    m_rows.push_back(row4);

    // Modifier / space row
    m_rows.push_back({
        MOD("fn","fn",1.1), MOD("⌃","Control_L",1.1), MOD("⌥","Alt_L",1.1),
        K{"⌘","Super_L",1.3, Kind::SUPER_L},
        K{"",       "space", 5.4, Kind::SPACE},
        K{"⌘","Super_R",1.3, Kind::SUPER_R},
        MOD("⌥","Alt_R",1.1),
        ARR("◂","Left"), ARR("▴","Up"), ARR("▾","Down"), ARR("▸","Right")
    });
}

void KeyboardWidget::set_layout(const std::string& code) {
    std::string layout = "qwerty", special;
    if      (code == "FR") layout  = "azerty";
    else if (code == "JA") layout  = "jis";
    else if (code == "KO") layout  = "hangul";
    else if (code == "ZH") layout  = "zhuyin";
    else if (code == "ES") special = "ntilde";
    else if (code == "PT") special = "ccedilla";
    if (layout == m_layout && special == m_special) return;
    m_layout = layout;
    m_special = special;
    build_layout();
    queue_draw();
}

void KeyboardWidget::set_mode(Mode m) {
    if (m_mode == m) return;
    m_mode = m;
    queue_draw();
    m_signal_changed.emit();
}

void KeyboardWidget::set_accent(double r, double g, double b) {
    m_ar = r; m_ag = g; m_ab = b;
    queue_draw();
}

bool KeyboardWidget::selectable(const Key& k) const {
    if (m_mode == Mode::F_KEY) return k.kind == Kind::FUNC;
    return k.kind == Kind::NORMAL || k.kind == Kind::FUNC ||
           k.kind == Kind::SPACE  || k.kind == Kind::ARROW;
}

std::vector<KeyboardWidget::Rect> KeyboardWidget::compute_rects() const {
    std::vector<Rect> out;
    const double W = get_allocated_width(), H = get_allocated_height();
    const int nrows = (int)m_rows.size();
    const double pad = 8.0, gap = 4.0;
    const double rowH = (H - 2*pad - (nrows-1)*gap) / nrows;
    double y = pad;
    for (const auto& row : m_rows) {
        double units = 0; for (const auto& k : row) units += k.units;
        const double avail = W - 2*pad - (row.size()-1)*gap;
        const double uw = avail / units;
        double x = pad;
        for (const auto& k : row) {
            double w = k.units * uw;
            out.push_back({&k, x, y, w, rowH});
            x += w + gap;
        }
        y += rowH + gap;
    }
    return out;
}

static void rounded_rect(const Cairo::RefPtr<Cairo::Context>& cr,
                         double x, double y, double w, double h, double r) {
    if (r > w/2) r = w/2;
    if (r > h/2) r = h/2;
    cr->begin_new_sub_path();
    cr->arc(x+w-r, y+r,   r, -M_PI/2, 0);
    cr->arc(x+w-r, y+h-r, r, 0,        M_PI/2);
    cr->arc(x+r,   y+h-r, r, M_PI/2,   M_PI);
    cr->arc(x+r,   y+r,   r, M_PI,     M_PI*1.5);
    cr->close_path();
}

bool KeyboardWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    const double W = get_allocated_width(), H = get_allocated_height();

    // Keyboard deck
    rounded_rect(cr, 0.5, 0.5, W-1, H-1, 10);
    cr->set_source_rgba(0.07, 0.07, 0.10, 1.0);
    cr->fill_preserve();
    cr->set_source_rgba(m_ar, m_ag, m_ab, 0.30);
    cr->set_line_width(1.0);
    cr->stroke();

    auto rects = compute_rects();
    for (const auto& R : rects) {
        const Key& k = *R.key;

        // Resolve visual state for the current mode/selection.
        enum { DIM, BACKLIT, FILLED, SELECTED } st = DIM;
        if (m_mode == Mode::F_KEY) {
            if (k.kind == Kind::FUNC) st = (k.fnum == m_sel_f) ? SELECTED : BACKLIT;
        } else {
            if (k.kind == Kind::SUPER_L || k.kind == Kind::SUPER_R) st = FILLED;
            else if (selectable(k) && k.name == m_sel_super)         st = SELECTED;
            else                                                     st = BACKLIT;
        }

        const double inset = 1.5;
        const double x = R.x + inset, y = R.y + inset,
                     w = R.w - 2*inset, h = R.h - 2*inset, rad = 4.5;

        // Soft halo for lit keys.
        if (st != DIM) {
            double a = (st == SELECTED) ? 0.34 : (st == FILLED ? 0.30 : 0.16);
            rounded_rect(cr, x-2, y-2, w+4, h+4, rad+2);
            cr->set_source_rgba(m_ar, m_ag, m_ab, a);
            cr->fill();
        }

        // Cap body.
        rounded_rect(cr, x, y, w, h, rad);
        if (st == SELECTED || st == FILLED) {
            double a = (st == SELECTED) ? 0.92 : 0.70;
            cr->set_source_rgba(m_ar, m_ag, m_ab, a);
            cr->fill_preserve();
            cr->set_source_rgba(1, 1, 1, 0.25);
            cr->set_line_width(1.0);
            cr->stroke();
        } else {
            cr->set_source_rgba(0.11, 0.11, 0.15, 1.0);
            cr->fill_preserve();
            if (st == BACKLIT) { cr->set_source_rgba(m_ar, m_ag, m_ab, 0.65); cr->set_line_width(1.2); }
            else               { cr->set_source_rgba(1, 1, 1, 0.08);          cr->set_line_width(1.0); }
            cr->stroke();
        }

        // Label.
        if (!k.label.empty()) {
            auto layout = create_pango_layout(k.label);
            Pango::FontDescription fd("monospace");
            int fs = (int)std::max(6.0, std::min(h * 0.42, 13.0));
            fd.set_size(fs * PANGO_SCALE);
            if (st == SELECTED || st == FILLED) fd.set_weight(Pango::WEIGHT_BOLD);
            layout->set_font_description(fd);
            int lw, lh; layout->get_pixel_size(lw, lh);
            cr->move_to(x + (w-lw)/2, y + (h-lh)/2);
            if (st == SELECTED || st == FILLED)      cr->set_source_rgba(0.06, 0.06, 0.09, 1.0);
            else if (st == BACKLIT) {
                double br = std::min(1.0, m_ar*1.3+0.25), bg = std::min(1.0, m_ag*1.3+0.25), bb = std::min(1.0, m_ab*1.3+0.25);
                cr->set_source_rgba(br, bg, bb, 1.0);
            } else                                   cr->set_source_rgba(1, 1, 1, 0.40);
            layout->show_in_cairo_context(cr);
        }
    }
    return true;
}

bool KeyboardWidget::on_button_press_event(GdkEventButton* e) {
    if (e->type != GDK_BUTTON_PRESS || e->button != 1) return false;
    for (const auto& R : compute_rects()) {
        if (e->x < R.x || e->x >= R.x+R.w || e->y < R.y || e->y >= R.y+R.h) continue;
        const Key& k = *R.key;
        if (!selectable(k)) return true;   // clicking a non-selectable key does nothing
        if (m_mode == Mode::F_KEY) {
            if (k.kind == Kind::FUNC) { m_sel_f = k.fnum; queue_draw(); m_signal_changed.emit(); }
        } else {
            m_sel_super = k.name; queue_draw(); m_signal_changed.emit();
        }
        return true;
    }
    return true;
}

void KeyboardWidget::set_binding(const std::string& b) {
    m_sel_f = 0; m_sel_super.clear();
    if (b.rfind("<Super>", 0) == 0) {
        m_mode = Mode::SUPER;
        m_sel_super = b.substr(7);
    } else if (b.size() >= 2 && (b[0] == 'F' || b[0] == 'f') && isdigit((unsigned char)b[1])) {
        m_mode = Mode::F_KEY;
        try { int n = std::stoi(b.substr(1)); if (n >= 1 && n <= 12) m_sel_f = n; } catch (...) {}
    } else {
        m_mode = Mode::F_KEY;
    }
    queue_draw();
}

std::string KeyboardWidget::get_binding() const {
    if (m_mode == Mode::F_KEY)
        return m_sel_f ? ("F" + std::to_string(m_sel_f)) : "";
    return m_sel_super.empty() ? "" : ("<Super>" + m_sel_super);
}

std::string KeyboardWidget::get_display() const {
    if (m_mode == Mode::F_KEY)
        return m_sel_f ? ("F" + std::to_string(m_sel_f)) : "";
    if (m_sel_super.empty()) return "";
    // Find the key to get a nice label.
    std::string lbl = m_sel_super;
    for (const auto& row : m_rows)
        for (const auto& k : row)
            if (k.name == m_sel_super) { if (!k.label.empty()) lbl = k.label; break; }
    if (lbl.size() == 1 && lbl[0] >= 'a' && lbl[0] <= 'z') lbl[0] = (char)toupper(lbl[0]);
    if (m_sel_super == "space") lbl = "Space";
    return "Super + " + lbl;
}
