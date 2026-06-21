<div align="center">

<img src="docs/logo.svg" width="116" alt="Vanilux">

# Vanilux

### Un lanzador de aplicaciones a pantalla completa para Linux — con alma de terminal.

[![Ver demo en vivo](https://img.shields.io/badge/%E2%96%B6%20Ver%20demo%20en%20vivo-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)

**Español** ·
[English](docs/readme/README.en.md) ·
[Português](docs/readme/README.pt.md) ·
[中文](docs/readme/README.zh.md) ·
[Français](docs/readme/README.fr.md) ·
[日本語](docs/readme/README.ja.md) ·
[한국어](docs/readme/README.ko.md) ·
[हिन्दी](docs/readme/README.hi.md) ·
[Русский](docs/readme/README.ru.md)

![Vanilux en acción](docs/media/vanilux_real.gif)

![versión](https://img.shields.io/badge/versi%C3%B3n-1.1-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-9%20idiomas-e09924?style=flat-square)

</div>

---

**Vanilux** es un lanzador de aplicaciones estilo *Launchpad* para Linux, con estética
de terminal oscuro/ámbar. Se invoca con **F4**, queda **residente** en memoria —para que
la siguiente apertura sea instantánea, como `rofi`— y es **personalizable hasta el píxel**:
cambiá el color del tema, la tecla rápida y el idioma… en vivo. Escrito en **C++17 + gtkmm 3**:
nativo, liviano y rápido. Sin Electron, sin runtime pesado.

## Características

| Característica | Descripción |
|---|---|
| Pantalla completa, estética TUI | Paneles `[ buscar ]`, `[ etiquetas ]`, `[ apps ]` y `[ estado ]` con bordes ámbar. |
| Descubrimiento automático | Parsea los `.desktop` del sistema y del usuario (respeta `NoDisplay`, `Hidden`, `TryExec`). |
| Búsqueda incremental | Filtra por nombre, sin distinguir mayúsculas, con *case-folding* Unicode. |
| Favoritos y Recientes | Persistentes en disco entre sesiones. |
| Glow ámbar en Cairo | El halo del ícono seleccionado se dibuja a mano con un *blur* real. |
| Color del tema en vivo | Un *color picker* recolorea toda la interfaz al instante. |
| 9 idiomas | Teclado en pantalla que adapta su layout (QWERTY · AZERTY · kana · hangul · zhuyin · cirílico · devanagari). |
| Navegación por teclado | Flechas, Enter, `f`, Esc — además del mouse. |
| Proceso residente | La primera apertura carga los íconos; las siguientes son inmediatas. |
| C++17 + gtkmm 3 | Nativo y liviano, sin Electron. |

## Capturas

### Vista de cuadrícula
![Vista de cuadrícula](docs/screenshots/grid.png)

### Configuración — tema, tecla rápida e idioma
![Menú de configuración](docs/screenshots/settings.png)

## Instalación

**Paquete `.deb` (recomendado · Ubuntu / Linux Mint)** — descargá `vanilux_1.1_amd64.deb` desde [Releases](https://github.com/facilized-pbustamante/vanilux/releases) e instalalo:

```bash
sudo apt install ./vanilux_1.1_amd64.deb
```

**O compilá desde el código fuente:**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

El instalador resuelve las dependencias, compila, copia el binario + CSS + iconos,
crea la entrada en el menú de aplicaciones y configura el atajo `F4` por vos
(detecta Cinnamon, GNOME o XFCE).

| | |
|---|---|
| Compilación manual | `make && sudo make install` |
| Dependencias | `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make` |
| Probado en | Ubuntu 24.04 · Linux Mint 22.3 (X11) — g++ 13.3, gtkmm 3.24, GTK 3.24 |

## Uso

| Acción | Tecla / gesto |
|---|---|
| Abrir / cerrar el lanzador | <kbd>F4</kbd> |
| Filtrar aplicaciones | empezá a escribir |
| Navegar la grilla | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Abrir la app seleccionada | <kbd>Enter</kbd> |
| Marcar/desmarcar favorito | <kbd>f</kbd> |
| Cerrar | <kbd>Esc</kbd> |
| Todo lo anterior | también con el mouse (hover = glow) |

## Personalización (en vivo)

Abrí la configuración con el botón de la llave (arriba a la derecha):

![Color del tema en vivo e idiomas](docs/media/vanilux_tema_idioma.gif)

| Ajuste | Qué hace |
|---|---|
| Color del tema | Rueda + colores predefinidos + hex; recolorea **todo** al instante. |
| Tecla rápida | Elegí entre <kbd>F1</kbd>–<kbd>F12</kbd> o **Súper + tecla** en un teclado en pantalla estilo chiclet. |
| Idioma | Español · English · Português · 中文 · Français · 日本語 · 한국어 · हिन्दी · Русский — cambia toda la UI al instante. |

## Tecnología

| Componente | Detalle |
|---|---|
| Lenguaje | C++17 |
| Toolkit | gtkmm 3 (GTK 3) |
| Gráficos | Cairo (glow dibujado a mano) |
| Display | X11 |
| Probado en | Ubuntu · Linux Mint |

## Demo web

En [`/landing`](landing/) hay una página de presentación con un **simulador interactivo**
del lanzador (apps inventadas) donde podés probar el cambio de color y de idioma en el navegador:
**[facilized-pbustamante.github.io/vanilux](https://facilized-pbustamante.github.io/vanilux/)**

## Contribuir

Issues y *pull requests* son bienvenidos.

---

<div align="center">

## ⭐ ¿Te gustó Vanilux?

### Dejá una estrella — es gratis y ayuda a que el proyecto crezca

[![Dejá una estrella](https://img.shields.io/badge/%E2%AD%90%20Dej%C3%A1%20una%20estrella-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

[![Star History Chart](https://api.star-history.com/svg?repos=facilized-pbustamante/vanilux&type=Date)](https://star-history.com/#facilized-pbustamante/vanilux&Date)

<sub>vani<b>lux</b> · hecho con C++ y café · estética terminal ámbar</sub>

</div>
