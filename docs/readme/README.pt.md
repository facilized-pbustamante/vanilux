<div align="center">

# 🟠 Vanilux

### Um lançador de aplicativos em tela cheia para Linux — com alma de terminal.

[Español](../../README.md) ·
[English](README.en.md) ·
**Português** ·
[中文](README.zh.md) ·
[Français](README.fr.md) ·
[日本語](README.ja.md) ·
[한국어](README.ko.md)

![Vanilux em ação](../media/vanilux_real.gif)

![versão](https://img.shields.io/badge/vers%C3%A3o-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![i18n](https://img.shields.io/badge/i18n-7%20idiomas-e09924?style=flat-square)

### 🌐 [Ver demo ao vivo →](https://facilized-pbustamante.github.io/vanilux/)

</div>

---

O **Vanilux** é um lançador de aplicativos estilo *Launchpad* para Linux, com estética
de terminal escuro/âmbar. Invoque com **F4**; ele fica **residente** na memória —para que
a próxima abertura seja instantânea, como o `rofi`— e é **personalizável até o pixel**:
mude a cor do tema, o atalho e o idioma… ao vivo. Escrito em **C++17 + gtkmm 3**: nativo,
leve e rápido. Sem Electron, sem runtime pesado.

## ✨ Recursos

- 🖥️ **Tela cheia, estética TUI** — painéis `[ search ]`, `[ tags ]`, `[ apps ]` e `[ status ]` com bordas âmbar.
- 🔍 **Descoberta automática** de apps analisando os `.desktop` do sistema e do usuário (respeita `NoDisplay`, `Hidden` e `TryExec`).
- ⚡ **Busca incremental** por nome, sem diferenciar maiúsculas, com *case-folding* Unicode.
- 🏷️ **Categorias**, ⭐ **Favoritos** e 🕘 **Recentes** persistentes em disco.
- 🌟 **Glow âmbar** ao redor do ícone selecionado, desenhado à mão com Cairo (borda nítida + halo uniforme via blur real).
- 🎨 **Cor do tema ao vivo** — um seletor de cor recolore toda a interface na hora.
- 🌐 **7 idiomas** com teclado na tela que adapta o layout a cada um (QWERTY · AZERTY · kana · hangul · zhuyin).
- ⌨️ **Navegação completa por teclado** (setas, Enter, `f`, Esc) além do mouse, com uma única seleção por vez (*focus-follows-mouse*).
- 🚀 **Processo residente** → a primeira abertura carrega os ícones; as próximas são imediatas.
- 🪶 **C++17 + gtkmm 3** — nativo e leve.

## 📸 Capturas

### Visão em grade
![Visão em grade](../screenshots/grid.png)

### Configurações — tema, atalho e idioma
![Menu de configurações](../screenshots/settings.png)

## 🚀 Instalação

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

O instalador resolve as dependências, compila, copia o binário + CSS + ícones,
cria a entrada no menu de aplicativos e **configura o atalho `F4`** para você
(detecta Cinnamon, GNOME ou XFCE).

> **Compilação manual:** `make && sudo make install`
> **Dependências:** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> Testado no **Linux Mint 22.3** (Ubuntu 24.04, X11) com g++ 13.3, gtkmm 3.24 e GTK 3.24.

## ⌨️ Uso

| Ação | Tecla / gesto |
|---|---|
| Abrir / fechar o lançador | <kbd>F4</kbd> |
| Filtrar aplicativos | comece a digitar |
| Navegar pela grade | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| Abrir o app selecionado | <kbd>Enter</kbd> |
| Marcar/desmarcar favorito | <kbd>f</kbd> |
| Fechar | <kbd>Esc</kbd> |
| Tudo isso | também com o mouse (hover = glow) |

## 🎨 Personalização (ao vivo)

Abra as configurações pela 🔧 (canto superior direito):

- **Cor do tema** — roda + cores predefinidas + hex; recolore **tudo** na hora.
- **Atalho** — escolha <kbd>F1</kbd>–<kbd>F12</kbd> ou **Super + tecla** em um teclado na tela estilo chiclet.
- **Idioma** — Español · English · Português · 中文 · Français · 日本語 · 한국어. Troca toda a UI na hora (e o teclado adapta o layout).

## 🧱 Tecnologia

**C++17** · **gtkmm 3** (GTK 3) · **Cairo** (glow desenhado à mão) · **X11**.
Sem dependências pesadas: apenas o toolkit do sistema.

## 🌐 Demo web

Em [`/landing`](../../landing/) há uma página de apresentação com um **simulador interativo**
do lançador (apps fictícios) onde você pode experimentar a troca de cor e idioma no navegador.

## 🤝 Contribuir

Issues e pull requests são bem-vindos. Se você curte estética de terminal ou minimalismo no Linux, deixe uma ⭐ no repo.

<div align="center"><sub>vani<b>lux</b> · feito com C++ e café ☕ · estética terminal âmbar</sub></div>
