#ifndef _UTILS_CONFIG_HPP_
#define _UTILS_CONFIG_HPP_

#include "app.hpp"
#include "gui_lib.hpp"
#include "utils_types.hpp"
#include "utils_parser_xml.hpp"

class Config {
  private:

    static HashStringMap map;

    static wxString currencyDefault;

    static wxString filePath;

    static const char *defaultConfigContent;

  public:

    static wxString currencyList[2];

    // Methods

    static int Create();

    static int Load();

    static void FileProcess(wxXmlNode *node);

    static int Init();

    static wxString GetFilePath();

    static wxString Get(wxString name);

    static int GetInt(wxString name);

    static int LoadServer();
};

#endif /* _UTILS_CONFIG_HPP_ */