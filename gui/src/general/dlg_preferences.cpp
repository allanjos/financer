/**
 *
 * \file  dlg_conf_currency.cpp
 * \version 1.0
 * \since 08/22/2007 00:50:55 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "utils_monetary.hpp"
#include "utils_config.hpp"
#include "dlg_preferences.hpp"
#include "dlg_main.hpp"
#include "pixmaps/config_16x16.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


PreferencesDialog::PreferencesDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           Lang::Get("Preferences"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  SetAutoLayout(true);

  SetSize(wxSize(600, 550));

  SetIcon(wxIcon(config_16x16_xpm));


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);

  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_CONFIG_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_CONFIG_TOOL_CLOSE_ID, Lang::Get("Close"), wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szTop->AddSpacer(3);


  wxNotebook *notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, GUI_AUI_NOTEBOOK_STYLE);

  szTop->Add(notebook, 1, wxALL|wxGROW|wxEXPAND, 0);

  // Geral configuration panel

  wxScrolledWindow *panelConfigGeneral = new wxScrolledWindow(notebook,
                                                              wxID_ANY,
                                                              wxDefaultPosition,
                                                              wxDefaultSize,
                                                              wxTAB_TRAVERSAL | wxHSCROLL | wxVSCROLL);

  notebook->AddPage(panelConfigGeneral, Lang::Get("General"), false);


  wxBoxSizer *szPanelGeneralMargin = new wxBoxSizer(wxVERTICAL);
  panelConfigGeneral->SetSizer(szPanelGeneralMargin);

  wxBoxSizer *szPanelGeneral = new wxBoxSizer(wxVERTICAL);
  szPanelGeneralMargin->Add(szPanelGeneral, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Application language

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szPanelGeneral->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  wxStaticText *label = new wxStaticText(panelConfigGeneral, wxID_STATIC, Lang::Get("Language") + wxT(": "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxLanguage = new wxChoice(panelConfigGeneral, wxID_ANY);
  szRow->Add(cboxLanguage, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxLanguage->Append("English", (wxClientData *) new ContainerIntData(0));

  cboxLanguage->SetSelection(0);


  // Database configuration panel

  wxScrolledWindow *panelConfigDB = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxHSCROLL|wxVSCROLL);

  notebook->AddPage(panelConfigDB, Lang::Get("Database"), false);


  wxBoxSizer *szPanelDbMargin = new wxBoxSizer(wxVERTICAL);

  panelConfigDB->SetSizer(szPanelDbMargin);

  panelConfigDB->SetScrollbars(1, 1, 20, 20);


  wxBoxSizer *szPanelDb = new wxBoxSizer(wxVERTICAL);
  szPanelDbMargin->Add(szPanelDb, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Margin

  szDatabaseInfo = new wxBoxSizer(wxVERTICAL);
  szPanelDb->Add(szDatabaseInfo, 1, wxALL | wxEXPAND | wxGROW, 0);


  // Database manager

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szDatabaseInfo->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  label = new wxStaticText(panelConfigDB, wxID_STATIC, Lang::Get("Manager") + wxT(": "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  wxString databaseManagersList[] = {wxT("SQLite"), wxT("PostgreSQL")};

  cboxDbManager = new wxChoice(panelConfigDB,
                               wxID_ANY,
                               wxDefaultPosition,
                               wxDefaultSize,
                               WXSIZEOF(databaseManagersList),
                               databaseManagersList);
  szRow->Add(cboxDbManager, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxDbManager->SetSelection(0);

  cboxDbManager->Connect(wxEVT_CHOICE,
                         wxCommandEventHandler(PreferencesDialog::OnDatabaseTypeRequest),
                         NULL,
                         this);

  szDatabaseInfo->AddSpacer(10);

  // PostgreSQL

  // Server IP

  szgDbPostgreSQLFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szDatabaseInfo->Add(szgDbPostgreSQLFields, 0, wxALL | wxGROW, 0);

  label = new wxStaticText(panelConfigDB, wxID_STATIC, Lang::Get("Server IP") + wxT(": "));
  szgDbPostgreSQLFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditServerIP = new wxTextCtrl(panelConfigDB, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgDbPostgreSQLFields->Add(teditServerIP, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Server port

  label = new wxStaticText(panelConfigDB, wxID_STATIC, Lang::Get("Port") + wxT(": "));
  szgDbPostgreSQLFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditServerDbPort = new wxTextCtrl(panelConfigDB, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1));
  szgDbPostgreSQLFields->Add(teditServerDbPort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Database name

  label = new wxStaticText(panelConfigDB, wxID_STATIC, Lang::Get("Database name") + wxT(": "));
  szgDbPostgreSQLFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditServerDbName = new wxTextCtrl(panelConfigDB, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(130, -1));
  szgDbPostgreSQLFields->Add(teditServerDbName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Database SSL mode

  label = new wxStaticText(panelConfigDB, wxID_STATIC, wxT("SSL: "));
  szgDbPostgreSQLFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  wxString sslModeList[] = {wxT("require"), wxT("prefer"), wxT("disable")};

  cboxServerDbSSLMode = new wxChoice(panelConfigDB,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           WXSIZEOF(sslModeList),
                                           sslModeList);
  szgDbPostgreSQLFields->Add(cboxServerDbSSLMode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Server user

  label = new wxStaticText(panelConfigDB, wxID_STATIC, wxT("Login: "));
  szgDbPostgreSQLFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditServerDbLogin = new wxTextCtrl(panelConfigDB, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(130, -1));
  szgDbPostgreSQLFields->Add(teditServerDbLogin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Server senha

  label = new wxStaticText(panelConfigDB, wxID_STATIC, Lang::Get("Password") + wxT(": "));
  szgDbPostgreSQLFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditServerDbPasswd = new wxTextCtrl(panelConfigDB,
                                               wxID_ANY,
                                               wxT(""),
                                               wxDefaultPosition,
                                               wxSize(125, -1),
                                               wxTE_PASSWORD);
  szgDbPostgreSQLFields->Add(teditServerDbPasswd, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // SQLite

  // Database file path

  szgDbSQLiteFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szDatabaseInfo->Add(szgDbSQLiteFields, 0, wxALL | wxGROW, 0);

  label = new wxStaticText(panelConfigDB, wxID_STATIC, Lang::Get("Database file") + wxT(": "));
  szgDbSQLiteFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditServerDbFilePath = new wxTextCtrl(panelConfigDB, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgDbSQLiteFields->Add(teditServerDbFilePath, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxGROW, 1);

  szgDbSQLiteFields->AddGrowableCol(1);

  // Button - Reconnect

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szDatabaseInfo->Add(szRow, 0, wxALL | wxEXPAND, 0);

  wxButton *buttonDBConnect = new wxButton(panelConfigDB,
                                           wxID_ANY,
                                           Lang::Get("Reconnect"));
  szRow->Add(buttonDBConnect, 0, wxTOP, 5);

  szDatabaseInfo->AddSpacer(10);

  // Checkbox to inform if the database must be setup on app initialization

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szDatabaseInfo->Add(szRow, 0, wxALL | wxEXPAND, 0);

  checkBoxSetupOnInit = new wxCheckBox(panelConfigDB, wxID_ANY, Lang::Get("Setup database on application initialization"));

  szRow->Add(checkBoxSetupOnInit, 0, wxTOP, 5);

  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szTop->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);


  // Top sizer ending configuration.

  this->CenterOnParent();


  LoadData();

  buttonDBConnect->Connect(wxEVT_BUTTON,
                           wxCommandEventHandler(PreferencesDialog::OnDatabaseReconnectRequest),
                           NULL,
                           this);
}


void PreferencesDialog::Terminate()
{
  Close();

  Destroy();
}


int PreferencesDialog::LoadData()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  LoadLanguages();

  Config::Load();

  UpdateDbFields();

  messenger->Register(Lang::Get("Configuration file data was loaded") + wxT("."), LogMessage::MSG_INFO);

  teditServerIP->SetValue(Config::Get("db:host"));

  teditServerDbPort->SetValue(Config::Get("db:port"));

  teditServerDbLogin->SetValue(Config::Get("user"));

  teditServerDbPasswd->SetValue(Config::Get("db:password"));

  teditServerDbName->SetValue(Config::Get("db:dbname"));

  cboxServerDbSSLMode->SetStringSelection(Config::Get("db:sslmode"));

  teditServerDbFilePath->SetValue(Config::Get("db:filepath"));

  if (Config::Get("db:setuponinit").Cmp("false") == 0) {
    checkBoxSetupOnInit->SetValue(false);
  }
  else {
    checkBoxSetupOnInit->SetValue(true);
  }

  for (unsigned int i = 0; i < cboxDbManager->GetCount(); i++) {
    if (cboxDbManager->GetString(i).Cmp(Config::Get("db:type")) == 0) {
      cboxDbManager->SetSelection(i);
    }
  }

  for (unsigned int i = 0; i < cboxLanguage->GetCount(); i++) {
    if (cboxLanguage->GetString(i).Cmp(Config::Get("lang")) == 0) {
      cboxLanguage->SetSelection(i);
    }
  }

  return 0;
}


void PreferencesDialog::LoadLanguages()
{
  HashStringMap::iterator it;

  for (it = Lang::hashMapLangFiles.begin(); it != Lang::hashMapLangFiles.end(); ++it) {
    wxString langName = it->first, langFilePath = it->second;

    if (langName.Cmp(Lang::Default) == 0) {
      continue;
    }

    cboxLanguage->Append(langName, (wxClientData *) new ContainerIntData(0));
  }
}


void PreferencesDialog::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (!wxFileName::FileExists(Config::GetFilePath())) {
    messenger->Register(Lang::Get("Configuration file does not exist") + wxT("."), LogMessage::MSG_WARN);
  }

  if (!doc.Load(Config::GetFilePath())) {
    messenger->Register(Lang::Get("Cannot open configuration file") + Config::GetFilePath() + wxT("."), LogMessage::MSG_WARN);
  }

  // Start processing the XML file
  if (doc.GetRoot()->GetName() != wxT("config")) {
    messenger->Register(Lang::Get("Configuration file is not valid") + wxT("."), LogMessage::MSG_ERROR);
  }


  statusBar->SetStatusText(Lang::Get("Configuration file data was loaded") + wxT("."));


  ModifyContent();


  if (doc.Save(Config::GetFilePath())) {
    statusBar->SetStatusText(Lang::Get("Configuration is saved") + wxT("."));

    Lang::Load();

    Config::Load();
  }
  else {
    messenger->Register(Lang::Get("Cannot write configuration in the configuration file") + wxT("."), LogMessage:: MSG_ERROR);
  }
}


int PreferencesDialog::ModifyContent()
{
  wxXmlNode *root = doc.DetachRoot();

  // @todo Delete root from memory

  wxXmlNode *nodeRoot = new wxXmlNode(wxXML_ELEMENT_NODE, "config");


  // Database node

  wxXmlNode *nodeDB = new wxXmlNode(wxXML_ELEMENT_NODE, "database");

  nodeDB->AddAttribute(wxT("setuponinit"), checkBoxSetupOnInit->GetValue() ? "true" : "false");

  // Database connection node

  wxXmlNode *nodeDbConnection = new wxXmlNode(wxXML_ELEMENT_NODE, "connection");

  nodeDbConnection->AddAttribute(wxT("host"), teditServerIP->GetValue());
  nodeDbConnection->AddAttribute(wxT("port"), teditServerDbPort->GetValue());
  nodeDbConnection->AddAttribute(wxT("dbname"), teditServerDbName->GetValue());
  nodeDbConnection->AddAttribute(wxT("user"), teditServerDbLogin->GetValue());
  nodeDbConnection->AddAttribute(wxT("password"), teditServerDbPasswd->GetValue());
  nodeDbConnection->AddAttribute(wxT("sslmode"), cboxServerDbSSLMode->GetStringSelection());
  nodeDbConnection->AddAttribute(wxT("filepath"), teditServerDbFilePath->GetValue());

  if (cboxDbManager->GetSelection() != wxNOT_FOUND)
    nodeDbConnection->AddAttribute(wxT("type"),
                                   cboxDbManager->GetString(cboxDbManager->GetSelection()));
  else
    nodeDbConnection->AddAttribute(wxT("type"), wxT(""));

  nodeDB->AddChild(nodeDbConnection);

  nodeRoot->AddChild(nodeDB);


  // Language information node

  wxXmlNode *nodeLang = new wxXmlNode(wxXML_ELEMENT_NODE, "language");

  if (cboxLanguage->GetSelection() != wxNOT_FOUND)
    nodeLang->AddAttribute(wxT("id"),
                           cboxLanguage->GetString(cboxLanguage->GetSelection()));
  else
    nodeLang->AddAttribute(wxT("id"), wxT(""));

  nodeRoot->AddChild(nodeLang);


  // Set root node

  doc.SetRoot(nodeRoot);

  delete root;

  return 0;
}


void PreferencesDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


void PreferencesDialog::OnDatabaseReconnectRequest(wxCommandEvent &event)
{
  if (DB::IsConnected()) {
    DB::Disconnect();

    statusBar->SetStatusText(Lang::Get("Database connection was terminated") + wxT("."));
  }

  int rc = DB::Connect(teditServerIP->GetValue(),
                       teditServerDbLogin->GetValue(),
                       teditServerDbPasswd->GetValue(),
                       teditServerDbName->GetValue(),
                       teditServerDbPort->GetValue(),
                       cboxServerDbSSLMode->GetStringSelection());

  if (rc != 0) {
    statusBar->SetStatusText(Lang::Get("Could not connect to the database") + wxT("."));

    DB::Disconnect();
  }
  else {
    statusBar->SetStatusText(Lang::Get("Database connection was redone") + wxT("."));
  }
}


void PreferencesDialog::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Terminate();
  }

  event.Skip();
}


void PreferencesDialog::UpdateDbFields()
{
  if (cboxDbManager->GetString(cboxDbManager->GetSelection()).Cmp("PostgreSQL") == 0) {
    szDatabaseInfo->Show(szgDbPostgreSQLFields);

    szDatabaseInfo->Hide(szgDbSQLiteFields);
  }
  else {
    szDatabaseInfo->Hide(szgDbPostgreSQLFields);

    szDatabaseInfo->Show(szgDbSQLiteFields);
  }

  szDatabaseInfo->Layout();
}


void PreferencesDialog::OnDatabaseTypeRequest(wxCommandEvent& event)
{
  UpdateDbFields();
}


BEGIN_EVENT_TABLE(PreferencesDialog, wxDialog)
  EVT_TOOL(DLG_CONFIG_TOOL_SAVE_ID, PreferencesDialog::OnSaveRequest)
  EVT_TOOL(DLG_CONFIG_TOOL_CLOSE_ID, PreferencesDialog::OnCloseRequest)
  EVT_CHAR_HOOK(PreferencesDialog::OnCharHook)
END_EVENT_TABLE()