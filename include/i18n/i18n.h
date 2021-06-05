#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#ifdef I18N_USE_FMT //Use fmt::format instead of c++20 std::format
#include <fmt/core.h>
#define i18n_format fmt::format
#else
#include <format>
#define i18n_format std::format
#endif

class i18n
{
public:

  i18n(const i18n&) = delete;
  i18n(i18n&&) = delete;
  i18n& operator=(const i18n&) = delete;
  i18n& operator=(i18n&&) = delete;

  static void Init(std::filesystem::path localePath = "locales", std::string locale = "en-US", std::string defaultLocale = "en-US", std::string localeExtension = ".locale")
  {
    Get().m_localePath = localePath;
    Get().m_defaultLocale = defaultLocale;
    Get().m_locale = locale;
    Get().m_localeExtension = localeExtension;
    Get().loadDefaultDictionary();
    SetLocale(locale);
  }

  static i18n& Get() { static i18n instance; return instance; }

  static void SetLocale(const std::string locale) { Get().ISetLocale(locale); }
  
  static std::string GetLocale() { return Get().m_locale; }

  static const std::string Translate(std::string msgid) { return Get().ITranslate(msgid); }

  template<typename... Types>
  static const std::string Translate(std::string msgid, Types... args) { return Get().ITranslate(msgid, std::forward<Types>(args)...); }

private:
  i18n() {}
  ~i18n() {}

  std::filesystem::path m_localePath;
  std::string m_localeExtension;
  bool m_defaultLocale_exists;
  std::string m_defaultLocale;
  std::string m_locale;
  std::unordered_map<std::string, std::string> m_defaultDictionary;
  std::unordered_map<std::string, std::string> m_dictionary;

  void ISetLocale(const std::string locale) { m_locale = locale; loadDictionary(locale); }

  std::string ITranslate(const std::string msgid)
  {
    if (!m_defaultLocale_exists) // If not using a locale file for the default locale
    {
      if (m_locale == m_defaultLocale) return msgid; // Current locale is the default locale
      if (!m_dictionary.count(msgid)) return msgid; // Tranlation doesn't exist in the locale file
      return m_dictionary[msgid]; // Return translated string from the dictionary
    }
    // If using a locale file for the default locale
    if (m_locale == m_defaultLocale) return m_defaultDictionary[msgid]; // Current locale is the default locale
    if (!m_dictionary.count(msgid)) return m_defaultDictionary[msgid]; // Tranlation doesn't exist in the locale file
    return m_dictionary[msgid]; // Return translated string from the dictionary
  }

  template<typename... Types>
  std::string ITranslate(std::string msgid, Types... args)
  {
    if (!m_defaultLocale_exists) // If not using a locale file for the default locale
    {
      if (m_locale == m_defaultLocale) return i18n_format(msgid, std::forward<Types>(args)...); // Current locale is the default locale
      if (!m_dictionary.count(msgid)) return i18n_format(msgid, std::forward<Types>(args)...); // Tranlation doesn't exist in the locale file
      return i18n_format(m_dictionary[msgid], std::forward<Types>(args)...); // Return translated string from the dictionary
    }
    // If using a locale file for the default locale
    if (m_locale == m_defaultLocale) return i18n_format(m_defaultDictionary[msgid], std::forward<Types>(args)...); // Current locale is the default locale
    if (!m_dictionary.count(msgid)) return i18n_format(m_defaultDictionary[msgid], std::forward<Types>(args)...); // Tranlation doesn't exist in the locale file
    return i18n_format(m_dictionary[msgid], std::forward<Types>(args)...); // Return translated string from the dictionary
  }

  std::unordered_map<std::string, std::string> parseDictionary(std::filesystem::path locale_path)
  {
    std::unordered_map<std::string, std::string> dictionary;
    enum lineType { MSG_ID, MSG_STR };
    std::string line;
    std::string prev_line;
    lineType prev_type;
    std::ifstream file (locale_path);
    while (std::getline(file, line))
    {
      if (line.find("msgid:") == 0)
      {
        line.erase(0, 6);
        while (line.find(" ") == 0) line.erase(0, 1);
        prev_line = line;
        prev_type = MSG_ID;
      }
      if (prev_type == MSG_ID && line.find("msgstr:") == 0)
      {
        line.erase(0, 7);
        while (line.find(" ") == 0) line.erase(0, 1);
        dictionary[prev_line] = line;
        prev_line = line;
        prev_type = MSG_STR;
      }
    }
    file.close();
    return dictionary;
  }

  void loadDefaultDictionary()
  {
    std::filesystem::path defaultLocale_path = getLocalePath(m_defaultLocale);
    if (!std::filesystem::exists(defaultLocale_path))
    {
      m_defaultLocale_exists = false;
      m_defaultDictionary.clear();
      return;
    }
    m_defaultLocale_exists = true;
    m_defaultDictionary = parseDictionary(defaultLocale_path);
  }

  void loadDictionary(const std::string locale)
  {
    if (locale == m_defaultLocale)
    {
      m_dictionary.clear();
      return;
    }
    std::filesystem::path locale_path = getLocalePath(locale);
    if (!std::filesystem::exists(locale_path))
    {
      m_locale = m_defaultLocale;
      m_dictionary.clear();
      return;
    }
    m_dictionary = parseDictionary(locale_path);
  }

  std::filesystem::path getLocalePath(std::string locale)
  {
    std::string locale_filename = locale + m_localeExtension;
    return (m_localePath / locale_filename);
  }
};