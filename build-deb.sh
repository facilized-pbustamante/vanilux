#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# build-deb.sh — construye vanilux_<VERSION>_amd64.deb de forma reproducible.
#
# El binario se compila DENTRO de Ubuntu 22.04 (glibc 2.35) para que funcione en
# Ubuntu 22.04+ y Linux Mint 21/22 sin problemas de glibc. El paquete incluye el
# binario (con los 9 idiomas embebidos), style.css y TODOS los iconos (banderas
# incluidas), más los scripts de post-instalación (atajo F4 + entrada de menú).
#
# Uso:  ./build-deb.sh
# ─────────────────────────────────────────────────────────────────────────────
set -euo pipefail
cd "$(dirname "$0")"
SRC_DIR="$(pwd)"
VERSION="$(cat VERSION)"
PKG="vanilux"
ARCH="amd64"
OUT="${PKG}_${VERSION}_${ARCH}.deb"

echo "==> Compilando el binario en Ubuntu 22.04 (glibc 2.35) vía Docker…"
docker run --rm -v "$SRC_DIR:/src:ro" -v "$SRC_DIR:/out" ubuntu:22.04 bash -c '
  set -eux
  export DEBIAN_FRONTEND=noninteractive
  apt-get update -qq
  apt-get install -y -qq build-essential make pkg-config libgtkmm-3.0-dev
  cp -r /src /build && cd /build
  make clean || true
  make
  cp vanilux /out/vanilux-docker
  chmod 755 /out/vanilux-docker
'

echo "==> Ensamblando el árbol del paquete…"
ROOT="$(mktemp -d)/${PKG}_${VERSION}"
mkdir -p "$ROOT/usr/bin" \
         "$ROOT/usr/share/vanilux/icons" \
         "$ROOT/usr/share/applications" \
         "$ROOT/DEBIAN"

install -m755 vanilux-docker      "$ROOT/usr/bin/vanilux"
install -m755 configure-vanilux   "$ROOT/usr/bin/configure-vanilux"
install -m644 src/style.css       "$ROOT/usr/share/vanilux/style.css"
install -m644 src/icons/*.svg     "$ROOT/usr/share/vanilux/icons/"

# control con la versión actual (toma VERSION, no la del archivo)
sed "s/^Version:.*/Version: ${VERSION}/" debian/control > "$ROOT/DEBIAN/control"
install -m755 debian/postinst "$ROOT/DEBIAN/postinst"
install -m755 debian/prerm    "$ROOT/DEBIAN/prerm"

echo "==> dpkg-deb --build…"
dpkg-deb --root-owner-group --build "$ROOT" "$OUT" >/dev/null
rm -f vanilux-docker
rm -rf "$(dirname "$ROOT")"

echo "==> Listo: $OUT"
dpkg-deb -I "$OUT" | sed -n '1,4p;/Version/p;/Depends/p'
echo "    idiomas embebidos: 9 (ES EN PT ZH FR JA KO HI RU) · banderas: $(ls src/icons/flag_*.svg | wc -l)"
echo "    !important en CSS empaquetado: $(dpkg-deb --fsys-tarfile "$OUT" | tar -xO ./usr/share/vanilux/style.css | grep -c '!important')"
