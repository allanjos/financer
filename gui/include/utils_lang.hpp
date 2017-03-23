#ifndef UTILS_LANG_HPP
#define UTILS_LANG_HPP

#include "gui_lib.hpp"
#include "utils_parser_xml.hpp"
#include "utils_types.hpp"

class Lang {
  public:

    static const char *Default;

    static HashStringMap hashMapStrings;

    static HashStringMap hashMapLangFiles;

    static HashStringMap hashMapStringsBuiltin;

    // Attributes

    static wxString currencyList[2];

    static wxString currencyDefault;

    static wxString filePath;

    static wxFileName execPathInfo;

    // Directory where application executable resides
    static wxString execDirectory;

    // Methods

    static int Load();

    static void FileProcess(wxXmlNode *node);

    static int Init();

    static void ProcessDir(wxString fullpath);

    static const wxString GetDefault();

    static const wxString Get(wxString text);

    static void LoadBuiltin();
};

#endif /* UTILS_LANG_HPP */