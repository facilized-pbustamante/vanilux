<div align="center">

# 🟠 Vanilux

### Un lanzador de aplicaciones a pantalla completa para Linux — con alma de terminal.

**Español** ·
[English](docs/readme/README.en.md) ·
[Português](docs/readme/README.pt.md) ·
[中文](docs/readme/README.zh.md) ·
[Français](docs/readme/README.fr.md) ·
[日本語](docs/readme/README.ja.md) ·
[한국어](docs/readme/README.ko.md)

![Vanilux en acción](docs/media/vanilux_real.gif)

![versión](https://img.shields.io/badge/versi%C3%B3n-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![i18n](https://img.shields.io/badge/i18n-7%20idiomas-e09924?style=flat-square)

### 🌐 [Ver demo en vivo →](https://facilized-pbustamante.github.io/vanilux/)

</div>

---

**Vanilux** es un lanzador de aplicaciones estilo *Launchpad* para Linux, con estética
de terminal oscuro/ámbar. Se invoca con **F4**, queda **residente** en memoria —para que
la siguiente apertura sea instantánea, como `rofi`— y es **personalizable hasta el píxel**:
cambiá el color del tema, la tecla rápida y el idioma… en vivo. Escrito en **C++17 + gtkmm 3**:
nativo, liviano y rápido. Sin Electron, sin runtime pesado.

## ✨ Características

- 🖥️ **Pantalla completa, estética TUI** — paneles `[ buscar ]`, `[ etiquetas ]`, `[ apps ]` y `[ estado ]` con bordes ámbar.
- 🔍 **Descubrimiento automático** de apps parseando los `.desktop` del sistema y de tu usuario (respeta `NoDisplay`, `Hidden` y `TryExec`).
- ⚡ **Búsqueda incremental** por nombre, sin distinguir mayúsculas, con *case-folding* Unicode.
- 🏷️ **Categorías**, ⭐ **Favoritos** y 🕘 **Recientes** persistentes en disco.
- 🌟 **Glow ámbar** alrededor del ícono seleccionado, dibujado a mano con Cairo (borde nítido + halo difuso por *blur* real).
- 🎨 **Color del tema en vivo** — un *color picker* recolorea toda la interfaz al instante.
- 🌐 **7 idiomas** con teclado en pantalla que adapta su *layout* a cada uno (QWERTY · AZERTY · kana · hangul · zhuyin).
- ⌨️ **Navegación completa por teclado** (flechas, Enter, `f`, Esc) además de mouse, con una sola selección a la vez (*focus-follows-mouse*).
- 🚀 **Proceso residente** → la primera apertura carga los íconos; las siguientes son inmediatas.
- 🪶 **C++17 + gtkmm 3** — nativo y liviano.

## 📸 Capturas

### Vista de cuadrícula
![Vista de cuadrícula](docs/screenshots/grid.png)

### Configuración — tema, tecla rápida e idioma
![Menú de configuración](docs/screenshots/settings.png)

## 🚀 Instalación

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

El instalador resuelve las dependencias, compila, copia el binario + CSS + iconos,
crea la entrada en el menú de aplicaciones y **configura el atajo `F4`** por vos
(detecta Cinnamon, GNOME o XFCE).

> **Compilación manual:** `make && sudo make install`
> **Dependencias:** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> Probado en **Linux Mint 22.3** (Ubuntu 24.04, X11) con g++ 13.3, gtkmm 3.24 y GTK 3.24.

## ⌨️ Uso

| Acción | Tecla / gesto |
|---|---|
| Abrir / cerrar el lanzador | <kbd>F4</kbd> |
| Filtrar aplicaciones | empezá a escribir |
| Navegar la grilla | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Abrir la app seleccionada | <kbd>Enter</kbd> |
| Marcar/desmarcar favorito | <kbd>f</kbd> |
| Cerrar | <kbd>Esc</kbd> |
| Todo lo anterior | también con el mouse (hover = glow) |

## 🎨 Personalización (en vivo)

Abrí la configuración con la 🔧 (arriba a la derecha):

- **Color del tema** — rueda + colores predefinidos + hex; recolorea **todo** al instante.
- **Tecla rápida** — elegí entre <kbd>F1</kbd>–<kbd>F12</kbd> o **Súper + tecla** en un teclado en pantalla estilo chiclet.
- **Idioma** — Español · English · Português · 中文 · Français · 日本語 · 한국어. Cambia toda la UI al instante (y el teclado adapta su layout).

## 🧱 Tecnología

**C++17** · **gtkmm 3** (GTK 3) · **Cairo** (glow dibujado a mano) · **X11**.
Sin dependencias pesadas: solo el toolkit del sistema.

## 🌐 Demo web

En [`/landing`](landing/) hay una página de presentación con un **simulador interactivo**
del lanzador (apps inventadas) donde podés probar el cambio de color y de idioma en el navegador.

## 🤝 Contribuir

Issues y *pull requests* son bienvenidos. Si te gusta la estética terminal o el minimalismo en Linux, dejá una ⭐ en el repo.

<div align="center"><sub>vani<b>lux</b> · hecho con C++ y café ☕ · estética terminal ámbar</sub></div>
