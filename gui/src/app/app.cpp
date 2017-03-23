#include "app.hpp"
#include "utils_config.hpp"
#include "utils_config_server.hpp"
#include "utils_lang.hpp"
#include "utils_monetary.hpp"
#include "app_version.hpp"
#include "dlg_main.hpp"


wxFileName App::execPathInfo;

wxString App::execDirectory = "";


void App::Init()
{
  App::execDirectory = wxStandardPaths::Get().GetExecutablePath();

  App::execPathInfo.Assign(App::execDirectory);
}

wxString App::GetExecDir()
{
  return App::execPathInfo.GetPath();
}


// Main application

IMPLEMENT_APP(MainApplication)


// Graphical library initialization

bool MainApplication::OnInit()
{
  wxString dbHostname;
  wxString dbUser;
  wxString dbPassword;

  // Initialize app

  App::Init();


  // Locale

  //wxLocale locale;

  //locale.Init(wxLANGUAGE_ENGLISH_US);


  // Load the image handlers

  wxInitAllImageHandlers();
  
  wxBitmap::InitStandardHandlers();

  // Logger

  wxLog *logger = new wxLogStream(&std::cout);


  wxLog::SetActiveTarget(logger);

  //wxLogMessage(wxT("Iniciando app"));


  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("Aplicação iniciada em ") + wxDateTime::Now().Format(wxT("%d/%m/%Y/ às %H:%M:%S")) + wxT("."), LogMessage::MSG_INFO);

  // Configuration

  Config::Init();

  Config::Create();

  // Loading configuration

  Config::Load();

  // Internationalization

  Lang::Init();

  // Load language

  Lang::Load();

  // Monetary configuration

  Monetary::Init();


  DB::GetInstance();


  int rc = DB::Connect();

  if (rc == 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Banco de dados conectado"), LogMessage::MSG_INFO);
  }


  Config::LoadServer();


  if (Config::Get("db:setuponinit").Cmp("true") == 0) {
    DB::Setup();
  }


  ConfigServer::Load();


  MainFrame *frame = new MainFrame(Lang::Get("App_description"));

  frame->Show(TRUE);

  SetTopWindow(frame);


  frame->Raise();


  return true;
}