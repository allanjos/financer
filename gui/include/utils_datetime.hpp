#ifndef UTILS_DATETIME_HPP
#define UTILS_DATETIME_HPP

#include "gui_lib.hpp"

class DT
{
  public:
  	enum {
  	  DATE_ENG_FORMAT = 1,
  	  DATE_BRA_FORMAT = 2
  	};

	static wxString ToDB(wxString str);

	static wxString FromDB(wxString str);

  static wxString ToString(wxString year, wxString month, wxString day);
};

#endif /* UTILS_DATETIME_HPP */