<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### ターミナルの魂を持つ、Linux 向けフルスクリーン・アプリランチャー。

[![ライブデモを見る](https://img.shields.io/badge/%E2%96%B6%20%E3%83%A9%E3%82%A4%E3%83%96%E3%83%87%E3%83%A2%E3%82%92%E8%A6%8B%E3%82%8B-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)

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
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-7%E8%A8%80%E8%AA%9E-e09924?style=flat-square)

</div>

---

**Vanilux** は、ダーク／アンバーのターミナル美学をまとった Linux 向けの *Launchpad* 風アプリランチャーです。
**F4** で呼び出し、メモリに**常駐**します——だから次回の起動は `rofi` のように一瞬。さらに**ピクセル単位まで
カスタマイズ可能**で、テーマカラー・ショートカット・言語をその場で変更できます。**C++17 + gtkmm 3** で記述され、
ネイティブ・軽量・高速。Electron なし、重いランタイムなし。

## 特長

| 特長 | 説明 |
|---|---|
| フルスクリーンの TUI 美学 | `[ search ]`・`[ tags ]`・`[ apps ]`・`[ status ]` パネルをアンバーの枠で。 |
| 自動検出 | システムとユーザーの `.desktop` を解析（`NoDisplay`・`Hidden`・`TryExec` に対応）。 |
| インクリメンタル検索 | 名前で絞り込み。大文字小文字を無視し Unicode ケースフォールディング対応。 |
| お気に入りと最近 | ディスクに永続化、セッションをまたいで保持。 |
| Cairo のアンバーグロー | 選択アイコンのハローを実ブラーで手描き。 |
| ライブのテーマカラー | カラーピッカーが UI 全体を即座に再着色。 |
| 7 言語 | オンスクリーンキーボードがレイアウトを適応（QWERTY · AZERTY · かな · ハングル · 注音）。 |
| キーボード操作 | 矢印・Enter・`f`・Esc — マウスも可。 |
| 常駐プロセス | 初回起動でアイコンを読み込み、以降は即時。 |
| C++17 + gtkmm 3 | ネイティブで軽量、Electron なし。 |

## スクリーンショット

### グリッド表示
![グリッド表示](../screenshots/grid.png)

### 設定 — テーマ・ショートカット・言語
![設定メニュー](../screenshots/settings.png)

## インストール

**`.deb` パッケージ（推奨 · Ubuntu / Linux Mint）** — [Releases](https://github.com/facilized-pbustamante/vanilux/releases) から `vanilux_1.0_amd64.deb` をダウンロードしてインストール：

```bash
sudo apt install ./vanilux_1.0_amd64.deb
```

**またはソースからビルド：**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

インストーラーが依存関係を解決し、ビルドし、バイナリ + CSS + アイコンをコピーし、
アプリメニューの項目を作成し、`F4` ショートカットを設定します（Cinnamon・GNOME・XFCE を自動判別）。

| | |
|---|---|
| 手動ビルド | `make && sudo make install` |
| 依存関係 | `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make` |
| 動作確認 | Ubuntu 24.04 · Linux Mint 22.3 (X11) — g++ 13.3, gtkmm 3.24, GTK 3.24 |

## 使い方

| 操作 | キー / 操作 |
|---|---|
| ランチャーを開く / 閉じる | <kbd>F4</kbd> |
| アプリを絞り込む | そのまま入力 |
| グリッドを移動 | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| 選択したアプリを開く | <kbd>Enter</kbd> |
| お気に入り切り替え | <kbd>f</kbd> |
| 閉じる | <kbd>Esc</kbd> |
| 上記すべて | マウスでも可（ホバー＝グロー） |

## カスタマイズ（ライブ）

レンチボタン（右上）から設定を開きます：

| 設定 | はたらき |
|---|---|
| テーマカラー | ホイール＋プリセット＋ hex。**すべて**を即座に再着色。 |
| ショートカット | チクレット風のオンスクリーンキーボードで <kbd>F1</kbd>–<kbd>F12</kbd> または **Super + キー** を選択。 |
| 言語 | Español · English · Português · 中文 · Français · 日本語 · 한국어 — UI 全体を即座に切り替え。 |

## 技術

| コンポーネント | 詳細 |
|---|---|
| 言語 | C++17 |
| ツールキット | gtkmm 3 (GTK 3) |
| グラフィックス | Cairo（手描きグロー） |
| ディスプレイ | X11 |
| 動作確認 | Ubuntu · Linux Mint |

## Web デモ

[`/landing`](../../landing/) に紹介ページがあり、ランチャーの**インタラクティブなシミュレーター**（架空アプリ）で
カラーと言語のライブ切り替えを試せます：
**[facilized-pbustamante.github.io/vanilux](https://facilized-pbustamante.github.io/vanilux/)**

## コントリビュート

Issue と Pull Request を歓迎します。

---

<div align="center">

## ⭐ Vanilux は気に入りましたか？

### スターをひとつ — 無料で、プロジェクトの後押しになります

[![スターをください](https://img.shields.io/badge/%E2%AD%90%20Star-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

<sub>vani<b>lux</b> · C++ とコーヒーで制作 · アンバーのターミナル美学</sub>

</div>
