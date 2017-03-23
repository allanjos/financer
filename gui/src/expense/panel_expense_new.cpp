/**
 *
 * \version 1.0
 * \since 08/19/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_lang.hpp"
#include "utils_image.hpp"
#include "utils_payment.hpp"
#include "utils_config.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "panel_expense_new.hpp"
#include "panel_income_expense.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "pixmaps/save_16x16.xpm"


PanelExpenseNew::PanelExpenseNew(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  this->isCanceled = true;

  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  // Main sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  wxDateTime dateTimeNow = wxDateTime::Now();


  wxBoxSizer *szRow;


  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_ESTATE_NEW_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL|wxEXPAND|wxGROW, GUI::WINDOW_MARGIN_SIZE);



  // Notebook to separate information by category

  wxNotebook *notebook= new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxGROW | wxEXPAND | wxALL, 0);


  // Panel to main information

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS);

  panelMain->SetAutoLayout(true);

  panelMain->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMain, Lang::Get("General"), true);

  wxBoxSizer *szMainMargin = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMainMargin);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMainMargin->Add(szMain, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  wxFont font;

  wxFlexGridSizer* szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szgMainFields, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Name

  wxStaticText *label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Name") + wxT(": "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  teditName = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szgMainFields->Add(teditName, 1, wxALL | wxEXPAND, 1);

  szgMainFields->AddGrowableCol(1);

  // Expense type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Type") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTypeFixed = new wxRadioButton(panelMain, DLG_EXPENSE_NEW_RADIO_EXPENSE_FIXED_ID, Lang::Get("Fixed"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  szRow->Add(radioBtnTypeFixed, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTypeFixed->SetValue(true);

  radioBtnTypeVariable = new wxRadioButton(panelMain, DLG_EXPENSE_NEW_RADIO_EXPENSE_VARIABLE_ID, Lang::Get("Variable"));
  szRow->Add(radioBtnTypeVariable, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  // Expense category

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Category") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  choiceExpenseCategory = new wxChoice(panelMain, DLG_EXPENSE_NEW_CHOICE_EXPENSE_CATEGORY_ID);
  szgMainFields->Add(choiceExpenseCategory, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Payment date

  labelPaymentDate = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Payment date") + wxT(": "));
  szgMainFields->Add(labelPaymentDate, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentDay = new wxSpinCtrl(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_DAY_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 31, 1);
  szRow->Add(spinPaymentDay, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentMonth = new wxSpinCtrl(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_MONTH_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 12, 1);
  szRow->Add(spinPaymentMonth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentYear = new wxSpinCtrl(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_YEAR_ID, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 1900, 2200, dateTimeNow.GetYear());
  szRow->Add(spinPaymentYear, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Payment period

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Frequency of payment") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  choicePaymentInterval = new wxChoice(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_INTERVAL_ID);
  szgMainFields->Add(choicePaymentInterval, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

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
                                                    Lang::Get("Output ledger account") + wxT(": "),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szgMainFields->Add(lblAccountParent, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  choiceAccountOutput = new wxChoice(panelMain, wxID_ANY);
  szgMainFields->Add(choiceAccountOutput, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

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

  checkNotFixedAmount = new wxCheckBox(panelMain, DLG_EXPENSE_NEW_CHOICE_AMOUNT_NOT_FIXED_ID, Lang::Get("The expense does not have a fixed amount") + wxT("."));
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
  szRow->Add(radioBtnNotPaid, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  radioBtnNotPaid->SetValue(true);

  // Expense status

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Status") + wxT(":"));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusEnabled = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnStatusEnabled->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnStatusEnabled, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusDisabled = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Disabled"));
  radioBtnStatusDisabled->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnStatusDisabled, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);


  // Notes tab

  wxPanel *panelNotes = new wxPanel(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Notes text control

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szNotes->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szRow->Add(teditNotes, 1, wxALL | wxEXPAND | wxGROW, 1);


  //szTop->SetSizeHints(this);

  SetAmountAsNotFixed();

  LoadCategories();

  // Configure initial payment date

  wxDateTime::Month month = dateTimeNow.GetMonth();
  int year = dateTimeNow.GetYear();

  OnDateChange();

  short unsigned int day = dateTimeNow.GetDay();

  //wxMessageBox(wxString::Format(wxT("day: %d, month: %d, year: %d"), day, (int) month, year));

  spinPaymentDay->SetValue(day);
  spinPaymentMonth->SetValue((int) month + 1);
  spinPaymentYear->SetValue(year);


  teditName->SetFocus();


  spinPaymentMonth->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                           wxSpinEventHandler(PanelExpenseNew::OnPaymentMonthSelect),
                           NULL,
                           this);

  spinPaymentYear->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                           wxSpinEventHandler(PanelExpenseNew::OnPaymentYearSelect),
                           NULL,
                           this);

  teditAmountValue->Connect(wxEVT_KILL_FOCUS,
                            wxFocusEventHandler(PanelExpenseNew::OnAmountValueKillFocus),
                            NULL,
                            this);

  teditInstallmentValue->Connect(wxEVT_KILL_FOCUS,
                                 wxFocusEventHandler(PanelExpenseNew::OnInstallmentValueKillFocus),
                                 NULL,
                                 this);

  LoadAccountList();
}


unsigned long PanelExpenseNew::GetExpenseId()
{
  return this->expenseId;
}


bool PanelExpenseNew::IsCanceled()
{
  return this->isCanceled;
}


int PanelExpenseNew::LoadAccountList()
{
  DBQuery dbquery;

  choiceAccountOutput->Freeze();

  choiceAccountOutput->Clear();

  choiceAccountOutput->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  choiceAccountOutput->SetSelection(0);

  wxString sql = wxT("SELECT * FROM ") + DB::GetTableName("accounting.account") +
                 wxT(" WHERE tipo_id = 'P'")
                 wxT(" ORDER BY codigo, lower(nome)");

  /*int rc = dbquery.Execute("SELECT * "
                           "FROM contabil.conta "
                           "WHERE tipo_id = 'P' "
                           "AND conta_parent_id NOT IN (SELECT account_id FROM contabil.conta WHERE conta_parent_id IS NOT NULL) "
                           "ORDER BY lower(nome)");*/

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de contas de saída cadastradas."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("account_id")));

      wxString caption = dbquery.GetFieldStrByName(index, "codigo") + wxT(". ") + dbquery.GetFieldStrByName(index, "nome");

      choiceAccountOutput->Append(caption, container);
    }
  }

  choiceAccountOutput->SetSelection(0);

  choiceAccountOutput->Thaw();

  return 0;
}


void PanelExpenseNew::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  this->isCanceled = false;

  if (teditName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("É necessário informar o nome descritivo para a despesa."));

    return;
  }

  if (!radioBtnTypeFixed->GetValue() && !radioBtnTypeVariable->GetValue()) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("É necessário informar o tipo de despesa."));

    return;
  }

  if (!radioBtnStatusEnabled->GetValue() && !radioBtnStatusDisabled->GetValue()) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("É necessário informar se a empresa está ativa ou inativa."));

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

  // Type

  fields.push_back(wxT("tipo_id"));

  if (radioBtnTypeFixed->GetValue()) {
    params.push_back(wxT("F"));
  }
  else {
    params.push_back(wxT("V"));
  }

  if (choiceExpenseCategory->GetSelection() > 0 && choiceExpenseCategory->GetClientObject(choiceExpenseCategory->GetSelection())) {
    fields.push_back(wxT("expense_category_id"));

    ContainerIntData *container = (ContainerIntData *) choiceExpenseCategory->GetClientObject(choiceExpenseCategory->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
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

  if (choiceAccountOutput->GetSelection() > 0 && choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection());

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

  if (choicePaymentInterval->GetSelection() >= 0 && choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection())) {
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


  sql = DBUtils::MountSQLInsert(DB::GetTableName("financial.expense"), fields);

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messageStr = wxT("Erro interno do banco: \n") + dbquery.GetMessageStr();
  }


  /*
  // Get good id

  //long expenseId = -1;

  if (!registeringError) {
    expenseId = DBUtils::GetSerialKey(DB::GetTableName("financial.expense"), wxT("expense_id"));

    if (expenseId < 1) {
      registeringError = true;

      messenger->Register(wxT("Não foi possível obter a identificação da despesa cadastrada.\n\n")
                          wxT("Erro interno do banco:\n") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
    }
  }


  // Technical information

  if (!registeringError && teditNotes->GetValue().Length() > 0) {
    fields.clear();
    params.clear();

    wxString insertValuesStr = wxT("");

    // Product ID

    fields.push_back(wxT("expense_id"));
    params.push_back(wxString::Format(wxT("%ld"), expenseId));

    // Technical information content

    fields.push_back(wxT("observacao"));
    params.push_back(teditNotes->GetValue());

    sql = DBUtils::MountSQLInsert(DB::GetTableName("financial.expense.notes"), fields);

    //wxMessageBox(sql);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      messageStr = (wxT("Erro no cadastro das informações técnicas sobre a despesa.\n\n")
                    wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());

      registeringError = true;
    }
  }
  */


  // Product registering error

  if (registeringError) {
    DBTransaction::Rollback();

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("Não foi possível cadastrar as informações sobre a despesa."));

    messenger->Register(wxT("Não foi possível cadastrar as informações sobre a despesa '") + teditName->GetValue() + wxT("'.\n\n") +
                        messageStr, LogMessage::MSG_ERROR);
  }
  // Good is successfully registered
  else {
    messenger->Register(wxT("As informações sobre a despesa '") + teditName->GetValue() + wxT("' foram cadastradas com sucesso."), LogMessage::MSG_INFO);

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

    wxString pageTitle = Lang::Get("New expense");

    mainFrame->PageRemove(pageTitle);

    Show(false);
  }
}


int PanelExpenseNew::LoadCategories()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("PanelExpenseNew::LoadCategories()", LogMessage::MSG_DBG);

  choiceExpenseCategory->Freeze();

  choiceExpenseCategory->Clear();

  choiceExpenseCategory->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  DBQuery dbquery;

  wxString sql = wxT("SELECT expense_category_id, nome ")
                 wxT("FROM ") + DB::GetTableName("financial.expense.category") + wxT(" ")
                 wxT("WHERE status = 'A' ")
                 wxT("ORDER BY nome");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter as categorias de despesa."), LogMessage::MSG_ERROR);

    messenger->Register(wxT("Erro interno do banco: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("expense_category_id")));

      choiceExpenseCategory->Append(Lang::Get(dbquery.GetFieldStrByName(index, wxT("nome"))), container);
    }
  }

  choiceExpenseCategory->SetSelection(0);

  choiceExpenseCategory->Thaw();

  return 0;
}


int PanelExpenseNew::OnDateChange()
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


void PanelExpenseNew::OnPaymentMonthSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelExpenseNew::OnPaymentYearSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelExpenseNew::OnPaymentIntervalSelect(wxCommandEvent &event)
{
  //if (choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) == (void *) 'D') {
  //  spinPaymentDay->Disable();
  //}
}


void PanelExpenseNew::SetAmountAsNotFixed()
{
  if (checkNotFixedAmount->GetValue()) {
    teditAmountValue->Disable();
  }
  else {
    teditAmountValue->Enable();
  }
}


void PanelExpenseNew::OnSetAmountAsNotFixed(wxCommandEvent &event)
{
  SetAmountAsNotFixed();
}


void PanelExpenseNew::OnExpenseTypeChange(wxCommandEvent &event)
{
  LoadCategories();
}


void PanelExpenseNew::OnAmountValueKillFocus(wxFocusEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditAmountValue != NULL && teditAmountValue->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditAmountValue->GetValue())) {
      MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

      mainFrame->SetStatusText(Lang::Get("The amount is not valid") + wxT("."));

      messenger->Register(Lang::Get("The amount is not valid") + wxT("."), LogMessage::MSG_ERROR);
    }
    else {
      teditAmountValue->SetValue(Monetary::Normalize(teditAmountValue->GetValue()));
    }
  }

  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    teditInstallmentValue->SetValue(teditAmountValue->GetValue());
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


void PanelExpenseNew::OnInstallmentValueKillFocus(wxFocusEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

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

  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    teditAmountValue->SetValue(teditInstallmentValue->GetValue());
  }
  else {
    if (teditAmountValue != NULL && teditAmountValue->GetValue().Length() > 0) {
      if (!Monetary::MonetaryIsValid(teditAmountValue->GetValue())) {
        MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

        mainFrame->SetStatusText(Lang::Get("The amount is not valid") + wxT("."));

        messenger->Register(Lang::Get("The amount is not valid") + wxT("."), LogMessage::MSG_ERROR);
      }
      else {
        teditAmountValue->SetValue(Monetary::Normalize(teditAmountValue->GetValue()));
      }
    }
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(PanelExpenseNew, wxScrolledWindow)
  EVT_TOOL(DLG_ESTATE_NEW_TOOL_SAVE_ID, PanelExpenseNew::OnSaveRequest)

  EVT_CHOICE(DLG_EXPENSE_NEW_CHOICE_PAYMENT_INTERVAL_ID, PanelExpenseNew::OnPaymentIntervalSelect)

  EVT_CHECKBOX(DLG_EXPENSE_NEW_CHOICE_AMOUNT_NOT_FIXED_ID, PanelExpenseNew::OnSetAmountAsNotFixed)

  EVT_RADIOBUTTON(DLG_EXPENSE_NEW_RADIO_EXPENSE_FIXED_ID, PanelExpenseNew::OnExpenseTypeChange)
  EVT_RADIOBUTTON(DLG_EXPENSE_NEW_RADIO_EXPENSE_VARIABLE_ID, PanelExpenseNew::OnExpenseTypeChange)
END_EVENT_TABLE()