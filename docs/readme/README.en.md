<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### A fullscreen application launcher for Linux — with a terminal soul.

[![See the live demo](https://img.shields.io/badge/%E2%96%B6%20See%20the%20live%20demo-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)

[Español](../../README.md) ·
**English** ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
[Français](README.fr.md) ·
[日本語](README.ja.md) ·
[한국어](README.ko.md) ·
[हिन्दी](README.hi.md) ·
[Русский](README.ru.md)

![Vanilux in action](../media/vanilux_real.gif)

![version](https://img.shields.io/badge/version-1.1-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-9%20languages-e09924?style=flat-square)

</div>

---

**Vanilux** is a *Launchpad*-style application launcher for Linux with a dark/amber
terminal aesthetic. Summon it with **F4**; it stays **resident** in memory —so the next
open is instant, like `rofi`— and it's **customizable down to the pixel**: change the
theme color, the hotkey and the language… live. Written in **C++17 + gtkmm 3**: native,
lightweight and fast. No Electron, no heavy runtime.

## Features

| Feature | Description |
|---|---|
| Fullscreen, TUI aesthetic | `[ search ]`, `[ tags ]`, `[ apps ]` and `[ status ]` panels with amber borders. |
| Automatic discovery | Parses the system and user `.desktop` files (honors `NoDisplay`, `Hidden`, `TryExec`). |
| Incremental search | Filters by name, case-insensitive, with Unicode case-folding. |
| Favorites & Recents | Persisted on disk across sessions. |
| Amber glow in Cairo | The selected icon's halo is hand-drawn with a real blur. |
| Live theme color | A color picker recolors the whole UI instantly. |
| 9 languages | On-screen keyboard that adapts its layout (QWERTY · AZERTY · kana · hangul · zhuyin · cyrillic · devanagari). |
| Keyboard navigation | Arrows, Enter, `f`, Esc — plus the mouse. |
| Resident process | The first open loads the icons; the next ones are immediate. |
| C++17 + gtkmm 3 | Native and lightweight, no Electron. |

## Screenshots

### Grid view
![Grid view](../screenshots/grid.png)

### Settings — theme, hotkey and language
![Settings menu](../screenshots/settings.png)

## Installation

**`.deb` package (recommended · Ubuntu / Linux Mint)** — download `vanilux_1.1_amd64.deb` from [Releases](https://github.com/facilized-pbustamante/vanilux/releases) and install it:

```bash
sudo apt install ./vanilux_1.1_amd64.deb
```

**Or build from source:**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

The installer resolves dependencies, builds, copies the binary + CSS + icons,
creates the application-menu entry and sets up the `F4` hotkey for you
(it detects Cinnamon, GNOME or XFCE).

| | |
|---|---|
| Manual build | `make && sudo make install` |
| Dependencies | `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make` |
| Tested on | Ubuntu 24.04 · Linux Mint 22.3 (X11) — g++ 13.3, gtkmm 3.24, GTK 3.24 |

## Usage

| Action | Key / gesture |
|---|---|
| Open / close the launcher | <kbd>F4</kbd> |
| Filter applications | just start typing |
| Navigate the grid | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Open the selected app | <kbd>Enter</kbd> |
| Toggle favorite | <kbd>f</kbd> |
| Close | <kbd>Esc</kbd> |
| All of the above | also with the mouse (hover = glow) |

## Customization (live)

Open settings via the wrench button (top right):

![Live theme color and languages](../media/vanilux_tema_idioma.gif)

| Setting | What it does |
|---|---|
| Theme color | Wheel + preset swatches + hex; recolors **everything** instantly. |
| Hotkey | Pick <kbd>F1</kbd>–<kbd>F12</kbd> or **Super + key** on a chiclet-style on-screen keyboard. |
| Language | Español · English · Português · 中文 · Français · 日本語 · 한국어 · हिन्दी · Русский — switches the whole UI instantly. |

## Tech

| Component | Detail |
|---|---|
| Language | C++17 |
| Toolkit | gtkmm 3 (GTK 3) |
| Graphics | Cairo (hand-drawn glow) |
| Display | X11 |
| Tested on | Ubuntu · Linux Mint |

## Web demo

[`/landing`](../../landing/) hosts a presentation page with an **interactive simulator**
of the launcher (made-up apps) where you can try the live color and language switching:
**[facilized-pbustamante.github.io/vanilux](https://facilized-pbustamante.github.io/vanilux/)**

## Contributing

Issues and pull requests are welcome.

---

<div align="center">

## ⭐ Enjoying Vanilux?

### Leave a star — it's free and helps the project grow

[![Leave a star](https://img.shields.io/badge/%E2%AD%90%20Leave%20a%20star-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

<sub>vani<b>lux</b> · made with C++ and coffee · amber terminal aesthetic</sub>

</div>
