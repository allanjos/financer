/**
 * \brief Accounts main dialog.
 *
 * \file  dlg_account_main.cpp
 * \version 1.0
 * \since 14/11/2008
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "gui.hpp"
#include "panel_account_new.hpp"
#include "panel_account_edit.hpp"
#include "panel_account_main.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/new_16x16.xpm"


#include "wx/mstream.h"


// Account main dialog

PanelAccountMain::PanelAccountMain(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  /*
  char *buffer;

  buffer = (char *) malloc(sizeof(10 * sizeof(char)));

  wxMemoryInputStream *memoryOutputStream = new wxMemoryInputStream(buffer, 10);
  */


  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_ACCOUNT_MAIN_TOOL_NEW_ID,
                   Lang::Get("New account"),
                   wxBitmap(new_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_ACCOUNT_MAIN_TOOL_REFRESH_ID,
                   Lang::Get("Refresh"),
                   wxBitmap(refresh_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Accounts listing

  // Data view control for account listing

  dataViewAccounts = new wxDataViewCtrl(this,
                                        DLG_ACCOUNT_MAIN_DATAVIEW_ACCOUNTS_ID,
                                        wxDefaultPosition,
                                        wxSize(100, 100),
                                        wxDV_SINGLE | wxDV_ROW_LINES | wxDV_VERT_RULES);

  szTop->Add(dataViewAccounts, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Data model

  dataViewModelAccounts = new AccountDataModel();

  dataViewAccounts->AssociateModel(dataViewModelAccounts.get());


  wxDataViewTextRendererAttr *textRendererAttrAccountName = new wxDataViewTextRendererAttr;

  wxDataViewColumn *dataViewColumnAccountName = new wxDataViewColumn(Lang::Get("Accounts"),
                                                                     textRendererAttrAccountName,
                                                                     0,
                                                                     400,
                                                                     wxALIGN_LEFT,
                                                                     wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
  dataViewAccounts->AppendColumn(dataViewColumnAccountName);

  // Account type

  wxDataViewTextRendererAttr *textRendererAccountType = new wxDataViewTextRendererAttr;

  wxDataViewColumn *dataViewColumnType = new wxDataViewColumn(Lang::Get("Type"),
                                                              textRendererAccountType,
                                                              1,
                                                              150,
                                                              wxALIGN_LEFT,
                                                              wxDATAVIEW_COL_SORTABLE|wxDATAVIEW_COL_REORDERABLE|wxDATAVIEW_COL_RESIZABLE);
  dataViewAccounts->AppendColumn(dataViewColumnType);

  // Account balance

  wxDataViewTextRendererAttr *textRendererAccountBalance = new wxDataViewTextRendererAttr;

  wxDataViewColumn *dataViewColumnBalance = new wxDataViewColumn(Lang::Get("Balance"),
                                                                 textRendererAccountBalance,
                                                                 2,
                                                                 150,
                                                                 wxALIGN_LEFT,
                                                                 wxDATAVIEW_COL_SORTABLE|wxDATAVIEW_COL_REORDERABLE|wxDATAVIEW_COL_RESIZABLE);
  dataViewAccounts->AppendColumn(dataViewColumnBalance);


  dataViewAccounts->SelectAll();



  AccountDataModelNode *dataViewItemRoot = dataViewModelAccounts->GetRoot();


  ExpandAccountDataViewCtrl(dataViewItemRoot);
}


PanelAccountMain::~PanelAccountMain()
{
  //dataViewModelAccounts->DecRef();
}


void PanelAccountMain::Terminate()
{
  Close();

  Destroy();
}


void PanelAccountMain::ExpandAccountDataViewCtrl(AccountDataModelNode *dataViewItem)
{
  for (size_t itemIndex = 0; itemIndex < dataViewItem->GetChildren().GetCount(); itemIndex++) {
    dataViewAccounts->Select((wxDataViewItem&) dataViewItem->GetChildren()[itemIndex]);

    wxDataViewItem item = dataViewAccounts->GetSelection();

    dataViewAccounts->Expand(item);

    dataViewAccounts->Unselect((wxDataViewItem&) dataViewItem->GetChildren()[itemIndex]);

    ExpandAccountDataViewCtrl(dataViewItem->GetChildren()[itemIndex]);
  }
}


void PanelAccountMain::OnNewDialogRequest(wxCommandEvent& event)
{
  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageCreate(&panelAccountNew, Lang::Get("New ledger account"), true);
}


void PanelAccountMain::OnKeyChar(wxKeyEvent& event)
{
  // CTRL-N - New item
  if (event.ControlDown() && event.GetKeyCode() == 14) {

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->PageCreate(&panelAccountNew, Lang::Get("New ledger account"), true);
  }
  else {
    event.Skip();
  }
}

void PanelAccountMain::OnKeyUp(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("%d"), event.GetKeyCode()));

  if (event.GetKeyCode() == 315 || event.GetKeyCode() == 317) {
    //listCtrlReport->SetFocus();

    //listCtrlReport->GetEventHandler()->ProcessEvent(event);
  }

  event.Skip();
}


void PanelAccountMain::OnRefreshRequest(wxCommandEvent &event)
{
  LoadList();
}


void PanelAccountMain::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == 341) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->PageCreate(&panelAccountNew, Lang::Get("New ledger account"), true);

    LoadList();
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


int PanelAccountMain::LoadList()
{
  // Reload the account data view model

  dataViewModelAccounts->LoadData();

  dataViewAccounts->AssociateModel(dataViewModelAccounts.get());

  // Expand data tree

  AccountDataModelNode *dataViewItemRoot = dataViewModelAccounts->GetRoot();

  ExpandAccountDataViewCtrl(dataViewItemRoot);

  return 0;
}


void PanelAccountMain::OnAccountDataViewItemActivated(wxDataViewEvent &event)
{
  if (event.GetItem().GetID() == dataViewModelAccounts->m_root) {
    return;
  }

  int accountId = dataViewModelAccounts->GetAccountId(event.GetItem());

  if (accountId <= 0) {
    return;
  }

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString title = Lang::Get("Ledger account") + wxT(" #") + wxString::Format("%d", accountId);

  if (mainFrame->PageIsOpened(title, true)) {
    return;
  }

  mainFrame->PageCreate(&panelAccountEdit, title, true);

  panelAccountEdit->LoadData(accountId);
}


BEGIN_EVENT_TABLE(PanelAccountMain, wxScrolledWindow)
  EVT_CHAR(PanelAccountMain::OnKeyChar)
  EVT_KEY_DOWN(PanelAccountMain::OnKeyUp)
  EVT_TOOL(DLG_ACCOUNT_MAIN_TOOL_NEW_ID, PanelAccountMain::OnNewDialogRequest)
  EVT_TOOL(DLG_ACCOUNT_MAIN_TOOL_REFRESH_ID, PanelAccountMain::OnRefreshRequest)
  EVT_CHAR_HOOK(PanelAccountMain::OnCharHook)
  EVT_DATAVIEW_ITEM_ACTIVATED(DLG_ACCOUNT_MAIN_DATAVIEW_ACCOUNTS_ID, PanelAccountMain::OnAccountDataViewItemActivated)
END_EVENT_TABLE()