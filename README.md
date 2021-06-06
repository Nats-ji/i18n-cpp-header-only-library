# i18n C++ Header Only Library

# Why?

I wanted to make a super simple to use / beginer friendly library for internationalize c++ projects.

This library was created for internationalize [Cyber Engine Tweaks](https://github.com/yamashi/CyberEngineTweaks).

This is a header only library, so you can just copy-paste the `i18n.h` to your project, include it and call it a day.

No need to build it.

This library can only provide basic functionalities.

If you want a more complete library please use [ICU4C](https://github.com/unicode-org/icu), [Boost.Locale](https://www.boost.org/doc/libs/release/libs/locale/) or [GNU gettext](https://www.gnu.org/software/gettext/).

If you don't want to deal with those complex libraries for a small project, you should use this library.

# Requirements
At least C++17 (not sure):

- This library requires `<format>` from C++20 standard library. You need Visual Studio 2019 v16.10 or higher and change your project language standard to C++20 for it to work.

- Alternatively, you can use [{fmt} library](https://github.com/fmtlib/fmt) instead. You need to define `I18N_USE_FMT` in your project.

# Usage

- Initialize
   ```cpp
   // i18n::Init(std::filesystem::path localePath = "locales", std::string locale = "en-US", std::string defaultLocale = "en-US", std::string defaultNS = "default", std::string localeExtension = ".locale")
   i18n::Init();
   ```

- Switch locale
   ```cpp
   i18n::SetLocale("zh-CN");
   ```

- Translate
   ```cpp
   std::string str1 = i18n::Translate("Hello World!");
   std::string str2 = i18n::Translate("You catched {0:d} carrots in {1:d} seconds", 5, 2); // Using <format> from C++20 std or {fmt}
   ```
- Use namespace
   locale file:
   ```
   ns: ns1
   msgid: Back
   msgstr: 返回

   ns: ns2
   msgid: Back
   msgstr: 背
   ```

   Code:
   ```cpp
   i18n::TranslateN("ns1", "Back"); // Output 返回
   i18n::TranslateN("ns2", "Back"); // Output 背
   ```

# Locale file format

The format is similar to GNU gettext's po file.

```
msgid: Hello World
msgstr: 你好世界

ns: ns1
msgid: Back
msgstr: 返回

ns: ns2
msgid: Back
msgstr: 背
```

# Loading order

1. The library will search for a locale file for the default locale (locale path and default locale can be set with `i18n::Init()` function).

2. If the default locale file exists. The library will parse and load it.

3. If the default locale file doesn't exist. The `i18n::Translate(std::string msgid)` function will return the `msgid`.

4. After switching to another locale, if the `msgid` doesn't exist in current locale, `i18n::Translate(std::string msgid)` function will return the default locale string.

# Build the example

### Requirements

- The example project can only be built on Windows (because I'm loading fonts from Windows directory). The library is portable.

- [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

- [xmake](https://github.com/xmake-io/xmake/releases)

### Build

1. Navigate to the repository using a command prompt.

2. Enter the following commands
   ```sh
   xmake -y
   xmake package
   xmake run
   ```

# Todo

- [x] Namespace support

- [ ] Cache the formated strings (To improve performance when using inside a loop)

- [ ] Plurals (This is more complicated than I thought)

- [ ] Locale file generation scripts (lua for xmake, and python)

- [ ] Add support for json files (i18next format)
