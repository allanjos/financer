#include "gui_lib.hpp"
#include "utils_config.hpp"
#include "utils_log.hpp"
#include "utils_lang.hpp"


wxString Config::currencyList[] = {wxT("R$"), wxT("US$")};
wxString Config::currencyDefault = wxT("R$"); /**< \todo Remove this. This is to store in the database. */

wxString Config::filePath = "";

HashStringMap Config::map;

const char *Config::defaultConfigContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                           "<config>\n"
                                           "  <database setuponinit=\"true\">\n"
                                           "    <connection host=\"localhost\" port=\"5432\" dbname=\"comercial\" "
                                                   "user=\"postgres\" password=\"postgres\" sslmode=\"disable\" "
                                                   "filepath=\"financer.db\" type=\"SQLite\"/>\n"
                                           "  </database>\n"
                                           "  <language id=\"English\"/>\n"
                                           "</config>"; 


int Config::Init()
{
	Config::filePath = App::GetExecDir() + wxFileName::GetPathSeparator() +  wxT("config") + wxFileName::GetPathSeparator() + wxT("config.xml");

	return 0;
}


wxString Config::GetFilePath()
{
  return Config::filePath;
}


int Config::Create()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("Config::Create()");

  if (!wxFile::Exists(Config::filePath)) {
    wxFileName fileInfo = wxFileName(Config::filePath);

    wxString configDir = fileInfo.GetPath();
    
    wxFileName::Mkdir(configDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    messenger->Register(Lang::Get("Creating configuration file") + wxT("."), LogMessage::MSG_INFO);

    wxFileOutputStream fileOutputConfig(Config::filePath);

    fileOutputConfig.Write(Config::defaultConfigContent, strlen(Config::defaultConfigContent));
  }

  return 0;
}


int Config::Load()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (!wxFile::Exists(Config::filePath)) {
    messenger->Register(wxT("Arquivo de configuração '") + Config::filePath + wxT("' não encontrado."), LogMessage::MSG_WARN);
  }
  else {
    wxXmlDocument doc;

    if (!doc.Load(Config::filePath)) {
      messenger->Register(wxT("Não foi possível abrir o arquivo de configuração") + Config::filePath + wxT("."), LogMessage::MSG_ERROR);

      return -1;
    }

    // Process the XML file

    if (doc.GetRoot()->GetName() != wxT("config")) {
      messenger->Register(wxT("O arquivo de configuração informado não é válido."), LogMessage::MSG_WARN);

      return -1;
    }

    // Process configuration file content

    Config::FileProcess(doc.GetRoot());
  }

  return 0;
}


void Config::FileProcess(wxXmlNode *node)
{
  wxXmlNode *child = node->GetChildren();

  while (child) {
    if (child->GetName() == wxT("database")) {
      Config::map["db:setuponinit"] = child->GetAttribute(wxT("setuponinit"), wxT(""));

      if (Config::map["db:setuponinit"].Length() == 0)
        Config::map["db:setuponinit"] = "true";
    }
    else if (child->GetName() == wxT("connection")) {
      Config::map["db:type"] = child->GetAttribute(wxT("type"), wxT(""));

      Config::map["db:host"] = child->GetAttribute(wxT("host"), wxT(""));

      Config::map["db:port"] = child->GetAttribute(wxT("port"), wxT(""));

      Config::map["db:user"] = child->GetAttribute(wxT("user"), wxT(""));

      Config::map["db:password"] = child->GetAttribute(wxT("password"), wxT(""));

      Config::map["db:dbname"] = child->GetAttribute(wxT("dbname"), wxT(""));

      Config::map["db:sslmode"] = child->GetAttribute(wxT("sslmode"), wxT(""));

      Config::map["db:filepath"] = child->GetAttribute(wxT("filepath"), wxT(""));
    }
    else if (child->GetName() == wxT("language")) {
      Config::map["lang"] = child->GetAttribute(wxT("id"), wxT(""));
    }
    else if (child->GetName() == wxT("date")) {
      Config::map["date:format"] = child->GetAttribute(wxT("format"), wxT(""));
    }
    else if (child->GetName() == wxT("rfid")) {
      Config::map["hw:rfid:port"] = child->GetAttribute(wxT("port"), wxT(""));

      Config::map["hw:rfid:speed"] = child->GetAttribute(wxT("speed"), wxT(""));

      //LogMessenger *messenger = LogMessenger::GetInstance();

      //messenger->Register(wxT("hw:rfid:port") + Config::map["hw:rfid:port"], LogMessage::MSG_WARN);
    }

    if (child->GetChildren()) {
      Config::FileProcess(child);
    }

    child = child->GetNext();
  }
}


wxString Config::Get(wxString name)
{
  return Config::map[name];
}


int Config::GetInt(wxString name)
{
  long value;

  Config::map[name].ToLong(&value);

  return (int) value;
}


int Config::LoadServer()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(Lang::Get("Config::LoadServer()"), LogMessage::MSG_DBG);

  DBQuery dbquery;

  wxString dbCommand = wxT("SELECT cfg.*, mc.sigla AS currency_abbr ")
                       wxT("FROM ") + DB::GetTableName("configuration") + wxT(" cfg ")
                       wxT("LEFT JOIN ") + DB::GetTableName("monetary.currency") + " mc ON (mc.currency_id = cfg.currency_id)";

  int rc = dbquery.Execute(dbCommand);

  if (rc != 0) {
    messenger->Register(dbCommand, LogMessage::MSG_DBG);

    messenger->Register(Lang::Get("Cannot get the current system configuration") + wxT("."), LogMessage::MSG_ERROR);

    return -1;
  }
  else if (dbquery.GetRowsCount() > 0) {
    // Default currency

    Config::map["currency"] = dbquery.GetFieldStrByName(0, wxT("currency_id"));

    messenger->Register(Lang::Get("currency=") + Config::map["currency"], LogMessage::MSG_DBG);

    // Currency abbr

    Config::map["currency.abbr"] = dbquery.GetFieldStrByName(0, wxT("currency_abbr"));

    messenger->Register(Lang::Get("currency abbr =") + Config::map["currency.abbr"], LogMessage::MSG_DBG);

    // Date format

    Config::map["date:format"] = dbquery.GetFieldStrByName(0, wxT("date_format_id"));

    messenger->Register(Lang::Get("date:format=") + Config::map["date:format"], LogMessage::MSG_DBG);

    // Number format

    Config::map["number:format"] = dbquery.GetFieldStrByName(0, wxT("number_format_id"));

    messenger->Register(Lang::Get("number:format=") + Config::map["number:format"], LogMessage::MSG_DBG);
  }

  return 0;
}