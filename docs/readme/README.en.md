<div align="center">

# 🟠 Vanilux

### A fullscreen application launcher for Linux — with a terminal soul.

[Español](../../README.md) ·
**English** ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
[Français](README.fr.md) ·
[日本語](README.ja.md) ·
[한국어](README.ko.md)

![Vanilux in action](../media/vanilux_real.gif)

![version](https://img.shields.io/badge/version-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![i18n](https://img.shields.io/badge/i18n-7%20languages-e09924?style=flat-square)

</div>

---

**Vanilux** is a *Launchpad*-style application launcher for Linux with a dark/amber
terminal aesthetic. Summon it with **F4**; it stays **resident** in memory —so the next
open is instant, like `rofi`— and it's **customizable down to the pixel**: change the
theme color, the hotkey and the language… live. Written in **C++17 + gtkmm 3**: native,
lightweight and fast. No Electron, no heavy runtime.

## ✨ Features

- 🖥️ **Fullscreen, TUI aesthetic** — `[ search ]`, `[ tags ]`, `[ apps ]` and `[ status ]` panels with amber borders.
- 🔍 **Automatic discovery** of apps by parsing the system and user `.desktop` files (honors `NoDisplay`, `Hidden` and `TryExec`).
- ⚡ **Incremental search** by name, case-insensitive, with Unicode case-folding.
- 🏷️ **Categories**, ⭐ **Favorites** and 🕘 **Recents**, persisted on disk.
- 🌟 **Amber glow** around the selected icon, hand-drawn with Cairo (crisp edge + uniform halo via a real blur).
- 🎨 **Live theme color** — a color picker recolors the entire UI instantly.
- 🌐 **7 languages** with an on-screen keyboard that adapts its layout to each (QWERTY · AZERTY · kana · hangul · zhuyin).
- ⌨️ **Full keyboard navigation** (arrows, Enter, `f`, Esc) plus mouse, with a single selection at a time (*focus-follows-mouse*).
- 🚀 **Resident process** → the first open loads the icons; the next ones are immediate.
- 🪶 **C++17 + gtkmm 3** — native and lightweight.

## 📸 Screenshots

### Grid view
![Grid view](../screenshots/grid.png)

### Settings — theme, hotkey and language
![Settings menu](../screenshots/settings.png)

## 🚀 Installation

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

The installer resolves dependencies, builds, copies the binary + CSS + icons,
creates the application-menu entry and **sets up the `F4` hotkey** for you
(it detects Cinnamon, GNOME or XFCE).

> **Manual build:** `make && sudo make install`
> **Dependencies:** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> Tested on **Linux Mint 22.3** (Ubuntu 24.04, X11) with g++ 13.3, gtkmm 3.24 and GTK 3.24.

## ⌨️ Usage

| Action | Key / gesture |
|---|---|
| Open / close the launcher | <kbd>F4</kbd> |
| Filter applications | just start typing |
| Navigate the grid | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Open the selected app | <kbd>Enter</kbd> |
| Toggle favorite | <kbd>f</kbd> |
| Close | <kbd>Esc</kbd> |
| All of the above | also with the mouse (hover = glow) |

## 🎨 Customization (live)

Open settings via the 🔧 (top right):

- **Theme color** — wheel + preset swatches + hex; recolors **everything** instantly.
- **Hotkey** — pick <kbd>F1</kbd>–<kbd>F12</kbd> or **Super + key** on a chiclet-style on-screen keyboard.
- **Language** — Español · English · Português · 中文 · Français · 日本語 · 한국어. Switches the whole UI instantly (and the keyboard adapts its layout).

## 🧱 Tech

**C++17** · **gtkmm 3** (GTK 3) · **Cairo** (hand-drawn glow) · **X11**.
No heavy dependencies: just the system toolkit.

## 🌐 Web demo

[`/landing`](../../landing/) hosts a presentation page with an **interactive simulator**
of the launcher (made-up apps) where you can try the live color and language switching in your browser.

## 🤝 Contributing

Issues and pull requests are welcome. If you like terminal aesthetics or Linux minimalism, drop a ⭐ on the repo.

<div align="center"><sub>vani<b>lux</b> · made with C++ and coffee ☕ · amber terminal aesthetic</sub></div>
