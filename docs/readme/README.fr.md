<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### Un lanceur d'applications plein écran pour Linux — à l'âme de terminal.

[Español](../../README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
**Français** ·
[日本語](README.ja.md) ·
[한국어](README.ko.md)

![Vanilux en action](../media/vanilux_real.gif)

![version](https://img.shields.io/badge/version-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-7%20langues-e09924?style=flat-square)

[![Voir la démo en direct](https://img.shields.io/badge/%E2%96%B6%20Voir%20la%20d%C3%A9mo%20en%20direct-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)
&nbsp;&nbsp;
[![Laissez une étoile](https://img.shields.io/badge/%E2%AD%90%20Laissez%20une%20%C3%A9toile-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)

</div>

---

**Vanilux** est un lanceur d'applications façon *Launchpad* pour Linux, à l'esthétique
terminal sombre/ambre. Appelez-le avec **F4** ; il reste **résident** en mémoire —pour que
la prochaine ouverture soit instantanée, comme `rofi`— et il est **personnalisable au pixel
près** : changez la couleur du thème, le raccourci et la langue… en direct. Écrit en
**C++17 + gtkmm 3** : natif, léger et rapide. Pas d'Electron, pas de runtime lourd.

## ✨ Fonctionnalités

- 🖥️ **Plein écran, esthétique TUI** — panneaux `[ search ]`, `[ tags ]`, `[ apps ]` et `[ status ]` aux bords ambre.
- 🔍 **Découverte automatique** des apps en analysant les `.desktop` du système et de l'utilisateur (respecte `NoDisplay`, `Hidden` et `TryExec`).
- ⚡ **Recherche incrémentale** par nom, insensible à la casse, avec *case-folding* Unicode.
- 🏷️ **Catégories**, ⭐ **Favoris** et 🕘 **Récents**, persistés sur disque.
- 🌟 **Halo ambre** autour de l'icône sélectionnée, dessiné à la main avec Cairo (bord net + halo uniforme via un vrai flou).
- 🎨 **Couleur du thème en direct** — un sélecteur de couleur recolore toute l'interface à l'instant.
- 🌐 **7 langues** avec un clavier à l'écran qui adapte sa disposition à chacune (QWERTY · AZERTY · kana · hangul · zhuyin).
- ⌨️ **Navigation clavier complète** (flèches, Enter, `f`, Esc) en plus de la souris, avec une seule sélection à la fois (*focus-follows-mouse*).
- 🚀 **Processus résident** → la première ouverture charge les icônes ; les suivantes sont immédiates.
- 🪶 **C++17 + gtkmm 3** — natif et léger.

## 📸 Captures

### Vue en grille
![Vue en grille](../screenshots/grid.png)

### Paramètres — thème, raccourci et langue
![Menu des paramètres](../screenshots/settings.png)

## 🚀 Installation

**Paquet `.deb` (recommandé · Ubuntu / Linux Mint)** — téléchargez `vanilux_1.0_amd64.deb` depuis [Releases](https://github.com/facilized-pbustamante/vanilux/releases) et installez-le :

```bash
sudo apt install ./vanilux_1.0_amd64.deb
```

**Ou compilez depuis les sources :**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

L'installateur résout les dépendances, compile, copie le binaire + CSS + icônes,
crée l'entrée dans le menu des applications et **configure le raccourci `F4`** pour vous
(il détecte Cinnamon, GNOME ou XFCE).

> **Compilation manuelle :** `make && sudo make install`
> **Dépendances :** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> Testé sur **Linux Mint 22.3** (Ubuntu 24.04, X11) avec g++ 13.3, gtkmm 3.24 et GTK 3.24.

## ⌨️ Utilisation

| Action | Touche / geste |
|---|---|
| Ouvrir / fermer le lanceur | <kbd>F4</kbd> |
| Filtrer les applications | commencez à taper |
| Naviguer dans la grille | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Ouvrir l'app sélectionnée | <kbd>Enter</kbd> |
| (Dé)marquer favori | <kbd>f</kbd> |
| Fermer | <kbd>Esc</kbd> |
| Tout ce qui précède | aussi à la souris (survol = halo) |

## 🎨 Personnalisation (en direct)

Ouvrez les paramètres via la 🔧 (en haut à droite) :

- **Couleur du thème** — roue + échantillons prédéfinis + hex ; recolore **tout** à l'instant.
- **Raccourci** — choisissez <kbd>F1</kbd>–<kbd>F12</kbd> ou **Super + touche** sur un clavier à l'écran style chiclet.
- **Langue** — Español · English · Português · 中文 · Français · 日本語 · 한국어. Bascule toute l'UI à l'instant (et le clavier adapte sa disposition).

## 🧱 Technologie

**C++17** · **gtkmm 3** (GTK 3) · **Cairo** (halo dessiné à la main) · **X11**.
Aucune dépendance lourde : juste le toolkit du système.

## 🌐 Démo web

[`/landing`](../../landing/) héberge une page de présentation avec un **simulateur interactif**
du lanceur (apps fictives) où vous pouvez tester le changement de couleur et de langue dans le navigateur.

## 🤝 Contribuer

Les issues et pull requests sont les bienvenues. Si vous aimez l'esthétique terminal ou le minimalisme sous Linux, laissez une ⭐ au dépôt.

---

<div align="center">

## ⭐ Vanilux vous plaît ?

### Laissez une étoile — c'est gratuit et ça aide le projet 🙌

[![star](https://img.shields.io/badge/%E2%AD%90%20Laissez%20une%20%C3%A9toile-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

</div>

<div align="center"><sub>vani<b>lux</b> · fait avec C++ et café ☕ · esthétique terminal ambre</sub></div>
