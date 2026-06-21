<div align="center">

<img src="../logo.svg" width="116" alt="Vanilux">

# Vanilux

### टर्मिनल की आत्मा वाला, Linux के लिए फुलस्क्रीन ऐप लॉन्चर।

[![लाइव डेमो देखें](https://img.shields.io/badge/%E2%96%B6%20%E0%A4%B2%E0%A4%BE%E0%A4%87%E0%A4%B5%20%E0%A4%A1%E0%A5%87%E0%A4%AE%E0%A5%8B%20%E0%A4%A6%E0%A5%87%E0%A4%96%E0%A5%87%E0%A4%82-e09924?style=for-the-badge)](https://facilized-pbustamante.github.io/vanilux/)

[Español](../../README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[中文](README.zh.md) ·
[Français](README.fr.md) ·
[日本語](README.ja.md) ·
[한국어](README.ko.md) ·
**हिन्दी** ·
[Русский](README.ru.md)

![Vanilux](../media/vanilux_real.gif)

![version](https://img.shields.io/badge/version-1.1-e09924?style=flat-square)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![gtkmm3](https://img.shields.io/badge/GTK-gtkmm%203-7AAD0C?style=flat-square&logo=gnome&logoColor=white)
![Ubuntu](https://img.shields.io/badge/Ubuntu-%E2%9C%93-E95420?style=flat-square&logo=ubuntu&logoColor=white)
![Linux Mint](https://img.shields.io/badge/Linux_Mint-%E2%9C%93-86C440?style=flat-square&logo=linuxmint&logoColor=white)
![i18n](https://img.shields.io/badge/i18n-9-e09924?style=flat-square)

</div>

---

**Vanilux** Linux के लिए *Launchpad* शैली का एक ऐप लॉन्चर है, जिसमें डार्क/एम्बर टर्मिनल सौंदर्य है।
इसे **F4** से बुलाएँ; यह मेमोरी में **रेज़िडेंट** रहता है — इसलिए अगली बार खोलना `rofi` की तरह तुरंत होता है —
और यह **पिक्सेल तक अनुकूलन योग्य** है: थीम का रंग, हॉटकी और भाषा… सब लाइव बदलें।
**C++17 + gtkmm 3** में लिखा गया: नेटिव, हल्का और तेज़। कोई Electron नहीं, कोई भारी रनटाइम नहीं।

## विशेषताएँ

| विशेषता | विवरण |
|---|---|
| फुलस्क्रीन, TUI सौंदर्य | एम्बर बॉर्डर वाले `[ search ]`, `[ tags ]`, `[ apps ]` और `[ status ]` पैनल। |
| स्वचालित खोज | सिस्टम और उपयोगकर्ता की `.desktop` फ़ाइलें पार्स करता है (`NoDisplay`, `Hidden`, `TryExec` का सम्मान)। |
| इंक्रीमेंटल खोज | नाम से छानें, केस-असंवेदनशील, Unicode केस-फ़ोल्डिंग के साथ। |
| पसंदीदा और हाल के | सत्रों के बीच डिस्क पर सहेजे जाते हैं। |
| Cairo में एम्बर ग्लो | चयनित आइकन का प्रभामंडल असली ब्लर के साथ हाथ से बनाया गया। |
| लाइव थीम रंग | एक कलर पिकर पूरी इंटरफ़ेस को तुरंत फिर से रंगता है। |
| 9 भाषाएँ | ऑन-स्क्रीन कीबोर्ड जो हर भाषा के लिए लेआउट बदलता है (QWERTY · AZERTY · कना · हंगुल · जूयिन · सिरिलिक)। |
| कीबोर्ड नेविगेशन | तीर, Enter, `f`, Esc — माउस भी। |
| रेज़िडेंट प्रोसेस | पहली बार आइकन लोड होते हैं; अगली बार तुरंत। |
| C++17 + gtkmm 3 | नेटिव और हल्का, कोई Electron नहीं। |

## स्क्रीनशॉट

### ग्रिड दृश्य
![ग्रिड दृश्य](../screenshots/grid.png)

### सेटिंग्स — थीम, हॉटकी और भाषा
![सेटिंग्स मेनू](../screenshots/settings.png)

## इंस्टॉलेशन

**`.deb` पैकेज (अनुशंसित · Ubuntu / Linux Mint)** — [Releases](https://github.com/facilized-pbustamante/vanilux/releases) से `vanilux_1.1_amd64.deb` डाउनलोड करें और इंस्टॉल करें:

```bash
sudo apt install ./vanilux_1.1_amd64.deb
```

**या स्रोत से बिल्ड करें:**

```bash
git clone https://github.com/facilized-pbustamante/vanilux
cd vanilux
./install.sh
```

इंस्टॉलर निर्भरताएँ हल करता है, बिल्ड करता है, बाइनरी + CSS + आइकन कॉपी करता है,
ऐप-मेनू प्रविष्टि बनाता है और आपके लिए `F4` हॉटकी सेट करता है (Cinnamon, GNOME या XFCE का पता लगाता है)।

| | |
|---|---|
| मैनुअल बिल्ड | `make && sudo make install` |
| निर्भरताएँ | `build-essential` · `libgtkmm-3.0-dev` · `pkg-config` · `make` |
| परीक्षण किया गया | Ubuntu 24.04 · Linux Mint 22.3 (X11) — g++ 13.3, gtkmm 3.24, GTK 3.24 |

## उपयोग

| क्रिया | कुंजी / इशारा |
|---|---|
| लॉन्चर खोलें / बंद करें | <kbd>F4</kbd> |
| ऐप्स छानें | बस टाइप करना शुरू करें |
| ग्रिड में नेविगेट करें | <kbd>←</kbd> <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> |
| चयनित ऐप खोलें | <kbd>Enter</kbd> |
| पसंदीदा टॉगल करें | <kbd>f</kbd> |
| बंद करें | <kbd>Esc</kbd> |
| उपरोक्त सभी | माउस से भी (होवर = ग्लो) |

## अनुकूलन (लाइव)

रेंच बटन (ऊपर दाएँ) से सेटिंग्स खोलें:

![लाइव थीम रंग और भाषाएँ](../media/vanilux_tema_idioma.gif)

| सेटिंग | क्या करता है |
|---|---|
| थीम रंग | व्हील + प्रीसेट + hex; **सब कुछ** तुरंत फिर से रंगता है। |
| हॉटकी | चिकलेट-शैली ऑन-स्क्रीन कीबोर्ड पर <kbd>F1</kbd>–<kbd>F12</kbd> या **Super + कुंजी** चुनें। |
| भाषा | Español · English · Português · 中文 · Français · 日本語 · 한국어 · हिन्दी · Русский — पूरी UI तुरंत बदलती है। |

## तकनीक

| घटक | विवरण |
|---|---|
| भाषा | C++17 |
| टूलकिट | gtkmm 3 (GTK 3) |
| ग्राफ़िक्स | Cairo (हाथ से बना ग्लो) |
| डिस्प्ले | X11 |
| परीक्षण किया गया | Ubuntu · Linux Mint |

## वेब डेमो

[`/landing`](../../landing/) में एक प्रस्तुति पृष्ठ है जिसमें लॉन्चर का **इंटरैक्टिव सिम्युलेटर** (काल्पनिक ऐप्स) है,
जहाँ आप ब्राउज़र में लाइव रंग और भाषा बदलना आज़मा सकते हैं:
**[facilized-pbustamante.github.io/vanilux](https://facilized-pbustamante.github.io/vanilux/)**

## योगदान

Issues और pull requests का स्वागत है।

---

<div align="center">

## ⭐ Vanilux पसंद आया?

### एक स्टार दें — यह मुफ़्त है और प्रोजेक्ट को बढ़ने में मदद करता है

[![स्टार दें](https://img.shields.io/badge/%E2%AD%90%20%E0%A4%B8%E0%A5%8D%E0%A4%9F%E0%A4%BE%E0%A4%B0%20%E0%A4%A6%E0%A5%87%E0%A4%82-ffb000?style=for-the-badge&logo=github&logoColor=14141c)](https://github.com/facilized-pbustamante/vanilux)
&nbsp;
[![GitHub stars](https://img.shields.io/github/stars/facilized-pbustamante/vanilux?style=for-the-badge&color=e09924&labelColor=14141c)](https://github.com/facilized-pbustamante/vanilux/stargazers)

[![Star History Chart](https://api.star-history.com/svg?repos=facilized-pbustamante/vanilux&type=Date)](https://star-history.com/#facilized-pbustamante/vanilux&Date)

<sub>vani<b>lux</b> · C++ और कॉफ़ी से बना ☕ · एम्बर टर्मिनल सौंदर्य</sub>

</div>
