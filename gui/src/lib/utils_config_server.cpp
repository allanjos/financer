#include "utils_config_server.hpp"
#include "utils_database.hpp"
#include "utils_lang.hpp"

HashStringMap ConfigServer::map;

int ConfigServer::Load()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("ConfigServer::Load()", LogMessage::MSG_DBG);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT c.* ")
                           wxT("FROM ") + DB::GetTableName("configuration") + wxT(" c"));

  if (rc != 0) {
    messenger->Register(Lang::Get("Cannot get the current system configuration") + wxT("."), LogMessage::MSG_ERROR);

    return -1;
  }
  else if (dbquery.GetRowsCount() > 0) {
    ConfigServer::map["purchase_account_output_id"] = dbquery.GetFieldStrByName(0, "purchase_account_output_id");
    ConfigServer::map["sale_account_input_id"] = dbquery.GetFieldStrByName(0, "sale_account_input_id");
    ConfigServer::map["currency_id"] = dbquery.GetFieldStrByName(0, "currency_id");
    ConfigServer::map["date_format_id"] = dbquery.GetFieldStrByName(0, "date_format_id");
    ConfigServer::map["sale_fee_validity_days"] = dbquery.GetFieldStrByName(0, "sale_fee_validity_days");
    ConfigServer::map["sale_fee"] = dbquery.GetFieldStrByName(0, "sale_fee");
    ConfigServer::map["sale_tax_rate_pis"] = dbquery.GetFieldStrByName(0, "sale_tax_rate_pis");
    ConfigServer::map["sale_tax_rate_cofins"] = dbquery.GetFieldStrByName(0, "sale_tax_rate_cofins");
    ConfigServer::map["sale_tax_rate_csll"] = dbquery.GetFieldStrByName(0, "sale_tax_rate_csll");
    ConfigServer::map["sale_tax_rate_irpj"] = dbquery.GetFieldStrByName(0, "sale_tax_rate_irpj");
  }

  return 0;
}


wxString ConfigServer::Get(wxString name)
{
  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT ") + name + wxT(" ")
                           wxT("FROM ") + DB::GetTableName("configuration"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(Lang::Get("Cannot get the current system configuration") + wxT("."), LogMessage::MSG_ERROR);

    return "";
  }
  else if (dbquery.GetRowsCount() > 0) {
    return dbquery.GetFieldStrByName(0, name);
  }

  return "";
}
