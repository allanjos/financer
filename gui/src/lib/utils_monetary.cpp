#include "utils_monetary.hpp"
#include "utils_lang.hpp"
#include "utils_config.hpp"
#include "utils_number.hpp"
#include "gui_lib.hpp"
#include "gui.hpp"
#include "utils_database.hpp"


Price::Price()
{
  this->value = 0.0;

  this->currencyStr = wxT("");

  this->currencyId = -1;
}


HashStringMap Monetary::fracSep;
HashStringMap Monetary::thousandSep;

HashStringMap Monetary::fracSepRegexChar;
HashStringMap Monetary::thousandSepRegexChar;


void Monetary::Init()
{
  wxString numberFormatEngMapKey = wxString::Format(wxT("%d"), Number::NUMBER_ENG_FORMAT);
  wxString numberFormatBraMapKey = wxString::Format(wxT("%d"), Number::NUMBER_BRA_FORMAT);

  Monetary::thousandSep[numberFormatEngMapKey] = ",";
  Monetary::thousandSep[numberFormatBraMapKey] = ".";

  Monetary::fracSep[numberFormatEngMapKey] = ".";
  Monetary::fracSep[numberFormatBraMapKey] = ",";

  Monetary::thousandSepRegexChar[numberFormatEngMapKey] = ",";
  Monetary::thousandSepRegexChar[numberFormatBraMapKey] = "\\.";

  Monetary::fracSepRegexChar[numberFormatEngMapKey] = "\\.";
  Monetary::fracSepRegexChar[numberFormatBraMapKey] = ",";
}


int Monetary::LoadCurrencyList(wxChoice *choiceCtrl)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("Monetary::LoadCurrencyList()"), LogMessage::MSG_DBG);

  DBQuery dbquery;

  choiceCtrl->Freeze();

  choiceCtrl->Clear();

  choiceCtrl->Append(wxT(""), (wxClientData *) NULL);

  wxString sql = "SELECT * FROM " + DB::GetTableName("monetary.currency");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de moedas cadastradas."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("currency_id")));

      choiceCtrl->Append(dbquery.GetFieldStrByName(index, "sigla") + wxT(" - ") + dbquery.GetFieldStrByName(index, "nome"), container);

      if (dbquery.GetFieldStrByName(index, "padrao").Cmp("t") ==  0) {
        choiceCtrl->SetSelection(choiceCtrl->GetCount() - 1);
      }
    }
  }

  choiceCtrl->SetSelection(0);

  choiceCtrl->Thaw();

  return rc;
}


int Monetary::GetDefaultCurrency(Currency &currency)
{
  DBQuery dbquery;

  wxString sql = "SELECT * FROM " + DB::GetTableName("monetary.currency") + " WHERE padrao = 't' LIMIT 1";

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a moeda padrão cadastrada."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      currency.id = dbquery.GetFieldIntByName(index, "currency_id");

      currency.abbr = dbquery.GetFieldStrByName(index, "sigla");

      currency.name = dbquery.GetFieldStrByName(index, "nome");
    }
  }

  return rc;
}

/* \todo This function is inconsistent. */
double Monetary::ValueFromStr(wxString str)
{
  if (Config::Get("number:format").Length() <= 0 || Monetary::thousandSep[Config::Get("number:format")].Length() <= 0) {
    return 0.0;
  }

  double value;

  wxRegEx regex(wxT("\\."), wxRE_EXTENDED | wxRE_ADVANCED);

  regex.Replace(&str, Monetary::fracSep[Config::Get("number:format")]);

  str.ToDouble(&value);

  return value;
}


double Monetary::ValueFromStr(const char *str)
{
  return Monetary::ValueFromStr(wxString(str, wxConvUTF8, strlen(str)));
}


wxString Monetary::Value2Str(double value, short precision)
{
  wxString format = wxString::Format(wxT("%%.%df"), precision);

  return wxString::Format(format, value);
}


bool Monetary::NumberIsValid(wxString str) {
  wxRegEx regex;

  if (regex.Compile(wxT("^[0-9]+([,.][0-9]+)?$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
    return true;
  }

  return false;
}


wxString Monetary::NumberNormalize(wxString str)
{
  wxRegEx regex(wxT("\\."), wxRE_EXTENDED | wxRE_ADVANCED);

  if (Config::Get("number:format").Length() > 0 && Monetary::thousandSep[Config::Get("number:format")].Length() > 0) {
    regex.Replace(&str, Monetary::fracSep[Config::Get("number:format")]);
  }

  return str;
}


bool Monetary::MonetaryIsValid(wxString str) {
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("Monetary::Normalize(): number:format=") + Config::Get("number:format") +
                      wxT(", thousand separator=") + Monetary::thousandSep[Config::Get("number:format")], LogMessage::MSG_DBG);

  wxRegEx regex;

  if (Config::Get("number:format").Length() > 0 && Monetary::thousandSep[Config::Get("number:format")].Length() > 0) {
    wxString formatId = Config::Get("number:format");

    wxString regexStr = wxT("^[0-9]+(") + Monetary::fracSepRegexChar[formatId] + wxT("[0-9]+)?$");

    if (regex.Compile(regexStr, wxRE_EXTENDED | wxRE_ADVANCED) && regex.Matches(str)) {
      return true;
    }

    regexStr = wxT("^[0-9]{1,3}?(") + Monetary::thousandSepRegexChar[formatId] + wxT("[0-9]{3})+(") + Monetary::fracSepRegexChar[formatId] + wxT("[0-9]+)?$");

    //if (regex.Compile(wxT("^[0-9]{1,3}?(\\.[0-9]{3})+(\\,[0-9]+)?$"), wxRE_EXTENDED | wxRE_ADVANCED) && regex.Matches(str))
    if (regex.Compile(regexStr, wxRE_EXTENDED | wxRE_ADVANCED) && regex.Matches(str)) {
      return true;
    }

    return false;
  }

  return false;
}


wxString Monetary::Normalize(wxString str)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("Monetary::Normalize(): number:format=") + Config::Get("number:format") +
                      wxT(", thousand separator=") + Monetary::thousandSep[Config::Get("number:format")], LogMessage::MSG_DBG);

  wxRegEx regex;

  if (Config::Get("number:format").Length() <= 0 || Monetary::thousandSep[Config::Get("number:format")].Length() <= 0) {
    return str;
  }

  wxString formatId = Config::Get("number:format");

  wxString regexStr = wxT("^([0-9]+)(") + Monetary::fracSepRegexChar[formatId] + wxT("[0-9]+)?$");

  if (regex.Compile(regexStr, wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
    wxString integerPart = regex.GetMatch(str, 1);
    wxString floatingPart = regex.GetMatch(str, 2);

    str = wxT("");

    while (regex.Compile(wxT("^([0-9]+)([0-9]{3})$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(integerPart)) {
      wxString head = regex.GetMatch(integerPart, 1);
      wxString tail = regex.GetMatch(integerPart, 2);

      //wxMessageBox(integerPart + wxT(" = ") + head + wxT(" + ") + tail);

      str = Monetary::thousandSep[formatId] + tail + str;

      integerPart = head;

      //wxMessageBox(wxT("integerPart: " + integerPart));
    }

    //wxMessageBox(wxT("head: ") + integerPart);

    if (floatingPart.Length() == 0) {
      floatingPart = Monetary::fracSep[formatId] + wxT("00");
    }
    else if (floatingPart.Length() == 2) {
      floatingPart = floatingPart + wxT("0");
    }

    str = integerPart + str + floatingPart;
  }

  return str;
}


wxString Monetary::Number2Monetary(wxString str) {
  wxRegEx regex;

  bool negativeNumber = false;

  if (Config::Get("number:format").Length() <= 0 ||
      Monetary::thousandSep[Config::Get("number:format")].Length() <= 0 ||
      Monetary::fracSep[Config::Get("number:format")].Length() <= 0) {
    return str;
  }

  wxString formatId = Config::Get("number:format");

  if (regex.Compile(wxT("^-"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
    negativeNumber = true;

    regex.Replace(&str, wxT(""));
  }

  if (regex.Compile(wxT("^([0-9]+)(\\.[0-9]+)?$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(str)) {
    wxString integerPart = regex.GetMatch(str, 1);
    wxString floatingPart = regex.GetMatch(str, 2);

    str = wxT("");

    while (regex.Compile(wxT("^([0-9]+)([0-9]{3})$"), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(integerPart)) {
      wxString head = regex.GetMatch(integerPart, 1);
      wxString tail = regex.GetMatch(integerPart, 2);

      //wxMessageBox(integerPart + wxT(" = ") + head + wxT(" + ") + tail);

      str = Monetary::thousandSep[formatId] + tail + str;

      integerPart = head;

      //wxMessageBox(wxT("integerPart: " + integerPart));
    }

    //wxMessageBox(wxT("head: ") + integerPart);

    if (floatingPart.Length() == 0) {
      floatingPart = Monetary::fracSep[formatId] + wxT("00");
    }
    else if (floatingPart.Length() == 2) {
      floatingPart = floatingPart + wxT("0");
    }

    if (regex.Compile(wxT("\\."), wxRE_EXTENDED|wxRE_ADVANCED) && regex.Matches(floatingPart)) {
      regex.Replace(&floatingPart, Monetary::fracSep[formatId]);
    }

    str = integerPart + str + floatingPart;
  }
  // If the format was not recognized, returns 0 (zero) quantity to maintain a monetary representation.
  else {
    str = wxT("0") + Monetary::fracSep[formatId] + wxT("00");
  }

  if (negativeNumber) {
    str = wxT("-") + str;
  }

  return str;
}


wxString Monetary::Number2Monetary(double number, int precision) {
  wxString precisionStr = wxT("%.") + wxString::Format(wxT("%d"), precision) + wxT("f");

  return Monetary::Number2Monetary(Monetary::Number2System(wxString::Format(precisionStr, number)));
}


wxString Monetary::Number2Database(wxString str) {
  wxRegEx regex(wxT("\\,"), wxRE_EXTENDED|wxRE_ADVANCED);

  regex.Replace(&str, wxT("."));

  return str;
}


wxString Monetary::Number2Database(double number, int precision) {
  if (Config::Get("number:format").Length() <= 0 || Monetary::fracSepRegexChar[Config::Get("number:format")].Length() <= 0) {
    return wxT("");
  }

  wxString formatId = Config::Get("number:format");

  wxString precisionStr = wxT("%.") + wxString::Format(wxT("%d"), precision) + wxT("f");

  wxString str = wxString::Format(precisionStr, number);

  wxRegEx regex(Monetary::fracSepRegexChar[formatId], wxRE_EXTENDED|wxRE_ADVANCED);

  regex.Replace(&str, wxT("."));

  return str;
}


wxString Monetary::Number2System(wxString str) {
  if (Config::Get("number:format").Length() <= 0 || Monetary::fracSepRegexChar[Config::Get("number:format")].Length() <= 0) {
    return wxT("");
  }

  wxString formatId = Config::Get("number:format");

  wxRegEx regex(Monetary::fracSepRegexChar[formatId], wxRE_EXTENDED | wxRE_ADVANCED);

  regex.Replace(&str, wxT("."));

  return str;
}


wxString Monetary::Monetary2Database(wxString monetaryStr) {
  if (Config::Get("number:format").Length() <= 0 ||
    Monetary::fracSepRegexChar[Config::Get("number:format")].Length() <= 0 ||
    Monetary::thousandSepRegexChar[Config::Get("number:format")].Length() <= 0) {
    return wxT("");
  }

  wxString formatId = Config::Get("number:format");

  wxRegEx regex;

  wxString regexStr = wxT("^[0-9]{1,3}?(") + Monetary::thousandSepRegexChar[formatId] + wxT("[0-9]{3})+(") + Monetary::fracSepRegexChar[formatId] + wxT("[0-9]+)?$");

  if (regex.Compile(regexStr, wxRE_EXTENDED | wxRE_ADVANCED) && regex.Matches(monetaryStr)) {
    // First, remove the dots that separates thousand factors units

    regex.Compile(Monetary::thousandSepRegexChar[formatId], wxRE_EXTENDED | wxRE_ADVANCED);

    regex.Replace(&monetaryStr, wxT(""));

    // Second, replace , that separates the integer and floating parts by a dot

    regex.Compile(Monetary::fracSepRegexChar[formatId], wxRE_EXTENDED | wxRE_ADVANCED);

    regex.Replace(&monetaryStr, wxT("."));
  }
  else if (regex.Compile(wxT("^([0-9]+)(") + Monetary::fracSepRegexChar[formatId] + wxT("[0-9]+)?$"), wxRE_EXTENDED | wxRE_ADVANCED) &&
           regex.Matches(monetaryStr)) {
    wxRegEx regex(Monetary::fracSepRegexChar[formatId], wxRE_EXTENDED | wxRE_ADVANCED);

    regex.Replace(&monetaryStr, wxT("."));
  }

  return monetaryStr;
}


wxString Monetary::Monetary2Number(wxString monetaryStr) {
  if (Config::Get("number:format").Length() <= 0 ||
    Monetary::fracSepRegexChar[Config::Get("number:format")].Length() <= 0 ||
    Monetary::thousandSepRegexChar[Config::Get("number:format")].Length() <= 0) {
    return wxT("");
  }

  wxString formatId = Config::Get("number:format");

  wxRegEx regex;

  wxString regexStr = wxT("^[0-9]{1,3}?(") + Monetary::thousandSepRegexChar[formatId] + wxT("[0-9]{3})+(") + Monetary::fracSepRegexChar[formatId] + wxT("[0-9]+)?$");

  if (regex.Compile(regexStr, wxRE_EXTENDED | wxRE_ADVANCED) && regex.Matches(monetaryStr)) {
    // First, remove the dots that separates thousand factors units

    regex.Compile(Monetary::thousandSepRegexChar[formatId], wxRE_EXTENDED | wxRE_ADVANCED);

    regex.Replace(&monetaryStr, wxT(""));
  }

  return monetaryStr;
}