/* ═══════════════════════════════════════════════════════════════════════
   Vanilux — landing page logic
   Identity: amber (#e09924) line-art on dark, monospace, TUI panels.
   ═══════════════════════════════════════════════════════════════════════ */
"use strict";

/* ── SVG icon library (stroke = currentColor → inherits the live theme) ──── */
const G = '<filter id="g"><feGaussianBlur stdDeviation="0.6" result="b"/><feMerge><feMergeNode in="b"/><feMergeNode in="SourceGraphic"/></feMerge></filter>';
function svg(inner, sw = 2) {
  return `<svg viewBox="0 0 48 48" fill="none" stroke="currentColor" stroke-width="${sw}"
    stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">${inner}</svg>`;
}
// category / control icons (mirror the app's motifs)
const ICONS = {
  grid:   svg('<rect x="8" y="8" width="12" height="12" rx="2"/><rect x="28" y="8" width="12" height="12" rx="2"/><rect x="8" y="28" width="12" height="12" rx="2"/><rect x="28" y="28" width="12" height="12" rx="2"/>'),
  list:   svg('<path d="M14 12h26M14 24h26M14 36h26"/><circle cx="8" cy="12" r="1.5" fill="currentColor"/><circle cx="8" cy="24" r="1.5" fill="currentColor"/><circle cx="8" cy="36" r="1.5" fill="currentColor"/>'),
  globe:  svg('<circle cx="24" cy="24" r="18"/><path d="M6 24h36M24 6a22 22 0 000 36"/><ellipse cx="24" cy="24" rx="8" ry="18"/>'),
  code:   svg('<path d="M14 16l-8 8 8 8M34 16l8 8-8 8M27 10l-6 28"/>'),
  music:  svg('<circle cx="14" cy="36" r="6"/><circle cx="34" cy="32" r="6"/><path d="M20 36V10l20-3v25"/>'),
  gear:   svg('<circle cx="24" cy="24" r="6"/><path d="M24 6v6M24 36v6M6 24h6M36 24h6M11 11l4 4M33 33l4 4M37 11l-4 4M15 33l-4 4"/>'),
  gamepad:svg('<rect x="6" y="16" width="36" height="18" rx="9"/><path d="M15 22v6M12 25h6"/><circle cx="31" cy="23" r="1.6" fill="currentColor"/><circle cx="35" cy="28" r="1.6" fill="currentColor"/>'),
  doc:    svg('<path d="M14 6h14l8 8v28H14z"/><path d="M28 6v8h8M19 24h12M19 31h12"/>'),
  star:   svg('<path d="M24 6l5.5 11.5L42 19l-9 8.8L35 40l-11-6-11 6 2-12.2L6 19l12.5-1.5z"/>'),
  clock:  svg('<circle cx="24" cy="24" r="18"/><path d="M24 13v11l7 5"/>'),
  wrench: svg('<path d="M33 8a9 9 0 00-9 12L9 35a3.5 3.5 0 005 5l15-15a9 9 0 0011-11l-5 5-4-1-1-4z"/>'),
};

/* ── i18n (reuses the app's real translation set) ───────────────────────── */
const LANGS = [
  { code:"ES", native:"Español" },{ code:"EN", native:"English" },
  { code:"PT", native:"Português"},{ code:"ZH", native:"中文" },
  { code:"FR", native:"Français"},{ code:"JA", native:"日本語" },
  { code:"KO", native:"한국어" },
];
const LI = { ES:0, EN:1, PT:2, ZH:3, FR:4, JA:5, KO:6 };
const TR = {
  search_placeholder:["Escribe para filtrar","Type to filter","Digite para filtrar","输入以筛选","Tapez pour filtrer","入力して絞り込み","입력하여 필터"],
  apps_word:["aplicaciones","apps","aplicativos","个应用","applications","個のアプリ","개 앱"],
  cat_all:["Todas","All","Todas","全部","Toutes","すべて","전체"],
  cat_internet:["Internet","Internet","Internet","网络","Internet","インターネット","인터넷"],
  cat_dev:["Programación","Development","Programação","开发","Développement","開発","개발"],
  cat_media:["Multimedia","Media","Multimídia","多媒体","Multimédia","メディア","미디어"],
  cat_system:["Sistema","System","Sistema","系统","Système","システム","시스템"],
  cat_office:["Oficina","Office","Escritório","办公","Bureau","オフィス","오피스"],
  cat_games:["Juegos","Games","Jogos","游戏","Jeux","ゲーム","게임"],
  tag_favorites:["Favoritos","Favorites","Favoritos","收藏","Favoris","お気に入り","즐겨찾기"],
  tag_recents:["Recientes","Recent","Recentes","最近","Récents","最近","최근"],
  sc_open:["abrir","open","abrir","打开","ouvrir","開く","열기"],
  sc_favorite:["favorito","favorite","favorito","收藏","favori","お気に入り","즐겨찾기"],
  sc_close:["cerrar","close","fechar","关闭","fermer","閉じる","닫기"],
  frame_search:["buscar","search","buscar","搜索","rechercher","検索","검색"],
  frame_tags:["etiquetas","tags","etiquetas","标签","étiquettes","タグ","태그"],
  frame_apps:["apps","apps","apps","应用","apps","アプリ","앱"],
  frame_status:["estado","status","estado","状态","statut","状態","상태"],
  cfg_color:["Color del tema","Theme color","Cor do tema","主题颜色","Couleur du thème","テーマカラー","테마 색상"],
  cfg_language:["Idioma","Language","Idioma","语言","Langue","言語","언어"],
  kc_fav:["f","f","f","f","f","は","ㄹ"],
  kc_esc:["esc","esc","esc","esc","échap","esc","esc"],

  /* ── Full-site copy ──────────────────────────────────────────────────── */
  nav_demo:["Demo","Demo","Demo","演示","Démo","デモ","데모"],
  nav_features:["Características","Features","Recursos","特性","Fonctionnalités","機能","기능"],
  nav_code:["Código","Code","Código","代码","Code","コード","코드"],
  nav_releases:["Releases ↗","Releases ↗","Releases ↗","版本 ↗","Versions ↗","リリース ↗","릴리스 ↗"],

  hero_h1:[
    'El lanzador de apps con alma de <span class="x">terminal</span>.',
    'The app launcher with a <span class="x">terminal</span> soul.',
    'O lançador de apps com alma de <span class="x">terminal</span>.',
    '拥有<span class="x">终端</span>之魂的应用启动器。',
    'Le lanceur d\'apps à l\'âme de <span class="x">terminal</span>.',
    '<span class="x">ターミナル</span>の魂を持つアプリランチャー。',
    '<span class="x">터미널</span>의 영혼을 가진 앱 런처.'],
  hero_sub:[
    "Lanzador a pantalla completa para Linux, estilo Launchpad, con estética ámbar de terminal. Residente, instantáneo y personalizable hasta el píxel.",
    "A fullscreen app launcher for Linux, Launchpad-style, with an amber terminal aesthetic. Resident, instant, and customizable down to the pixel.",
    "Lançador em tela cheia para Linux, estilo Launchpad, com estética âmbar de terminal. Residente, instantâneo e personalizável até o pixel.",
    "面向 Linux 的全屏应用启动器，Launchpad 风格，琥珀色终端美学。常驻、即时，可定制到每个像素。",
    "Un lanceur d'applications plein écran pour Linux, façon Launchpad, à l'esthétique terminal ambre. Résident, instantané et personnalisable au pixel près.",
    "Linux 向けのフルスクリーン・アプリランチャー。Launchpad 風、アンバーのターミナル美学。常駐・瞬時・ピクセル単位までカスタマイズ可能。",
    "런치패드 스타일의 Linux 전체 화면 앱 런처. 앰버 터미널 미학. 상주하며 즉각적이고 픽셀 단위까지 커스터마이즈할 수 있습니다."],
  hero_dl:["↓ Descargar","↓ Download","↓ Baixar","↓ 下载","↓ Télécharger","↓ ダウンロード","↓ 다운로드"],
  hero_try:["▶ Probar la demo","▶ Try the demo","▶ Testar a demo","▶ 试用演示","▶ Essayer la démo","▶ デモを試す","▶ 데모 체험"],
  hero_hint:[
    'Se invoca con <span class="kbd">F4</span> y queda residente — la siguiente apertura es inmediata.',
    'Summon it with <span class="kbd">F4</span>; it stays resident — the next open is instant.',
    'Invoque com <span class="kbd">F4</span> e fica residente — a próxima abertura é imediata.',
    '用 <span class="kbd">F4</span> 唤出，常驻内存 —— 下次打开即时完成。',
    'Appelez-le avec <span class="kbd">F4</span> ; il reste résident — la prochaine ouverture est immédiate.',
    '<span class="kbd">F4</span> で呼び出して常駐 — 次回の起動は一瞬です。',
    '<span class="kbd">F4</span> 로 호출하면 상주합니다 — 다음 실행은 즉시.'],

  sim_eyebrow:["Simulador en vivo","Live simulator","Simulador ao vivo","实时模拟器","Simulateur en direct","ライブシミュレーター","라이브 시뮬레이터"],
  sim_title:["Tomale el peso. Acá mismo.","Take it for a spin. Right here.","Experimente. Aqui mesmo.","现在就来试试。","Essayez-le. Ici même.","ここで触ってみて。","지금 여기서 체험해 보세요."],
  sim_desc:[
    'Esto es Vanilux corriendo en tu navegador (con apps inventadas para la demo). Buscá, filtrá por categorías, marcá favoritos — y, sobre todo, <span class="amber">cambiá el color del tema y el idioma</span>: todo se recolorea y se retraduce al instante, igual que en la app real.',
    'This is Vanilux running in your browser (with made-up apps for the demo). Search, filter by category, star favorites — and above all, <span class="amber">change the theme color and the language</span>: everything recolors and re-translates instantly, just like the real app.',
    'Isto é o Vanilux rodando no seu navegador (com apps fictícios para a demo). Busque, filtre por categorias, marque favoritos — e, acima de tudo, <span class="amber">mude a cor do tema e o idioma</span>: tudo se recolore e se retraduz na hora, como no app real.',
    '这是在你浏览器中运行的 Vanilux（演示用的是虚构应用）。搜索、按类别筛选、收藏——更重要的是，<span class="amber">更改主题颜色和语言</span>：一切都会即时变色并重新翻译，和真实应用一样。',
    'Voici Vanilux dans votre navigateur (avec des apps fictives pour la démo). Cherchez, filtrez par catégorie, ajoutez des favoris — et surtout, <span class="amber">changez la couleur du thème et la langue</span> : tout se recolore et se retraduit à l\'instant, comme dans la vraie app.',
    'これはブラウザで動く Vanilux です（デモ用の架空アプリ）。検索、カテゴリ絞り込み、お気に入り登録 — そして何より、<span class="amber">テーマカラーと言語を変えて</span>みてください。すべてが即座に再着色・再翻訳されます。実際のアプリと同じように。',
    '브라우저에서 실행되는 Vanilux입니다(데모용 가상 앱). 검색하고 카테고리로 필터링하고 즐겨찾기를 표시하세요 — 무엇보다 <span class="amber">테마 색상과 언어를 바꿔</span> 보세요. 실제 앱처럼 모든 것이 즉시 다시 칠해지고 번역됩니다.'],

  feat_eyebrow:["Características","Features","Recursos","特性","Fonctionnalités","機能","기능"],
  feat_title:["Diseñado para no estorbar.","Built to stay out of your way.","Feito para não atrapalhar.","为不打扰你而设计。","Conçu pour ne pas vous gêner.","邪魔をしない設計。","방해하지 않도록 설계했습니다."],
  feat_desc:[
    "Cada decisión apunta a lo mismo: que abras lo que buscás en milisegundos y vuelvas a lo tuyo.",
    "Every decision serves one goal: open what you need in milliseconds and get back to work.",
    "Cada decisão serve a um objetivo: abrir o que você precisa em milissegundos e voltar ao que importa.",
    "每个决定都指向同一目标：在毫秒内打开你要的，然后回到正事。",
    "Chaque choix vise la même chose : ouvrir ce que vous cherchez en millisecondes et revenir à votre travail.",
    "すべての判断は一つの目的のため。必要なものをミリ秒で開き、すぐ作業に戻る。",
    "모든 결정은 한 가지 목표를 향합니다: 필요한 것을 밀리초 만에 열고 하던 일로 돌아가기."],

  f1t:["Pantalla completa, estilo TUI","Fullscreen, TUI-style","Tela cheia, estilo TUI","全屏 TUI 风格","Plein écran, style TUI","フルスクリーン・TUI 風","전체 화면, TUI 스타일"],
  f1d:[
    'Paneles <code>[ search ]</code>, <code>[ tags ]</code>, <code>[ apps ]</code> y <code>[ status ]</code> con bordes ámbar. Tu escritorio, modo terminal.',
    'Panels <code>[ search ]</code>, <code>[ tags ]</code>, <code>[ apps ]</code> and <code>[ status ]</code> with amber borders. Your desktop, terminal mode.',
    'Painéis <code>[ search ]</code>, <code>[ tags ]</code>, <code>[ apps ]</code> e <code>[ status ]</code> com bordas âmbar. Seu desktop, modo terminal.',
    '面板 <code>[ search ]</code>、<code>[ tags ]</code>、<code>[ apps ]</code> 和 <code>[ status ]</code>，琥珀色边框。你的桌面，终端模式。',
    'Panneaux <code>[ search ]</code>, <code>[ tags ]</code>, <code>[ apps ]</code> et <code>[ status ]</code> aux bords ambre. Votre bureau, mode terminal.',
    '<code>[ search ]</code>・<code>[ tags ]</code>・<code>[ apps ]</code>・<code>[ status ]</code> パネルをアンバーの枠で。デスクトップをターミナル風に。',
    '<code>[ search ]</code>, <code>[ tags ]</code>, <code>[ apps ]</code>, <code>[ status ]</code> 패널을 앰버 테두리로. 당신의 데스크톱, 터미널 모드.'],
  f2t:["Descubrimiento automático","Automatic discovery","Descoberta automática","自动发现","Découverte automatique","自動検出","자동 탐색"],
  f2d:[
    "Parsea los .desktop del sistema y de tu usuario. Respeta NoDisplay, Hidden y TryExec. Cero configuración.",
    "Parses the system and user .desktop files. Honors NoDisplay, Hidden and TryExec. Zero config.",
    "Analisa os .desktop do sistema e do usuário. Respeita NoDisplay, Hidden e TryExec. Zero configuração.",
    "解析系统与用户的 .desktop 文件，遵循 NoDisplay、Hidden 和 TryExec。零配置。",
    "Analyse les .desktop du système et de l'utilisateur. Respecte NoDisplay, Hidden et TryExec. Zéro config.",
    "システムとユーザーの .desktop を解析。NoDisplay・Hidden・TryExec に対応。設定不要。",
    "시스템과 사용자의 .desktop 파일을 파싱합니다. NoDisplay, Hidden, TryExec 준수. 설정 불필요."],
  f3t:["Búsqueda incremental","Incremental search","Busca incremental","增量搜索","Recherche incrémentale","インクリメンタル検索","점진적 검색"],
  f3d:[
    "Filtra por nombre al instante, sin distinguir mayúsculas, con case-folding Unicode. Empezá a escribir y listo.",
    "Filters by name instantly, case-insensitive with Unicode case-folding. Just start typing.",
    "Filtra por nome na hora, sem diferenciar maiúsculas, com case-folding Unicode. É só começar a digitar.",
    "按名称即时筛选，不区分大小写，支持 Unicode 大小写折叠。开始输入即可。",
    "Filtre par nom à l'instant, insensible à la casse, avec case-folding Unicode. Tapez, c'est tout.",
    "名前で即フィルタ。大文字小文字を無視し Unicode ケースフォールディング対応。入力するだけ。",
    "이름으로 즉시 필터링, 대소문자 구분 없이 유니코드 케이스 폴딩 지원. 입력만 하면 됩니다."],
  f4t:["Favoritos y Recientes","Favorites & Recents","Favoritos e Recentes","收藏与最近","Favoris et Récents","お気に入りと最近","즐겨찾기와 최근"],
  f4d:[
    "Marcá lo que más usás; Vanilux recuerda lo último que abriste. Persistente en disco entre sesiones.",
    "Star what you use most; Vanilux remembers what you opened last. Persisted on disk across sessions.",
    "Marque o que mais usa; o Vanilux lembra o que você abriu por último. Persistente em disco entre sessões.",
    "收藏常用应用；Vanilux 记住你最近打开的。跨会话持久保存到磁盘。",
    "Marquez vos préférés ; Vanilux retient ce que vous avez ouvert en dernier. Persistant sur disque entre sessions.",
    "よく使うものを登録。最後に開いたアプリも記憶。ディスクに保存しセッションをまたいで維持。",
    "자주 쓰는 앱을 즐겨찾기로; 마지막에 연 앱도 기억합니다. 세션 간 디스크에 저장."],
  f5t:["Glow ámbar en Cairo","Amber glow in Cairo","Glow âmbar em Cairo","Cairo 琥珀辉光","Halo ambre en Cairo","Cairo のアンバーグロー","Cairo 앰버 글로우"],
  f5d:[
    "El halo del ícono seleccionado se dibuja a mano con un blur real — borde nítido y resplandor uniforme.",
    "The selected icon's halo is hand-drawn with a real blur — crisp edge, uniform glow.",
    "O halo do ícone selecionado é desenhado à mão com blur real — borda nítida e brilho uniforme.",
    "选中图标的光晕用真实模糊手工绘制——边缘清晰，辉光均匀。",
    "Le halo de l'icône sélectionnée est dessiné à la main avec un vrai flou — bord net, lueur uniforme.",
    "選択アイコンのハローを実ブラーで手描き。輪郭はシャープ、グローは均一。",
    "선택된 아이콘의 후광을 실제 블러로 직접 그립니다 — 선명한 가장자리, 균일한 광채."],
  f6t:["Tema en vivo · 7 idiomas","Live theme · 7 languages","Tema ao vivo · 7 idiomas","实时主题 · 7 种语言","Thème en direct · 7 langues","ライブテーマ・7言語","실시간 테마 · 7개 언어"],
  f6d:[
    "Color picker que recolorea todo en tiempo real y un teclado en pantalla para el atajo. Español, inglés, portugués, 中文, français, 日本語, 한국어.",
    "A color picker that recolors everything in real time and an on-screen keyboard for the hotkey. Spanish, English, Portuguese, 中文, français, 日本語, 한국어.",
    "Um seletor de cor que recolore tudo em tempo real e um teclado na tela para o atalho. Espanhol, inglês, português, 中文, français, 日本語, 한국어.",
    "实时为所有元素变色的取色器，以及用于设置快捷键的屏幕键盘。西班牙语、英语、葡萄牙语、中文、法语、日语、韩语。",
    "Un sélecteur de couleur qui recolore tout en temps réel et un clavier à l'écran pour le raccourci. Espagnol, anglais, portugais, 中文, français, 日本語, 한국어.",
    "すべてをリアルタイムで再着色するカラーピッカーと、ショートカット設定用のオンスクリーンキーボード。スペイン語・英語・ポルトガル語・中文・français・日本語・한국어。",
    "모든 요소를 실시간으로 다시 칠하는 색상 선택기와 단축키용 화면 키보드. 스페인어, 영어, 포르투갈어, 中文, français, 日本語, 한국어."],
  f7t:["Residente = instantáneo","Resident = instant","Residente = instantâneo","常驻 = 即时","Résident = instantané","常駐 = 瞬時","상주 = 즉시"],
  f7d:[
    "Queda en memoria tras el primer arranque. La apertura siguiente es inmediata, como rofi. Se invoca con F4.",
    "Stays in memory after the first launch. The next open is immediate, like rofi. Summoned with F4.",
    "Fica na memória após o primeiro início. A próxima abertura é imediata, como o rofi. Invocado com F4.",
    "首次启动后常驻内存。下次打开即时完成，像 rofi 一样。用 F4 唤出。",
    "Reste en mémoire après le premier lancement. L'ouverture suivante est immédiate, comme rofi. Appelé avec F4.",
    "初回起動後はメモリに常駐。次回は rofi のように一瞬で開く。F4 で呼び出し。",
    "첫 실행 후 메모리에 상주합니다. 다음 실행은 rofi처럼 즉시. F4로 호출."],
  f8t:["C++17 + gtkmm3","C++17 + gtkmm3","C++17 + gtkmm3","C++17 + gtkmm3","C++17 + gtkmm3","C++17 + gtkmm3","C++17 + gtkmm3"],
  f8d:[
    "Nativo, liviano y rápido. Sin Electron, sin runtime pesado. Solo el toolkit del sistema.",
    "Native, lightweight and fast. No Electron, no heavy runtime. Just the system toolkit.",
    "Nativo, leve e rápido. Sem Electron, sem runtime pesado. Só o toolkit do sistema.",
    "原生、轻量、快速。没有 Electron，没有臃肿运行时。只用系统工具包。",
    "Natif, léger et rapide. Pas d'Electron, pas de runtime lourd. Juste le toolkit du système.",
    "ネイティブで軽量・高速。Electron なし、重いランタイムなし。システムのツールキットだけ。",
    "네이티브, 경량, 빠름. Electron 없음, 무거운 런타임 없음. 시스템 툴킷만 사용."],

  spec_open:["apertura residente","resident open","abertura residente","常驻打开","ouverture résidente","常駐起動","상주 실행"],
  spec_langs:["idiomas","languages","idiomas","种语言","langues","言語","개 언어"],
  spec_colors:["colores de tema","theme colors","cores de tema","主题颜色","couleurs de thème","テーマカラー","테마 색상"],
  spec_kbd:["teclado · sin mouse","keyboard · no mouse","teclado · sem mouse","键盘 · 无需鼠标","clavier · sans souris","キーボード・マウス不要","키보드 · 마우스 불필요"],

  dl_eyebrow:["Empezá ahora","Get started","Comece agora","现在开始","Commencez","今すぐ始める","지금 시작하기"],
  dl_title:["Llevátelo a tu Linux.","Get it on your Linux.","Leve para o seu Linux.","装到你的 Linux 上。","Installez-le sur votre Linux.","あなたの Linux へ。","당신의 Linux에 설치하세요."],
  dl_desc:[
    'Cloná, compilá e instalá. El instalador configura el atajo <span class="kbd">F4</span> por vos.',
    'Clone, build and install. The installer sets up the <span class="kbd">F4</span> hotkey for you.',
    'Clone, compile e instale. O instalador configura o atalho <span class="kbd">F4</span> para você.',
    '克隆、编译、安装。安装脚本会为你配置 <span class="kbd">F4</span> 快捷键。',
    'Clonez, compilez et installez. L\'installateur configure le raccourci <span class="kbd">F4</span> pour vous.',
    'クローンしてビルド、インストール。インストーラーが <span class="kbd">F4</span> ショートカットを設定します。',
    '클론하고 빌드하고 설치하세요. 설치 프로그램이 <span class="kbd">F4</span> 단축키를 설정해 줍니다.'],
  dl_copy:["copiar","copy","copiar","复制","copier","コピー","복사"],
  dl_releases:["↓ Ver releases","↓ View releases","↓ Ver releases","↓ 查看版本","↓ Voir les versions","↓ リリースを見る","↓ 릴리스 보기"],
  dl_code:["★ Código en GitHub","★ Code on GitHub","★ Código no GitHub","★ GitHub 上的代码","★ Code sur GitHub","★ GitHub のコード","★ GitHub의 코드"],

  foot_made:[
    'vani<b class="amber">lux</b> · hecho con C++ y café ☕ · estética terminal ámbar',
    'vani<b class="amber">lux</b> · made with C++ and coffee ☕ · amber terminal aesthetic',
    'vani<b class="amber">lux</b> · feito com C++ e café ☕ · estética terminal âmbar',
    'vani<b class="amber">lux</b> · 用 C++ 和咖啡 ☕ 打造 · 琥珀终端美学',
    'vani<b class="amber">lux</b> · fait avec C++ et café ☕ · esthétique terminal ambre',
    'vani<b class="amber">lux</b> · C++ とコーヒー ☕ で制作 · アンバーのターミナル美学',
    'vani<b class="amber">lux</b> · C++와 커피 ☕로 제작 · 앰버 터미널 미학'],
  foot_top:["Arriba ↑","Top ↑","Topo ↑","返回顶部 ↑","Haut ↑","上へ ↑","맨 위로 ↑"],
};
let LANG = "ES";
const t = (k) => (TR[k] ? TR[k][LI[LANG]] : k);

/* Pick the initial language from the browser locale (≈ the user's region);
   fall back to English when it isn't one of the 7 supported. */
function detectLang(){
  const map={es:"ES",en:"EN",pt:"PT",zh:"ZH",fr:"FR",ja:"JA",ko:"KO"};
  const list=(navigator.languages&&navigator.languages.length)
    ? navigator.languages : [navigator.language||"en"];
  for(const l of list){
    const p=String(l).toLowerCase().split(/[-_]/)[0];
    if(map[p]) return map[p];
  }
  return "EN";
}

/* ── Categories (sidebar) ───────────────────────────────────────────────── */
const CATS = [
  { id:"all",       icon:"grid"  },
  { id:"favorites", icon:"star"  },
  { id:"recents",   icon:"clock" },
  { id:"internet",  icon:"globe" },
  { id:"dev",       icon:"code"  },
  { id:"media",     icon:"music" },
  { id:"system",    icon:"gear"  },
  { id:"office",    icon:"doc"   },
  { id:"games",     icon:"gamepad"},
];
const catLabel = (id) =>
  id==="favorites" ? t("tag_favorites") :
  id==="recents"   ? t("tag_recents")   : t("cat_"+id);

/* ── Invented apps (made-up icons, as requested) ────────────────────────── */
const A = (name, cat, inner) => ({ name, cat, icon: svg(inner) });
const APPS = [
  A("Nebula Code","dev",   '<path d="M16 16l-8 8 8 8M32 16l8 8-8 8"/><circle cx="24" cy="24" r="3" fill="currentColor"/>'),
  A("Quantum Term","system",'<rect x="6" y="9" width="36" height="30" rx="3"/><path d="M13 19l6 5-6 5M25 29h10"/>'),
  A("Pixel Forge","media", '<path d="M24 6l16 9v18l-16 9-16-9V15z"/><path d="M24 6v18l16-9M24 24v18M24 24L8 15"/>'),
  A("Synthwave","media",   '<path d="M6 24h6l4-12 6 24 5-18 4 9h11"/>'),
  A("Orbit Mail","internet",'<circle cx="24" cy="24" r="8"/><ellipse cx="24" cy="24" rx="20" ry="8" transform="rotate(35 24 24)"/>'),
  A("Cryo Notes","office", '<path d="M24 6v36M9 15l30 18M39 15L9 33"/><circle cx="24" cy="24" r="3" fill="currentColor"/>'),
  A("Vortex Web","internet",'<path d="M24 8a16 16 0 11-11 27c8 4 18-2 16-12-1-6-9-7-12-2"/>'),
  A("Helix DB","dev",      '<path d="M14 8c0 8 20 8 20 16s-20 8-20 16M34 8c0 8-20 8-20 16s20 8 20 16"/>'),
  A("Lumen Paint","media", '<path d="M24 6c8 10 12 16 12 22a12 12 0 01-24 0c0-6 4-12 12-22z"/>'),
  A("Echo Chat","internet",'<path d="M8 12h24v16H18l-8 7V28H8z"/><path d="M36 20h4v16l-6-5h-8"/>'),
  A("Forge Build","dev",   '<circle cx="24" cy="16" r="7"/><path d="M24 23v8M16 38h16M20 31h8"/>'),
  A("Astro Sheet","office",'<rect x="8" y="8" width="32" height="32" rx="2"/><path d="M8 20h32M8 30h32M20 8v32"/>'),
  A("Pulse Mon","system",  '<path d="M6 24h8l4-10 6 20 5-14 4 4h9"/>'),
  A("Glyph Type","office", '<path d="M14 36l10-26 10 26M18 27h12"/>'),
  A("Retro Arcade","games",'<rect x="7" y="16" width="34" height="18" rx="9"/><path d="M16 22v6M13 25h6"/><circle cx="30" cy="23" r="1.7" fill="currentColor"/><circle cx="34" cy="28" r="1.7" fill="currentColor"/>'),
  A("Comet Run","games",   '<circle cx="30" cy="18" r="7"/><path d="M24 24L8 40M20 18L9 23M27 28l-6 9"/>'),
  A("Cipher Vault","system",'<rect x="11" y="20" width="26" height="20" rx="3"/><path d="M16 20v-5a8 8 0 0116 0v5M24 28v6"/>'),
  A("Flux Player","media", '<path d="M18 12l20 12-20 12z"/>'),
];

/* ── State ──────────────────────────────────────────────────────────────── */
let activeCat = "all";
let query = "";
const favs = new Set(["Synthwave","Nebula Code","Orbit Mail"]);
const recents = ["Quantum Term","Lumen Paint","Astro Sheet","Vortex Web","Echo Chat"];

/* ── Theme ──────────────────────────────────────────────────────────────── */
function hexToRgb(h){const n=parseInt(h.slice(1),16);return[(n>>16)&255,(n>>8)&255,n&255];}
function setTheme(hex){
  const [r,g,b]=hexToRgb(hex);
  const root=document.documentElement.style;
  root.setProperty("--accent",hex);
  root.setProperty("--accent-rgb",`${r},${g},${b}`);
  // brighter variant (same formula as the app: *1.3 + 30, clamped)
  const br=Math.min(255,r*1.3+30)|0, bg=Math.min(255,g*1.3+30)|0, bb=Math.min(255,b*1.3+30)|0;
  root.setProperty("--accent-bright",`rgb(${br},${bg},${bb})`);
  THEME_RGB=[r/255,g/255,b/255];
  document.querySelectorAll(".sw").forEach(s=>s.classList.toggle("active",s.dataset.hex===hex));
}
const SWATCHES=["#e09924","#00fdd6","#ff5470","#a855f7","#3b82f6","#43d675","#ff7a18"];

/* ═══════════════════════ RENDER ═══════════════════════ */
function renderTags(){
  const box=document.getElementById("simTags");
  box.innerHTML=CATS.map(c=>
    `<div class="tag ${c.id===activeCat?"active":""}" data-cat="${c.id}">
       ${ICONS[c.icon]}<span>${catLabel(c.id)}</span>
     </div>`).join("");
  box.querySelectorAll(".tag").forEach(el=>el.onclick=()=>{activeCat=el.dataset.cat;renderTags();renderGrid();});
}

function filteredApps(){
  let list = APPS;
  if(activeCat==="favorites") list=APPS.filter(a=>favs.has(a.name));
  else if(activeCat==="recents") list=recents.map(n=>APPS.find(a=>a.name===n)).filter(Boolean);
  else if(activeCat!=="all") list=APPS.filter(a=>a.cat===activeCat);
  if(query) list=list.filter(a=>a.name.toLowerCase().includes(query.toLowerCase()));
  return list;
}

function renderGrid(){
  const grid=document.getElementById("simGrid");
  const list=filteredApps();
  if(!list.length){
    grid.innerHTML=`<div class="empty-msg">∅ sin resultados</div>`;
  }else{
    grid.innerHTML=list.map(a=>
      `<div class="app ${favs.has(a.name)?"fav":""}" data-name="${a.name}">
         <span class="star">${ICONS.star}</span>
         <div class="ico">${a.icon}</div>
         <div class="name">${a.name}</div>
       </div>`).join("");
    grid.querySelectorAll(".app").forEach(el=>{
      el.querySelector(".star").onclick=(e)=>{
        e.stopPropagation();
        const n=el.dataset.name;
        favs.has(n)?favs.delete(n):favs.add(n);
        renderGrid();updateStatus();
      };
      el.onclick=()=>{el.animate(
        [{transform:"scale(1)"},{transform:"scale(.9)"},{transform:"scale(1)"}],
        {duration:220,easing:"ease-out"});};
    });
  }
  updateStatus();
}

function updateStatus(){
  document.getElementById("simCount").textContent=`${filteredApps().length} ${t("apps_word")}`;
  document.getElementById("simCat").textContent=catLabel(activeCat);
}

function applyI18n(){
  document.documentElement.lang=LANG.toLowerCase();
  // Translate every tagged element across the whole site. innerHTML is safe here
  // — all values come from our own TR table (some carry intentional markup).
  document.querySelectorAll("[data-i18n]").forEach(el=>{el.innerHTML=t(el.dataset.i18n);});
  const s=document.getElementById("simSearch"); if(s) s.placeholder=t("search_placeholder");
  renderTags();renderGrid();
  buildHeroPreview();   // rebuild the hero mockup in the new language
  document.querySelectorAll(".lang").forEach(l=>l.classList.toggle("active",l.dataset.lang===LANG));
}

/* ── Flags (compact, reuse app palette) ─────────────────────────────────── */
const FLAGS={
ES:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#c60b1e"/><rect y="4" width="24" height="8" fill="#ffc400"/></svg>`,
EN:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#012169"/><path d="M0 0l24 16M24 0L0 16" stroke="#fff" stroke-width="3"/><path d="M12 0v16M0 8h24" stroke="#fff" stroke-width="4.5"/><path d="M12 0v16M0 8h24" stroke="#c8102e" stroke-width="2.6"/></svg>`,
PT:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#da291c"/><rect width="9.5" height="16" fill="#046a38"/><circle cx="9.5" cy="8" r="2.4" fill="none" stroke="#ffe000" stroke-width="1.1"/></svg>`,
ZH:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#de2910"/><path d="M5 2.2l.9 2.8H9l-2.4 1.8.9 2.8L5 7.9 2.5 9.6l.9-2.8L1 5h3z" fill="#ffde00"/></svg>`,
FR:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#fff"/><rect width="8" height="16" fill="#0055a4"/><rect x="16" width="8" height="16" fill="#ef4135"/></svg>`,
JA:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#fff"/><circle cx="12" cy="8" r="4.2" fill="#bc002d"/></svg>`,
KO:`<svg viewBox="0 0 24 16"><rect width="24" height="16" fill="#fff"/><circle cx="12" cy="8" r="3.4" fill="#cd2e3a"/><path d="M12 4.6a3.4 3.4 0 010 6.8 1.7 1.7 0 000-3.4 1.7 1.7 0 010-3.4z" fill="#0047a0"/></svg>`,
};

function buildControls(){
  // swatches
  const sw=document.getElementById("swatches");
  sw.innerHTML=SWATCHES.map(h=>`<div class="sw" data-hex="${h}" style="background:${h};box-shadow:0 0 10px ${h}55"></div>`).join("")
    +`<label class="sw sw-custom" title="Color personalizado">+<input type="color" id="cpick" value="#e09924"></label>`;
  sw.querySelectorAll(".sw[data-hex]").forEach(s=>s.onclick=()=>setTheme(s.dataset.hex));
  document.getElementById("cpick").addEventListener("input",e=>setTheme(e.target.value));
  // languages
  const lg=document.getElementById("langs");
  lg.innerHTML=LANGS.map(l=>
    `<div class="lang ${l.code===LANG?"active":""}" data-lang="${l.code}" title="${l.native}">
       <span class="flag">${FLAGS[l.code]}</span><span class="lc">${l.code}</span>
     </div>`).join("");
  lg.querySelectorAll(".lang").forEach(el=>el.onclick=()=>{LANG=el.dataset.lang;applyI18n();});
}

/* ── Toolbar (grid/list + wrench) ───────────────────────────────────────── */
function buildToolbar(){
  document.getElementById("vGrid").innerHTML=ICONS.grid;
  document.getElementById("vList").innerHTML=ICONS.list;
  document.getElementById("vWrench").innerHTML=ICONS.wrench;
  const grid=document.getElementById("simGrid");
  document.querySelectorAll(".sim-tool[data-view]").forEach(b=>b.onclick=()=>{
    document.querySelectorAll(".sim-tool[data-view]").forEach(x=>x.classList.remove("active"));
    b.classList.add("active");
    grid.style.gridTemplateColumns = b.dataset.view==="list"
      ? "1fr" : "repeat(auto-fill,minmax(92px,1fr))";
    grid.querySelectorAll(".app").forEach(a=>{
      a.style.flexDirection = b.dataset.view==="list" ? "row" : "column";
      a.style.justifyContent = b.dataset.view==="list" ? "flex-start" : "center";
      a.style.gap = b.dataset.view==="list" ? "12px" : "7px";
    });
  });
  document.getElementById("vWrench").onclick=()=>{
    document.querySelector(".sim-controls").scrollIntoView({behavior:"smooth",block:"center"});
    document.querySelectorAll(".sim-controls .frame").forEach(f=>{
      f.animate([{boxShadow:"0 0 0 rgba(var(--accent-rgb),0)"},
        {boxShadow:"0 0 24px rgba(var(--accent-rgb),.6)"},
        {boxShadow:"0 0 0 rgba(var(--accent-rgb),0)"}],{duration:900});
    });
  };
}

/* ── Features grid (titles/descs translated in place via data-i18n) ─────── */
const FEATURES=[
  {icon:"globe", tk:"f1t",dk:"f1d"},{icon:"grid", tk:"f2t",dk:"f2d"},
  {icon:"code",  tk:"f3t",dk:"f3d"},{icon:"star", tk:"f4t",dk:"f4d"},
  {icon:"gear",  tk:"f5t",dk:"f5d"},{icon:"wrench",tk:"f6t",dk:"f6d"},
  {icon:"clock", tk:"f7t",dk:"f7d"},{icon:"doc",  tk:"f8t",dk:"f8d"},
];
function buildFeatures(){
  document.getElementById("featGrid").innerHTML=FEATURES.map(f=>
    `<div class="feat reveal"><div class="fico">${ICONS[f.icon]}</div>
     <h3 data-i18n="${f.tk}"></h3><p data-i18n="${f.dk}"></p></div>`).join("");
}

/* ── Hero app preview (static showcase mockup of the real app) ──────────── */
function buildHeroPreview(){
  const host=document.getElementById("heroPreview");
  if(!host) return;
  const tags=[
    {id:"all",on:true},{id:"favorites"},{id:"recents"},
    {id:"internet"},{id:"dev"},{id:"media"},
  ];
  const show=["Nebula Code","Quantum Term","Pixel Forge","Synthwave",
              "Orbit Mail","Cryo Notes","Vortex Web","Helix DB"];
  const favSet=new Set(["Synthwave"]);
  const glowOne="Nebula Code";
  const apps=show.map(n=>APPS.find(a=>a.name===n)).filter(Boolean);

  host.innerHTML=`
    <div class="hero-app">
      <div class="ha-bar">
        <span class="d r"></span><span class="d y"></span><span class="d g"></span>
        <span class="ttl">vanilux — :: pantalla completa ::</span>
      </div>
      <div class="ha-body">
        <div class="ha-panel">
          <span class="lab">${t("frame_search")}</span>
          <div class="ha-search">
            <span class="p">&gt;</span><span>${t("search_placeholder")}</span>
            <span class="tools">
              <span class="tl on">${ICONS.grid}</span>
              <span class="tl">${ICONS.list}</span>
              <span class="tl">${ICONS.wrench}</span>
            </span>
          </div>
        </div>
        <div class="ha-cols">
          <div class="ha-panel ha-side">
            <span class="lab">${t("frame_tags")}</span>
            ${tags.map(tg=>`<div class="tg ${tg.on?"on":""}">${ICONS[CATS.find(c=>c.id===tg.id).icon]}<span>${catLabel(tg.id)}</span></div>`).join("")}
          </div>
          <div class="ha-panel">
            <span class="lab">${t("frame_apps")}</span>
            <div class="ha-grid">
              ${apps.map(a=>`<div class="ha-app ${a.name===glowOne?"glow":""}">
                ${favSet.has(a.name)?`<span class="st">${ICONS.star}</span>`:""}
                <div class="ic">${a.icon}</div><div class="nm">${a.name}</div></div>`).join("")}
            </div>
          </div>
        </div>
        <div class="ha-panel">
          <span class="lab">${t("frame_status")}</span>
          <div class="ha-status"><span class="ws">~/apps</span><span>18 ${t("apps_word")}</span><span>${catLabel("all")}</span></div>
        </div>
      </div>
    </div>`;
}

/* ── Brand logo (mini grid mark) ───────────────────────────────────────── */
function buildBrand(){
  document.getElementById("brandLogo").innerHTML=
    `<svg viewBox="0 0 48 48" fill="none" stroke="var(--accent)" stroke-width="3" stroke-linejoin="round">
       <rect x="7" y="7" width="14" height="14" rx="3"/><rect x="27" y="7" width="14" height="14" rx="3"/>
       <rect x="7" y="27" width="14" height="14" rx="3"/><rect x="27" y="27" width="14" height="14" rx="3"
        fill="var(--accent)" opacity=".85"/></svg>`;
}

/* ── Copy install command ──────────────────────────────────────────────── */
function setupCopy(){
  const btn=document.getElementById("copyBtn");
  btn.onclick=()=>{
    const txt=document.getElementById("dlCmd").textContent;
    navigator.clipboard?.writeText(txt).then(()=>{
      btn.textContent="✓";setTimeout(()=>btn.textContent=t("dl_copy"),1500);
    });
  };
}

/* ── Scroll reveal ─────────────────────────────────────────────────────── */
function setupReveal(){
  const io=new IntersectionObserver((es)=>es.forEach(e=>{
    if(e.isIntersecting){e.target.classList.add("in");io.unobserve(e.target);}
  }),{threshold:.12});
  document.querySelectorAll(".reveal").forEach(el=>io.observe(el));
}

/* ═══════════════════════ THREE.JS BACKGROUND ═══════════════════════ */
let THEME_RGB=[224/255,153/255,36/255];
function initThree(){
  if(typeof THREE==="undefined"||window.matchMedia("(prefers-reduced-motion: reduce)").matches) return;
  const canvas=document.getElementById("bg-canvas");
  const scene=new THREE.Scene();
  scene.fog=new THREE.FogExp2(0x0d0d12,0.055);
  const cam=new THREE.PerspectiveCamera(60,1,0.1,100);
  cam.position.z=18;
  const renderer=new THREE.WebGLRenderer({canvas,alpha:true,antialias:true});
  renderer.setPixelRatio(Math.min(window.devicePixelRatio,2));

  // particle field
  const N=140, R=26;
  const pos=new Float32Array(N*3), seed=[];
  for(let i=0;i<N;i++){
    const x=(Math.random()-.5)*R, y=(Math.random()-.5)*R*.7, z=(Math.random()-.5)*R;
    pos[i*3]=x;pos[i*3+1]=y;pos[i*3+2]=z;
    seed.push({sx:0.2+Math.random()*0.6,ph:Math.random()*Math.PI*2});
  }
  const pGeo=new THREE.BufferGeometry();
  pGeo.setAttribute("position",new THREE.BufferAttribute(pos,3));
  const pMat=new THREE.PointsMaterial({size:0.42,color:0xe09924,transparent:true,opacity:0.9,
    blending:THREE.AdditiveBlending,depthWrite:false});
  const points=new THREE.Points(pGeo,pMat);
  scene.add(points);

  // constellation lines
  const lGeo=new THREE.BufferGeometry();
  const maxLines=N*5;
  const lPos=new Float32Array(maxLines*6);
  lGeo.setAttribute("position",new THREE.BufferAttribute(lPos,3));
  const lMat=new THREE.LineBasicMaterial({color:0xe09924,transparent:true,opacity:0.13,
    blending:THREE.AdditiveBlending,depthWrite:false});
  const lines=new THREE.LineSegments(lGeo,lMat);
  scene.add(lines);

  // faint wireframe icosahedron drifting behind
  const ico=new THREE.LineSegments(
    new THREE.EdgesGeometry(new THREE.IcosahedronGeometry(7,1)),
    new THREE.LineBasicMaterial({color:0xe09924,transparent:true,opacity:0.07}));
  ico.position.set(0,0,-6);
  scene.add(ico);

  const mouse={x:0,y:0,tx:0,ty:0};
  window.addEventListener("mousemove",e=>{
    mouse.tx=(e.clientX/window.innerWidth-.5);
    mouse.ty=(e.clientY/window.innerHeight-.5);
  });

  function resize(){
    const w=window.innerWidth, h=window.innerHeight;
    renderer.setSize(w,h,false);cam.aspect=w/h;cam.updateProjectionMatrix();
  }
  window.addEventListener("resize",resize);resize();

  const LINK=4.2;
  let tms=0;
  function frame(){
    requestAnimationFrame(frame);
    tms+=0.006;
    // recolor to live theme
    const col=new THREE.Color(THEME_RGB[0],THEME_RGB[1],THEME_RGB[2]);
    pMat.color.copy(col);lMat.color.copy(col);ico.material.color.copy(col);

    const p=pGeo.attributes.position.array;
    for(let i=0;i<N;i++){
      p[i*3+1]+=Math.sin(tms*seed[i].sx+seed[i].ph)*0.004;
    }
    pGeo.attributes.position.needsUpdate=true;

    // rebuild near-links
    let li=0;
    for(let i=0;i<N && li<maxLines;i++){
      for(let j=i+1;j<N && li<maxLines;j++){
        const dx=p[i*3]-p[j*3],dy=p[i*3+1]-p[j*3+1],dz=p[i*3+2]-p[j*3+2];
        if(dx*dx+dy*dy+dz*dz<LINK*LINK){
          lPos[li*6]=p[i*3];lPos[li*6+1]=p[i*3+1];lPos[li*6+2]=p[i*3+2];
          lPos[li*6+3]=p[j*3];lPos[li*6+4]=p[j*3+1];lPos[li*6+5]=p[j*3+2];
          li++;
        }
      }
    }
    lGeo.setDrawRange(0,li*2);
    lGeo.attributes.position.needsUpdate=true;

    mouse.x+=(mouse.tx-mouse.x)*0.04;mouse.y+=(mouse.ty-mouse.y)*0.04;
    points.rotation.y=tms*0.06+mouse.x*0.5;
    points.rotation.x=mouse.y*0.3;
    lines.rotation.copy(points.rotation);
    ico.rotation.y+=0.0014;ico.rotation.x+=0.0008;
    cam.position.x+=(mouse.x*4-cam.position.x)*0.04;
    cam.position.y+=(-mouse.y*3-cam.position.y)*0.04;
    cam.lookAt(0,0,0);
    renderer.render(scene,cam);
  }
  frame();
}

/* ═══════════════════════ BOOT ═══════════════════════ */
document.addEventListener("DOMContentLoaded",()=>{
  LANG=detectLang();          // auto-pick language from the visitor's locale
  buildBrand();
  buildToolbar();
  buildControls();
  buildFeatures();            // creates feature nodes with data-i18n
  setTheme("#e09924");
  applyI18n();                // translates the whole site + builds hero preview
  setupCopy();
  setupReveal();
  initThree();
  document.getElementById("simSearch").addEventListener("input",e=>{query=e.target.value;renderGrid();});
});
