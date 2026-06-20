<div align="center">

# 🟠 Vanilux

### ターミナルの魂を持つ、Linux 向けフルスクリーン・アプリランチャー。

[Español](../../README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
[Français](README.fr.md) ·
**日本語** ·
[한국어](README.ko.md)

![Vanilux の動作](../media/vanilux_real.gif)

![バージョン](https://img.shields.io/badge/version-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![i18n](https://img.shields.io/badge/i18n-7%E8%A8%80%E8%AA%9E-e09924?style=flat-square)

### 🌐 [ライブデモを見る →](https://facilized-pbustamante.github.io/vanilux/)

</div>

---

**Vanilux** は、ダーク／アンバーのターミナル美学をまとった Linux 向けの *Launchpad* 風アプリランチャーです。
**F4** で呼び出し、メモリに**常駐**します——だから次回の起動は `rofi` のように一瞬。さらに**ピクセル単位まで
カスタマイズ可能**で、テーマカラー・ショートカット・言語をその場で変更できます。**C++17 + gtkmm 3** で記述され、
ネイティブ・軽量・高速。Electron なし、重いランタイムなし。

## ✨ 特長

- 🖥️ **フルスクリーンの TUI 美学** — `[ search ]`・`[ tags ]`・`[ apps ]`・`[ status ]` パネルをアンバーの枠で。
- 🔍 システムとユーザーの `.desktop` を解析する**自動検出**（`NoDisplay`・`Hidden`・`TryExec` に対応）。
- ⚡ 名前による**インクリメンタル検索**。大文字小文字を無視し、Unicode ケースフォールディング対応。
- 🏷️ **カテゴリ**・⭐ **お気に入り**・🕘 **最近**（ディスクに永続化）。
- 🌟 選択アイコンを囲む**アンバーグロー**。Cairo で手描き（シャープな輪郭＋実ブラーによる均一なハロー）。
- 🎨 **ライブのテーマカラー** — カラーピッカーが UI 全体を即座に再着色。
- 🌐 **7 言語**。オンスクリーンキーボードが各言語にレイアウトを適応（QWERTY · AZERTY · かな · ハングル · 注音）。
- ⌨️ マウスに加えて**完全なキーボード操作**（矢印・Enter・`f`・Esc）。選択は常に 1 つだけ（*フォーカス追従*）。
- 🚀 **常駐プロセス** → 初回起動でアイコンを読み込み、以降は即時。
- 🪶 **C++17 + gtkmm 3** — ネイティブで軽量。

## 📸 スクリーンショット

### グリッド表示
![グリッド表示](../screenshots/grid.png)

### 設定 — テーマ・ショートカット・言語
![設定メニュー](../screenshots/settings.png)

## 🚀 インストール

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

インストーラーが依存関係を解決し、ビルドし、バイナリ + CSS + アイコンをコピーし、
アプリメニューの項目を作成し、**`F4` ショートカットを設定**します（Cinnamon・GNOME・XFCE を自動判別）。

> **手動ビルド：** `make && sudo make install`
> **依存関係：** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> **Linux Mint 22.3**（Ubuntu 24.04、X11）で g++ 13.3・gtkmm 3.24・GTK 3.24 にて検証。

## ⌨️ 使い方

| 操作 | キー / 操作 |
|---|---|
| ランチャーを開く / 閉じる | <kbd>F4</kbd> |
| アプリを絞り込む | そのまま入力 |
| グリッドを移動 | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| 選択したアプリを開く | <kbd>Enter</kbd> |
| お気に入り切り替え | <kbd>f</kbd> |
| 閉じる | <kbd>Esc</kbd> |
| 上記すべて | マウスでも可（ホバー＝グロー） |

## 🎨 カスタマイズ（ライブ）

🔧（右上）から設定を開きます：

- **テーマカラー** — ホイール＋プリセット＋ hex。**すべて**を即座に再着色。
- **ショートカット** — チクレット風のオンスクリーンキーボードで <kbd>F1</kbd>–<kbd>F12</kbd> または **Super + キー** を選択。
- **言語** — Español · English · Português · 中文 · Français · 日本語 · 한국어。UI 全体を即座に切り替え（キーボードもレイアウトを適応）。

## 🧱 技術

**C++17** · **gtkmm 3**（GTK 3）· **Cairo**（手描きグロー）· **X11**。
重い依存はなし。システムのツールキットだけ。

## 🌐 Web デモ

[`/landing`](../../landing/) に紹介ページがあり、ランチャーの**インタラクティブなシミュレーター**（架空アプリ）で、
ブラウザ上でカラーと言語のライブ切り替えを試せます。

## 🤝 コントリビュート

Issue と Pull Request を歓迎します。ターミナル美学や Linux のミニマリズムが好きなら、リポジトリに ⭐ をどうぞ。

<div align="center"><sub>vani<b>lux</b> · C++ とコーヒー ☕ で制作 · アンバーのターミナル美学</sub></div>
