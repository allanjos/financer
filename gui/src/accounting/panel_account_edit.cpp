/**
 * Account editing dialog.
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
#include "utils_datetime.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "dlg_main.hpp"
#include "panel_account_edit.hpp"
#include "panel_account_main.hpp"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"


PanelAccountEdit::PanelAccountEdit(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  SetAutoLayout(true);

  this->accountId = -1;

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(PANEL_ACCOUNT_EDIT_TOOL_SAVE_ID,
                   Lang::Get("Save"),
                   wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_ACCOUNT_EDIT_TOOL_DELETE_ID,
                   wxT(" Excluir"),
                   wxBitmap(delete_16x16_xpm));

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
  szFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  teditName = new wxTextCtrl(panelMain, wxID_ANY, wxT(""));
  szFields->Add(teditName, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Account code

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Code") + wxT(":"));
  szFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  teditAccountCode = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szFields->Add(teditAccountCode, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Description

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Brief") + wxT(":"));
  szFields->Add(label, 0, wxALL|wxALIGN_TOP|wxALIGN_RIGHT, 1);

  teditDescription = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szFields->Add(teditDescription, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 1);

  //teditDescription->SetMaxLength(255);

  // Account type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Type") + wxT(":"));
  szFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szFields->Add(szRow, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  // Active account

  radioBtnAccountTypeActive = new wxRadioButton(panelMain,
                                                PANEL_ACCOUNT_EDIT_RADIOBUTTON_ACTIVE_ID,
                                                Lang::Get("Asset"),
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                wxRB_GROUP);
  szRow->Add(radioBtnAccountTypeActive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  //radioBtnAccountTypeActive->SetValue(true);

  // Passive account

  radioBtnAccountTypePassive = new wxRadioButton(panelMain,
                                                 PANEL_ACCOUNT_EDIT_RADIOBUTTON_PASSIVE_ID,
                                                 Lang::Get("Liability"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  szRow->Add(radioBtnAccountTypePassive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  // Account of results

  radioBtnAccountTypeResult = new wxRadioButton(panelMain,
                                                PANEL_ACCOUNT_EDIT_RADIOBUTTON_OF_RESULTS_ID,
                                                Lang::Get("Profit and loss"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szRow->Add(radioBtnAccountTypeResult, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  // Account parent

  lblAccountParent = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Is part of") + wxT(":"));
  szFields->Add(lblAccountParent, 0, wxALL | wxALIGN_TOP | wxALIGN_RIGHT, 1);

  treeCtrlAccounts = new wxTreeCtrl(panelMain,
                                    PANEL_ACCOUNT_EDIT_TREECTRL_ACCOUNT_LIST_ID,
                                    wxDefaultPosition,
                                    wxSize(-1, 150),
                                    wxTR_DEFAULT_STYLE | wxTR_HAS_BUTTONS);

  szFields->Add(treeCtrlAccounts, 1, wxALL | wxEXPAND | wxGROW, 1);

  // Account type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Subtype") + wxT(":"));
  szFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  cboxAccountSubType = new wxChoice(panelMain, wxID_ANY);
  szFields->Add(cboxAccountSubType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Creation date

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Creation date") + wxT(": "));
  szFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  teditCreationDate = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(75, -1));
  szRow->Add(teditCreationDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarBalanceDate = new wxBitmapButton(panelMain,
                                              PANEL_ACCOUNT_EDIT_BTN_CREATION_DATE_ID,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition, wxDefaultSize);
  szRow->Add(btnCalendarBalanceDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarBalanceDate->SetMinSize(wxSize(-1, teditCreationDate->GetSize().GetHeight()));

  szFields->AddGrowableCol(1);
  szFields->AddGrowableRow(4);

  // Balance

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Balance"));

  GUI::FormatSectionLabel(label);

  szMain->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  // General balance

  wxFlexGridSizer *szBalanceFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szBalanceFields, 0, wxALL | wxEXPAND, 0);

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("General balance") + wxT(": "));
  szBalanceFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("R$ 1.000,00"));
  szBalanceFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);


  // Movement

  wxPanel *panelMovement = new wxPanel(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

  notebook->AddPage(panelMovement, Lang::Get("Events"), false);

  wxBoxSizer *szMarginMovement = new wxBoxSizer(wxVERTICAL);
  panelMovement->SetSizer(szMarginMovement);

  wxBoxSizer *szMovement = new wxBoxSizer(wxVERTICAL);
  szMarginMovement->Add(szMovement, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Search

  wxBoxSizer *szMovementNew = new wxBoxSizer(wxHORIZONTAL);
  szMovement->Add(szMovementNew, 0, wxALL | wxEXPAND, 5);

  label = new wxStaticText(panelMovement, wxID_STATIC, Lang::Get("Search for") + wxT(": "));
  szMovementNew->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditMovementSearch = new wxTextCtrl(panelMovement, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szMovementNew->Add(teditMovementSearch, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  szMovementNew->AddSpacer(10);

  // Movement date

  label = new wxStaticText(panelMovement, wxID_STATIC, Lang::Get("Date") + wxT(": "));
  szMovementNew->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditMovementDate = new wxTextCtrl(panelMovement, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(75, -1));
  szMovementNew->Add(teditMovementDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarMovementDate = new wxBitmapButton(panelMovement,
                                               PANEL_ACCOUNT_EDIT_BTN_BALANCE_DATE_ID,
                                               wxBitmap(calendar_bmp_btn_xpm));
  szMovementNew->Add(btnCalendarMovementDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarMovementDate->SetMinSize(wxSize(-1, teditMovementDate->GetSize().GetHeight()));


  szMovementNew->AddSpacer(10);


  cboxMovementType = new wxChoice(panelMovement, wxID_ANY);
  szMovementNew->Add(cboxMovementType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  szMovementNew->AddSpacer(10);


  btnCalendarMovementAdd = new wxButton(panelMovement,
                                        PANEL_ACCOUNT_EDIT_BTN_MOVEMENT_SEARCH_ID,
                                        Lang::Get("Search event"));
  szMovementNew->Add(btnCalendarMovementAdd, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Movement listing

  label = new wxStaticText(panelMovement, wxID_STATIC, Lang::Get("Registered events"));
  szMovement->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  wxBoxSizer *szMovementListing = new wxBoxSizer(wxVERTICAL);
  szMovement->Add(szMovementListing, 1, (wxALL & ~wxTOP)|wxEXPAND, 5);

  listCtrlMovement = new wxListCtrl(panelMovement,
                                    wxID_ANY,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  wxListItem listItemColDate;
  listItemColDate.SetText(Lang::Get("Date"));
  listCtrlMovement->InsertColumn(0, listItemColDate);
  listCtrlMovement->SetColumnWidth(0, 80);

  wxListItem listItemColType;
  listItemColType.SetText(Lang::Get("Type"));
  listCtrlMovement->InsertColumn(1, listItemColType);
  listCtrlMovement->SetColumnWidth(1, 150);

  wxListItem listItemColValue;
  listItemColValue.SetText(Lang::Get("Amount"));
  listCtrlMovement->InsertColumn(2, listItemColValue);
  listCtrlMovement->SetColumnWidth(2, 100);

  wxListItem listItemColBrief;
  listItemColBrief.SetText(Lang::Get("Description"));
  listCtrlMovement->InsertColumn(3, listItemColBrief);
  listCtrlMovement->SetColumnWidth(3, 310);

  szMovementListing->Add(listCtrlMovement, 1, wxALL | wxEXPAND | wxGROW, 0);


  // General balance

  wxFlexGridSizer *szMovementValues = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMovementListing->Add(szMovementValues, 0, wxTOP, 5);

  label = new wxStaticText(panelMovement, wxID_STATIC, Lang::Get("General balance") + wxT(": "));
  szMovementValues->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditGeneralBalance = new wxTextCtrl(panelMovement, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT);
  szMovementValues->Add(teditGeneralBalance, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  // Notes

  wxPanel *panelNotes = new wxPanel(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Notes

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szNotes->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szRow->Add(teditNotes, 1, wxALL | wxEXPAND | wxGROW, 1);

  // Load data

  LoadAccountTypes();

  //LoadAccountList();

  // Movements

  LoadMovementTypes();

  wxDateTime timeNow = wxDateTime::Now();  

  teditMovementDate->SetValue(timeNow.Format(wxT("%d/%m/%Y"), wxDateTime::CET));


  teditName->SetFocus();
}


bool PanelAccountEdit::LoadData(int accountId)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxString::Format(wxT("SELECT nome, codigo, description, tipo_id, subtipo_id, conta_parent_id, ")
                           wxT("to_char(criacao_date, 'DD/MM/YYYY') AS creation_dt ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" WHERE account_id = %d"), accountId);
  }
  else {
    sql = wxString::Format(wxT("SELECT nome, codigo, description, tipo_id, subtipo_id, conta_parent_id, ")
                           wxT("date(criacao_date) AS creation_dt ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" WHERE account_id = %d"), accountId);
  }

  messenger->Register(sql, LogMessage::MSG_DBG);

  DBQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre a conta selecionada."));

    return false;
  }

  if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("As informações sobre a conta selecionada não foram encontradas."));

    return false;
  }

  // Result set OK

  messenger->Register(wxT("conta_parent_id=") + dbquery.GetFieldStrByName(0, wxT("conta_parent_id")));

  this->accountId = accountId;


  // Account name

  teditName->SetValue(dbquery.GetFieldStrByName(0, wxT("nome")));

  // Account code

  teditAccountCode->SetValue(dbquery.GetFieldStrByName(0, wxT("codigo")));

  // Account description

  teditDescription->SetValue(dbquery.GetFieldStrByName(0, wxT("description")));

  // Creation date

  teditCreationDate->SetValue(DT::FromDB(dbquery.GetFieldStrByName(0, wxT("creation_dt"))));

  // Account type

  messenger->Register("accounting account type = " + dbquery.GetFieldStrByName(0, wxT("tipo_id")));

  if (dbquery.GetFieldStrByName(0, wxT("tipo_id")).Cmp(wxT("A")) == 0) {
    radioBtnAccountTypeActive->SetValue(true);
  }
  else if (dbquery.GetFieldStrByName(0, wxT("tipo_id")).Cmp(wxT("P")) == 0) {
    radioBtnAccountTypePassive->SetValue(true);
  }
  else if (dbquery.GetFieldStrByName(0, wxT("tipo_id")).Cmp(wxT("R")) == 0) {
    radioBtnAccountTypeResult->SetValue(true);
  }

  // Account subtype

  if (dbquery.GetFieldIntByName(0, wxT("subtipo_id")) > 0) {
    int typeId = dbquery.GetFieldIntByName(0, wxT("subtipo_id"));

    for (size_t index = 0; index < cboxAccountSubType->GetCount(); index++) {
      if (cboxAccountSubType->GetClientObject(index) != NULL) {
        ContainerIntData *container = (ContainerIntData *) cboxAccountSubType->GetClientObject(index);

        if (container->GetData() == typeId)
          cboxAccountSubType->SetSelection(index);
      }
    }
  }

  // Account parent

  accountParentId = -1;

  bool isChild = false;

  if (dbquery.GetFieldIntByName(0, wxT("conta_parent_id")) > 0) {
    accountParentId = dbquery.GetFieldIntByName(0, wxT("conta_parent_id"));

    messenger->Register(wxString::Format(wxT("conta_parent_id=%d"), accountParentId));

    isChild = true;
  }

  // Account list to set the parent

  LoadAccountList();


  if (isChild) {
    SelectAccountFromTreeCtrl(treeCtrlAccounts, treeCtrlAccounts->GetRootItem(), accountParentId);
  }

  return true;
}


int PanelAccountEdit::LoadAccountTypes()
{
  cboxAccountSubType->Freeze();

  cboxAccountSubType->Clear();

  cboxAccountSubType->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  LogMessenger *messenger = LogMessenger::GetInstance();

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  wxString sql = wxT("SELECT *")
                 wxT(" FROM ") + DB::GetTableName("accounting.account.subtype") +
                 wxT(" ORDER BY lower(nome)");

  DBQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de subtipos de contas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("subtipo_id")));

      cboxAccountSubType->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxAccountSubType->SetSelection(0);

  cboxAccountSubType->Thaw();

  return 0;
}


int PanelAccountEdit::LoadMovementTypes()
{
  /*
  cboxMovementType->Freeze();

  cboxMovementType->Clear();

  cboxMovementType->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));


  DBQuery dbquery;

  LogMessenger *messenger = LogMessenger::GetInstance();

  wxString sql = wxT("SELECT * FROM contabil.movimentacao_tipo ORDER BY lower(nome)");

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de tipos de movimentações.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("tipo_id")));

      cboxMovementType->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }


  cboxMovementType->SetSelection(0);

  cboxMovementType->Thaw();
  */

  return 0;
}


int PanelAccountEdit::LoadAccountList()
{
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


  DBQuery dbquery;
  int index = 0;

  LogMessenger *messenger = LogMessenger::GetInstance();

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas cadastradas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (index = 0; index < dbquery.GetRowsCount(); index++) {
      if (accountId == dbquery.GetFieldIntByName(index, wxT("account_id"))) {
        continue;
      }

      TreeItemDataAccount *treeItemDataAccount = new TreeItemDataAccount(dbquery.GetFieldIntByName(index, wxT("account_id")));

      wxTreeItemId treeItemId = treeCtrlAccounts->AppendItem(treeItemRootId,
                                                             dbquery.GetFieldStrByName(index, wxT("codigo")) + ". " + dbquery.GetFieldStrByName(index, wxT("nome")));

      treeCtrlAccounts->SetItemData(treeItemId, treeItemDataAccount);

      if (accountParentId != -1 && accountParentId == dbquery.GetFieldIntByName(index, wxT("account_id"))) {
        treeCtrlAccounts->SelectItem(treeItemId);
      }

      // Load children

      LoadAccountChildList(treeItemId);
    }
  }


  treeCtrlAccounts->ExpandAll();

  treeCtrlAccounts->SelectItem(treeCtrlAccounts->GetRootItem());


  return 0;
}


int PanelAccountEdit::LoadAccountChildList(wxTreeItemId treeItemParentId)
{
  if (!treeItemParentId.IsOk() || treeCtrlAccounts->GetItemData(treeItemParentId) == NULL) {
    return -1;
  }

  int accountId = ((TreeItemDataAccount *) treeCtrlAccounts->GetItemData(treeItemParentId))->GetAccountId();

  DBQuery dbquery;

  LogMessenger *messenger = LogMessenger::GetInstance();

  wxString sql = wxString::Format(wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
                                  wxT(" WHERE conta_parent_id = %d ORDER BY codigo, lower(nome)"), accountId);

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas-filhas cadastradas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      if (accountId == dbquery.GetFieldIntByName(index, wxT("account_id"))) {
        continue;
      }

      int itemId = dbquery.GetFieldIntByName(index, wxT("account_id"));

      if (itemId == this->accountId) {
        continue;
      }


      TreeItemDataAccount *treeItemDataAccount = new TreeItemDataAccount(itemId);

      wxTreeItemId treeItemId = treeCtrlAccounts->AppendItem(treeItemParentId,
                                                             dbquery.GetFieldStrByName(index, wxT("codigo")) + ". " + dbquery.GetFieldStrByName(index, wxT("nome")));

      treeCtrlAccounts->SetItemData(treeItemId, treeItemDataAccount);

      if (accountParentId != -1 && accountParentId == dbquery.GetFieldIntByName(index, wxT("account_id"))) {
        treeCtrlAccounts->SelectItem(treeItemId);
      }

      // Load children

      LoadAccountChildList(treeItemId);
    }
  }


  return 0;
}


int PanelAccountEdit::SelectAccountFromTreeCtrl(wxTreeCtrl *treeCtrl, wxTreeItemId treeItemId, int accountId)
{
  if (treeCtrl->GetItemData(treeItemId) != NULL &&
      ((TreeItemDataAccount *) treeCtrl->GetItemData(treeItemId))->GetAccountId() == accountId) {
    treeCtrl->SelectItem(treeItemId);

    return true;
  }

  bool rv = false;

  wxTreeItemIdValue cookie;

  treeItemId = treeCtrl->GetFirstChild(treeItemId, cookie);

  if (treeItemId.IsOk() > 0) {
    rv = SelectAccountFromTreeCtrl(treeCtrl, treeItemId, accountId);

    if (!rv) {
      treeItemId = treeCtrl->GetNextChild(treeItemId, cookie);

      while (treeItemId.IsOk() > 0) {
        rv = SelectAccountFromTreeCtrl(treeCtrl, treeItemId, accountId);

        if (rv) {
          break;
        }

        treeItemId = treeCtrl->GetNextChild(treeItemId, cookie);
      }
    }

  }

  return rv;
}


void PanelAccountEdit::OnCreationDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditCreationDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarMovementDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarMovementDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditCreationDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                                calendarDialog.selectedMonth + wxT("/") + 
                                calendarDialog.selectedYear);
  }
}


void PanelAccountEdit::OnBalanceInitialDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditMovementDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarMovementDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarMovementDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditMovementDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                                calendarDialog.selectedMonth + wxT("/") + 
                                calendarDialog.selectedYear);
  }
}


void PanelAccountEdit::Terminate()
{
  Close();

  Destroy();
}


void PanelAccountEdit::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Informe o nome da conta."));

    return;
  }

  if (!radioBtnAccountTypeActive->GetValue() &&
      !radioBtnAccountTypePassive->GetValue() &&
      !radioBtnAccountTypeResult->GetValue()) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Informe o tipo da conta."));

    return;
  }

  bool error = false;
  wxString sqlErrorMesssageStr = wxT("");
  std::vector<wxString> fields, params, valueModifiers;
  DBQuery dbquery;

  // Name

  fields.push_back(wxT("nome"));
  params.push_back(teditName->GetValue());
  valueModifiers.push_back("");

  // Code

  fields.push_back(wxT("codigo"));
  if (teditAccountCode->GetValue().Length() > 0) {
    params.push_back(teditAccountCode->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

  // Description

  fields.push_back(wxT("description"));

  if (teditDescription->GetValue().Length() > 0) {
    params.push_back(teditDescription->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

  // Creation date

  if (teditCreationDate->GetValue().Length() > 0) {
    fields.push_back(wxT("criacao_date"));

    params.push_back(DT::ToDB(teditCreationDate->GetValue()));
  }
  valueModifiers.push_back("");

  // Parent

  fields.push_back(wxT("conta_parent_id"));

  if (treeCtrlAccounts->GetSelection().IsOk() &&
      treeCtrlAccounts->GetItemData(treeCtrlAccounts->GetSelection()) != NULL) {
    messenger->Register("treeCtrlAccounts OK", LogMessage::MSG_DBG);

    int accountParentId = ((TreeItemDataAccount *) treeCtrlAccounts->GetItemData(treeCtrlAccounts->GetSelection()))->GetAccountId();

    messenger->Register(wxString::Format("accountParentId=%d", accountParentId), LogMessage::MSG_DBG);

    params.push_back(wxString::Format(wxT("%d"), accountParentId));
  }
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

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
  else {
    params.push_back(wxEmptyString);
  }

  valueModifiers.push_back("");

  // Subtype

  fields.push_back(wxT("subtipo_id"));

  if (cboxAccountSubType->GetSelection() >= 0 && cboxAccountSubType->GetClientObject(cboxAccountSubType->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxAccountSubType->GetClientObject(cboxAccountSubType->GetSelection());

    if (container->GetData() > 0)
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    else
      params.push_back(wxEmptyString);
  }
  else {
    params.push_back(wxEmptyString);
  }

  valueModifiers.push_back("");

  // Updating main data

  if (!error) {
    wxString sql = DBUtils::MountSQLUpdate(DB::GetTableName("accounting.account"),
                                           fields,
                                           wxT("account_id = ") + wxString::Format(wxT("%d"), this->accountId),
                                           valueModifiers);

    //wxMessageBox(sql);

    // Executing database command

    int rc = dbquery.Execute(sql, params);


    if (rc != 0) {
      error = true;

      messenger->Register(dbquery.GetMessageStr(), LogMessage::MSG_DBG);
    }
  }

  if (error) {
    wxMessageBox(wxT("Não foi possível modificar as informações sobre a conta."));
  }
  else {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("As informações sobre a conta '") + teditName->GetValue() + wxT("' foram modificadas com sucesso."));

    // Reload main panel data

    if (mainFrame->PageIsOpened(Lang::Get("Accounting"))) {
      PanelAccountMain *panelAccountMain = (PanelAccountMain *) mainFrame->GetPage(Lang::Get("Accounting"));

      if (panelAccountMain) {
        panelAccountMain->LoadList();
      }
    }
  }
}


void PanelAccountEdit::OnCharHook(wxKeyEvent& event)
{
  event.Skip();
}


void PanelAccountEdit::OnSetAsActiveAccount(wxCommandEvent& event)
{
  LoadAccountList();
}


void PanelAccountEdit::OnSetAsPassiveAccount(wxCommandEvent& event)
{
  LoadAccountList();
}


void PanelAccountEdit::OnSetAsOfResultsAccount(wxCommandEvent& event)
{
  LoadAccountList();
}


BEGIN_EVENT_TABLE(PanelAccountEdit, wxScrolledWindow)
  EVT_TOOL(PANEL_ACCOUNT_EDIT_TOOL_SAVE_ID, PanelAccountEdit::OnSaveRequest)
  EVT_CHAR_HOOK(PanelAccountEdit::OnCharHook)
  EVT_BUTTON(PANEL_ACCOUNT_EDIT_BTN_BALANCE_DATE_ID, PanelAccountEdit::OnBalanceInitialDateChooseRequest)
  EVT_BUTTON(PANEL_ACCOUNT_EDIT_BTN_CREATION_DATE_ID, PanelAccountEdit::OnCreationDateChooseRequest)
  EVT_RADIOBUTTON(PANEL_ACCOUNT_EDIT_RADIOBUTTON_ACTIVE_ID, PanelAccountEdit::OnSetAsActiveAccount)
  EVT_RADIOBUTTON(PANEL_ACCOUNT_EDIT_RADIOBUTTON_PASSIVE_ID, PanelAccountEdit::OnSetAsPassiveAccount)
  EVT_RADIOBUTTON(PANEL_ACCOUNT_EDIT_RADIOBUTTON_OF_RESULTS_ID, PanelAccountEdit::OnSetAsOfResultsAccount)
END_EVENT_TABLE()