#include "gui_lib.hpp"
#include "utils_lang.hpp"
#include "utils_log.hpp"
#include "utils_config.hpp"

const char *Lang::Default = "en-US";

wxString Lang::currencyList[] = {wxT("R$"), wxT("US$")};
wxString Lang::currencyDefault = wxT("R$"); /**< \todo Remove this. This is to store in the database. */

wxString Lang::filePath = "";

wxFileName Lang::execPathInfo;

wxString Lang::execDirectory = "";

HashStringMap Lang::hashMapStrings;
HashStringMap Lang::hashMapLangFiles;

HashStringMap Lang::hashMapStringsBuiltin;


void Lang::LoadBuiltin()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("Lang::LoadBuiltin()", LogMessage::MSG_DBG);

  Lang::hashMapStrings.clear();

  Lang::hashMapStrings["App_description"] = "Personal finance management";
  Lang::hashMapStrings["Close_application"] = "Close application";
  Lang::hashMapStrings["About_application"] = "About application";
}


int Lang::Init()
{
  Lang::LoadBuiltin();

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("Lang::Init()", LogMessage::MSG_DBG);

  Lang::execDirectory = wxStandardPaths::Get().GetExecutablePath();

  Lang::execPathInfo.Assign(Lang::execDirectory);

  Lang::filePath = Lang::execPathInfo.GetPath() + wxFileName::GetPathSeparator() +  wxT("config") + wxFileName::GetPathSeparator() + wxT("lang");

  messenger->Register("Lang::filePath: " + Lang::filePath, LogMessage::MSG_DBG);

  Lang::hashMapLangFiles.clear();

  Lang::ProcessDir(Lang::filePath);

  return 0;
}


void Lang::ProcessDir(wxString fullpath)
{
  if (wxDirExists(fullpath)) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register("Directory: " + fullpath, LogMessage::MSG_DBG);

    wxString filename = wxFileNameFromPath(fullpath);

    wxDir dir(fullpath);

    if (!dir.IsOpened()) {
      return;
    }

    wxString path = wxT("");

    bool cont = dir.GetFirst(&path);

    while (cont) {
      //wxMessageBox(path);

      ProcessDir(fullpath + wxFileName::GetPathSeparator() + path);

      cont = dir.GetNext(&path);
    }
  }
  else if (wxFileExists(fullpath)) {
    wxFileName fileFullpath(fullpath);

    wxString filename = fileFullpath.GetName();

    Lang::hashMapLangFiles[filename] = fullpath;

    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("File: ") + fullpath, LogMessage::MSG_DBG);
  }
  else {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("File does not exist: ") + fullpath, LogMessage::MSG_DBG);
  }
}


int Lang::Load()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("Lang::Load()", LogMessage::MSG_DBG);

  if (Config::Get("lang").Length() > 0) {
    Lang::filePath = Lang::hashMapLangFiles[Config::Get("lang")];
  }
  else {
    Lang::filePath = "English";
  }

  // Load builtin language
  if (Lang::filePath.Cmp("English") == 0) {
    Lang::LoadBuiltin();
  }
  // Load language from file
  else {
    if (!wxFile::Exists(Lang::filePath)) {
      messenger->Register(wxT("Arquivo de configuração '") + Lang::filePath + wxT("' não encontrado."), LogMessage::MSG_WARN);
    }
    else {
      wxXmlDocument doc;

      if (!doc.Load(Lang::filePath)) {
        messenger->Register(wxT("Não foi possível abrir o arquivo de configuração") + Lang::filePath + wxT("."), LogMessage::MSG_ERROR);

        return -1;
      }

      // Process the XML file

      if (doc.GetRoot()->GetName() != wxT("resources")) {
        messenger->Register(wxT("O arquivo de configuração informado não é válido."), LogMessage::MSG_WARN);

        return -1;
      }

      // Process configuration file content

      Lang::FileProcess(doc.GetRoot());
    }
  }

  return 0;
}


void Lang::FileProcess(wxXmlNode *node)
{
  wxXmlNode *child = node->GetChildren();

  while (child) {
    //LogMessenger *messenger = LogMessenger::GetInstance();

    //messenger->Register("lang node: " + child->GetName(), LogMessage::MSG_DBG);

    if (child->GetName() == wxT("string")) {
      wxString name = child->GetAttribute(wxT("name"), wxT(""));

      if (child->GetChildren()) {
        wxXmlNode *textNode = child->GetChildren();

        Lang::hashMapStrings[name] = textNode->GetContent();
      }
      else {
        Lang::hashMapStrings[name] = "";
      }
    }

    if (child->GetChildren()) {
      Lang::FileProcess(child);
    }

    child = child->GetNext();
  }
}


const wxString Lang::GetDefault()
{
  return Lang::Default;
}


const wxString Lang::Get(wxString text)
{
  if (text.Length() == 0) {
    return wxT("");
  }
  else if (Lang::hashMapStrings[text].Length() > 0) {
    return Lang::hashMapStrings[text];
  }
  else {
    return text;
  }
}