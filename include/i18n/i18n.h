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
  typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> dictionary;
  
public:

  i18n(const i18n&) = delete;
  i18n(i18n&&) = delete;
  i18n& operator=(const i18n&) = delete;
  i18n& operator=(i18n&&) = delete;

  static void Init(std::filesystem::path localePath = "locales", std::string locale = "en-US", std::string defaultLocale = "en-US", std::string localeExtension = ".locale");

  static i18n& GetInstance();

  static void SetLocale(const std::string locale);
  
  static std::string GetLocale();

  template<typename... Types>
  static const std::string Translate(std::string msgid, Types... args);

  template<typename... Types>
  static const std::string TranslateN(std::string nameSpace, std::string msgid, Types... args);

private:
  i18n() {}
  ~i18n() {}

  std::filesystem::path m_localePath;
  std::string m_localeExtension;
  bool m_defaultLocale_exists;
  std::string m_defaultLocale;
  std::string m_locale;
  dictionary m_defaultDictionary;
  dictionary m_dictionary;
  std::string m_defaultNS {"default"};

  void ISetLocale(const std::string locale);

  template<typename... Types>
  std::string ITranslate(std::string msgid, Types... args);

  template<typename... Types>
  std::string ITranslateN(std::string ns, std::string msgid, Types... args);

  dictionary parseDictionary(std::filesystem::path locale_path);

  void loadDefaultDictionary();

  void loadDictionary(const std::string locale);

  std::filesystem::path getLocalePath(std::string locale);
};

inline void i18n::Init(std::filesystem::path localePath, std::string locale, std::string defaultLocale, std::string localeExtension)
{
  GetInstance().m_localePath = localePath;
  GetInstance().m_defaultLocale = defaultLocale;
  GetInstance().m_locale = locale;
  GetInstance().m_localeExtension = localeExtension;
  GetInstance().loadDefaultDictionary();
  SetLocale(locale);
}

inline i18n& i18n::GetInstance()
{
  static i18n instance;
  return instance;
}

inline void i18n::SetLocale(const std::string locale)
{
  GetInstance().ISetLocale(locale);
}

inline std::string i18n::GetLocale() {
  return GetInstance().m_locale;
}

template<typename... Types>
inline const std::string i18n::Translate(std::string msgid, Types... args) {
  return GetInstance().ITranslate(msgid, std::forward<Types>(args)...);
}

template<typename... Types>
inline const std::string i18n::TranslateN(std::string nameSpace, std::string msgid, Types... args) {
  return GetInstance().ITranslateN(nameSpace, msgid, std::forward<Types>(args)...);
}

inline void i18n::ISetLocale(const std::string locale)
{
  m_locale = locale;
  loadDictionary(locale);
}

template<typename... Types>
inline std::string i18n::ITranslate(std::string msgid, Types... args)
{
  switch (sizeof...(Types) == 0)
  {
  case true:
    if (!m_defaultLocale_exists) // If not using a locale file for the default locale
    {
      if (m_locale == m_defaultLocale) return msgid; // Current locale is the default locale
      if (!m_dictionary[m_defaultNS].count(msgid)) return msgid; // Tranlation doesn't exist in the locale file
      return m_dictionary[m_defaultNS][msgid]; // Return translated string from the dictionary
    }
    // If using a locale file for the default locale
    if (m_locale == m_defaultLocale) return m_defaultDictionary[m_defaultNS][msgid]; // Current locale is the default locale
    if (!m_dictionary[m_defaultNS].count(msgid)) return m_defaultDictionary[m_defaultNS][msgid]; // Tranlation doesn't exist in the locale file
    return m_dictionary[m_defaultNS][msgid]; // Return translated string from the dictionary
    break;
  
  case false:
    if (!m_defaultLocale_exists) // If not using a locale file for the default locale
    {
      if (m_locale == m_defaultLocale) return i18n_format(msgid, std::forward<Types>(args)...); // Current locale is the default locale
      if (!m_dictionary[m_defaultNS].count(msgid)) return i18n_format(msgid, std::forward<Types>(args)...); // Tranlation doesn't exist in the locale file
      return i18n_format(m_dictionary[m_defaultNS][msgid], std::forward<Types>(args)...); // Return translated string from the dictionary
    }
    // If using a locale file for the default locale
    if (m_locale == m_defaultLocale) return i18n_format(m_defaultDictionary[m_defaultNS][msgid], std::forward<Types>(args)...); // Current locale is the default locale
    if (!m_dictionary[m_defaultNS].count(msgid)) return i18n_format(m_defaultDictionary[m_defaultNS][msgid], std::forward<Types>(args)...); // Tranlation doesn't exist in the locale file
    return i18n_format(m_dictionary[m_defaultNS][msgid], std::forward<Types>(args)...); // Return translated string from the dictionary
    break;
  }
}

template<typename... Types>
inline std::string i18n::ITranslateN(std::string ns, std::string msgid, Types... args)
{
  switch (sizeof...(Types) == 0)
  {
  case true:
    if (!m_defaultLocale_exists) // If not using a locale file for the default locale
    {
      if (m_locale == m_defaultLocale) return msgid; // Current locale is the default locale
      if (!m_dictionary[ns].count(msgid)) return msgid; // Tranlation doesn't exist in the locale file
      return m_dictionary[ns][msgid]; // Return translated string from the dictionary
    }
    // If using a locale file for the default locale
    if (m_locale == m_defaultLocale) return m_defaultDictionary[ns][msgid]; // Current locale is the default locale
    if (!m_dictionary[ns].count(msgid)) return m_defaultDictionary[ns][msgid]; // Tranlation doesn't exist in the locale file
    return m_dictionary[ns][msgid]; // Return translated string from the dictionary
    break;

  case false:
    if (!m_defaultLocale_exists) // If not using a locale file for the default locale
    {
      if (m_locale == m_defaultLocale) return i18n_format(msgid, std::forward<Types>(args)...); // Current locale is the default locale
      if (!m_dictionary[ns].count(msgid)) return i18n_format(msgid, std::forward<Types>(args)...); // Tranlation doesn't exist in the locale file
      return i18n_format(m_dictionary[ns][msgid], std::forward<Types>(args)...); // Return translated string from the dictionary
    }
    // If using a locale file for the default locale
    if (m_locale == m_defaultLocale) return i18n_format(m_defaultDictionary[ns][msgid], std::forward<Types>(args)...); // Current locale is the default locale
    if (!m_dictionary[ns].count(msgid)) return i18n_format(m_defaultDictionary[ns][msgid], std::forward<Types>(args)...); // Tranlation doesn't exist in the locale file
    return i18n_format(m_dictionary[ns][msgid], std::forward<Types>(args)...); // Return translated string from the dictionary
    break;
  }
}

inline i18n::dictionary i18n::parseDictionary(std::filesystem::path locale_path)
{
  dictionary dictionary;
  enum lineType { NS, MSG_ID, MSG_STR };
  std::string line;
  std::string ns_cache, msgid_cache;
  lineType prev_type;
  std::ifstream file (locale_path);

  while (std::getline(file, line))
  {
    if (line.find("ns:") == 0)
    {
      line.erase(0, 3);
      while (line.find(" ") == 0) line.erase(0, 1);
      msgid_cache.clear();
      ns_cache = line;
      prev_type = NS;
    }
    if (line.find("msgid:") == 0)
    {
      line.erase(0, 6);
      while (line.find(" ") == 0) line.erase(0, 1);
      msgid_cache = line;
      if (prev_type != NS) ns_cache = m_defaultNS;
      prev_type = MSG_ID;
    }
    if (line.find("msgstr:") == 0)
    {
      if (prev_type = MSG_ID)
      {
        line.erase(0,7);
        while (line.find(" ") == 0) line.erase(0, 1);
        dictionary[ns_cache][msgid_cache] = line;
      }
      ns_cache.clear();
      msgid_cache.clear();
      prev_type = MSG_STR;
    }
  }
  file.close();
  return dictionary;
}

inline void i18n::loadDefaultDictionary()
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

inline void i18n::loadDictionary(const std::string locale)
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

inline std::filesystem::path i18n::getLocalePath(std::string locale)
{
  std::string locale_filename = locale + m_localeExtension;
  return (m_localePath / locale_filename);
}