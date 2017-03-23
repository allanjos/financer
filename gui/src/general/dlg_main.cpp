/**
 * Application main window.
 *
 * \file  dlg_main.cpp
 * \version 1.0
 * \since 10/27/2006 04:41:31 PM AMT
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <sys/types.h>
#include <sys/stat.h>

#include "utils_include.hpp"
#include "utils_filesystem.hpp"
#include "utils_parser_xml.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"

#include "dlg_main.hpp"
#include "app.hpp"
#include "dlg_preferences.hpp"
#include "dlg_config_server.hpp"
#include "dlg_about.hpp"
//#include "panel_locality_main.hpp"

// Panels
#include "panel_currency.hpp"
#include "panel_account_edit.hpp"
#include "panel_account_main.hpp"
#include "panel_income_expense.hpp"
#include "panel_expense_edit.hpp"
#include "panel_person_main.hpp"
#include "panel_tax_main.hpp"
#include "panel_estate_main.hpp"
#include "panel_locality_main.hpp"
#include "panel_bank_main.hpp"
#include "panel_guide.hpp"
#include "panel_owner_edit.hpp"

#include "pixmaps/account_chart_16x16.xpm"
#include "pixmaps/accounting_16x16.xpm"
#include "pixmaps/backup_16x16.xpm"
#include "pixmaps/bank_16x16.xpm"
#include "pixmaps/coins_16x16.xpm"
#include "pixmaps/config_16x16.xpm"
#include "pixmaps/contacts_16x16.xpm"
#include "pixmaps/estate_16x16.xpm"
#include "pixmaps/fullscreen_16x16.xpm"
#include "pixmaps/help_16x16.xpm"
#include "pixmaps/icon.xpm"
#include "pixmaps/income_16x16.xpm"
#include "pixmaps/info_16x16.xpm"
#include "pixmaps/owner_16x16.xpm"
#include "pixmaps/preferences_16x16.xpm"
#include "pixmaps/quit_16x16.xpm"
#include "pixmaps/report_16x16.xpm"
#include "pixmaps/refresh_16x16.xpm"


using namespace std;


#define IMG_INDEX_FOLDER       0
#define IMG_INDEX_FILE         1
#define IMG_INDEX_FILE_AUDIO   2
#define IMG_INDEX_FILE_LOG     3
#define IMG_INDEX_FILE_CONF    4


void MonitorTimer::SetFrame(MainFrame *frame)
{
  this->frame = frame;
}


void MonitorTimer::Notify()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  wxDateTime dateTimeNow = wxDateTime::Now();

  messenger->Register(wxT("Monitor executado em ") + dateTimeNow.Format(wxT("%d/%m/%Y/ às %H:%M:%S")) + wxT("."), LogMessage::MSG_INFO);
}


/**
 * Main frame window creation.
 */
MainFrame::MainFrame(const wxString& title):
  wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxFULL_REPAINT_ON_RESIZE)
{
  isShowed = false;
  isCreated = false;
  //panelSummary = NULL;
  panelAccounting = NULL;
  panelIncomeExpense = NULL;

  notebook = NULL;

  logViewDialog = NULL;


  SetAutoLayout(true);


  wxFileSystem::AddHandler(new wxZipFSHandler);

  // Window size

  /*
  int width = 1024, height = 768;

  wxSize screenSize = wxGetDisplaySize();

  if (screenSize.GetHeight() < 800) {
    height = ceil((float) screenSize.GetHeight() * 0.9);
  }

  width = ceil((float) screenSize.GetWidth() * 0.8);
  */

  int x, y, width, height;

  wxClientDisplayRect(&x, &y, &width, &height);

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxString::Format(wxT("screen x=%d, y=%d, width=%d, height=%d"), x, y, width, height));

  if (width > 1024) {
    width = 1024;
  }
  else {
    width -= 100;
  }

  if (height >= 900) {
    height = 850;
  }
  else {
    height -= 100;
  }

  messenger->Register(wxString::Format(wxT("frame width=%d, height=%d"), width, height));

  SetSize(wxSize(width, height));

  // Window position

  Centre();

  //currentDirectory = wxGetCwd();

  SetIcon(wxIcon(icon_xpm));

  // Main menu

  wxMenu *mainMenu = new wxMenu;

  // Local configuration

  wxMenuItem *mnItemConfigLocal = new wxMenuItem(mainMenu,
                                                 DLG_MAIN_MENU_CONFIG_LOCAL_ID,
                                                 Lang::Get("Preferences") + wxT("\tCTRL-P"));

  mnItemConfigLocal->SetBitmap(wxBitmap(preferences_16x16_xpm));

  mainMenu->Append(mnItemConfigLocal);

  // Fullscreen mode

  wxMenuItem *mnItemFullscreen = new wxMenuItem(mainMenu, DLG_MAIN_MENU_FULLSCREEN_ID, Lang::Get("Fullscreen") + wxT("\tF11"));

  mnItemFullscreen->SetBitmap(wxBitmap(fullscreen_16x16_xpm));

  mainMenu->Append(mnItemFullscreen);

  // Backup

  wxMenuItem *mnItemBackup = new wxMenuItem(mainMenu, wxID_ANY, Lang::Get("Backup") + wxT("\tCTRL-B"));

  mnItemBackup->SetBitmap(wxBitmap(backup_16x16_xpm));

  mainMenu->Append(mnItemBackup);

  // Main menu - Log viewer

  wxMenuItem *mnItemLogView = new wxMenuItem(mainMenu, DLG_MAIN_MENU_LOG_VIEW_ID, Lang::Get("Execution messages") + wxT("\tF12"));

  mnItemLogView->SetBitmap(wxBitmap(report_16x16_xpm));

  mainMenu->Append(mnItemLogView);

  // Main menu - Log viewer

  wxMenuItem *mnItemQuit = new wxMenuItem(mainMenu, wxID_EXIT, Lang::Get("Close_application") + wxT("\tCTRL-Q"));

  mnItemQuit->SetBitmap(wxBitmap(quit_16x16_xpm));

  mainMenu->Append(mnItemQuit);


  // Menu - Modules

  wxMenu *modulesMenu = new wxMenu;

  // General configuration

  wxMenuItem *mnItemConfigGeneral = new wxMenuItem(modulesMenu,
                                                   DLG_MAIN_MENU_CONFIG_GENERAL_ID,
                                                   Lang::Get("General configuration"));
  mnItemConfigGeneral->SetBitmap(wxBitmap(config_16x16_xpm));
  modulesMenu->Append(mnItemConfigGeneral);

  modulesMenu->AppendSeparator();

  // Enterprise module

  wxMenuItem *mnItemModulesEnterprise = new wxMenuItem(modulesMenu,
                                                       DLG_MAIN_MENU_MODULES_ENTERPRISE_ID,
                                                       Lang::Get("Owner") + wxT("\tCTRL-F2"));
  mnItemModulesEnterprise->SetBitmap(wxBitmap(owner_16x16_xpm));
  modulesMenu->Append(mnItemModulesEnterprise);

  modulesMenu->AppendSeparator();

  // People / Contacts

  wxMenuItem *mnItemModulesPeople = new wxMenuItem(modulesMenu,
                                                   DLG_MAIN_MENU_MODULES_CONTACTS_MAIN_ID,
                                                   Lang::Get("Contacts") + wxT("\tCTRL-F3"));
  mnItemModulesPeople->SetBitmap(wxBitmap(contacts_16x16_xpm));
  modulesMenu->Append(mnItemModulesPeople);

  modulesMenu->AppendSeparator();

  // Estate module

  wxMenuItem *mnItemModulesEstate = new wxMenuItem(modulesMenu,
                                                   DLG_MAIN_MENU_MODULES_ESTATE_ID,
                                                   Lang::Get("Estate") + wxT("\tCTRL-F4"));
  mnItemModulesEstate->SetBitmap(wxBitmap(estate_16x16_xpm));
  modulesMenu->Append(mnItemModulesEstate);

  modulesMenu->AppendSeparator();

  // Accounting summary module

  /*
  wxMenuItem *mnItemModulesAccountingSummary = new wxMenuItem(configMenu,
                                                              DLG_MAIN_MENU_MODULES_ACCOUNTING_SUMMARY_ID,
                                                              TAB_TITLE_FINANCIAL_SUMMARY wxT("\tF5"),
                                                              wxT("Visualização das informações básicas de contabilidade."));
  mnItemModulesAccountingSummary->SetBitmap(wxBitmap(chart_line_16x16_xpm));
  modulesMenu->Append(mnItemModulesAccountingSummary);
  */

  // Chart of accounts

  wxMenuItem *mnItemModulesAccountsChart = new wxMenuItem(modulesMenu,
                                                          DLG_MAIN_MENU_MODULES_ACCOUNTS_CHART_ID,
                                                          Lang::Get("Accounting") + wxT("\tCTRL-F5"));
  mnItemModulesAccountsChart->SetBitmap(wxBitmap(account_chart_16x16_xpm));
  modulesMenu->Append(mnItemModulesAccountsChart);

  // Banks

  wxMenuItem *mnItemModulesBankAccount = new wxMenuItem(modulesMenu,
                                                        DLG_MAIN_MENU_MODULES_BANKS_MAIN_ID,
                                                        Lang::Get("Banks") + wxT("\tCTRL-F6"));
  mnItemModulesBankAccount->SetBitmap(wxBitmap(bank_16x16_xpm));
  modulesMenu->Append(mnItemModulesBankAccount);

  // Chart of accounts

  wxMenuItem *mnItemModulesIncomesExpenses = new wxMenuItem(modulesMenu,
                                                            DLG_MAIN_MENU_MODULES_INCOMES_EXPENSE_MAIN_ID,
                                                            Lang::Get("Incomes and expenses") + wxT("\tCTRL-F7"));
  mnItemModulesIncomesExpenses->SetBitmap(wxBitmap(income_16x16_xpm));
  modulesMenu->Append(mnItemModulesIncomesExpenses);

  // Economy - Currency

  wxMenuItem *mnItemModulesCurrency = new wxMenuItem(modulesMenu,
                                                     DLG_MAIN_MENU_CONF_CURRENCY_ID,
                                                     Lang::Get("Currencies") + wxT("\tCTRL-F8"));
  mnItemModulesCurrency->SetBitmap(wxBitmap(coins_16x16_xpm));
  modulesMenu->Append(mnItemModulesCurrency);

  /*
  // Document management

  modulesMenu->AppendSeparator();

  wxMenuItem *mnItemModulesDocuments = new wxMenuItem(modulesMenu,
                                                      DLG_MAIN_MENU_MODULES_DOCUMENTS_MAIN_ID,
                                                      wxT("Documentos\tALT-F3"),
                                                      wxT("Gerenciamento de fluxo de documentos físicos e virtuais"));
  mnItemModulesDocuments->SetBitmap(wxBitmap(document_16x16_xpm));
  modulesMenu->Append(mnItemModulesDocuments);
  */

  /*
  // Localities

  modulesMenu->AppendSeparator();

  wxMenuItem *mnItemConfigLocality = new wxMenuItem(modulesMenu,
                                                    DLG_MAIN_MENU_CONF_LOCALITY_ID,
                                                    wxT("&Localidades"),
                                                    wxT("Gerências de localidades (cidades, estados, países)"));
  mnItemConfigLocality->SetBitmap(wxBitmap(globe_16x16_xpm));
  modulesMenu->Append(mnItemConfigLocality);
  */

  // Menu - Information, help, update

  wxMenu *helpMenu = new wxMenu;

  wxMenuItem *mnItemHelpMain = new wxMenuItem(helpMenu,
                                              DLG_MAIN_MENU_HELP_MAIN_ID,
                                              Lang::Get("Guide") + wxT("\tF1"));
  mnItemHelpMain->SetBitmap(wxBitmap(help_16x16_xpm));
  helpMenu->Append(mnItemHelpMain);

  wxMenuItem *mnItemUpdate = new wxMenuItem(helpMenu,
                                            DLG_MAIN_MENU_UPDATE_MAIN_ID,
                                            Lang::Get("Check update") + wxT("\tCTRL-U"));
  mnItemUpdate->SetBitmap(wxBitmap(refresh_16x16_xpm));
  helpMenu->Append(mnItemUpdate);

  wxMenuItem *mnItemHelpAbout = new wxMenuItem(helpMenu,
                                               wxID_ABOUT,
                                               Lang::Get("About_application") + wxT("\tCTRL-H"));
  mnItemHelpAbout->SetBitmap(wxBitmap(info_16x16_xpm));
  helpMenu->Append(mnItemHelpAbout);


  // Menu bar

  wxMenuBar *menuBar = new wxMenuBar();

  menuBar->Append(mainMenu, "&" + Lang::Get("General"));

  menuBar->Append(modulesMenu, "&" + Lang::Get("Modules"));

  //menuBar->Append(licitacaoMenu, wxT("&Licitações"));

  menuBar->Append(helpMenu, "&" + Lang::Get("Help"));

  SetMenuBar(menuBar);


  // Top sizer

  auto szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);


  // Toolbar

  auto szToolbars = new wxBoxSizer(wxHORIZONTAL);

  szTop->Add(szToolbars, 0, wxALL | wxEXPAND, 0);


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);
  szToolbars->Add(toolBar, 1, wxALL | wxEXPAND, 0);


  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool((int) DLG_MAIN_TOOLBAR_CONFIG_MAIN_ID,
                   wxT(""),
                   wxBitmap(config_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool((int) DLG_MAIN_TOOLBAR_ENTERPRISE_MAIN_ID,
                   Lang::Get("Owner"),
                   wxBitmap(owner_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool((int) DLG_MAIN_TOOLBAR_CONTACTS_MAIN_ID,
                   Lang::Get("Contacts"),
                   wxBitmap(contacts_16x16_xpm));

  toolBar->Connect((int) DLG_MAIN_TOOLBAR_CONTACTS_MAIN_ID,
                   wxEVT_TOOL,
                   wxCommandEventHandler(MainFrame::OnToolbarContactsRequest),
                   NULL,
                   this);

  toolBar->AddSeparator();

  toolBar->AddTool((int) DLG_MAIN_TOOLBAR_ESTATE_ID,
                   Lang::Get("Estate"),
                   wxBitmap(estate_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool((int) DLG_MAIN_TOOLBAR_BANK_ACCOUNTS_ID,
                   Lang::Get("Banking"),
                   wxBitmap(bank_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool((int) DLG_MAIN_TOOLBAR_INCOMES_EXPENSE_MAIN_ID,
                   Lang::Get("Incomes and expenses"),
                   wxBitmap(income_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_MAIN_TOOLBAR_ACCOUNT_MAIN_ID,
                   Lang::Get("Accounting"),
                   wxBitmap(accounting_16x16_xpm));

  toolBar->Realize();


  // Panel to buttons and status

  wxPanel *panelStatus = new wxPanel(this, wxID_ANY);

  panelStatus->SetAutoLayout(true);

  szTop->Add(panelStatus, 0, wxALL|wxGROW, 0);

  // Set sizer

  wxBoxSizer *szPanelStatus = new wxBoxSizer(wxVERTICAL);

  panelStatus->SetSizer(szPanelStatus);

  // Sizer for panel inner controls

  wxBoxSizer *szPanelStatusControls = new wxBoxSizer(wxHORIZONTAL);

  szPanelStatus->Add(szPanelStatusControls, 0, wxALL | wxALIGN_RIGHT, 1);


  // Main notebook

  notebook = new wxAuiNotebook(this,
                               DLG_MAIN_NOTEBOOK_MAIN_ID,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxAUI_NB_DEFAULT_STYLE);

  // Add main notebook to the sizer after panel was inserted

  szTop->Add(notebook, 1, wxALL | wxEXPAND | wxGROW, 0);


  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szTop->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);


  szTop->Layout();
  Layout();
  wxGetApp().GetTopWindow()->Layout();


  // Enable events

  notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler(MainFrame::OnTabClosed));

  isCreated = true;

  /*
  monitorTimer.SetFrame(this);

  monitorTimer.Start(10000);
  */
}


MainFrame::~MainFrame()
{
  monitorTimer.Stop();

  while (monitorTimer.IsRunning()) {
    wxMilliSleep(500);
  }

  /* Close the connection to the database */

  DB::Disconnect();
}


wxStatusBar *MainFrame::GetStatusBar()
{
  return statusBar;
}


void MainFrame::SetStatusText(wxString msg)
{
  statusBar->SetStatusText(msg);
}


/*
 * Event callbacks
 */


/* On tab closed. */
void MainFrame::OnTabClosed(wxAuiNotebookEvent& event)
{
  //LogMessenger *messenger = LogMessenger::GetInstance();

  //messenger->Register(wxString::Format("MainFrame::OnTabClosed() page=%d", event.GetSelection()),
  //                    LogMessage::MSG_INFO);

  /*
  wxAuiNotebook* ctrl = (wxAuiNotebook*) event.GetEventObject();

  int res = wxMessageBox(wxT("Confirma o fechamento da aba " + ctrl->GetPageText(event.GetSelection()) + "?"),
                         wxT("Fechamento de aba"),
                         wxYES_NO,
                         this);
  if (res != wxYES) {
    event.Veto();

    return;
  }
  */

  // Mark panel as closed

  //if (ctrl->GetPage(event.GetSelection()) == panelSummary) {
  //}
}


void MainFrame::OnFullscreenDialogRequest(wxCommandEvent& WXUNUSED(event))
{
  SwitchFullscreen();
}


void MainFrame::OnLogViewDialogRequest(wxCommandEvent& WXUNUSED(event))
{
  if (logViewDialog == NULL) {
    logViewDialog = new LogViewDialog(this);
  }

  if (!logViewDialog->IsShown()) {
    logViewDialog->Show(true);
  }
}

void MainFrame::OnCommandEventEnterpriseRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelOwnerEdit, Lang::Get("Owner"), true);

  panelOwnerEdit->LoadData();
}

void MainFrame::OnCommandEventEstateRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelEstateMain, Lang::Get("Estate"), true);

  panelEstateMain->LoadData();
}

/*
void MainFrame::OnCommandEventAccountingSummaryRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelSummary, TAB_TITLE_FINANCIAL_SUMMARY, true);

  panelSummary->LoadData();
}
*/

void MainFrame::OnCommandEventAccountsChartRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelAccounting, Lang::Get("Accounting"), true);
}

void MainFrame::OnCommandEventBanksRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelBanks, Lang::Get("Banks"), true);
}

void MainFrame::OnCommandEventIncomesExpensesRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelIncomeExpense, Lang::Get("Incomes and expenses"), true);
}

void MainFrame::OnCommandEventPeopleMainRequest(wxCommandEvent& WXUNUSED(event))
{
  PageCreate(&panelPersonMain, Lang::Get("Contacts"), true);
}

void MainFrame::OnConfigLocalDialogRequest(wxCommandEvent& WXUNUSED(event))
{
  PreferencesDialog PreferencesDialog(this);

  PreferencesDialog.ShowModal();
}


void MainFrame::OnConfigGeneralDialogRequest(wxCommandEvent& WXUNUSED(event))
{
  CallAfter(&MainFrame::ShowConfigDialog);
}


void MainFrame::ShowConfigDialog()
{
  ConfigServerDialog dialog(this);

  dialog.ShowModal();
}


void MainFrame::OnHelpRequest(wxCommandEvent& WXUNUSED(event))
{
  /*
  wxString helpDirectory = wxStandardPaths::Get().GetExecutablePath();

  wxFileName executablePathInfo(helpDirectory);

  //wxMessageBox(helpDirectory);

  //wxHelpController *helpController = new wxHelpController;

  PanelGuide helpDialog(this);

  helpDialog.ShowModal();

  //helpController->DisplaySection(wxT("audconfigurator.html"));
  */

  PageCreate(&panelGuide, "Manual", true);
}


void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  AboutDialog aboutDialog(this);

  aboutDialog.ShowModal();
}


void MainFrame::CloseRequest(wxCloseEvent &event)
{
  CloseApplication();
}


void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  CloseApplication();
}


void MainFrame::CloseApplication() {
  Destroy();
}


bool MainFrame::PageIsOpened(wxString title, bool select)
{
  unsigned int pageIndex;

  for (pageIndex = 0; pageIndex < notebook->GetPageCount(); pageIndex++) {
    if (notebook->GetPageText(pageIndex).Cmp(title) == 0) {
      if (select) {
        notebook->SetSelection(pageIndex);
      }

      return true;
    }
  }

  return false;
}


bool MainFrame::PageDelete(wxString title)
{
  unsigned int pageIndex;

  for (pageIndex = 0; pageIndex < notebook->GetPageCount(); pageIndex++) {
    if (notebook->GetPageText(pageIndex).Cmp(title) == 0) {
      notebook->DeletePage(pageIndex);

      return true;
    }
  }

  return false;
}


bool MainFrame::PageRemove(wxString title)
{
  unsigned int pageIndex;

  for (pageIndex = 0; pageIndex < notebook->GetPageCount(); pageIndex++) {
    if (notebook->GetPageText(pageIndex).Cmp(title) == 0) {
      CallAfter(&MainFrame::PageRemoveDelayed, pageIndex);

      return true;
    }
  }

  return false;
}


void MainFrame::PageRemoveDelayed(const unsigned int &pageIndex)
{
  /*
  wxString pageTitle = wxString::Format(wxT("Despesa %d"), this->expenseId);

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageRemove(pageTitle);

  //Show(false);
  */

  notebook->DeletePage(pageIndex);
}


wxWindow *MainFrame::GetPage(wxString title)
{
  unsigned int pageIndex;

  for (pageIndex = 0; pageIndex < notebook->GetPageCount(); pageIndex++) {
    if (notebook->GetPageText(pageIndex).Cmp(title) == 0) {
      return notebook->GetPage(pageIndex);
    }
  }

  return NULL;
}


bool MainFrame::PageSelect(wxString title)
{
  unsigned int pageIndex;

  for (pageIndex = 0; pageIndex < notebook->GetPageCount(); pageIndex++) {
    if (notebook->GetPageText(pageIndex).Cmp(title) == 0) {
      notebook->SetSelection(pageIndex);

      return true;
    }
  }

  return false;
}


void MainFrame::OnAccountMainDialogRequest(wxCommandEvent& event)
{
  PageCreate(&panelAccounting, Lang::Get("Accounting"), true);
}


/*
void MainFrame::OnAccountSummaryDialogRequest(wxCommandEvent& event)
{
  PageCreate(&panelSummary, TAB_TITLE_FINANCIAL_SUMMARY, true);

  panelSummary->LoadData();
}
*/


void MainFrame::OnIncomesExpensesMainDialogRequest(wxCommandEvent& event)
{
  PageCreate(&panelIncomeExpense, Lang::Get("Incomes and expenses"), true);
}


/*
void MainFrame::OnTaxesMainDialogequest(wxCommandEvent& event)
{
  PageCreate(&panelTaxMain, TAB_TITLE_TAXES, true);
}
*/


void MainFrame::OnEstateMainDialogRequest(wxCommandEvent& event) {
  PageCreate(&panelEstateMain, Lang::Get("Estate"), true);

  panelEstateMain->LoadData();
}


void MainFrame::OnEnterpriseMainDialogRequest(wxCommandEvent& event) {
  PageCreate(&panelOwnerEdit, Lang::Get("Owner"), true);

  panelOwnerEdit->LoadData();
}


void MainFrame::OnConfCurrencyDialogRequest(wxCommandEvent& event)
{
  PageCreate(&panelCurrency, Lang::Get("Currencies"), true);
}


/*
void MainFrame::OnConfLocalityDialogRequest(wxCommandEvent& event)
{
  PageCreate(&panelLocalityMain, TABLE_TITLE_LOCALITIES, true);
}
*/


void MainFrame::OnShow(wxShowEvent &event)
{
  isShowed = true;

  event.Skip();
}


bool MainFrame::LayoutWindow()
{
  if (isCreated) {
    //SetStatusText(wxT("MainFrame::OnReSize"));

    /*
    mgr.SetDockSizeConstraint(0.5, -1);

    panelProducts->Layout();
    panelProducts->GetContainingSizer()->Layout();

    panelLicitacao->Layout();
    panelLicitacao->GetContainingSizer()->Layout();

    panelClients->Layout();
    panelClients->GetContainingSizer()->Layout();

    panelSuppliers->Layout();
    panelSuppliers->GetContainingSizer()->Layout();
    */

    if (notebook) {
      notebook->Layout();
    }

    // panelSummary->Layout();


    //Layout();
    wxGetApp().GetTopWindow()->Layout();

    /*
    mgr.GetPane(wxT("panel_products")).MinSize(wxSize(GetSize().GetWidth() / 2 - 20, -1));
    mgr.GetPane(wxT("panel_products")).BestSize(wxSize(GetSize().GetWidth() / 2 - 10, -1));

    mgr.GetPane(wxT("panel_licitacao")).MinSize(wxSize(GetSize().GetWidth() / 2 - 20, -1));
    mgr.GetPane(wxT("panel_licitacao")).BestSize(wxSize(GetSize().GetWidth() / 2 - 20, -1));

    mgr.GetPane(wxT("panel_suppliers")).MinSize(wxSize(GetSize().GetWidth() / 2 - 20, -1));
    mgr.GetPane(wxT("panel_suppliers")).BestSize(wxSize(GetSize().GetWidth() / 2 - 20, -1));

    mgr.GetPane(wxT("panel_clients")).MinSize(wxSize(GetSize().GetWidth() / 2 - 20, -1));
    mgr.GetPane(wxT("panel_clients")).BestSize(wxSize(GetSize().GetWidth() / 2 - 10, -1));

    mgr.Update();

    mgr.GetPane(wxT("panel_products")).MinSize(wxSize(0, -1));
    mgr.GetPane(wxT("panel_licitacao")).MinSize(wxSize(0, -1));
    mgr.GetPane(wxT("panel_suppliers")).MinSize(wxSize(0, -1));
    mgr.GetPane(wxT("panel_clients")).MinSize(wxSize(0, -1));
    */
  }

  return true;
}


void MainFrame::OnReSize(wxSizeEvent &event)
{
  event.Skip();

  LayoutWindow();
}


void MainFrame::OnMaximize(wxMaximizeEvent &event)
{
  LayoutWindow();

  event.Skip();
}


void MainFrame::OnIconize(wxIconizeEvent &event)
{
  //if (!Iconized()) {
  //  wxMessageBox(wxT("MainFrame::OnIconize"));
  //}

  //event.Skip();
}


void MainFrame::OnToolbarContactsRequest(wxCommandEvent& event)
{
  PageCreate(&panelPersonMain, Lang::Get("Contacts"), true);
}

/*
void MainFrame::OnDropDownToolbarCommerce(wxAuiToolBarEvent& event)
{
  if (event.IsDropDownClicked()) {
    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());

    tb->SetToolSticky(event.GetId(), true);

    // create the popup menu
    wxMenu menuPopup;

    wxMenuItem* menuItemCommerceProducts =  new wxMenuItem(&menuPopup,
                                                           DLG_MAIN_TOOLBAR_PRODUCT_MAIN_ID,
                                                           wxT("Produtos"),
                                                           wxT("Produtos"));
    menuItemCommerceProducts->SetBitmap(wxBitmap(product_16x16_xpm));
    menuPopup.Append(menuItemCommerceProducts);

    wxMenuItem* menuItemCommerceSuppliers = new wxMenuItem(&menuPopup,
                                                           DLG_MAIN_TOOLBAR_SUPPLIER_MAIN_ID,
                                                           wxT("Fornecedores"),
                                                           wxT("Fornecedores"));
    menuItemCommerceSuppliers->SetBitmap(wxBitmap(supplier_16x16_xpm));
    menuPopup.Append(menuItemCommerceSuppliers);

    menuPopup.AppendSeparator();

    wxMenuItem* menuItemCommerceInvoice = new wxMenuItem(&menuPopup,
                                                         DLG_MAIN_MENU_INVOICE_MAIN_ID,
                                                         wxT("Notas Fiscais"),
                                                         wxT("Notas Fiscais de compra e venda"));
    menuItemCommerceInvoice->SetBitmap(wxBitmap(purchase_16x16_xpm));
    menuPopup.Append(menuItemCommerceInvoice);

    menuPopup.AppendSeparator();

    wxMenuItem* menuItemCommercePurchaseRequest = new wxMenuItem(&menuPopup,
                                                                 DLG_MAIN_MENU_PURCHASE_REQUEST_MAIN_ID,
                                                                 wxT("Pedidos de compras"),
                                                                 wxT("Pedidos de compras"));
    menuItemCommercePurchaseRequest->SetBitmap(wxBitmap(purchase_16x16_xpm));
    menuPopup.Append(menuItemCommercePurchaseRequest);

    wxMenuItem* menuItemCommercePurchase = new wxMenuItem(&menuPopup,
                                                          DLG_MAIN_MENU_PURCHASE_MAIN_ID,
                                                          wxT("Compras"),
                                                          wxT("Compras"));
    menuItemCommercePurchase->SetBitmap(wxBitmap(purchase_16x16_xpm));
    menuPopup.Append(menuItemCommercePurchase);

    menuPopup.AppendSeparator();

    // Sales requests main dialog

    //wxMenuItem* menuItemCommerceSaleRequest = new wxMenuItem(&menuPopup,
    //                                                         wxID_ANY,
    //                                                         wxT("Pedidos de vendas"),
    //                                                         wxT("Pedidos de vendas"));
    //menuItemCommerceSaleRequest->SetBitmap(wxBitmap(sale_16x16_xpm));
    //menuPopup.Append(menuItemCommerceSaleRequest);

    // Sales main dialog

    wxMenuItem* menuItemCommerceSales = new wxMenuItem(&menuPopup,
                                                       DLG_MAIN_MENU_SALE_MAIN_ID,
                                                       wxT("Vendas"),
                                                       wxT("Vendas"));
    menuItemCommerceSales->SetBitmap(wxBitmap(cashdesk_16x16_xpm));
    menuPopup.Append(menuItemCommerceSales);

    // Point of sale

    wxMenuItem* menuItemCommercePointOfSale = new wxMenuItem(&menuPopup,
                                                             DLG_MAIN_MENU_SALE_POINT_ID,
                                                             wxT("Ponto de Vendas"),
                                                             wxT("Interface de registro de vendas."));
    menuItemCommercePointOfSale->SetBitmap(wxBitmap(scanner_16x16_xpm));
    menuPopup.Append(menuItemCommercePointOfSale);

    wxMenuItem* menuItemCommerceBudget = new wxMenuItem(&menuPopup,
                                                        DLG_MAIN_MENU_SALE_BUDGET_MAIN_ID,
                                                        wxT("Orçamentos"),
                                                        wxT("Orçamentos"));
    menuItemCommerceBudget->SetBitmap(wxBitmap(calculator_16x16_xpm));
    menuPopup.Append(menuItemCommerceBudget);


    // line up our menu with the button

    wxRect rect = tb->GetToolRect(event.GetId());
    wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());

    pt = ScreenToClient(pt);

    PopupMenu(&menuPopup, pt);

    // make sure the button is "un-stuck"
    tb->SetToolSticky(event.GetId(), false);
  }
}
*/


/*
void MainFrame::OnPersonMainDialogRequest(wxCommandEvent &event)
{
  PersonMainDialog personMainDialog(this);

  personMainDialog.ShowModal();
}
*/


void MainFrame::OnNotebookPageClose(wxAuiNotebookEvent& event)
{
}


void MainFrame::SwitchFullscreen()
{
  if (IsFullScreen()) {
    ShowFullScreen(false);
  }
  else {
    ShowFullScreen(true, wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOBORDER);
  }
}


void MainFrame::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F11> - Fullscreen
  if (event.GetKeyCode() == WXK_F11) {
    SwitchFullscreen();
  }
  else {
    event.Skip();
  }
}


// Event table

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_CHAR_HOOK(MainFrame::OnCharHook)

  EVT_AUINOTEBOOK_PAGE_CLOSE(DLG_MAIN_NOTEBOOK_MAIN_ID, MainFrame::OnNotebookPageClose)

  EVT_MENU(DLG_MAIN_MENU_MODULES_ENTERPRISE_ID, MainFrame::OnCommandEventEnterpriseRequest)
  EVT_MENU(DLG_MAIN_MENU_MODULES_ESTATE_ID, MainFrame::OnCommandEventEstateRequest)
  EVT_MENU(DLG_MAIN_MENU_MODULES_ACCOUNTS_CHART_ID, MainFrame::OnCommandEventAccountsChartRequest)
  EVT_MENU(DLG_MAIN_MENU_MODULES_BANKS_MAIN_ID, MainFrame::OnCommandEventBanksRequest)
  EVT_MENU(DLG_MAIN_MENU_MODULES_INCOMES_EXPENSE_MAIN_ID, MainFrame::OnCommandEventIncomesExpensesRequest)
  EVT_MENU(DLG_MAIN_MENU_MODULES_CONTACTS_MAIN_ID, MainFrame::OnCommandEventPeopleMainRequest)
  EVT_MENU(DLG_MAIN_MENU_LOG_VIEW_ID, MainFrame::OnLogViewDialogRequest)
  EVT_MENU(DLG_MAIN_MENU_FULLSCREEN_ID, MainFrame::OnFullscreenDialogRequest)
  EVT_MENU(DLG_MAIN_MENU_CONFIG_LOCAL_ID, MainFrame::OnConfigLocalDialogRequest)
  EVT_MENU(DLG_MAIN_MENU_CONFIG_GENERAL_ID, MainFrame::OnConfigGeneralDialogRequest)
  EVT_MENU(DLG_MAIN_TOOLBAR_ENTERPRISE_MAIN_ID, MainFrame::OnEnterpriseMainDialogRequest)
  EVT_MENU(DLG_MAIN_TOOLBAR_ESTATE_ID, MainFrame::OnEstateMainDialogRequest)
  //EVT_MENU(DLG_MAIN_MENU_MODULES_TAXES_MAIN_ID, MainFrame::OnCommandEventTaxesRequest)
  //EVT_MENU(DLG_MAIN_MENU_MODULES_ACCOUNTING_SUMMARY_ID, MainFrame::OnCommandEventAccountingSummaryRequest)
  EVT_MENU(DLG_MAIN_MENU_CONF_CURRENCY_ID, MainFrame::OnConfCurrencyDialogRequest)
  //EVT_MENU(DLG_MAIN_MENU_CONF_LOCALITY_ID, MainFrame::OnConfLocalityDialogRequest)
  EVT_MENU(DLG_MAIN_MENU_HELP_MAIN_ID, MainFrame::OnHelpRequest)
  EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
  EVT_MENU(wxID_EXIT, MainFrame::OnQuit)

  //EVT_TOOL(DLG_MAIN_TOOLBAR_CONTACTS_MAIN_ID, MainFrame::OnPersonMainDialogRequest)

  EVT_TOOL(DLG_MAIN_TOOLBAR_CONFIG_MAIN_ID, MainFrame::OnConfigGeneralDialogRequest)
  EVT_TOOL(DLG_MAIN_TOOLBAR_ACCOUNT_MAIN_ID, MainFrame::OnAccountMainDialogRequest)
  EVT_TOOL(DLG_MAIN_TOOLBAR_INCOMES_EXPENSE_MAIN_ID, MainFrame::OnIncomesExpensesMainDialogRequest)
  EVT_TOOL(DLG_MAIN_TOOLBAR_BANK_ACCOUNTS_ID, MainFrame::OnCommandEventBanksRequest)
  EVT_TOOL(DLG_MAIN_TOOLBAR_ENTERPRISE_MAIN_ID, MainFrame::OnEnterpriseMainDialogRequest)

  //EVT_MENU(DLG_MAIN_MENU_ACCOUNTING_SUMMARY_ID, MainFrame::OnAccountSummaryDialogRequest)
  //EVT_TOOL(DLG_MAIN_TOOLBAR_INCOMES_MAIN_ID, MainFrame::OnIncomesMainDialogRequest)
  //EVT_TOOL(DLG_MAIN_TOOLBAR_EXPENSES_MAIN_ID, MainFrame::OnExpensesMainDialogRequest)
  //EVT_TOOL(DLG_MAIN_MENU_TAXES_MAIN_ID, MainFrame::OnTaxesMainDialogequest)

  EVT_BUTTON(wxID_EXIT, MainFrame::OnQuit)

  //EVT_ENTER_WINDOW(MainFrame::OnProductsListingMouseEnter)

  EVT_CLOSE(MainFrame::CloseRequest)
  EVT_SHOW(MainFrame::OnShow)
  EVT_SIZE(MainFrame::OnReSize)
  EVT_MAXIMIZE(MainFrame::OnMaximize)
  EVT_ICONIZE(MainFrame::OnIconize)

END_EVENT_TABLE()