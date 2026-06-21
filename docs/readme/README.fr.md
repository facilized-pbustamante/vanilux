<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### Un lanceur d'applications plein écran pour Linux — à l'âme de terminal.

[![Voir la démo en direct](https://img.shields.io/badge/%E2%96%B6%20Voir%20la%20d%C3%A9mo%20en%20direct-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)

[Español](../../README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
**Français** ·
[日本語](README.ja.md) ·
[한국어](README.ko.md) ·
[हिन्दी](README.hi.md) ·
[Русский](README.ru.md)

![Vanilux en action](../media/vanilux_real.gif)

![version](https://img.shields.io/badge/version-1.1-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-9%20langues-e09924?style=flat-square)

</div>

---

**Vanilux** est un lanceur d'applications façon *Launchpad* pour Linux, à l'esthétique
terminal sombre/ambre. Appelez-le avec **F4** ; il reste **résident** en mémoire —pour que
la prochaine ouverture soit instantanée, comme `rofi`— et il est **personnalisable au pixel
près** : changez la couleur du thème, le raccourci et la langue… en direct. Écrit en
**C++17 + gtkmm 3** : natif, léger et rapide. Pas d'Electron, pas de runtime lourd.

## Fonctionnalités

| Fonctionnalité | Description |
|---|---|
| Plein écran, esthétique TUI | Panneaux `[ search ]`, `[ tags ]`, `[ apps ]` et `[ status ]` aux bords ambre. |
| Découverte automatique | Analyse les `.desktop` du système et de l'utilisateur (respecte `NoDisplay`, `Hidden`, `TryExec`). |
| Recherche incrémentale | Filtre par nom, insensible à la casse, avec *case-folding* Unicode. |
| Favoris et Récents | Persistés sur disque entre les sessions. |
| Halo ambre en Cairo | Le halo de l'icône sélectionnée est dessiné à la main avec un vrai flou. |
| Couleur du thème en direct | Un sélecteur de couleur recolore toute l'interface à l'instant. |
| 9 langues | Clavier à l'écran qui adapte sa disposition (QWERTY · AZERTY · kana · hangul · zhuyin · cyrillique · devanagari). |
| Navigation clavier | Flèches, Enter, `f`, Esc — ainsi que la souris. |
| Processus résident | La première ouverture charge les icônes ; les suivantes sont immédiates. |
| C++17 + gtkmm 3 | Natif et léger, sans Electron. |

## Captures

### Vue en grille
![Vue en grille](../screenshots/grid.png)

### Paramètres — thème, raccourci et langue
![Menu des paramètres](../screenshots/settings.png)

## Installation

**Paquet `.deb` (recommandé · Ubuntu / Linux Mint)** — téléchargez `vanilux_1.1_amd64.deb` depuis [Releases](https://github.com/facilized-pbustamante/vanilux/releases) et installez-le :

```bash
sudo apt install ./vanilux_1.1_amd64.deb
```

**Ou compilez depuis les sources :**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

L'installateur résout les dépendances, compile, copie le binaire + CSS + icônes,
crée l'entrée dans le menu des applications et configure le raccourci `F4` pour vous
(il détecte Cinnamon, GNOME ou XFCE).

| | |
|---|---|
| Compilation manuelle | `make && sudo make install` |
| Dépendances | `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make` |
| Testé sur | Ubuntu 24.04 · Linux Mint 22.3 (X11) — g++ 13.3, gtkmm 3.24, GTK 3.24 |

## Utilisation

| Action | Touche / geste |
|---|---|
| Ouvrir / fermer le lanceur | <kbd>F4</kbd> |
| Filtrer les applications | commencez à taper |
| Naviguer dans la grille | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Ouvrir l'app sélectionnée | <kbd>Enter</kbd> |
| (Dé)marquer favori | <kbd>f</kbd> |
| Fermer | <kbd>Esc</kbd> |
| Tout ce qui précède | aussi à la souris (survol = halo) |

## Personnalisation (en direct)

Ouvrez les paramètres via le bouton clé (en haut à droite) :

![Couleur du thème en direct et langues](../media/vanilux_tema_idioma.gif)

| Réglage | Effet |
|---|---|
| Couleur du thème | Roue + échantillons + hex ; recolore **tout** à l'instant. |
| Raccourci | Choisissez <kbd>F1</kbd>–<kbd>F12</kbd> ou **Super + touche** sur un clavier à l'écran style chiclet. |
| Langue | Español · English · Português · 中文 · Français · 日本語 · 한국어 · हिन्दी · Русский — bascule toute l'UI à l'instant. |

## Technologie

| Composant | Détail |
|---|---|
| Langage | C++17 |
| Toolkit | gtkmm 3 (GTK 3) |
| Graphiques | Cairo (halo dessiné à la main) |
| Affichage | X11 |
| Testé sur | Ubuntu · Linux Mint |

## Démo web

[`/landing`](../../landing/) héberge une page de présentation avec un **simulateur interactif**
du lanceur (apps fictives) où tester le changement de couleur et de langue :
**[facilized-pbustamante.github.io/vanilux](https://facilized-pbustamante.github.io/vanilux/)**

## Contribuer

Les issues et pull requests sont les bienvenues.

---

<div align="center">

## ⭐ Vanilux vous plaît ?

### Laissez une étoile — c'est gratuit et ça aide le projet

[![Laissez une étoile](https://img.shields.io/badge/%E2%AD%90%20Laissez%20une%20%C3%A9toile-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

[![Star History Chart](https://api.star-history.com/svg?repos=facilized-pbustamante/vanilux&type=Date)](https://star-history.com/#facilized-pbustamante/vanilux&Date)

<sub>vani<b>lux</b> · fait avec C++ et café · esthétique terminal ambre</sub>

</div>
