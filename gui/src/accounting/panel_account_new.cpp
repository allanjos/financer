/**
 * Account registering dialog.
 *
 * \file  dlg_account_new.cpp
 * \version 1.0
 * \since 22/10/2008
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "dlg_main.hpp"
#include "panel_account_new.hpp"
#include "panel_account_main.hpp"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"


PanelAccountNew::PanelAccountNew(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  SetAutoLayout(true);

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(PANEL_ACCOUNT_NEW_TOOL_SAVE_ID,
                   Lang::Get("Save"),
                   wxBitmap(save_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szMargin->AddSpacer(5);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Notebook to separate information by category

  wxNotebook *notebook= new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxGROW|wxEXPAND|wxALL, 1);


  // Panel to main information

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS);

  panelMain->SetAutoLayout(true);

  panelMain->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMain, Lang::Get("General"), true);

  wxBoxSizer *szMainMargin = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMainMargin);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMainMargin->Add(szMain, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);

  // Account main fields

  wxFlexGridSizer *szFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szFields, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Account name

  wxStaticText *label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Name") + wxT(":"));
  szFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditName = new wxTextCtrl(panelMain, wxID_ANY, wxT(""));
  szFields->Add(teditName, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Account code

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Code") + wxT(":"));
  szFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditAccountCode = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szFields->Add(teditAccountCode, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Description

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Brief") + wxT(":"));
  szFields->Add(label, 0, wxALL | wxALIGN_TOP | wxALIGN_RIGHT, 1);

  teditDescription = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szFields->Add(teditDescription, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 1);

  //teditDescription->SetMaxLength(255);

  // Account type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Type") + wxT(":"));
  szFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szFields->Add(szRow, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  // Active account

  radioBtnAccountTypeActive = new wxRadioButton(panelMain,
                                                PANEL_ACCOUNT_NEW_RADIOBUTTON_ACTIVE_ID,
                                                Lang::Get("Asset"),
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                wxRB_GROUP);
  szRow->Add(radioBtnAccountTypeActive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  //radioBtnAccountTypeActive->SetValue(true);

  // Passive account

  radioBtnAccountTypePassive = new wxRadioButton(panelMain,
                                                 PANEL_ACCOUNT_NEW_RADIOBUTTON_PASSIVE_ID,
                                                 Lang::Get("Liability"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  szRow->Add(radioBtnAccountTypePassive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  // Account of results

  radioBtnAccountTypeResult = new wxRadioButton(panelMain,
                                                PANEL_ACCOUNT_NEW_RADIOBUTTON_OF_RESULTS_ID,
                                                Lang::Get("Profit and loss"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szRow->Add(radioBtnAccountTypeResult, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);


  // Account parent

  lblAccountParent = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Is part of") + wxT(":"));
  szFields->Add(lblAccountParent, 0, wxALL | wxALIGN_TOP | wxALIGN_RIGHT, 1);

  treeCtrlAccounts = new wxTreeCtrl(panelMain,
                                    PANEL_ACCOUNT_NEW_TREECTRL_ACCOUNT_LIST_ID,
                                    wxDefaultPosition,
                                    wxSize(-1, 150),
                                    wxTR_DEFAULT_STYLE | wxTR_HAS_BUTTONS);

  szFields->Add(treeCtrlAccounts, 1, wxALL | wxEXPAND | wxGROW, 1);

  // Account type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Subtype") + wxT(":"));
  szFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  cboxAccountSubType = new wxChoice(panelMain, wxID_ANY);
  szFields->Add(cboxAccountSubType, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Creation date

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Creation date") + wxT(": "));
  szFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  teditCreationDate = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(75, -1));
  szRow->Add(teditCreationDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarBalanceDate = new wxBitmapButton(panelMain,
                                              PANEL_ACCOUNT_NEW_BTN_BALANCE_DATE_ID,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition, wxDefaultSize);
  szRow->Add(btnCalendarBalanceDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarBalanceDate->SetMinSize(wxSize(-1, teditCreationDate->GetSize().GetHeight()));

  wxDateTime dateTime = wxDateTime::Now();

  teditCreationDate->SetValue(dateTime.Format(wxT("%d/%m/%Y")));


  szFields->AddGrowableCol(1);
  szFields->AddGrowableRow(4);


  // Notes

  wxPanel *panelNotes = new wxPanel(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);

  // Notes about licitação

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szNotes->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szRow->Add(teditNotes, 1, wxALL | wxEXPAND | wxGROW, 1);

  // Load data

  LoadAccountTypes();

  LoadAccountList();


  teditName->SetFocus();
}


int PanelAccountNew::LoadAccountTypes()
{
  cboxAccountSubType->Freeze();

  cboxAccountSubType->Clear();

  cboxAccountSubType->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  LogMessenger *messenger = LogMessenger::GetInstance();

  wxString sql = wxT("SELECT * FROM ") + DB::GetTableName("accounting.account.subtype") + wxT(" ORDER BY lower(nome)");

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  DBQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de subtipos de contas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("subtipo_id")));

      cboxAccountSubType->Append(dbquery.GetFieldStrByName(index, "nome"), container);
    }
  }

  cboxAccountSubType->SetSelection(0);

  cboxAccountSubType->Thaw();

  return 0;
}


int PanelAccountNew::LoadAccountList()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  treeCtrlAccounts->DeleteAllItems();

  if (!radioBtnAccountTypeActive->GetValue() &&
      !radioBtnAccountTypePassive->GetValue() &&
      !radioBtnAccountTypeResult->GetValue()) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Informe o tipo da conta."));

    return -1;
  }


  wxString rootCaption;

  if (radioBtnAccountTypeActive->GetValue()) {
    rootCaption = wxT("1. ") + Lang::Get("Assets");
  }
  else if (radioBtnAccountTypePassive->GetValue()) {
    rootCaption = wxT("2. ") + Lang::Get("Liabilities");
  }
  else if (radioBtnAccountTypeResult->GetValue()) {
    rootCaption = wxT("3. ") + Lang::Get("Profit and loss");
  }

  wxTreeItemId treeItemRootId = treeCtrlAccounts->AddRoot(rootCaption);

  treeCtrlAccounts->ExpandAll();


  wxString sql = wxT("");

  if (radioBtnAccountTypeActive->GetValue()) {
    sql = wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
          wxT(" WHERE tipo_id = 'A' AND conta_parent_id IS NULL ORDER BY codigo, lower(nome)");
  }
  else if (radioBtnAccountTypePassive->GetValue()) {
    sql = wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
          wxT(" WHERE tipo_id = 'P' AND conta_parent_id IS NULL ORDER BY codigo, lower(nome)");
  }
  else if (radioBtnAccountTypeResult->GetValue()) {
    sql = wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
          wxT(" WHERE tipo_id = 'R' AND conta_parent_id IS NULL ORDER BY codigo, lower(nome)");
  }

  messenger->Register(sql, LogMessage::MSG_DBG);

  DBQuery dbquery;
  int index = 0;

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas cadastradas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (index = 0; index < dbquery.GetRowsCount(); index++) {
      TreeItemDataAccount *treeItemDataAccount = new TreeItemDataAccount(dbquery.GetFieldIntByName(index, wxT("account_id")));

      wxTreeItemId treeItemId = treeCtrlAccounts->AppendItem(treeItemRootId,
                                                             dbquery.GetFieldStrByName(index, wxT("codigo")) + ". " + dbquery.GetFieldStrByName(index, wxT("nome")));

      treeCtrlAccounts->SetItemData(treeItemId, treeItemDataAccount);

      // Load children

      LoadAccountChildList(treeItemId);
    }
  }

  treeCtrlAccounts->ExpandAll();

  treeCtrlAccounts->SelectItem(treeCtrlAccounts->GetRootItem());

  return 0;
}


int PanelAccountNew::LoadAccountChildList(wxTreeItemId treeItemParentId)
{
  if (!treeItemParentId.IsOk() || treeCtrlAccounts->GetItemData(treeItemParentId) == NULL) {
    return -1;
  }

  int accountId = ((TreeItemDataAccount *) treeCtrlAccounts->GetItemData(treeItemParentId))->GetAccountId();

  DBQuery dbquery;
  int index = 0;

  wxString sql = wxString::Format(wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
                                  wxT(" WHERE conta_parent_id = %d ORDER BY codigo, lower(nome)"), accountId);

  LogMessenger *messenger = LogMessenger::GetInstance();

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas filhas cadastradas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (index = 0; index < dbquery.GetRowsCount(); index++) {
      TreeItemDataAccount *treeItemDataAccount = new TreeItemDataAccount(dbquery.GetFieldIntByName(index, wxT("account_id")));

      wxTreeItemId treeItemId = treeCtrlAccounts->AppendItem(treeItemParentId,
                                                             dbquery.GetFieldStrByName(index, wxT("codigo")) + ". " + dbquery.GetFieldStrByName(index, wxT("nome")));

      treeCtrlAccounts->SetItemData(treeItemId, treeItemDataAccount);

      // Load children

      LoadAccountChildList(treeItemId);
    }
  }

  return 0;
}


void PanelAccountNew::OnSetAsActiveAccount(wxCommandEvent& event)
{
  LoadAccountList();
}


void PanelAccountNew::OnSetAsPassiveAccount(wxCommandEvent& event)
{
  LoadAccountList();
}


void PanelAccountNew::OnSetAsOfResultsAccount(wxCommandEvent& event)
{
  LoadAccountList();
}


void PanelAccountNew::OnBalanceInitialDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditCreationDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarBalanceDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarBalanceDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditCreationDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                                calendarDialog.selectedMonth + wxT("/") + 
                                calendarDialog.selectedYear);
  }
}


void PanelAccountNew::Terminate()
{
  Close();

  Destroy();
}


void PanelAccountNew::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Informe o nome da conta."));

    return;
  }

  /*
  if (cboxAccountSubType->GetClientObject(cboxAccountSubType->GetSelection()) == NULL) {
    statusBar->SetStatusText(wxT("Informe o tipo da conta."));

    return;
  }
  */
  
  if (!radioBtnAccountTypeActive->GetValue() &&
      !radioBtnAccountTypePassive->GetValue() &&
      !radioBtnAccountTypeResult->GetValue()) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Informe o tipo da conta."));

    return;
  }

  bool registeringError = false;
  wxString sqlErrorMesssageStr = wxT("");
  std::vector<wxString> fields, params, valueModifiers;


  // Name

  fields.push_back(wxT("nome"));
  params.push_back(teditName->GetValue());
  valueModifiers.push_back(wxEmptyString);

  // Code

  if (teditAccountCode->GetValue().Length() > 0) {
    fields.push_back(wxT("codigo"));

    params.push_back(teditAccountCode->GetValue());

    valueModifiers.push_back(wxEmptyString);
  }

  // Description

  if (teditDescription->GetValue().Length() > 0) {
    fields.push_back(wxT("description"));

    params.push_back(teditDescription->GetValue());

    valueModifiers.push_back(wxEmptyString);
  }

  // Creation date

  if (teditCreationDate->GetValue().Length() > 0) {
    fields.push_back(wxT("criacao_date"));

    params.push_back(teditCreationDate->GetValue());

    valueModifiers.push_back("to_date($@, 'DD/MM/YYYY')");
  }

  // Parent

  if (treeCtrlAccounts->GetSelection().IsOk() &&
      treeCtrlAccounts->GetItemData(treeCtrlAccounts->GetSelection()) != NULL) {
    messenger->Register("treeCtrlAccounts OK", LogMessage::MSG_DBG);

    int accountId = ((TreeItemDataAccount *) treeCtrlAccounts->GetItemData(treeCtrlAccounts->GetSelection()))->GetAccountId();

    fields.push_back(wxT("conta_parent_id"));

    params.push_back(wxString::Format(wxT("%d"), accountId));

    valueModifiers.push_back(wxEmptyString);
  }

  // Type

  fields.push_back(wxT("tipo_id"));

  if (radioBtnAccountTypeActive->GetValue()) {
    params.push_back(wxT("A"));
  }
  else if (radioBtnAccountTypePassive->GetValue()) {
    params.push_back(wxT("P"));
  }
  else if (radioBtnAccountTypeResult->GetValue()) {
    params.push_back(wxT("R"));
  }

  valueModifiers.push_back(wxEmptyString);

  // Subtype

  if (cboxAccountSubType->GetSelection() >= 0 && cboxAccountSubType->GetClientObject(cboxAccountSubType->GetSelection())) {

    ContainerIntData *container = (ContainerIntData *) cboxAccountSubType->GetClientObject(cboxAccountSubType->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("subtipo_id"));

      params.push_back(wxString::Format(wxT("%d"), container->GetData()));

      valueModifiers.push_back(wxEmptyString);
    }
  }


  DBQuery dbquery;

  // Init transaction

  int rc = DBTransaction::Begin();

  if (rc != 0) {
    registeringError = true;

    sqlErrorMesssageStr = dbquery.GetMessageStr();
  }

  if (!registeringError) {
    wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("accounting.account"), fields);

    //wxMessageBox(sql);

    // Executing database command

    int rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      sqlErrorMesssageStr = wxString(dbquery.GetMessageStr());
    }

    // End transaction

    rc = DBTransaction::Commit();
  }

  if (registeringError) {
    wxMessageBox(wxString(wxT("Não foi possível cadastrar as informações sobre a conta.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + sqlErrorMesssageStr);
  }
  else {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    // Reload main panel data
    if (mainFrame->PageIsOpened(Lang::Get("Accounting"))) {
      PanelAccountMain *panelAccountMain = (PanelAccountMain *) mainFrame->GetPage(Lang::Get("Accounting"));

      if (panelAccountMain) {
        panelAccountMain->LoadList();
      }
    }

    // Remove this panel

    mainFrame->PageRemove(Lang::Get("New ledger account"));
  }
}


void PanelAccountNew::OnCharHook(wxKeyEvent& event)
{
  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Terminate();
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(PanelAccountNew, wxScrolledWindow)
  EVT_TOOL(PANEL_ACCOUNT_NEW_TOOL_SAVE_ID, PanelAccountNew::OnSaveRequest)
  EVT_KEY_DOWN(PanelAccountNew::OnCharHook)
  EVT_BUTTON(PANEL_ACCOUNT_NEW_BTN_BALANCE_DATE_ID, PanelAccountNew::OnBalanceInitialDateChooseRequest)
  EVT_RADIOBUTTON(PANEL_ACCOUNT_NEW_RADIOBUTTON_ACTIVE_ID, PanelAccountNew::OnSetAsActiveAccount)
  EVT_RADIOBUTTON(PANEL_ACCOUNT_NEW_RADIOBUTTON_PASSIVE_ID, PanelAccountNew::OnSetAsPassiveAccount)
  EVT_RADIOBUTTON(PANEL_ACCOUNT_NEW_RADIOBUTTON_OF_RESULTS_ID, PanelAccountNew::OnSetAsOfResultsAccount)
END_EVENT_TABLE()