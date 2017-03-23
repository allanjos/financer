#include "utils_datetime.hpp"
#include "utils_log.hpp"
#include "utils_config.hpp"

wxString DT::ToDB(wxString str)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("DT::ToDB(") + str + wxT(")"), LogMessage::MSG_DBG);

  wxRegEx regex;

  wxString result = "";

  if (Config::GetInt("date:format") == DT::DATE_ENG_FORMAT) {
    if (regex.Compile(wxT("^([0-9]{1,2})/([0-9]{1,2})/([0-9]{4})$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
      wxString month = regex.GetMatch(str, 1);
      wxString day = regex.GetMatch(str, 2);
      wxString year = regex.GetMatch(str, 3);

      result = year + wxT("-") + month + wxT("-") + day;
    }
  }
  else if (Config::GetInt("date:format") == DT::DATE_BRA_FORMAT) {
    if (regex.Compile(wxT("^([0-9]{1,2})/([0-9]{1,2})/([0-9]{4})$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
      wxString day = regex.GetMatch(str, 1);
      wxString month = regex.GetMatch(str, 2);
      wxString year = regex.GetMatch(str, 3);

      result = year + wxT("-") + month + wxT("-") + day;
    }
  }

  return result;
}

wxString DT::FromDB(wxString str)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("DT::FromDB(") + str + wxT(")"), LogMessage::MSG_DBG);

  wxRegEx regex;

  wxString result;

  if (regex.Compile(wxT("^([0-9]{1,4})-([0-9]{1,2})-([0-9]{2})$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
    wxString year = regex.GetMatch(str, 1);
    wxString month = regex.GetMatch(str, 2);
    wxString day = regex.GetMatch(str, 3);

    if (Config::GetInt("date:format") == DT::DATE_ENG_FORMAT) {
      result = month + wxT("/") + day + wxT("/") + year;
    }
    else if (Config::GetInt("date:format") == DT::DATE_BRA_FORMAT) {
      result = day + wxT("/") + month + wxT("/") + year;
    }
  }

  return result;
}

wxString DT::ToString(wxString year, wxString month, wxString day)
{
  return day + wxT("/") + month + wxT("/") + year;
}