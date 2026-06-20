<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### 터미널의 영혼을 가진 Linux 전체 화면 앱 런처.

[![라이브 데모 보기](https://img.shields.io/badge/%E2%96%B6%20%EB%9D%BC%EC%9D%B4%EB%B8%8C%20%EB%8D%B0%EB%AA%A8%20%EB%B3%B4%EA%B8%B0-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)

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
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-7%EA%B0%9C%20%EC%96%B8%EC%96%B4-e09924?style=flat-square)

</div>

---

**Vanilux**는 다크/앰버 터미널 미학을 입은 Linux용 *런치패드* 스타일 앱 런처입니다.
**F4**로 호출하면 메모리에 **상주**합니다 — 그래서 다음 실행은 `rofi`처럼 즉각적입니다 — 그리고
**픽셀 단위까지 커스터마이즈**할 수 있습니다: 테마 색상, 단축키, 언어를 실시간으로 변경하세요.
**C++17 + gtkmm 3**로 작성되어 네이티브하고 가볍고 빠릅니다. Electron 없음, 무거운 런타임 없음.

## 기능

| 기능 | 설명 |
|---|---|
| 전체 화면, TUI 미학 | `[ search ]`, `[ tags ]`, `[ apps ]`, `[ status ]` 패널을 앰버 테두리로. |
| 자동 탐색 | 시스템과 사용자의 `.desktop`을 파싱(`NoDisplay`, `Hidden`, `TryExec` 준수). |
| 점진적 검색 | 이름 기준, 대소문자 구분 없이 유니코드 케이스 폴딩 지원. |
| 즐겨찾기와 최근 | 세션 간 디스크에 영구 저장. |
| Cairo 앰버 글로우 | 선택된 아이콘의 후광을 실제 블러로 직접 그림. |
| 실시간 테마 색상 | 색상 선택기가 전체 UI를 즉시 다시 칠함. |
| 7개 언어 | 화면 키보드가 레이아웃을 적응(QWERTY · AZERTY · 가나 · 한글 · 주음). |
| 키보드 내비게이션 | 화살표, Enter, `f`, Esc — 마우스도 가능. |
| 상주 프로세스 | 첫 실행에서 아이콘 로드, 이후에는 즉시. |
| C++17 + gtkmm 3 | 네이티브하고 가벼움, Electron 없음. |

## 스크린샷

### 그리드 보기
![그리드 보기](../screenshots/grid.png)

### 설정 — 테마, 단축키, 언어
![설정 메뉴](../screenshots/settings.png)

## 설치

**`.deb` 패키지 (권장 · Ubuntu / Linux Mint)** — [Releases](https://github.com/facilized-pbustamante/vanilux/releases) 에서 `vanilux_1.0_amd64.deb` 를 받아 설치:

```bash
sudo apt install ./vanilux_1.0_amd64.deb
```

**또는 소스에서 빌드:**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

설치 스크립트가 의존성을 해결하고, 빌드하고, 바이너리 + CSS + 아이콘을 복사하고,
앱 메뉴 항목을 만들고, `F4` 단축키를 설정합니다(Cinnamon, GNOME, XFCE 자동 감지).

| | |
|---|---|
| 수동 빌드 | `make && sudo make install` |
| 의존성 | `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make` |
| 테스트 환경 | Ubuntu 24.04 · Linux Mint 22.3 (X11) — g++ 13.3, gtkmm 3.24, GTK 3.24 |

## 사용법

| 동작 | 키 / 제스처 |
|---|---|
| 런처 열기 / 닫기 | <kbd>F4</kbd> |
| 앱 필터링 | 그냥 입력 시작 |
| 그리드 이동 | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| 선택한 앱 열기 | <kbd>Enter</kbd> |
| 즐겨찾기 토글 | <kbd>f</kbd> |
| 닫기 | <kbd>Esc</kbd> |
| 위 모든 동작 | 마우스로도 가능(호버 = 글로우) |

## 커스터마이즈 (실시간)

렌치 버튼(오른쪽 위)으로 설정을 엽니다:

| 설정 | 동작 |
|---|---|
| 테마 색상 | 휠 + 프리셋 + hex; **모든 것**을 즉시 다시 칠함. |
| 단축키 | 치클릿 스타일 화면 키보드에서 <kbd>F1</kbd>–<kbd>F12</kbd> 또는 **Super + 키** 선택. |
| 언어 | Español · English · Português · 中文 · Français · 日本語 · 한국어 — 전체 UI를 즉시 전환. |

## 기술

| 구성 요소 | 상세 |
|---|---|
| 언어 | C++17 |
| 툴킷 | gtkmm 3 (GTK 3) |
| 그래픽 | Cairo(직접 그린 글로우) |
| 디스플레이 | X11 |
| 테스트 환경 | Ubuntu · Linux Mint |

## 웹 데모

[`/landing`](../../landing/)에는 런처의 **인터랙티브 시뮬레이터**(가상 앱)가 있는 소개 페이지가 있어
실시간 색상·언어 전환을 체험할 수 있습니다:
**[facilized-pbustamante.github.io/vanilux](https://facilized-pbustamante.github.io/vanilux/)**

## 기여

이슈와 풀 리퀘스트를 환영합니다.

---

<div align="center">

## ⭐ Vanilux가 마음에 드나요?

### 스타 하나 — 무료이고 프로젝트에 큰 힘이 됩니다

[![스타를 눌러주세요](https://img.shields.io/badge/%E2%AD%90%20Star-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

<sub>vani<b>lux</b> · C++와 커피로 제작 · 앰버 터미널 미학</sub>

</div>
