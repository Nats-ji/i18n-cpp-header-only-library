# i18n C++ Header Only Library

# Why?

I wanted to make a simple to use / beginer friendly library for internationalize c++ projects.

This library was created for internationalize [Cyber Engine Tweaks](https://github.com/yamashi/CyberEngineTweaks).

This is a header only library, so you can just copy-paste the `i18n.h` to your project, include it and call it a day.

No need to build it.

# Requirements

- This library requires `<format>` from C++20 standard library. You need Visual Studio 2019 v16.10 or higher and change your project language standard to c++20 for it to work.

- Alternatively, you can use [{fmt} library](https://github.com/fmtlib/fmt) instead. You need to define `I18N_USE_FMT` in your project.

# Usage

- Initialize
   ```cpp
   // i18n::Init(std::filesystem::path localePath = "locales", std::string locale = "en-US", std::string defaultLocale = "en-US", std::string localeExtension = ".locale")
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

# Locale file format

The format is similar to GNU gettext's po file.

```
msgid: Hello World
msgstr: 你好世界
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

1. Domain support

2. Plurals

3. Locale file generate scripts

4. Add support for json files