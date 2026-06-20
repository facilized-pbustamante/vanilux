<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### 一款拥有终端之魂的 Linux 全屏应用启动器。

[Español](../../README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
**中文** ·
[Français](README.fr.md) ·
[日本語](README.ja.md) ·
[한국어](README.ko.md)

![Vanilux 演示](../media/vanilux_real.gif)

![版本](https://img.shields.io/badge/%E7%89%88%E6%9C%AC-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-7%20%E7%A7%8D%E8%AF%AD%E8%A8%80-e09924?style=flat-square)

[![查看在线演示](https://img.shields.io/badge/%E2%96%B6%20%E6%9F%A5%E7%9C%8B%E5%9C%A8%E7%BA%BF%E6%BC%94%E7%A4%BA-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)
&nbsp;&nbsp;
[![点个 Star](https://img.shields.io/badge/%E2%AD%90%20%E7%82%B9%E4%B8%AA%20Star-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)

</div>

---

**Vanilux** 是一款 Linux 上 *Launchpad* 风格的应用启动器，采用深色／琥珀色的终端美学。
用 **F4** 唤出；它会**常驻**内存——因此下次打开瞬间完成，就像 `rofi`——并且**可定制到每个像素**：
实时更改主题颜色、快捷键和语言。使用 **C++17 + gtkmm 3** 编写：原生、轻量、快速。
没有 Electron，没有臃肿的运行时。

## ✨ 特性

- 🖥️ **全屏 TUI 美学** —— `[ search ]`、`[ tags ]`、`[ apps ]` 和 `[ status ]` 面板，琥珀色边框。
- 🔍 **自动发现** 应用，解析系统和用户的 `.desktop` 文件（遵循 `NoDisplay`、`Hidden` 和 `TryExec`）。
- ⚡ **增量搜索**，按名称、不区分大小写，支持 Unicode 大小写折叠。
- 🏷️ **类别**、⭐ **收藏** 和 🕘 **最近**，持久化到磁盘。
- 🌟 **琥珀辉光** 环绕选中图标，用 Cairo 手工绘制（清晰边缘 + 真实模糊的均匀光晕）。
- 🎨 **实时主题颜色** —— 取色器即时为整个界面重新着色。
- 🌐 **7 种语言**，屏幕键盘会按语言适配布局（QWERTY · AZERTY · 假名 · 谚文 · 注音）。
- ⌨️ **完整键盘导航**（方向键、Enter、`f`、Esc）以及鼠标，一次只有一个选中项（*焦点跟随鼠标*）。
- 🚀 **常驻进程** —— 首次打开加载图标，之后即时显示。
- 🪶 **C++17 + gtkmm 3** —— 原生且轻量。

## 📸 截图

### 网格视图
![网格视图](../screenshots/grid.png)

### 设置 —— 主题、快捷键和语言
![设置菜单](../screenshots/settings.png)

## 🚀 安装

**`.deb` 安装包（推荐 · Ubuntu / Linux Mint）** —— 从 [Releases](https://github.com/facilized-pbustamante/vanilux/releases) 下载 `vanilux_1.0_amd64.deb` 并安装：

```bash
sudo apt install ./vanilux_1.0_amd64.deb
```

**或从源码编译：**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

安装脚本会解决依赖、编译、复制二进制 + CSS + 图标、
创建应用菜单项，并为你**配置 `F4` 快捷键**（自动检测 Cinnamon、GNOME 或 XFCE）。

> **手动编译：** `make && sudo make install`
> **依赖：** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> 在 **Linux Mint 22.3**（Ubuntu 24.04，X11）上测试，g++ 13.3、gtkmm 3.24、GTK 3.24。

## ⌨️ 使用

| 操作 | 按键 / 手势 |
|---|---|
| 打开 / 关闭启动器 | <kbd>F4</kbd> |
| 筛选应用 | 直接开始输入 |
| 在网格中导航 | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| 打开选中的应用 | <kbd>Enter</kbd> |
| 切换收藏 | <kbd>f</kbd> |
| 关闭 | <kbd>Esc</kbd> |
| 以上全部 | 也可用鼠标（悬停 = 辉光） |

## 🎨 自定义（实时）

通过 🔧（右上角）打开设置：

- **主题颜色** —— 色轮 + 预设色块 + 十六进制；**即时**为一切重新着色。
- **快捷键** —— 在 chiclet 风格的屏幕键盘上选择 <kbd>F1</kbd>–<kbd>F12</kbd> 或 **Super + 键**。
- **语言** —— Español · English · Português · 中文 · Français · 日本語 · 한국어。即时切换整个界面（键盘也会适配布局）。

## 🧱 技术

**C++17** · **gtkmm 3**（GTK 3）· **Cairo**（手绘辉光）· **X11**。
没有沉重依赖：只用系统工具包。

## 🌐 网页演示

[`/landing`](../../landing/) 提供了一个展示页面，内含启动器的**交互式模拟器**（虚构应用），
你可以在浏览器里体验实时换色和换语言。

## 🤝 贡献

欢迎提交 issue 和 pull request。如果你喜欢终端美学或 Linux 极简主义，请给仓库点个 ⭐。

---

<div align="center">

## ⭐ 喜欢 Vanilux 吗？

### 点一个 Star —— 免费，还能帮项目被更多人看到 🙌

[![star](https://img.shields.io/badge/%E2%AD%90%20%E7%82%B9%E4%B8%AA%20Star-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

</div>

<div align="center"><sub>vani<b>lux</b> · 用 C++ 和咖啡 ☕ 打造 · 琥珀终端美学</sub></div>
