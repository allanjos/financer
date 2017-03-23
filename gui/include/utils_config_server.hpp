#ifndef UTILS_CONFIG_SERVER_HPP
#define UTILS_CONFIG_SERVER_HPP

#include "gui_lib.hpp"
#include "utils_config.hpp"

class ConfigServer
{
  public:
    static HashStringMap map;

    static int Load();

    static wxString Get(wxString name);
};

#endif /* UTILS_CONFIG_SERVER_HPP */
