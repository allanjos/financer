/**
 * Supplier main dialog.
 *
 * \version 1.0
 * \since 24/09/2009 01:06
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"

#include "panel_bank_main.hpp"
#include "panel_bank_account_new.hpp"
#include "panel_bank_account_edit.hpp"
#include "dlg_bank_list.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "pixmaps/new_16x16.xpm"
#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/search_16x16.xpm"
#include "pixmaps/bank_16x16.xpm"
#include "pixmaps/account_16x16.xpm"
#include "pixmaps/account_movement_16x16.xpm"

PanelBankMain::PanelBankMain(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  this->isLoaded = false;

  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  // Main sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  // Toolbar - New button

  toolBar->AddTool(PANEL_BANK_MAIN_TOOL_NEW_ID, Lang::Get("New account"), wxBitmap(new_16x16_xpm));

  // Toolbar - Refresh button

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_BANK_MAIN_TOOL_REFRESH_ID, Lang::Get("Refresh"), wxBitmap(refresh_16x16_xpm));

  toolBar->AddSeparator();

  // Toolbar - Banks listing

  toolBar->AddTool(PANEL_BANK_MAIN_TOOL_BANK_LIST_ID, Lang::Get("Banks"), wxBitmap(bank_16x16_xpm));

  toolBar->AddSeparator();

  // Toolbar - Search controls

  toolBar->AddControl(new wxStaticBitmap(toolBar, wxID_ANY, wxBitmap(search_16x16_xpm)));

  wxTextCtrl *teditSearch = new wxTextCtrl(toolBar, wxID_ANY, wxT(""));

  teditSearch->SetMinSize(wxSize(150, 23));

  toolBar->AddControl(teditSearch);

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Registered incomes


  // Registered movements

  wxStaticText *label = new wxStaticText(this, wxID_ANY, Lang::Get("Bank accounts"));
  szTop->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);


  wxBoxSizer *szIncomesAccounts = new wxBoxSizer(wxVERTICAL);
  szTop->Add(szIncomesAccounts, 1, wxEXPAND | wxGROW, 5);


  // Accounts listing

  listCtrlAccounts = new wxListCtrl(this,
                                   DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
                                   wxDefaultPosition,
                                   wxSize(5, 5),
                                   wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  szIncomesAccounts->Add(listCtrlAccounts, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Assign an image list to the control

  wxImageList* imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(account_16x16_xpm));
  listCtrlAccounts->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColBankName;
  itemColBankName.SetText(Lang::Get("Bank"));
  listCtrlAccounts->InsertColumn(0, itemColBankName);
  listCtrlAccounts->SetColumnWidth(0, 250);

  wxListItem itemColAccountType;
  itemColAccountType.SetText(Lang::Get("Type"));
  listCtrlAccounts->InsertColumn(1, itemColAccountType);
  listCtrlAccounts->SetColumnWidth(1, 100);

  wxListItem itemColAccountNumber;
  itemColAccountNumber.SetText(Lang::Get("Number"));
  listCtrlAccounts->InsertColumn(2, itemColAccountNumber);
  listCtrlAccounts->SetColumnWidth(2, 100);

  wxListItem itemColAccountAgency;
  itemColAccountAgency.SetText(Lang::Get("Agency"));
  listCtrlAccounts->InsertColumn(3, itemColAccountAgency);
  listCtrlAccounts->SetColumnWidth(3, 100);

  wxListItem itemColAmountBalance;
  itemColAmountBalance.SetText(Lang::Get("Balance"));
  itemColAmountBalance.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlAccounts->InsertColumn(4, itemColAmountBalance);
  listCtrlAccounts->SetColumnWidth(4, 100);


  // Registered account events

  label = new wxStaticText(this, wxID_ANY, Lang::Get("Bank account events"));
  szTop->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  wxBoxSizer *szIncomesMovement = new wxBoxSizer(wxVERTICAL);
  szTop->Add(szIncomesMovement, 1, wxEXPAND | wxGROW, 5);

  // Incomes listing

  listCtrlMovement = new wxListCtrl(this,
                                    DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
                                    wxDefaultPosition,
                                    wxSize(5, 5),
                                    wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  szIncomesMovement->Add(listCtrlMovement, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Assign an image list to the control

  imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(account_movement_16x16_xpm));
  listCtrlMovement->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColMovementBankName;
  itemColMovementBankName.SetText(Lang::Get("Bank"));
  listCtrlMovement->InsertColumn(0, itemColMovementBankName);
  listCtrlMovement->SetColumnWidth(0, 250);

  wxListItem itemColMovementAccount;
  itemColMovementAccount.SetText(Lang::Get("Account"));
  listCtrlMovement->InsertColumn(1, itemColMovementAccount);
  listCtrlMovement->SetColumnWidth(1, 100);

  wxListItem itemColMovementAccountAgency;
  itemColMovementAccountAgency.SetText(Lang::Get("Amount"));
  listCtrlMovement->InsertColumn(2, itemColMovementAccountAgency);
  listCtrlMovement->SetColumnWidth(2, 100);

  wxListItem itemColMovementType;
  itemColMovementType.SetText(Lang::Get("Type"));
  listCtrlMovement->InsertColumn(3, itemColMovementType);
  listCtrlMovement->SetColumnWidth(3, 150);

  wxListItem itemColMovementDateTime;
  itemColMovementDateTime.SetText(Lang::Get("Date and time"));
  listCtrlMovement->InsertColumn(4, itemColMovementDateTime);
  listCtrlMovement->SetColumnWidth(4, 100);


  LoadData();

  listCtrlAccounts->Connect(wxEVT_LIST_ITEM_ACTIVATED,
                            wxListEventHandler(PanelBankMain::OnBankAccountListItemEditDialogRequest),
                            NULL,
                            this);


  this->isLoaded = true;
}


void PanelBankMain::Terminate()
{
  Close();

  Destroy();
}



int PanelBankMain::LoadData()
{
  LoadAccounts();

  LoadMovements();

  return 0;
}


int PanelBankMain::LoadAccounts()
{
  listCtrlAccounts->DeleteAllItems();

  DBQuery dbquery;
  DBQuery dbqueryPhone;
  std::vector<wxString> params;

  wxString sql = wxT("SELECT c.account_id, c.account_type_id, t.nome AS conta_tipo_nome, ")
                 wxT("c.bank_id, b.nome AS banco_nome, ")
                 wxT("c.accounting_account_id, c.codigo, c.agency, ")
                 wxT("c.titular_nome, c.status ")
                 wxT("FROM ") + DB::GetTableName("bank.account") + wxT(" c ")
                 wxT("LEFT JOIN ") + DB::GetTableName("bank") + wxT(" b ON (c.bank_id = b.bank_id) ")
                 wxT("LEFT JOIN ") + DB::GetTableName("bank.account.type") + wxT(" t ON (c.account_type_id = t.account_type_id) ")
                 wxT("ORDER BY lower(b.nome), c.agency, c.codigo");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Could not get the bank accounts list."), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlAccounts->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("banco_nome")), 0);

      listCtrlAccounts->SetItemData(resultIndex, dbquery.GetFieldLongByName(resultIndex, wxT("account_id")));

      listCtrlAccounts->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("conta_tipo_nome")));

      listCtrlAccounts->SetItem(resultIndex, 2, dbquery.GetFieldStrByName(resultIndex, wxT("codigo")));

      listCtrlAccounts->SetItem(resultIndex, 3, dbquery.GetFieldStrByName(resultIndex, wxT("agency")));
    }
  }

  return 0;
}


int PanelBankMain::LoadMovements()
{
  listCtrlMovement->DeleteAllItems();

  DBQuery dbquery;
  DBQuery dbqueryPhone;
  std::vector<wxString> params;

  int rc = dbquery.Execute(wxT("SELECT m.movement_id, m.valor, m.status, ")
                           wxT("b.bank_id, b.nome AS banco_nome, ")
                           wxT("m.account_movement_type_id, mt.nome AS movimentacao_nome ")
                           wxT("FROM ") + DB::GetTableName("bank.account.event") + wxT(" m ")
                           wxT("LEFT JOIN ") + DB::GetTableName("bank.account.event.type") + wxT(" mt ON (m.account_movement_type_id = mt.account_movement_type_id) ")
                           wxT("LEFT JOIN ") + DB::GetTableName("bank.account") + wxT(" c ON (c.account_id = m.account_id) ")
                           wxT("LEFT JOIN ") + DB::GetTableName("bank") + wxT(" b ON (c.bank_id = b.bank_id) ")
                           wxT("ORDER BY m.register_dt DESC, m.valor"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Could not get the banks movements list."), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlMovement->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("banco_nome")), 0);

      listCtrlMovement->SetItemData(resultIndex, dbquery.GetFieldLongByName(resultIndex, wxT("movement_id")));

      //listCtrlMovement->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("conta_tipo_nome")));

      listCtrlMovement->SetItem(resultIndex, 2, dbquery.GetFieldStrByName(resultIndex, wxT("valor")));

      listCtrlMovement->SetItem(resultIndex, 3, dbquery.GetFieldStrByName(resultIndex, wxT("movimentacao_nome")));

      listCtrlMovement->SetItem(resultIndex, 4, dbquery.GetFieldStrByName(resultIndex, wxT("register_dt")));

      //GUI::ListSetRowBgColor(listCtrlMovement, resultIndex);
    }
  }

  return 0;
}


void PanelBankMain::OnNewDialogRequest(wxCommandEvent& event)
{
  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString caption = wxString::Format(Lang::Get("New bank account"));

  PanelBankAccountNew *panelBankAccountNew;

  mainFrame->PageCreate(&panelBankAccountNew, caption, true);
}


void PanelBankMain::OnBankAccountListItemEditDialogRequest(wxListEvent& event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("PanelBankMain::OnBankAccountListItemEditDialogRequest()"), LogMessage::MSG_DBG);

  wxListItem listItem = event.GetItem();

  int data = listItem.GetData();

  messenger->Register(wxString::Format("Bank account ID: %d", data), LogMessage::MSG_DBG);

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString caption = Lang::Get("Bank account") + wxT(" #") + wxString::Format("%d", data);

  if (!mainFrame->PageIsOpened(caption, true)) {
    PanelBankAccountEdit *panelBankAccountEdit;

    mainFrame->PageCreate(&panelBankAccountEdit, caption, true);

    panelBankAccountEdit->LoadData(data);
  }
}


void PanelBankMain::OnRefreshRequest(wxCommandEvent& event)
{
  LoadData();
}


void PanelBankMain::OnBankListDialogRequest(wxCommandEvent& event)
{
  BankListDialog dialog(this);

  dialog.ShowModal();
}


BEGIN_EVENT_TABLE(PanelBankMain, wxScrolledWindow)
  EVT_TOOL(PANEL_BANK_MAIN_TOOL_NEW_ID, PanelBankMain::OnNewDialogRequest)
  EVT_TOOL(PANEL_BANK_MAIN_TOOL_REFRESH_ID, PanelBankMain::OnRefreshRequest)
  EVT_TOOL(PANEL_BANK_MAIN_TOOL_BANK_LIST_ID, PanelBankMain::OnBankListDialogRequest)
END_EVENT_TABLE()