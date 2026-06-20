<div align="center">

# 🟠 Vanilux

### 터미널의 영혼을 가진 Linux 전체 화면 앱 런처.

[Español](../../README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
[Français](README.fr.md) ·
[日本語](README.ja.md) ·
**한국어**

![Vanilux 실행 모습](../media/vanilux_real.gif)

![버전](https://img.shields.io/badge/version-1.0-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-X11-FCC624?style=flat-square&logo=linux&logoColor=black)
![i18n](https://img.shields.io/badge/i18n-7%EA%B0%9C%20%EC%96%B8%EC%96%B4-e09924?style=flat-square)

</div>

---

**Vanilux**는 다크/앰버 터미널 미학을 입은 Linux용 *런치패드* 스타일 앱 런처입니다.
**F4**로 호출하면 메모리에 **상주**합니다 — 그래서 다음 실행은 `rofi`처럼 즉각적입니다 — 그리고
**픽셀 단위까지 커스터마이즈**할 수 있습니다: 테마 색상, 단축키, 언어를 실시간으로 변경하세요.
**C++17 + gtkmm 3**로 작성되어 네이티브하고 가볍고 빠릅니다. Electron 없음, 무거운 런타임 없음.

## ✨ 기능

- 🖥️ **전체 화면, TUI 미학** — `[ search ]`, `[ tags ]`, `[ apps ]`, `[ status ]` 패널을 앰버 테두리로.
- 🔍 시스템과 사용자의 `.desktop`을 파싱하는 **자동 탐색**(`NoDisplay`, `Hidden`, `TryExec` 준수).
- ⚡ 이름 기준 **점진적 검색**, 대소문자 구분 없이 유니코드 케이스 폴딩 지원.
- 🏷️ **카테고리**, ⭐ **즐겨찾기**, 🕘 **최근**을 디스크에 영구 저장.
- 🌟 선택된 아이콘을 감싸는 **앰버 글로우**, Cairo로 직접 그림(선명한 가장자리 + 실제 블러로 균일한 후광).
- 🎨 **실시간 테마 색상** — 색상 선택기가 전체 UI를 즉시 다시 칠합니다.
- 🌐 **7개 언어**, 화면 키보드가 각 언어에 맞게 레이아웃을 적응(QWERTY · AZERTY · 가나 · 한글 · 주음).
- ⌨️ 마우스에 더해 **완전한 키보드 내비게이션**(화살표, Enter, `f`, Esc), 한 번에 하나만 선택(*포커스 따라가기*).
- 🚀 **상주 프로세스** → 첫 실행에서 아이콘을 로드하고 이후에는 즉시.
- 🪶 **C++17 + gtkmm 3** — 네이티브하고 가벼움.

## 📸 스크린샷

### 그리드 보기
![그리드 보기](../screenshots/grid.png)

### 설정 — 테마, 단축키, 언어
![설정 메뉴](../screenshots/settings.png)

## 🚀 설치

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

설치 스크립트가 의존성을 해결하고, 빌드하고, 바이너리 + CSS + 아이콘을 복사하고,
앱 메뉴 항목을 만들고, **`F4` 단축키를 설정**해 줍니다(Cinnamon, GNOME, XFCE 자동 감지).

> **수동 빌드:** `make && sudo make install`
> **의존성:** `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make`
> **Linux Mint 22.3**(Ubuntu 24.04, X11)에서 g++ 13.3, gtkmm 3.24, GTK 3.24로 테스트.

## ⌨️ 사용법

| 동작 | 키 / 제스처 |
|---|---|
| 런처 열기 / 닫기 | <kbd>F4</kbd> |
| 앱 필터링 | 그냥 입력 시작 |
| 그리드 이동 | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| 선택한 앱 열기 | <kbd>Enter</kbd> |
| 즐겨찾기 토글 | <kbd>f</kbd> |
| 닫기 | <kbd>Esc</kbd> |
| 위 모든 동작 | 마우스로도 가능(호버 = 글로우) |

## 🎨 커스터마이즈 (실시간)

🔧(오른쪽 위)로 설정을 엽니다:

- **테마 색상** — 휠 + 프리셋 + hex; **모든 것**을 즉시 다시 칠합니다.
- **단축키** — 치클릿 스타일 화면 키보드에서 <kbd>F1</kbd>–<kbd>F12</kbd> 또는 **Super + 키**를 선택.
- **언어** — Español · English · Português · 中文 · Français · 日本語 · 한국어. 전체 UI를 즉시 전환(키보드도 레이아웃 적응).

## 🧱 기술

**C++17** · **gtkmm 3**(GTK 3) · **Cairo**(직접 그린 글로우) · **X11**.
무거운 의존성 없음: 시스템 툴킷만 사용.

## 🌐 웹 데모

[`/landing`](../../landing/)에는 런처의 **인터랙티브 시뮬레이터**(가상 앱)가 있는 소개 페이지가 있어,
브라우저에서 실시간 색상 및 언어 전환을 체험할 수 있습니다.

## 🤝 기여

이슈와 풀 리퀘스트를 환영합니다. 터미널 미학이나 Linux 미니멀리즘을 좋아한다면 저장소에 ⭐를 남겨 주세요.

<div align="center"><sub>vani<b>lux</b> · C++와 커피 ☕로 제작 · 앰버 터미널 미학</sub></div>
