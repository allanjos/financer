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
#include "panel_bank_account_new.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "dlg_calendar.hpp"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/calendar_16x16.xpm"


PanelBankAccountNew::PanelBankAccountNew(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
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

  toolBar->AddTool(PANEL_BANK_ACCOUNT_NEW_TOOL_SAVE_ID,
                   Lang::Get("Save"),
                   wxBitmap(save_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);


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
  szgMainFields->Add(choiceType, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

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

  teditCreationDate = new wxTextCtrl(panelMain, PANEL_BANK_ACCOUNT_NEW_TEDIT_CREATION_DATE_ID, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditCreationDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCreationDateChoose = new wxBitmapButton(panelMain,
                                             PANEL_BANK_ACCOUNT_NEW_BUTTON_CREATION_DATE_CHOOSE_ID,
                                             wxBitmap(calendar_16x16_xpm));
  szRow->Add(btnCreationDateChoose, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCreationDateChoose->SetMinSize(wxSize(-1, teditCreationDate->GetSize().GetHeight()));

  // Accounting account

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Ledger account") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  choiceAccountingAccount = new wxChoice(panelMain, wxID_ANY);
  szgMainFields->Add(choiceAccountingAccount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  // Opening balance

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Opening balance") + wxT(": "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditBalance = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgMainFields->Add(teditBalance, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditBalance->SetMaxLength(20);


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


  LoadBankList();

  LoadAccountList();

  // Events

  teditBalance->Connect(wxEVT_KILL_FOCUS,
                        wxFocusEventHandler(PanelBankAccountNew::OnBalanceKillFocus),
                        NULL,
                        this);

  teditBalance->SetValue(wxT("0"));

  teditBalance->SetFocus();


  // Set initial focus

  teditOwnerName->SetFocus();
}


void PanelBankAccountNew::OnCreationDateChooseRequest(wxCommandEvent &event)
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


void PanelBankAccountNew::OnBalanceKillFocus(wxFocusEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditBalance != NULL && teditBalance->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditBalance->GetValue())) {
      messenger->Register(Lang::Get("The opening balance is not valid") + wxT("."), LogMessage::MSG_INFO);
    }
    else {
      teditBalance->SetValue(Monetary::Normalize(teditBalance->GetValue()));
    }
  }

  event.Skip();
}


void PanelBankAccountNew::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

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

  DBQuery dbquery;
  std::vector<wxString> fields, params, valueModifiers;
  bool registeringError = false;
  int bankAccountId;

  // Init transaction

  int rc = DBTransaction::Begin();

  if (rc != 0) {
    registeringError = true;

    messenger->Register(wxT("Erro interno do banco: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }

  if (registeringError) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Não foi possível iniciar a transação."));

    return;
  }

  // Inserting main information

  if (choiceType->GetClientObject(choiceType->GetSelection()) != NULL) {
    fields.push_back(wxT("account_type_id"));

    ContainerIntData *container = (ContainerIntData *) choiceType->GetClientObject(choiceType->GetSelection());

    params.push_back(wxString::Format(wxT("%c"), container->GetData()));

    valueModifiers.push_back("");
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

  if (choiceAccountingAccount->GetClientObject(choiceAccountingAccount->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountingAccount->GetClientObject(choiceAccountingAccount->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("accounting_account_id"));

      params.push_back(wxString::Format(wxT("%d"), container->GetData()));

      valueModifiers.push_back("");
    }
  }

  wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("bank.account"), fields, valueModifiers);

  messenger->Register(sql, LogMessage::MSG_DBG);

  rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messenger->Register(wxT("DB error: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);
  }

  // Get the account inserted ID from the database

  if (!registeringError) {
    messenger->Register("Getting bank account id.", LogMessage::MSG_DBG);

    bankAccountId = DBUtils::GetSerialKey(DB::GetTableName("bank.account"), "account_id");

    if (bankAccountId < 1) {
      registeringError = true;

      messenger->Register(wxT("Não foi possível obter a identificação da conta bancária cadastrada."), LogMessage::MSG_ERROR);

      messenger->Register(wxT("DB error: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);
    }
  }

  // Registering the balance

  if (!registeringError) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("account_id"));
    params.push_back(wxString::Format(wxT("%d"), bankAccountId));

    fields.push_back(wxT("account_movement_type_id"));
    params.push_back(wxT("A"));

    fields.push_back(wxT("valor"));
    params.push_back(Monetary::Monetary2Database(teditBalance->GetValue()));

    wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("bank.account.event"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      messenger->Register(wxT("Não foi possível registrar o saldo inicial informado para a conta bancária."), LogMessage::MSG_ERROR);

      messenger->Register(wxT("DB error: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);
    }
  }

  // Successfully registering

  if (!registeringError) {
    // Commit transaction

    rc = DBTransaction::Commit();

    // Close panel

    wxString pageTitle = Lang::Get("New bank account");

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    // Reload main panel data

    PanelBankMain *panelBankMain = (PanelBankMain *) mainFrame->GetPage(Lang::Get("Banks"));

    if (panelBankMain) {
      panelBankMain->LoadData();
    }

    // Remove this panel

    mainFrame->PageRemove(pageTitle);
  }
  else {
    // Rollback transaction

    rc = DBTransaction::Rollback();

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Não foi possível cadastrar as informações sobre a conta bancária."));
  }
}


void PanelBankAccountNew::OnCloseRequest(wxCommandEvent& event)
{
  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString pageTitle = Lang::Get("New bank account");

  mainFrame->PageRemove(pageTitle);
}


void PanelBankAccountNew::OnCharHook(wxKeyEvent& event)
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


int PanelBankAccountNew::LoadBankList()
{
  DBQuery dbquery;
  wxString sql = wxT("SELECT bank_id, compe, nome ")
                 wxT("FROM ") + DB::GetTableName("bank") + wxT(" ")
                 wxT("ORDER BY nome");

  choiceBank->Freeze();

  choiceBank->Clear();

  choiceBank->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de bancos cadastrados."), LogMessage::MSG_ERROR);
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


int PanelBankAccountNew::LoadAccountList()
{
  DBQuery dbquery;

  choiceAccountingAccount->Freeze();

  choiceAccountingAccount->Clear();

  choiceAccountingAccount->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  choiceAccountingAccount->SetSelection(0);

  int rc = dbquery.Execute("SELECT * "
                           "FROM " + DB::GetTableName("accounting.account") + " "
                           "WHERE tipo_id = 'A' "
                           "AND conta_parent_id NOT IN (SELECT account_id "
                                                       "FROM " + DB::GetTableName("bank.account") + " "
                                                         "WHERE conta_parent_id IS NOT NULL) "
                           "ORDER BY lower(nome)");

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de contas contábeis cadastradas."), LogMessage::MSG_ERROR);
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


BEGIN_EVENT_TABLE(PanelBankAccountNew, wxScrolledWindow)
  EVT_TOOL(PANEL_BANK_ACCOUNT_NEW_TOOL_SAVE_ID, PanelBankAccountNew::OnSaveRequest)
  EVT_CHAR_HOOK(PanelBankAccountNew::OnCharHook)
  EVT_BUTTON(PANEL_BANK_ACCOUNT_NEW_BUTTON_CREATION_DATE_CHOOSE_ID, PanelBankAccountNew::OnCreationDateChooseRequest)
END_EVENT_TABLE()