/**
 *
 * \file  dlg_conf_currency.cpp
 * \version 1.0
 * \since 08/22/2007 00:50:55 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "gui_controls.hpp"
#include "utils_monetary.hpp"
#include "utils_config.hpp"
#include "utils_lang.hpp"
#include "dlg_bank_account_balance_update.hpp"
#include "dlg_main.hpp"
#include "dlg_calendar.hpp"
#include "pixmaps/config_16x16.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"
#include "pixmaps/calendar_bmp_btn.xpm"


BankAccountBalanceUpdate::BankAccountBalanceUpdate(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxT("Atualização de saldo de conta bancária"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  SetAutoLayout(true);

  SetSize(wxSize(600, 300));

  SetIcon(wxIcon(config_16x16_xpm));


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);

  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this, 
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_BANK_ACCOUNT_BALANCE_UPDATE_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_BANK_ACCOUNT_BALANCE_UPDATE_TOOL_CLOSE_ID, Lang::Get("Close"), wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);

  szTop->AddSpacer(5);

  // Fields

  wxFlexGridSizer *szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szTop->Add(szgMainFields, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Account type

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, wxT("Tipo de conta: "), wxDefaultPosition, wxSize(150, -1), wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  labelAccountType = new wxStaticText(this, wxID_ANY, wxT(""));
  szgMainFields->Add(labelAccountType, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Account owner name

  label = new wxStaticText(this, wxID_STATIC, wxT("Titular: "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  labelOwnerName = new wxStaticText(this, wxID_ANY, wxT(""));
  szgMainFields->Add(labelOwnerName, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

  // Account type

  label = new wxStaticText(this, wxID_STATIC, wxT("Banco: "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  labelBankName = new wxStaticText(this, wxID_ANY, wxT(""));
  szgMainFields->Add(labelBankName, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Account number

  label = new wxStaticText(this, wxID_STATIC, wxT("Número da conta: "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  labelAccountNumber = new wxStaticText(this, wxID_ANY, wxT(""));
  szgMainFields->Add(labelAccountNumber, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Bank agency

  label = new wxStaticText(this, wxID_STATIC, wxT("Agência: "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  labelAccountAgency = new wxStaticText(this, wxID_ANY, wxT(""));
  szgMainFields->Add(labelAccountAgency, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // New balance

  label = new wxStaticText(this, wxID_STATIC, wxT("Novo saldo: "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditNewBalance = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1), wxALIGN_RIGHT);
  szgMainFields->Add(teditNewBalance, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Balance date

  label = new wxStaticText(this, wxID_STATIC, wxT("Data: "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  teditBalanceDate = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(75, -1));
  szRow->Add(teditBalanceDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarBalanceDate = new wxBitmapButton(this,
                                              wxID_ANY,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition, wxDefaultSize);
  szRow->Add(btnCalendarBalanceDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarBalanceDate->SetMinSize(wxSize(-1, teditBalanceDate->GetSize().GetHeight()));

  // Default date

  teditBalanceDate->SetValue(wxDateTime::Now().Format(wxT("%d/%m/%Y")));


  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szTop->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);


  // Top sizer ending configuration.

  this->CenterOnParent();

  // Event handlers

  btnCalendarBalanceDate->Connect(wxEVT_BUTTON,
                                  wxCommandEventHandler(BankAccountBalanceUpdate::OnBalanceDateChooseRequest),
                                  NULL,
                                  this);

  teditNewBalance->Connect(wxEVT_KILL_FOCUS,
                           wxFocusEventHandler(BankAccountBalanceUpdate::OnBalanceValueKillFocus),
                           NULL,
                           this);

  teditNewBalance->SetFocus();
}


void BankAccountBalanceUpdate::Terminate()
{
  Close();

  Destroy();
}


int BankAccountBalanceUpdate::LoadData(int id)
{
  this->accountId = id;

  DBQuery dbquery;

  std::vector<wxString> params;

  // Person information

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT c.account_type_id, c.bank_id, c.accounting_account_id, c.codigo, c.agency, c.titular_nome, c.status, ")
          wxT("to_char(c.creation_dt, 'DD/MM/YYYY') AS creation_dt, b.nome AS bank_name ")
          wxT("FROM ") + DB::GetTableName("bank.account") + wxT(" c ")
          wxT("LEFT JOIN ") + DB::GetTableName("bank") +  wxT(" b ON (b.bank_id = c.bank_id) ")
          wxT("WHERE c.account_id = $1");
  }
  else {
    sql = wxT("SELECT c.account_type_id, c.bank_id, c.accounting_account_id, c.codigo, c.agency, c.titular_nome, c.status, ")
          wxT("strftime('%d/%m/%Y', c.creation_dt) AS creation_dt, b.nome AS bank_name ")
          wxT("FROM ") + DB::GetTableName("bank.account") + wxT(" c ")
          wxT("LEFT JOIN ") + DB::GetTableName("bank") +  wxT(" b ON (b.bank_id = c.bank_id) ")
          wxT("WHERE c.account_id = $1");
  }

  params.push_back(wxString::Format(wxT("%d"), accountId));

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre a conta bancária.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("As informações sobre a conta bancária não foram encontradas."));

    return -1;
  }

  labelOwnerName->SetLabel(dbquery.GetFieldStrByName(0, wxT("titular_nome")));

  labelAccountAgency->SetLabel(dbquery.GetFieldStrByName(0, wxT("agency")));

  labelAccountNumber->SetLabel(dbquery.GetFieldStrByName(0, wxT("codigo")));

  labelBankName->SetLabel(dbquery.GetFieldStrByName(0, wxT("bank_name")));

  if (dbquery.GetFieldStrByName(0, wxT("account_type_id")).Length() > 0) {
    if (dbquery.GetFieldAnsiStrByName(0, wxT("account_type_id"))[0] == 'C')
      labelAccountType->SetLabel(wxT("Corrente"));
    else if (dbquery.GetFieldAnsiStrByName(0, wxT("account_type_id"))[0] == 'P')
      labelAccountType->SetLabel(wxT("Conta-Poupança"));
    else if (dbquery.GetFieldAnsiStrByName(0, wxT("account_type_id"))[0] == 'S')
      labelAccountType->SetLabel(wxT("Conta-Salário"));
    else if (dbquery.GetFieldAnsiStrByName(0, wxT("account_type_id"))[0] == 'D')
      labelAccountType->SetLabel(wxT("CDB"));
    else
      labelAccountType->SetLabel(wxT("Não informado"));
  }

  return 0;
}


void BankAccountBalanceUpdate::OnBalanceDateChooseRequest(wxCommandEvent& event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditBalanceDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarBalanceDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarBalanceDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditBalanceDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                               calendarDialog.selectedMonth + wxT("/") + 
                               calendarDialog.selectedYear);
  }
}


void BankAccountBalanceUpdate::OnSaveRequest(wxCommandEvent& event)
{
  std::vector<wxString> fields, params, valueModifiers;

  fields.push_back(wxT("account_id"));
  params.push_back(wxString::Format(wxT("%d"), accountId));
  valueModifiers.push_back(wxEmptyString);

  fields.push_back(wxT("account_movement_type_id"));
  params.push_back(wxT("A"));
  valueModifiers.push_back(wxEmptyString);

  fields.push_back(wxT("valor"));
  params.push_back(Monetary::Monetary2Database(teditNewBalance->GetValue()));
  valueModifiers.push_back(wxEmptyString);

  if (teditBalanceDate->GetValue().Length() > 0) {
    fields.push_back(wxT("operation_dt"));

    if (DB::GetType() == DB_TYPE_POSTGRESQL) {
      params.push_back(teditBalanceDate->GetValue());

      valueModifiers.push_back("to_date($@, 'DD/MM/YYYY')");
    }
    else {
      wxDateTime dateTime;

      dateTime.ParseFormat(teditBalanceDate->GetValue(), wxT("%d/%m/%Y"));

      params.push_back(dateTime.Format("%Y-%m-%d"));

      valueModifiers.push_back("");
    }
  }

  wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("bank.account.event"), fields, valueModifiers);

  DBQuery dbquery;

  int rc = dbquery.Execute(sql, params);

  bool registeringError = false;

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível registrar o saldo inicial informado para a conta bancária."));

    registeringError = true;
  }

  // Successfully registering

  if (!registeringError) {
    // Commit transaction

    rc = DBTransaction::Commit();

    Close();
  }
  else {
    // Rollback transaction

    DBTransaction::Rollback();

    wxMessageBox(wxT("Ocorreu erro na atualização do saldo. Revise as informações."), wxT("Erro"), wxOK | wxICON_ERROR);
  }
}


void BankAccountBalanceUpdate::OnCloseRequest(wxCommandEvent& event)
{
  Terminate();
}


void BankAccountBalanceUpdate::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Terminate();
  }
  else {
    event.Skip();
  }
}


void BankAccountBalanceUpdate::OnBalanceValueKillFocus(wxFocusEvent& event)
{
  if (teditNewBalance != NULL && teditNewBalance->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditNewBalance->GetValue())) {
      teditNewBalance->SetForegroundColour(*wxRED);

      statusBar->SetStatusText(wxT("O valor do pagamento não é válido."));
    }
    else {
      teditNewBalance->SetForegroundColour(*wxBLACK);

      teditNewBalance->SetValue(Monetary::Normalize(teditNewBalance->GetValue()));

      statusBar->SetStatusText("");
    }
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(BankAccountBalanceUpdate, wxDialog)
  EVT_TOOL(DLG_BANK_ACCOUNT_BALANCE_UPDATE_TOOL_SAVE_ID, BankAccountBalanceUpdate::OnSaveRequest)
  EVT_TOOL(DLG_BANK_ACCOUNT_BALANCE_UPDATE_TOOL_CLOSE_ID, BankAccountBalanceUpdate::OnCloseRequest)
  EVT_CHAR_HOOK(BankAccountBalanceUpdate::OnCharHook)
END_EVENT_TABLE()