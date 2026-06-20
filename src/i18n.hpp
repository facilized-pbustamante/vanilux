#pragma once
// ── Vanilux i18n ────────────────────────────────────────────────────────────
// Tiny in-memory translation table. tr("key") returns the string for the
// currently selected language; set_language() switches it. No gettext / .mo
// files — the whole UI is small enough to keep here, and switching is instant.
#include <string>
#include <array>
#include <unordered_map>

enum class Lang { ES = 0, EN, PT, ZH, FR, JA, KO };
static constexpr int LANG_COUNT = 7;

struct LangMeta {
    Lang        lang;
    const char* code;    // 2-letter chip label
    const char* native;  // native name (tooltip)
    const char* flag;    // icon basename in src/icons/
};

// Display order of the language picker.
static const LangMeta LANGS[LANG_COUNT] = {
    {Lang::ES, "ES", "Español",    "flag_es.svg"},
    {Lang::EN, "EN", "English",    "flag_en.svg"},
    {Lang::PT, "PT", "Português",  "flag_pt.svg"},
    {Lang::ZH, "ZH", "中文",       "flag_zh.svg"},
    {Lang::FR, "FR", "Français",   "flag_fr.svg"},
    {Lang::JA, "JA", "日本語",     "flag_ja.svg"},
    {Lang::KO, "KO", "한국어",     "flag_ko.svg"},
};

inline Lang& current_lang() { static Lang l = Lang::EN; return l; }
inline void  set_language(Lang l) { current_lang() = l; }

inline Lang lang_from_code(const std::string& c) {
    for (const auto& m : LANGS) if (c == m.code) return m.lang;
    return Lang::EN;
}
inline const char* lang_code(Lang l) {
    for (const auto& m : LANGS) if (m.lang == l) return m.code;
    return "EN";
}

// key → { ES, EN, PT, ZH, FR, JA, KO }
inline const std::unordered_map<std::string, std::array<const char*, LANG_COUNT>>& i18n_table() {
    static const std::unordered_map<std::string, std::array<const char*, LANG_COUNT>> T = {
        {"search_placeholder", {"Escribe para filtrar", "Type to filter", "Digite para filtrar", "输入以筛选", "Tapez pour filtrer", "入力して絞り込み", "입력하여 필터"}},
        {"apps_word",          {"aplicaciones", "apps", "aplicativos", "个应用", "applications", "個のアプリ", "개 앱"}},

        {"cat_all",      {"Todas", "All", "Todas", "全部", "Toutes", "すべて", "전체"}},
        {"cat_internet", {"Internet", "Internet", "Internet", "网络", "Internet", "インターネット", "인터넷"}},
        {"cat_dev",      {"Programación", "Development", "Programação", "开发", "Développement", "開発", "개발"}},
        {"cat_media",    {"Multimedia", "Media", "Multimídia", "多媒体", "Multimédia", "メディア", "미디어"}},
        {"cat_system",   {"Sistema", "System", "Sistema", "系统", "Système", "システム", "시스템"}},
        {"cat_android",  {"Android", "Android", "Android", "安卓", "Android", "Android", "안드로이드"}},
        {"cat_office",   {"Oficina", "Office", "Escritório", "办公", "Bureau", "オフィス", "오피스"}},
        {"cat_games",    {"Juegos", "Games", "Jogos", "游戏", "Jeux", "ゲーム", "게임"}},
        {"cat_other",    {"Otros", "Other", "Outros", "其他", "Autres", "その他", "기타"}},

        {"tag_favorites", {"Favoritos", "Favorites", "Favoritos", "收藏", "Favoris", "お気に入り", "즐겨찾기"}},
        {"tag_recents",   {"Recientes", "Recent", "Recentes", "最近", "Récents", "最近", "최근"}},

        {"sc_navigate", {"navegar", "navigate", "navegar", "导航", "naviguer", "移動", "이동"}},
        {"sc_open",     {"abrir", "open", "abrir", "打开", "ouvrir", "開く", "열기"}},
        {"sc_favorite", {"favorito", "favorite", "favorito", "收藏", "favori", "お気に入り", "즐겨찾기"}},
        {"sc_close",    {"cerrar", "close", "fechar", "关闭", "fermer", "閉じる", "닫기"}},

        {"frame_search", {"buscar", "search", "buscar", "搜索", "rechercher", "検索", "검색"}},
        {"frame_tags",   {"etiquetas", "tags", "etiquetas", "标签", "étiquettes", "タグ", "태그"}},
        {"frame_apps",   {"apps", "apps", "apps", "应用", "apps", "アプリ", "앱"}},
        {"frame_status", {"estado", "status", "estado", "状态", "statut", "状態", "상태"}},

        {"tt_settings", {"Configurar tema, tecla e idioma", "Configure theme, hotkey & language", "Configurar tema, atalho e idioma", "配置主题、快捷键和语言", "Configurer thème, raccourci et langue", "テーマ・ショートカット・言語を設定", "테마, 단축키, 언어 설정"}},
        {"tt_classify", {"Clasificar en categorías", "Classify into categories", "Classificar em categorias", "归类到类别", "Classer par catégories", "カテゴリに分類", "카테고리로 분류"}},
        {"lbl_categories", {"Categorías", "Categories", "Categorias", "类别", "Catégories", "カテゴリ", "카테고리"}},
        {"auto_suffix", {"(auto)", "(auto)", "(auto)", "(自动)", "(auto)", "(自動)", "(자동)"}},

        {"cfg_title",    {"Configuración", "Settings", "Configurações", "设置", "Paramètres", "設定", "설정"}},
        {"cfg_color",    {"Color del tema", "Theme color", "Cor do tema", "主题颜色", "Couleur du thème", "テーマカラー", "테마 색상"}},
        {"cfg_hotkey",   {"Tecla rápida", "Hotkey", "Atalho", "快捷键", "Raccourci", "ショートカット", "단축키"}},
        {"cfg_language", {"Idioma", "Language", "Idioma", "语言", "Langue", "言語", "언어"}},

        {"mode_f",     {"Tecla F", "F key", "Tecla F", "F 键", "Touche F", "F キー", "F 키"}},
        {"mode_super", {"Súper + tecla", "Super + key", "Super + tecla", "Super + 键", "Super + touche", "Super + キー", "Super + 키"}},

        {"btn_cancel", {"Cancelar", "Cancel", "Cancelar", "取消", "Annuler", "キャンセル", "취소"}},
        {"btn_save",   {"Guardar", "Save", "Salvar", "保存", "Enregistrer", "保存", "저장"}},
        {"cfg_combo",  {"Combinación", "Combination", "Combinação", "组合", "Combinaison", "組み合わせ", "조합"}},
        {"cfg_pick",   {"Elegí una tecla", "Pick a key", "Escolha uma tecla", "选择一个键", "Choisissez une touche", "キーを選択", "키를 선택하세요"}},

        {"cp_hue",        {"Matiz", "Hue", "Matiz", "色相", "Teinte", "色相", "색조"}},
        {"cp_saturation", {"Saturación", "Saturation", "Saturação", "饱和度", "Saturation", "彩度", "채도"}},
        {"cp_value",      {"Valor", "Value", "Valor", "明度", "Valeur", "明度", "명도"}},
        {"cp_red",        {"Rojo", "Red", "Vermelho", "红", "Rouge", "赤", "빨강"}},
        {"cp_green",      {"Verde", "Green", "Verde", "绿", "Vert", "緑", "초록"}},
        {"cp_blue",       {"Azul", "Blue", "Azul", "蓝", "Bleu", "青", "파랑"}},
        // Status-bar shortcut key caps. 'f' (favorite) mirrors that physical key
        // on the localized keyboard; 'esc' is localized where a term exists (FR).
        {"kc_fav",     {"f", "f", "f", "f", "f", "は", "ㄹ"}},
        {"kc_esc",     {"esc", "esc", "esc", "esc", "échap", "esc", "esc"}},
    };
    return T;
}

inline std::string tr(const std::string& key) {
    const auto& T = i18n_table();
    auto it = T.find(key);
    if (it == T.end()) return key;   // missing key → show the key (dev aid)
    return it->second[static_cast<int>(current_lang())];
}
