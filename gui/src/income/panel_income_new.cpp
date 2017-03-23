/**
 *
 * \version 1.0
 * \since 08/19/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_image.hpp"
#include "utils_lang.hpp"
#include "utils_config.hpp"
#include "utils_payment.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "panel_income_new.hpp"
#include "panel_income_expense.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "pixmaps/save_16x16.xpm"


PanelIncomeNew::PanelIncomeNew(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  this->isCanceled = true;

  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  // Main sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  wxBoxSizer *szRow;

  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_ESTATE_NEW_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Title

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 5);

  wxFont font;

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, Lang::Get("Name: "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szRow->Add(teditName, 1, wxALL | wxEXPAND, 1);


  // Notebook

  wxNotebook *notebook= new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxEXPAND | wxALL | wxGROW, 1);


  // Main tab

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxHSCROLL | wxVSCROLL);

  panelMain->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMain, Lang::Get("General"), true);


  wxBoxSizer *szMarginMain = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMarginMain);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMarginMain->Add(szMain, 1, wxALL | wxEXPAND | wxGROW, 10);

  // Tab content

  wxFlexGridSizer* szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szgMainFields, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Expense subtype

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Category") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  choiceIncomeCategory = new wxChoice(panelMain, DLG_INCOME_NEW_CHOICE_EXPENSE_CATEGORY_ID);
  szgMainFields->Add(choiceIncomeCategory, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Work type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Work hours") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  choiceWorkType = new wxChoice(panelMain, DLG_INCOME_NEW_CHOICE_EXPENSE_CATEGORY_ID);
  szgMainFields->Add(choiceWorkType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  choiceWorkType->Append("", (wxClientData *) NULL);

  // Payment date

  labelPaymentDate = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Payment date") + wxT(": "));
  szgMainFields->Add(labelPaymentDate, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentDay = new wxSpinCtrl(panelMain, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 31, 1);
  szRow->Add(spinPaymentDay, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentMonth = new wxSpinCtrl(panelMain, DLG_INCOME_NEW_CHOICE_PAYMENT_MONTH_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 12, 1);
  szRow->Add(spinPaymentMonth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentYear = new wxSpinCtrl(panelMain, DLG_INCOME_NEW_CHOICE_PAYMENT_YEAR_ID, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 1850, 3000, 2009);
  szRow->Add(spinPaymentYear, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Payment period

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Frequency of payment") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  choicePaymentInterval = new wxChoice(panelMain, DLG_INCOME_NEW_CHOICE_PAYMENT_INTERVAL_ID);
  szgMainFields->Add(choicePaymentInterval, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  choicePaymentInterval->Append(Lang::Get("Single lump"), (wxClientData *) new ContainerIntData((int) 'U'));
  choicePaymentInterval->Append(Lang::Get("Daily"), (wxClientData *) new ContainerIntData((int) 'D'));
  choicePaymentInterval->Append(Lang::Get("Weekly"), (wxClientData *) new ContainerIntData((int) 'S'));
  choicePaymentInterval->Append(Lang::Get("Fortnightly"), (wxClientData *) new ContainerIntData((int) 'Q'));
  choicePaymentInterval->Append(Lang::Get("Monthly"), (wxClientData *) new ContainerIntData((int) 'M'));
  choicePaymentInterval->Append(Lang::Get("Semiannual"), (wxClientData *) new ContainerIntData((int) 'W'));
  choicePaymentInterval->Append(Lang::Get("Anual"), (wxClientData *) new ContainerIntData((int) 'A'));

  choicePaymentInterval->SetSelection(0);

  // Account input

  wxStaticText *lblAccountParent = new wxStaticText(panelMain,
                                                    wxID_STATIC,
                                                    wxT("Conta para crédito: "),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szgMainFields->Add(lblAccountParent, 0, wxALL|wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  choiceAccountInput = new wxChoice(panelMain, wxID_ANY);
  szgMainFields->Add(choiceAccountInput, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Expense amount

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Amount") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  teditAmountValue = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditAmountValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, Config::Get("currency.abbr"));
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  checkNotFixedAmount = new wxCheckBox(panelMain, DLG_INCOME_NEW_CHOICE_AMOUNT_NOT_FIXED_ID, Lang::Get("The income does not have a fixed amount") + wxT("."));
  szRow->Add(checkNotFixedAmount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Expense installment value

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Installment amount") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  teditInstallmentValue = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditInstallmentValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, Config::Get("currency.abbr"));
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Expense is paid

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Paid") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnPaid = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnPaid->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnPaid, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnNotPaid = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("No"));
  radioBtnNotPaid->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnNotPaid, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  radioBtnNotPaid->SetValue(true);

  // Expense status

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Status: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusEnabled = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnStatusEnabled->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnStatusEnabled, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusDisabled = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Disabled"));
  radioBtnStatusDisabled->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnStatusDisabled, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);


  szgMainFields->AddGrowableCol(1);


  // Notes tab

  wxScrolledWindow *panelNotes = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxHSCROLL | wxVSCROLL);

  panelNotes->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);


  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, 10);


  // Notes

  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1, 30), wxTE_MULTILINE);
  szNotes->Add(teditNotes, 1, wxALL | wxEXPAND, 1);


  // Load data

  SetAmountAsNotFixed();

  LoadIncomeCategories();

  LoadWorkTypes();

  // Configure initial payment date

  wxDateTime dateTimeNow = wxDateTime::Now();

  wxDateTime::Month month = dateTimeNow.GetMonth();

  int year = dateTimeNow.GetYear();

  OnDateChange();

  short unsigned int day = dateTimeNow.GetDay();

  //wxMessageBox(wxString::Format(wxT("day: %d, month: %d, year: %d"), day, (int) month, year));

  spinPaymentDay->SetValue(day);
  spinPaymentMonth->SetValue((int) month + 1);
  spinPaymentYear->SetValue(year);


  LoadAccountList();


  teditName->SetFocus();


  teditAmountValue->Connect(wxEVT_KILL_FOCUS,
                            wxFocusEventHandler(PanelIncomeNew::OnAmountValueKillFocus),
                            NULL,
                            this);

  teditInstallmentValue->Connect(wxEVT_KILL_FOCUS,
                                 wxFocusEventHandler(PanelIncomeNew::OnInstallmentValueKillFocus),
                                 NULL,
                                 this);

  spinPaymentMonth->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                            wxSpinEventHandler(PanelIncomeNew::OnPaymentMonthSelect),
                            NULL,
                            this);

  spinPaymentYear->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                           wxSpinEventHandler(PanelIncomeNew::OnPaymentYearSelect),
                           NULL,
                           this);
}


void PanelIncomeNew::Terminate()
{
  Close();

  Destroy();
}


int PanelIncomeNew::LoadAccountList()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  DBQuery dbquery;

  choiceAccountInput->Freeze();

  choiceAccountInput->Clear();

  choiceAccountInput->Append("Selecione", (wxClientData *) new ContainerIntData(0));

  choiceAccountInput->SetSelection(0);

  wxString sql = wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
                 wxT(" WHERE tipo_id = 'A'")
                 wxT(" AND conta_parent_id NOT IN (SELECT account_id FROM ") + DB::GetTableName("accounting.account") + wxT(" ")
                                              wxT("WHERE conta_parent_id IS NOT NULL)")
                 wxT(" ORDER BY codigo, lower(nome)");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas-contábeis."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, "account_id"));

      choiceAccountInput->Append(dbquery.GetFieldStrByName(index, "nome"), container);
    }
  }

  choiceAccountInput->SetSelection(0);

  choiceAccountInput->Thaw();

  return 0;
}


unsigned long PanelIncomeNew::GetExpenseId()
{
  return this->expenseId;
}


bool PanelIncomeNew::IsCanceled()
{
  return this->isCanceled;
}


void PanelIncomeNew::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditName->GetValue().Length() < 1) {
    messenger->Register(wxT("É necessário informar o nome descritivo para a receita."), LogMessage::MSG_INFO);

    return;
  }

  if (!radioBtnStatusEnabled->GetValue() && !radioBtnStatusDisabled->GetValue()) {
    messenger->Register(wxT("É necessário informar se a empresa está ativa ou inativa."), LogMessage::MSG_INFO);

    return;
  }


  std::vector<wxString> params, fields;
  bool registeringError = false;
  wxString messageStr = wxT("");
  wxString sql = wxT("");

  // Product name

  fields.push_back(wxT("nome"));
  params.push_back(teditName->GetValue());

  // Status

  fields.push_back(wxT("status"));

  if (radioBtnStatusEnabled->GetValue()) {
    params.push_back(wxT("A"));
  }
  else {
    params.push_back(wxT("I"));
  }

  if (choiceIncomeCategory->GetSelection() > 0 && choiceIncomeCategory->GetClientObject(choiceIncomeCategory->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceIncomeCategory->GetClientObject(choiceIncomeCategory->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("income_category_id"));
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    }
  }

  if (choiceWorkType->GetSelection() >= 0 && choiceWorkType->GetClientObject(choiceWorkType->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceWorkType->GetClientObject(choiceWorkType->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("regime_trabalho_id"));
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    }
  }

  if (spinPaymentDay->IsEnabled()) {
    fields.push_back(wxT("pagamento_dia"));
    params.push_back(wxString::Format(wxT("%d"), spinPaymentDay->GetValue()));
  }

  if (spinPaymentMonth->IsEnabled()) {
    fields.push_back(wxT("pagamento_mes"));
    params.push_back(wxString::Format(wxT("%d"), spinPaymentMonth->GetValue()));
  }

  if (spinPaymentYear->IsEnabled()) {
    fields.push_back(wxT("pagamento_ano"));
    params.push_back(wxString::Format(wxT("%d"), spinPaymentYear->GetValue()));
  }

  // Output account

  if (choiceAccountInput->GetSelection() > 0 && choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back("account_id");
      params.push_back(wxString::Format("%d", container->GetData()));
    }
  }

  // Expense amount value

  if (checkNotFixedAmount->GetValue()) {
    fields.push_back(wxT("montante_fixo"));
    params.push_back(wxT("0"));
  }
  else {
    fields.push_back(wxT("montante_fixo"));
    params.push_back(wxT("1"));

    if (teditAmountValue->GetValue().Length() > 0) {
      fields.push_back(wxT("montante_valor"));
      params.push_back(Monetary::Monetary2Database(teditAmountValue->GetValue()));
    }
  }

  // Installment value

  if (teditInstallmentValue->GetValue().Length() > 0) {
    fields.push_back(wxT("parcela_valor"));
    params.push_back(Monetary::Monetary2Database(teditInstallmentValue->GetValue()));
  }

  // Intallment period

  if (choicePaymentInterval->GetSelection() > 0 && choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("installment_periodicity"));
      params.push_back(wxString::Format(wxT("%c"), container->GetData()));
    }
  }

  // Paid or not

  fields.push_back(wxT("quitada"));

  if (radioBtnPaid->GetValue()) {
    params.push_back(wxT("1"));
  }
  else {
    params.push_back(wxT("0"));
  }


  DBQuery dbquery;

  // Init transaction

  DBTransaction::Begin();

  // Insert

  sql = DBUtils::MountSQLInsert(DB::GetTableName("financial.income"), fields);

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messageStr = wxT("Erro interno do banco:\n") + dbquery.GetMessageStr();
  }


  // Get good id

  //long expenseId = -1;
  /*
  if (!registeringError) {
    expenseId = DBUtils::GetSerialKey(DB::GetTableName("financial.income"), wxT("income_id"));

    if (expenseId < 1) {
      registeringError = true;

      messageStr = wxT("Não foi possível obter a identificação da receita cadastrada.\n\n")
                   wxT("Erro interno do banco:\n") + dbquery.GetMessageStr();
    }
  }


  // Technical information

  if (!registeringError && teditNotes->GetValue().Length() > 0) {
    fields.clear();
    params.clear();

    wxString insertValuesStr = wxT("");

    // Product ID

    fields.push_back(wxT("income_id"));
    params.push_back(wxString::Format(wxT("%ld"), expenseId));

    // Technical information content

    fields.push_back(wxT("observacao"));
    params.push_back(teditNotes->GetValue());

    sql = DBUtils::MountSQLInsert(DB::GetTableName("financial.income.notes"), fields);

    //wxMessageBox(sql);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      messageStr = (wxT("Erro no cadastro das informações técnicas sobre a receita.\n\n")
                    wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());

      registeringError = true;
    }
  }
  */


  // Product registering error

  if (registeringError) {
    DBTransaction::Rollback();

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Não foi possível cadastrar as informações sobre a receita."));

    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível cadastrar as informações sobre a receita '") + teditName->GetValue() + wxT("'.\n\n") +
                        messageStr, LogMessage::MSG_ERROR);
  }
  // Good is successfully registered
  else {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("As informações sobre a receita '") + teditName->GetValue() + wxT("' foram cadastradas com sucesso."), LogMessage::MSG_ERROR);

    // End transaction

    DBTransaction::Commit();

    // Reference to main frame

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    // Reload main panel data

    PanelIncomeExpense *panelIncomeExpense = (PanelIncomeExpense *) mainFrame->GetPage(Lang::Get("Incomes and expenses"));

    if (panelIncomeExpense) {
      panelIncomeExpense->LoadData();
    }

    // Close panel

    wxString pageTitle = Lang::Get("New income");

    mainFrame->PageRemove(pageTitle);

    Show(false);
  }

  this->isCanceled = false;
}


int PanelIncomeNew::LoadIncomeCategories()
{
  choiceIncomeCategory->Freeze();

  choiceIncomeCategory->Clear();

  choiceIncomeCategory->Append(Lang::Get("Select"), (wxClientData *) NULL);


  DBQuery dbquery;

  wxString sql = wxT("SELECT income_category_id, nome ")
                 wxT("FROM ") + DB::GetTableName("financial.income.category") + wxT(" ")
                 wxT("WHERE status = 'A' ");

  //wxMessageBox(sql);

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter as categorias de receita.\n\n")
                        wxT("Erro interno do banco: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() < 1) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não há categorias de receitas cadastradas. É necessário cadastrar categorias de receitas."), LogMessage::MSG_WARN);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("income_category_id")));

      choiceIncomeCategory->Append(Lang::Get(dbquery.GetFieldStrByName(index, wxT("nome"))), (wxClientData *) container);
    }
  }

  choiceIncomeCategory->Thaw();

  choiceIncomeCategory->SetSelection(0);

  return 0;
}


int PanelIncomeNew::LoadWorkTypes()
{
  choiceWorkType->Freeze();

  choiceWorkType->Clear();

  DBQuery dbquery;

  wxString sql = wxT("SELECT regime_trabalho_id, nome ")
                 wxT("FROM trabalhista.regime_trabalho ")
                 wxT("ORDER BY nome");

  //wxMessageBox(sql);

  int rc = dbquery.Execute(sql);

  choiceWorkType->Append(Lang::Get("Not applicable"), (wxClientData *) NULL);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter os regimes de trabalho.\n\n")
                        wxT("Erro interno do banco: ") + dbquery.GetMessageStr(), LogMessage::MSG_WARN);
  }
  else if (dbquery.GetRowsCount() < 1) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não há regimes de trabalho cadastrados."), LogMessage::MSG_WARN);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("regime_trabalho_id")));

      choiceWorkType->Append(dbquery.GetFieldStrByName(index, wxT("nome")), (wxClientData *) container);
    }
  }

  choiceWorkType->SetSelection(0);

  choiceWorkType->Thaw();

  return 0;
}


void PanelIncomeNew::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // Exit <ESC>
  switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
      Close();
      break;
  }

  event.Skip();
}


int PanelIncomeNew::OnDateChange()
{
  long month, year;

  //day = spinPaymentDay->GetValue();
  month = spinPaymentMonth->GetValue();
  year = spinPaymentYear->GetValue();

  int dayMax = 28;

  if (month == 2) {
    if (year % 400 == 0) {
      dayMax = 29;  
    }
    else {
      if (year % 4 == 0 && year % 100 != 0) {
        dayMax = 29;
      }
    }
  }
  else  if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
    dayMax = 31;
  }
  else {
    dayMax = 30;
  }

  spinPaymentDay->SetRange(1, dayMax);

  return 0;
}


void PanelIncomeNew::OnPaymentMonthSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelIncomeNew::OnPaymentYearSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelIncomeNew::OnPaymentIntervalSelect(wxCommandEvent &event)
{
  //if (choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) == (void *) 'D') {
  //  spinPaymentDay->Disable();
  //}
}


void PanelIncomeNew::SetAmountAsNotFixed()
{
  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    checkNotFixedAmount->SetValue(false);

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Se o pagamento possui parcela única, o montante deve ser fixo."));
  }

  if (checkNotFixedAmount->GetValue()) {
    teditAmountValue->Disable();
  }
  else {
    teditAmountValue->Enable();
  }
}


void PanelIncomeNew::OnSetAmountAsNotFixed(wxCommandEvent &event)
{
  SetAmountAsNotFixed();
}


void PanelIncomeNew::OnExpenseTypeChange(wxCommandEvent &event)
{
  LoadIncomeCategories();
}


void PanelIncomeNew::OnAmountValueKillFocus(wxFocusEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditAmountValue != NULL && teditAmountValue->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditAmountValue->GetValue())) {
      messenger->Register(Lang::Get("The amount is not valid") + wxT("."), LogMessage::MSG_ERROR);

      MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

      mainFrame->SetStatusText(Lang::Get("The amount is not valid") + wxT("."));
    }
    else {
      teditAmountValue->SetValue(Monetary::Normalize(teditAmountValue->GetValue()));
    }
  }

  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection())) {

      ContainerIntData *container = (ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection());

      if (container->GetData() == (int) 'U') {
          teditInstallmentValue->SetValue(teditAmountValue->GetValue());
      }
  }
  else {
    if (teditInstallmentValue != NULL && teditInstallmentValue->GetValue().Length() > 0) {
      if (!Monetary::MonetaryIsValid(teditInstallmentValue->GetValue())) {
        MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

        mainFrame->SetStatusText(Lang::Get("The installment amount is not valid") + wxT("."));

        messenger->Register(Lang::Get("The installment amount is not valid") + wxT("."), LogMessage::MSG_ERROR);
      }
      else {
        teditInstallmentValue->SetValue(Monetary::Normalize(teditInstallmentValue->GetValue()));
      }
    }
  }

  event.Skip();
}


void PanelIncomeNew::OnInstallmentValueKillFocus(wxFocusEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditInstallmentValue != NULL && teditInstallmentValue->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditInstallmentValue->GetValue())) {
      messenger->Register(wxT("O valor da parcela não é válido."), LogMessage::MSG_INFO);
    }
    else {
      teditInstallmentValue->SetValue(Monetary::Normalize(teditInstallmentValue->GetValue()));
    }
  }

  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection())) {


      ContainerIntData *container = (ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection());

      if (container->GetData() == (int) 'U') {
        teditAmountValue->SetValue(teditInstallmentValue->GetValue());
      }
  }
  else {
    if (teditAmountValue != NULL && teditAmountValue->GetValue().Length() > 0) {
      if (!Monetary::MonetaryIsValid(teditAmountValue->GetValue())) {
        messenger->Register(wxT("O valor do montante não é válido."), LogMessage::MSG_INFO);
      }
      else {
        teditAmountValue->SetValue(Monetary::Normalize(teditAmountValue->GetValue()));
      }
    }
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(PanelIncomeNew, wxScrolledWindow)
  EVT_TOOL(DLG_ESTATE_NEW_TOOL_SAVE_ID, PanelIncomeNew::OnSaveRequest)

  EVT_KEY_UP(PanelIncomeNew::OnCharHook)

  //EVT_SPINCTRL(DLG_INCOME_NEW_CHOICE_PAYMENT_MONTH_ID, PanelIncomeNew::OnPaymentMonthSelect)
  //EVT_SPINCTRL(DLG_INCOME_NEW_CHOICE_PAYMENT_YEAR_ID, PanelIncomeNew::OnPaymentYearSelect)

  EVT_CHOICE(DLG_INCOME_NEW_CHOICE_PAYMENT_INTERVAL_ID, PanelIncomeNew::OnPaymentIntervalSelect)

  EVT_CHECKBOX(DLG_INCOME_NEW_CHOICE_AMOUNT_NOT_FIXED_ID, PanelIncomeNew::OnSetAmountAsNotFixed)

  EVT_RADIOBUTTON(DLG_INCOME_NEW_RADIO_EXPENSE_FIXED_ID, PanelIncomeNew::OnExpenseTypeChange)
  EVT_RADIOBUTTON(DLG_INCOME_NEW_RADIO_EXPENSE_VARIABLE_ID, PanelIncomeNew::OnExpenseTypeChange)
END_EVENT_TABLE()