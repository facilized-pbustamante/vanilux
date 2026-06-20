#!/usr/bin/env bash
#
# vanilux — instalador todo-en-uno
#
#   ./install.sh                     instala (elige idioma, tecla F4 default)
#   ./install.sh --key '<Alt>F1'     instala con Alt+F1
#   PREFIX=/usr ./install.sh         cambia prefijo
#
set -euo pipefail

VERSION="$(cat "$(dirname "$0")/VERSION" 2>/dev/null || echo 0.2)"
APP="vanilux"
PREFIX="${PREFIX:-/usr/local}"
APPS_DIR="/usr/share/applications"
BIN="$PREFIX/bin/$APP"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CONFIG_DIR="$HOME/.config/vanilux"
CONFIG_FILE="$CONFIG_DIR/config.txt"

# ── argumentos ──────────────────────────────────────────────────────────────
HOTKEY=""
while [ $# -gt 0 ]; do
  case "$1" in
    --key) shift; HOTKEY="$1" ;;
    *) echo "Uso: ./install.sh [--key <tecla>]" >&2; exit 1 ;;
  esac
  shift
done

# ── colores ──────────────────────────────────────────────────────────────────
if [ -t 1 ]; then
  B="\033[1m"; G="\033[32m"; Y="\033[33m"; R="\033[31m"; C="\033[36m"; N="\033[0m"
else
  B=""; G=""; Y=""; R=""; C=""; N=""
fi
say()  { printf "${C}»${N} %s\n" "$*"; }
ok()   { printf "${G}✔${N} %s\n" "$*"; }
warn() { printf "${Y}!${N} %s\n" "$*"; }
die()  { printf "${R}✘${N} %s\n" "$*" >&2; exit 1; }

# ── sudo ─────────────────────────────────────────────────────────────────────
if [ "$(id -u)" -eq 0 ]; then
  SUDO="" RUNNING_AS_ROOT=1
else
  command -v sudo >/dev/null 2>&1 || die "Se necesita 'sudo' (o correr como root)."
  SUDO="sudo"; RUNNING_AS_ROOT=0
fi

printf "${B}┌──────────────────────────────────────────────────┐${N}\n"
printf "${B}│   vanilux — instalador  (v%s)               │${N}\n" "$VERSION"
printf "${B}└──────────────────────────────────────────────────┘${N}\n"

cd "$SCRIPT_DIR"

# ── 0) detectar idioma (sin interacción) ─────────────────────────────────────
SYS_LANG="${LANG:-es_CL.UTF-8}"
case "$SYS_LANG" in
  pt*|PT*) LANG_CODE="PT" ;;
  fr*|FR*) LANG_CODE="FR" ;;
  ja*|JA*) LANG_CODE="JA" ;;
  ko*|KO*) LANG_CODE="KO" ;;
  zh*|ZH*) LANG_CODE="ZH" ;;
  en*|EN*) LANG_CODE="EN" ;;
  *)       LANG_CODE="ES" ;;
esac
ok "Idioma autodetectado: $LANG_CODE (desde \$LANG=$SYS_LANG)"

# ── 1) dependencias ─────────────────────────────────────────────────────────
say "Instalando dependencias…"
if [ -x "./$APP" ] && file "./$APP" | grep -qi "ELF.*executable" >/dev/null 2>&1; then
  PREBUILT=1
  if   command -v apt-get >/dev/null 2>&1; then
    $SUDO apt-get update -qq
    $SUDO apt-get install -y libgtkmm-3.0-1v5 papirus-icon-theme 2>/dev/null
  elif command -v dnf >/dev/null 2>&1; then
    $SUDO dnf install -y gtkmm30 papirus-icon-theme 2>/dev/null
  elif command -v pacman >/dev/null 2>&1; then
    $SUDO pacman -S --needed --noconfirm gtkmm3 papirus-icon-theme 2>/dev/null
  elif command -v zypper >/dev/null 2>&1; then
    $SUDO zypper install -y gtkmm3 papirus-icon-theme 2>/dev/null
  fi
else
  PREBUILT=0
  if   command -v apt-get >/dev/null 2>&1; then
    $SUDO apt-get update -qq
    $SUDO apt-get install -y build-essential make pkg-config libgtkmm-3.0-dev papirus-icon-theme 2>/dev/null
  elif command -v dnf >/dev/null 2>&1; then
    $SUDO dnf install -y gcc-c++ make pkgconf-pkg-config gtkmm30-devel papirus-icon-theme 2>/dev/null
  elif command -v pacman >/dev/null 2>&1; then
    $SUDO pacman -S --needed --noconfirm base-devel gtkmm3 papirus-icon-theme 2>/dev/null
  elif command -v zypper >/dev/null 2>&1; then
    $SUDO zypper install -y gcc-c++ make pkg-config gtkmm3-devel papirus-icon-theme 2>/dev/null
  fi
  command -v g++  >/dev/null 2>&1 || die "Falta g++ (compilador C++)."
  command -v make >/dev/null 2>&1 || die "Falta make."
fi
command -v ldconfig >/dev/null 2>&1 && ldconfig 2>/dev/null || true
ok "Dependencias listas."

# ── 2) compilar (o usar binario pre-compilado) ──────────────────────────────
if [ "$PREBUILT" = "1" ]; then
  say "Binario pre-compilado detectado, saltando compilación…"
else
  say "Compilando…"
  make clean >/dev/null 2>&1 || true
  make
fi
ok "Listo: ./$APP $(./$APP --version 2>/dev/null || echo "v$VERSION")"

# ── 3) instalar binario + CSS + iconos ──────────────────────────────────────
say "Instalando en $PREFIX (requiere privilegios)…"
$SUDO make install PREFIX="$PREFIX"
ok "Instalado en $BIN"

# ── 4) entrada en el menú de aplicaciones ───────────────────────────────────
say "Creando entrada en el menú de aplicaciones…"
$SUDO mkdir -p "$APPS_DIR"
$SUDO tee "$APPS_DIR/$APP.desktop" >/dev/null <<EOF
[Desktop Entry]
Type=Application
Version=1.0
Name=Vanilux
GenericName=Application Launcher
Comment=Lanzador de aplicaciones a pantalla completa (estilo Launchpad)
Exec=$BIN
Icon=$PREFIX/share/$APP/icons/grid_amber.svg
Terminal=false
Categories=Utility;
Keywords=launcher;apps;menu;lanzador;aplicaciones;
StartupNotify=false
EOF
$SUDO update-desktop-database "$APPS_DIR" >/dev/null 2>&1 || true
ok "Entrada de menú creada."

# ── 5) escribir config (idioma + tecla rápida por defecto F4) ───────────────
say "Guardando configuración…"
mkdir -p "$CONFIG_DIR"
cat > "$CONFIG_FILE" <<EOF
color=#e09924
hotkey=${HOTKEY:-F4}
lang=$LANG_CODE
EOF
ok "Configuración guardada ($CONFIG_FILE)."

# ── 6) configurar atajo ─────────────────────────────────────────────────────
setup_keybind() {
  local key="$1"
  command -v gsettings >/dev/null 2>&1 || return 1
  local schemas; schemas="$(gsettings list-schemas 2>/dev/null || true)"

  if printf '%s\n' "$schemas" | grep -qx "org.cinnamon.desktop.keybindings"; then
    local s="org.cinnamon.desktop.keybindings"
    local relo="org.cinnamon.desktop.keybindings.custom-keybinding"
    local p="/org/cinnamon/desktop/keybindings/custom-keybindings/vanilux/"
    local cur; cur="$(gsettings get "$s" custom-list 2>/dev/null || echo '@as []')"
    case "$cur" in
      *"'vanilux'"*) : ;;
      "@as []"|"[]"|"") gsettings set "$s" custom-list "['vanilux']" ;;
      *) gsettings set "$s" custom-list "${cur%]}, 'vanilux']" ;;
    esac
    gsettings set "$relo:$p" name "'Vanilux'"
    gsettings set "$relo:$p" command "'$BIN'"
    gsettings set "$relo:$p" binding "['$key']"
    echo "cinnamon"; return 0
  fi

  if printf '%s\n' "$schemas" | grep -qx "org.gnome.settings-daemon.plugins.media-keys"; then
    local s="org.gnome.settings-daemon.plugins.media-keys"
    local relo="org.gnome.settings-daemon.plugins.media-keys.custom-keybinding"
    local p="/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/vanilux/"
    local cur; cur="$(gsettings get "$s" custom-keybindings 2>/dev/null || echo '@as []')"
    case "$cur" in
      *"$p"*) : ;;
      "@as []"|"[]"|"") gsettings set "$s" custom-keybindings "['$p']" ;;
      *) gsettings set "$s" custom-keybindings "${cur%]}, '$p']" ;;
    esac
    gsettings set "$relo:$p" name 'Vanilux'
    gsettings set "$relo:$p" command "$BIN"
    gsettings set "$relo:$p" binding "['$key']"
    echo "gnome"; return 0
  fi

  if command -v xfconf-query >/dev/null 2>&1; then
    xfconf-query -c xfce4-keyboard-shortcuts -n -t string -p "/commands/custom/$key" -s "$BIN" 2>/dev/null || \
    xfconf-query -c xfce4-keyboard-shortcuts -t string -p "/commands/custom/$key" -s "$BIN" 2>/dev/null || true
    echo "xfce"; return 0
  fi

  return 1
}

HOTKEY="${HOTKEY:-F4}"
say "Configurando tecla rápida ${B}$HOTKEY${N}…"
if [ "$RUNNING_AS_ROOT" -eq 1 ]; then
  warn "Corriendo como root: el atajo es por-usuario, no se puede setear aquí."
  warn "Corré esto como tu usuario para configurarlo:"
  warn "  gsettings ... (o asignalo a mano en Teclado → Atajos)."
else
  KB="$(setup_keybind "$HOTKEY" || true)"
  if [ -n "${KB:-}" ]; then
    ok "Atajo $HOTKEY configurado ($KB)."
  else
    warn "No pude configurar $HOTKEY automáticamente en este entorno."
    warn "Asignalo a mano: Atajos de teclado → comando '$BIN' → tecla $HOTKEY."
  fi
fi

# ── listo ────────────────────────────────────────────────────────────────────
killall "$APP" 2>/dev/null || true

printf "\n${G}${B}¡Instalación completa! (vanilux %s)${N}\n" "$VERSION"
echo   "  • Idioma:     ${B}$LANG_CODE${N}"
echo   "  • Atajo:      ${B}$HOTKEY${N}"
echo   "  • Lanzá con   ${B}$HOTKEY${N} o desde el menú de aplicaciones."
echo   "  • Para desinstalar:  ./uninstall.sh"
echo   "  • Para cambiar idioma/tecla: abrí Vanilux → Configuración"
