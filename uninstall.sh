#!/usr/bin/env bash
#
# vanilux — desinstalador
#
set -euo pipefail

APP="vanilux"
PREFIX="${PREFIX:-/usr/local}"
APPS_DIR="/usr/share/applications"

if [ "$(id -u)" -eq 0 ]; then SUDO=""; RUNNING_AS_ROOT=1; else SUDO="sudo"; RUNNING_AS_ROOT=0; fi

echo "» Deteniendo instancia residente…"
killall "$APP" 2>/dev/null || true
rm -f /tmp/$APP.pid

echo "» Quitando binario, datos compartidos y entrada de menú…"
$SUDO rm -f  "$PREFIX/bin/$APP"
$SUDO rm -rf "$PREFIX/share/$APP"
$SUDO rm -f  "$APPS_DIR/$APP.desktop"
$SUDO update-desktop-database "$APPS_DIR" >/dev/null 2>&1 || true

# Quitar el atajo F4 (por-usuario)
if [ "$RUNNING_AS_ROOT" -eq 0 ] && command -v gsettings >/dev/null 2>&1; then
  if gsettings list-schemas 2>/dev/null | grep -qx "org.cinnamon.desktop.keybindings"; then
    cur="$(gsettings get org.cinnamon.desktop.keybindings custom-list 2>/dev/null || echo '@as []')"
    if printf '%s' "$cur" | grep -q "'vanilux'"; then
      new="$(printf '%s' "$cur" | sed "s/, *'vanilux'//; s/'vanilux', *//; s/\['vanilux'\]/@as []/")"
      gsettings set org.cinnamon.desktop.keybindings custom-list "$new" 2>/dev/null || true
      echo "» Atajo F4 (Cinnamon) removido."
    fi
  fi
fi

echo "✔ Desinstalado. (La config de usuario en ~/.config/$APP/ no se toca.)"
echo "  Para borrarla también:  rm -rf ~/.config/$APP"
