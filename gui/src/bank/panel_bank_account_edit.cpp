/**
 * New product dialog.
 *
 * \file  panel_bank_account_new.cpp
 * \version 1.0
 * \since 2013
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_image.hpp"
#include "utils_lang.hpp"
#include "utils_monetary.hpp"
#include "gui_controls.hpp"
#include "panel_bank_main.hpp"
#include "panel_bank_account_edit.hpp"
#include "dlg_main.hpp"
#include "dlg_calendar.hpp"
#include "dlg_bank_account_balance_update.hpp"
#include "app.hpp"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"
#include "pixmaps/balance_16x16.xpm"
#include "pixmaps/calendar_16x16.xpm"
#include "pixmaps/account_movement_16x16.xpm"


PanelBankAccountEdit::PanelBankAccountEdit(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
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

  toolBar->AddTool(PANEL_BANK_ACCOUNT_EDIT_TOOL_SAVE_ID,
                   Lang::Get("Save"),
                   wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_BANK_ACCOUNT_EDIT_TOOL_BALANCE_UPDATE_ID, Lang::Get("Update balance"), wxBitmap(balance_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_BANK_ACCOUNT_EDIT_TOOL_DELETE_ID, Lang::Get("Delete"), wxBitmap(delete_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szMargin->AddSpacer(5);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Notebook

  wxNotebook *notebook= new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxEXPAND | wxALL | wxGROW, 1);


  // Main info

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxHSCROLL | wxVSCROLL);

  panelMain->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMain, Lang::Get("General"), true);


  wxBoxSizer *szMarginMain = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMarginMain);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMarginMain->Add(szMain, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  // Fields

  szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szgMainFields, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Account type

  wxStaticText *label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Type") + wxT(": "), wxDefaultPosition, wxSize(150, -1), wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  choiceType = new wxChoice(panelMain, PANEL_PERSON_NEW_CHOICE_PERSON_TYPE_ID);
  szgMainFields->Add(choiceType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  choiceType->Append(Lang::Get("Checking account"), (wxClientData *) new ContainerIntData((int) 'C'));;
  choiceType->Append(Lang::Get("Savings account"), (wxClientData *) new ContainerIntData((int) 'P'));
  choiceType->Append(Lang::Get("Payroll account"), (wxClientData *) new ContainerIntData((int) 'S'));
  choiceType->Append(Lang::Get("Certificate of deposit"), (wxClientData *) new ContainerIntData((int) 'D'));

  choiceType->SetSelection(0);

  // Account owner name

  labelOwnerName = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Titular") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(labelOwnerName, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  teditOwnerName = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1));
  szgMainFields->Add(teditOwnerName, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

  // Bank

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Bank") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  choiceBank = new wxChoice(panelMain, PANEL_PERSON_NEW_CHOICE_PERSON_TYPE_ID);
  szgMainFields->Add(choiceBank, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account number

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Number") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditNumber = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgMainFields->Add(teditNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditNumber->SetMaxLength(30);

  // Bank agency

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Agency") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditAgency = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szgMainFields->Add(teditAgency, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditAgency->SetMaxLength(10);

  // Creation date

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Creation date") + wxT(": "));
  szgMainFields->Add(label, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  teditCreationDate = new wxTextCtrl(panelMain, PANEL_BANK_ACCOUNT_EDIT_TEDIT_CREATION_DATE_ID, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditCreationDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCreationDateChoose = new wxBitmapButton(panelMain,
                                             PANEL_BANK_ACCOUNT_EDIT_BUTTON_CREATION_DATE_CHOOSE_ID,
                                             wxBitmap(calendar_16x16_xpm));
  szRow->Add(btnCreationDateChoose, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCreationDateChoose->SetMinSize(wxSize(-1, teditCreationDate->GetSize().GetHeight()));

  // Accounting account

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Ledger account") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  choiceAccountingAccount = new wxChoice(panelMain, wxID_ANY);
  szgMainFields->Add(choiceAccountingAccount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  szgMainFields->AddGrowableCol(1);


  // Notes tab

  wxPanel *panelNotes = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1, 30), wxTE_MULTILINE);
  szNotes->Add(teditNotes, 1, wxALL | wxEXPAND, 1);


  // Account movement

  wxScrolledWindow *panelMovement = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxHSCROLL | wxVSCROLL);

  panelMovement->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMovement, Lang::Get("Events"), false);

  wxBoxSizer *szPanelMovementMargin = new wxBoxSizer(wxVERTICAL);

  panelMovement->SetSizer(szPanelMovementMargin);

  // Top sizer

  wxBoxSizer *szPanelMovement = new wxBoxSizer(wxVERTICAL);

  szPanelMovementMargin->Add(szPanelMovement, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Registered movements

  listCtrlMovement = new wxListCtrl(panelMovement,
                                    DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
                                    wxDefaultPosition,
                                    wxSize(5, 5),
                                    wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);

  szPanelMovement->Add(listCtrlMovement, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Assign an image list to the control

  wxImageList *imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(account_movement_16x16_xpm));
  listCtrlMovement->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColMovementAccountAgency;
  itemColMovementAccountAgency.SetText(Lang::Get("Amount"));
  itemColMovementAccountAgency.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlMovement->InsertColumn(0, itemColMovementAccountAgency);
  listCtrlMovement->SetColumnWidth(0, 130);

  wxListItem itemColMovementType;
  itemColMovementType.SetText(Lang::Get("Event type"));
  listCtrlMovement->InsertColumn(1, itemColMovementType);
  listCtrlMovement->SetColumnWidth(1, 200);

  wxListItem itemColMovementDateTime;
  itemColMovementDateTime.SetText(Lang::Get("Date and time"));
  listCtrlMovement->InsertColumn(2, itemColMovementDateTime);
  listCtrlMovement->SetColumnWidth(2, 200);

  wxListItem itemColRegisteringDateTime;
  itemColRegisteringDateTime.SetText(wxT("Registering"));
  listCtrlMovement->InsertColumn(3, itemColRegisteringDateTime);
  listCtrlMovement->SetColumnWidth(3, 200);


  // Load data

  LoadBankList();

  LoadAccountList();

  LoadMovements();
}


void PanelBankAccountEdit::OnCreationDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditCreationDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCreationDateChoose->GetScreenPosition().x;
  calendarPosition.y = btnCreationDateChoose->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditCreationDate->SetValue(calendarDialog.selectedDay + wxT("/") +
                                calendarDialog.selectedMonth + wxT("/") +
                                calendarDialog.selectedYear);
  }
}


int PanelBankAccountEdit::LoadData(int id)
{
  this->accountId = id;

  DBQuery dbquery;

  std::vector<wxString> params;

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT account_type_id, bank_id, accounting_account_id, codigo, agency, titular_nome, status, ")
          wxT("to_char(creation_dt, 'DD/MM/YYYY') AS creation_dt ")
          wxT("FROM ") + DB::GetTableName("bank.account") + wxT(" ")
          wxT("WHERE account_id = $1");
  }
  else {
    sql = wxT("SELECT account_type_id, bank_id, accounting_account_id, codigo, agency, titular_nome, status, ")
          wxT("strftime('%d/%m/%Y', creation_dt) AS creation_dt ")
          wxT("FROM ") + DB::GetTableName("bank.account") + wxT(" ")
          wxT("WHERE account_id = $1");
  }

  params.push_back(wxString::Format(wxT("%d"), this->accountId));

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Não foi possível obter as informações sobre a conta bancária."));

    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("DB error: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);

    return -1;
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("As informações sobre a conta bancária não foram encontradas."));

    return -1;
  }

  teditOwnerName->SetValue(dbquery.GetFieldStrByName(0, wxT("titular_nome")));

  teditNumber->SetValue(dbquery.GetFieldStrByName(0, wxT("codigo")));

  teditAgency->SetValue(dbquery.GetFieldStrByName(0, wxT("agency")));

  teditCreationDate->SetValue(dbquery.GetFieldStrByName(0, wxT("creation_dt")));

  if (dbquery.GetFieldStrByName(0, wxT("account_type_id")).Length() > 0) {
    for (size_t index = 0; index < choiceType->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceType->GetClientObject(index);

      if (dbquery.GetFieldAnsiStrByName(0, wxT("account_type_id"))[0] == container->GetData()) {
        choiceType->SetSelection(index);

        break;
      }
    }
  }

  if (dbquery.GetFieldStrByName(0, wxT("bank_id")).Length() > 0) {
    for (size_t index = 0; index < choiceBank->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceBank->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("bank_id")) == container->GetData()) {
        choiceBank->SetSelection(index);

        break;
      }
    }
  }

  if (dbquery.GetFieldStrByName(0, wxT("accounting_account_id")).Length() > 0) {
    for (size_t index = 0; index < choiceAccountingAccount->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceAccountingAccount->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("accounting_account_id")) == container->GetData()) {
        choiceAccountingAccount->SetSelection(index);

        break;
      }
    }
  }

  // Load account movements

  LoadMovements();

  return 0;
}


int PanelBankAccountEdit::LoadMovements()
{
  listCtrlMovement->DeleteAllItems();

  DBQuery dbquery;
  DBQuery dbqueryPhone;
  std::vector<wxString> params;

  LogMessenger *messenger = LogMessenger::GetInstance();

  params.push_back(wxString::Format("%d", this->accountId));

  messenger->Register(wxString::Format("ID da conta: %d", this->accountId), LogMessage::MSG_DBG);

  wxString sql = wxT("SELECT m.movement_id, m.valor, m.status, m.register_dt, m.operation_dt, ")
                 wxT("m.account_movement_type_id, mt.nome AS movimentacao_nome, ")
                 wxT("b.bank_id, b.nome AS banco_nome ")
                 wxT("FROM ") + DB::GetTableName("bank.account.event") + wxT(" m ")
                 wxT("LEFT JOIN ") + DB::GetTableName("bank.account.event.type") + wxT(" mt ON (m.account_movement_type_id = mt.account_movement_type_id) ")
                 wxT("LEFT JOIN ") + DB::GetTableName("bank.account") + wxT(" c ON (c.account_id = m.account_id) ")
                 wxT("LEFT JOIN ") + DB::GetTableName("bank") + wxT(" b ON (c.bank_id = b.bank_id) ")
                 wxT("WHERE c.account_id = $1 ")
                 wxT("ORDER BY m.operation_dt DESC, m.register_dt DESC, m.valor");

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    messenger->Register(wxT("Erro na obtenção da movimentação da conta bancária."), LogMessage::MSG_ERROR);

    messenger->Register(wxT("DB error: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlMovement->InsertItem(resultIndex, Monetary::Normalize(dbquery.GetFieldStrByName(resultIndex, wxT("valor"))), 0);

      listCtrlMovement->SetItemData(resultIndex, dbquery.GetFieldLongByName(resultIndex, wxT("movement_id")));

      listCtrlMovement->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("movimentacao_nome")));

      listCtrlMovement->SetItem(resultIndex, 2, dbquery.GetFieldStrByName(resultIndex, wxT("operation_dt")));

      listCtrlMovement->SetItem(resultIndex, 3, dbquery.GetFieldStrByName(resultIndex, wxT("register_dt")));

      GUI::ListSetRowBgColor(listCtrlMovement, resultIndex);
    }
  }

  return 0;
}


void PanelBankAccountEdit::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("PanelBankAccountEdit::OnSaveRequest"), LogMessage::MSG_DBG);

  if (teditOwnerName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("O nome do titular da conta não foi informado."));

    return;
  }

  if (choiceBank->GetSelection() < 0 || choiceBank->GetClientObject(choiceBank->GetSelection()) == NULL) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("O banco não foi informado."));

    return;
  }

  if (teditNumber->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("O número da conta bancária não foi informada."));

    return;
  }

  if (teditAgency->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("A agência da conta bancária não foi informada."));

    return;
  }

  if (choiceAccountingAccount->GetSelection() < 0 || choiceAccountingAccount->GetClientObject(choiceAccountingAccount->GetSelection()) == NULL) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("A conta contábil não foi informada."));

    return;
  }


  DBQuery dbquery;
  std::vector<wxString> fields, params, valueModifiers;
  bool registeringError = false;
  wxString errorMessageStr = wxT("");

  // Init transaction

  int rc = DBTransaction::Begin();

  if (rc != 0) {
    registeringError = true;

    errorMessageStr = wxT("Erro interno do banco: ") + dbquery.GetMessageStr();
  }

  if (registeringError) {
    wxMessageBox(wxT("Não foi possível iniciar a transação."));

    return;
  }

  // Inserting main information

  if (choiceType->GetClientObject(choiceType->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) choiceType->GetClientObject(choiceType->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("account_type_id"));

      params.push_back(wxString::Format(wxT("%c"), container->GetData()));

      valueModifiers.push_back("");
    }
  }

  if (teditOwnerName->GetValue().Length() > 0) {
    fields.push_back(wxT("titular_nome"));

    params.push_back(teditOwnerName->GetValue());

    valueModifiers.push_back("");
  }

  if (choiceBank->GetClientObject(choiceBank->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) choiceBank->GetClientObject(choiceBank->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("bank_id"));

      params.push_back(wxString::Format(wxT("%d"), container->GetData()));

      valueModifiers.push_back("");
    }
  }

  if (teditNumber->GetValue().Length() > 0) {
    fields.push_back(wxT("codigo"));

    params.push_back(teditNumber->GetValue());

    valueModifiers.push_back("");
  }

  if (teditAgency->GetValue().Length() > 0) {
    fields.push_back(wxT("agency"));

    params.push_back(teditAgency->GetValue());

    valueModifiers.push_back("");
  }

  if (choiceAccountingAccount->GetClientObject(choiceAccountingAccount->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountingAccount->GetClientObject(choiceAccountingAccount->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("accounting_account_id"));

      params.push_back(wxString::Format(wxT("%d"), container->GetData()));

      valueModifiers.push_back("");
    }
  }

  if (teditCreationDate->GetValue().Length() > 0) {
    fields.push_back(wxT("creation_dt"));

    if (DB::GetType() == DB_TYPE_POSTGRESQL) {
      params.push_back(teditCreationDate->GetValue());

      valueModifiers.push_back("to_date($@, 'DD/MM/YYYY')");
    }
    else {
      wxDateTime dateTime;

      dateTime.ParseFormat(teditCreationDate->GetValue(), wxT("%d/%m/%Y"));

      params.push_back(dateTime.Format("%Y-%m-%d"));

      valueModifiers.push_back("");
    }
  }

  wxString sql = DBUtils::MountSQLUpdate(DB::GetTableName("bank.account"),
                                         fields,
                                         wxT("account_id = ") + wxString::Format(wxT("%d"), this->accountId),
                                         valueModifiers);

  rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messenger->Register(wxT("DB error: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }


  // Successfully registering

  if (!registeringError) {
    // Commit transaction

    rc = DBTransaction::Commit();


    // Close panel

    wxString pageTitle = wxT("Nova Conta Bancária");

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    // Reload main panel data

    PanelBankMain *panelBankMain = (PanelBankMain *) mainFrame->GetPage(wxT("Bancos"));

    if (panelBankMain) {
      panelBankMain->LoadData();
    }

    // Remove this panel

    mainFrame->PageRemove(pageTitle);
  }
  else {
    // Rollback transaction

    rc = DBTransaction::Rollback();

    wxMessageBox(wxT("Não foi possível modificar as informações sobre a conta bancária."), wxT("Erro"), wxOK | wxICON_ERROR);
  }
}


void PanelBankAccountEdit::OnDeleteRequest(wxCommandEvent &event) {
  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  if (wxYES != wxMessageBox(wxT("Você confirma a exclusão das informações sobre a conta bancária?"),
                            wxT("Confirmação de exclusão de informações sobre conta bancária"),
                            wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION,
                            this)) {

    mainFrame->SetStatusText(wxT("A exclusão das informações sobre a conta bancária foi cancelada."));

    return;
  }

  wxString sqlCommand = wxString::Format(wxT("DELETE FROM ") + DB::GetTableName("bank.account") + wxT(" ") +
                                         wxT("WHERE account_id = %d"),
                                         accountId);

  DBQuery dbquery;

  int rc = dbquery.Execute(sqlCommand);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível excluir as informações sobre a conta bancária."),
                 wxT("Erro"),
                 wxOK,
                 this);
  }
  else {
    PanelBankMain *panel = (PanelBankMain *) mainFrame->GetPage(Lang::Get("Banks"));

    if (panel) {
      panel->LoadData();
    }

    wxString pageTitle = Lang::Get("Bank account") + wxT(" #") + wxString::Format("%d", accountId);

    mainFrame->PageRemove(pageTitle);
  }
}


void PanelBankAccountEdit::OnCloseRequest(wxCommandEvent& event)
{
  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString pageTitle = wxT("Nova Conta Bancária");

  mainFrame->PageRemove(pageTitle);
}


void PanelBankAccountEdit::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == 341) {
    /*
    ProductTypeNewDialog productTypeNewDialog(this);

    productTypeNewDialog.ShowModal();

    LoadList();
    */
  }
  // Exit <ESC>
  /*else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }
  */

  event.Skip();
}


int PanelBankAccountEdit::LoadBankList()
{
  DBQuery dbquery;
  wxString sql = wxT("SELECT bank_id, compe, nome ")
                 wxT("FROM ") + DB::GetTableName("bank") + wxT(" ")
                 wxT("ORDER BY nome");

  choiceBank->Freeze();

  choiceBank->Clear();

  choiceBank->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData(0));

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem dos bancos cadastrados."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {

    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      wxString caption = wxT("");

      caption += dbquery.GetFieldStrByName(index, wxT("nome"));

      if (dbquery.GetFieldStrByName(index, wxT("compe")).Length() > 0) {
        caption += wxT(" (") + dbquery.GetFieldStrByName(index, wxT("compe")) + wxT(")");
      }

      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("bank_id")));

      choiceBank->Append(caption, container);
    }
  }

  choiceBank->SetSelection(0);

  choiceBank->Thaw();

  return 0;
}


int PanelBankAccountEdit::LoadAccountList()
{
  DBQuery dbquery;

  choiceAccountingAccount->Freeze();

  choiceAccountingAccount->Clear();

  choiceAccountingAccount->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData(0));

  int rc = dbquery.Execute("SELECT * "
                           "FROM " + DB::GetTableName("accounting.account") + " "
                           "WHERE tipo_id = 'A' "
                           "AND conta_parent_id NOT IN (SELECT account_id "
                                                       "FROM " + DB::GetTableName("bank.account") + " "
                                                         "WHERE conta_parent_id IS NOT NULL) "
                           "ORDER BY lower(nome)");

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de contas de saída cadastradas."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, "account_id"));

      choiceAccountingAccount->Append(dbquery.GetFieldStrByName(index, "nome"), container);
    }
  }

  choiceAccountingAccount->SetSelection(0);

  choiceAccountingAccount->Thaw();

  return 0;
}


void PanelBankAccountEdit::OnUpdateBalanceRequest(wxCommandEvent& event)
{
  BankAccountBalanceUpdate dialog(this);

  dialog.LoadData(this->accountId);

  dialog.ShowModal();

  LoadMovements();
}


BEGIN_EVENT_TABLE(PanelBankAccountEdit, wxScrolledWindow)
  EVT_TOOL(PANEL_BANK_ACCOUNT_EDIT_TOOL_SAVE_ID, PanelBankAccountEdit::OnSaveRequest)
  EVT_TOOL(PANEL_BANK_ACCOUNT_EDIT_TOOL_DELETE_ID, PanelBankAccountEdit::OnDeleteRequest)
  EVT_TOOL(PANEL_BANK_ACCOUNT_EDIT_TOOL_BALANCE_UPDATE_ID, PanelBankAccountEdit::OnUpdateBalanceRequest)
  EVT_CHAR_HOOK(PanelBankAccountEdit::OnCharHook)
  EVT_BUTTON(PANEL_BANK_ACCOUNT_EDIT_BUTTON_CREATION_DATE_CHOOSE_ID, PanelBankAccountEdit::OnCreationDateChooseRequest)
END_EVENT_TABLE()