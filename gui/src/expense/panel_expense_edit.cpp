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
#include "utils_payment.hpp"
#include "utils_lang.hpp"
#include "utils_config.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "panel_expense_edit.hpp"
#include "dlg_expense_payment_add.hpp"
#include "dlg_expense_payment_edit.hpp"
#include "app.hpp"
#include "dlg_main.hpp"
#include "pixmaps/expense_16x16.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"


PanelExpenseEdit::PanelExpenseEdit(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  expenseId = -1;
  spinPaymentDay = NULL;
  spinPaymentMonth = NULL;
  spinPaymentYear = NULL;

  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);


  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  wxDateTime dateTimeNow = wxDateTime::Now();


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_EXPENSE_EDIT_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_EXPENSE_EDIT_TOOL_DELETE_ID, Lang::Get("Delete"), wxBitmap(delete_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL|wxEXPAND|wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Name

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 5);


  wxFont font;

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, Lang::Get("Name") + wxT(": "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szRow->Add(teditName, 1, wxALL | wxEXPAND, 1);


  // Notebook to separate product information by category

  notebook = new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxEXPAND | wxALL | wxGROW, 0);


  // Main info

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxHSCROLL|wxVSCROLL);

  panelMain->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMain, Lang::Get("General"), true);

  wxBoxSizer *szMarginMain = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMarginMain);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMarginMain->Add(szMain, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  wxFlexGridSizer* szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szgMainFields, 0, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);

  // Expense type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Type") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTypeFixed = new wxRadioButton(panelMain, DLG_EXPENSE_EDIT_RADIO_EXPENSE_FIXED_ID, Lang::Get("Fixed"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  szRow->Add(radioBtnTypeFixed, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTypeFixed->SetValue(true);

  radioBtnTypeVariable = new wxRadioButton(panelMain, DLG_EXPENSE_EDIT_RADIO_EXPENSE_VARIABLE_ID, Lang::Get("Variable"));
  szRow->Add(radioBtnTypeVariable, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  szgMainFields->AddGrowableCol(1);

  // Expense category

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Category") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  choiceExpenseCategory = new wxChoice(panelMain, DLG_EXPENSE_EDIT_CHOICE_EXPENSE_CATEGORY_ID);
  szgMainFields->Add(choiceExpenseCategory, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Payment date

  labelPaymentDate = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Payment date") + wxT(": "));
  szgMainFields->Add(labelPaymentDate, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentDay = new wxSpinCtrl(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_DAY_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 31, 1);
  szRow->Add(spinPaymentDay, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentMonth = new wxSpinCtrl(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_MONTH_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 12, 1);
  szRow->Add(spinPaymentMonth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentMonth->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                           wxSpinEventHandler(PanelExpenseEdit::OnPaymentMonthSelect),
                           NULL,
                           this);

  spinPaymentYear = new wxSpinCtrl(panelMain, DLG_EXPENSE_NEW_CHOICE_PAYMENT_YEAR_ID, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 1900, 2200, dateTimeNow.GetYear());
  szRow->Add(spinPaymentYear, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentYear->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                           wxSpinEventHandler(PanelExpenseEdit::OnPaymentYearSelect),
                           NULL,
                           this);

  // Payment period

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Frequency of payment") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  choicePaymentInterval = new wxChoice(panelMain, DLG_EXPENSE_EDIT_CHOICE_PAYMENT_INTERVAL_ID);
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
                                                    Lang::Get("Input ledger account") + wxT(": "),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szgMainFields->Add(lblAccountParent, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  choiceAccountOutput = new wxChoice(panelMain, wxID_ANY);
  szgMainFields->Add(choiceAccountOutput, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

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

  checkNotFixedAmount = new wxCheckBox(panelMain,
                                       DLG_EXPENSE_EDIT_CHOICE_AMOUNT_NOT_FIXED_ID,
                                       Lang::Get("The expense does not have a fixed amount") + wxT("."));
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
  szRow->Add(radioBtnPaid, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnNotPaid = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("No"));
  szRow->Add(radioBtnNotPaid, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  radioBtnNotPaid->SetValue(true);

  // Expense status

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Status: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusEnabled = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  szRow->Add(radioBtnStatusEnabled, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusDisabled = new wxRadioButton(panelMain, wxID_ANY, Lang::Get("Disabled"));
  szRow->Add(radioBtnStatusDisabled, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);


  // Installments tab

  wxPanel *panelInstallments = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelInstallments, Lang::Get("Installments"), false);

  wxBoxSizer *szMarginInstallments = new wxBoxSizer(wxVERTICAL);
  panelInstallments->SetSizer(szMarginInstallments);

  wxBoxSizer *szInstallments = new wxBoxSizer(wxVERTICAL);
  szMarginInstallments->Add(szInstallments, 1, wxALL|wxEXPAND|wxGROW, 5);


  // Installments toolbar

  /*
  wxAuiToolBar *toolBarInstallments = new wxAuiToolBar(panelInstallments,
                                                       wxID_ANY,
                                                       wxDefaultPosition,
                                                       wxDefaultSize,
                                                       wxAUI_TB_DEFAULT_STYLE|wxAUI_TB_TEXT|wxAUI_TB_HORZ_TEXT|wxAUI_TB_PLAIN_BACKGROUND);

  toolBarInstallments->SetToolBitmapSize(wxSize(16, 16));

  toolBarInstallments->AddTool(DLG_EXPENSE_EDIT_TOOL_INSTALLMENT_ADD_ID, wxT(" Adicionar parcela"), wxBitmap(add_16x16_xpm), wxT("Adicionar nova parcela de pagamento."));

  toolBarInstallments->Realize();

  szInstallments->Add(toolBarInstallments, 0, wxALL, 0);
  */

  wxButton *buttonInstallmentAdd = new wxButton(panelInstallments, DLG_EXPENSE_EDIT_TOOL_INSTALLMENT_ADD_ID, Lang::Get("Add installment"));
  szInstallments->Add(buttonInstallmentAdd, 0, wxALL | wxALIGN_RIGHT, 5);



  listCtrlInstallments = new wxListCtrl(panelInstallments,
                                        DLG_EXPENSE_EDIT_LISTCTRL_INSTALLMENTS_ID,
                                        wxDefaultPosition,
                                        wxSize(30, 30),
                                        wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);

  szInstallments->Add(listCtrlInstallments, 1, wxALL | wxEXPAND | wxGROW, 5);

  wxImageList* imageListInstallments = new wxImageList(16, 16);
  imageListInstallments->Add(wxIcon(expense_16x16_xpm));
  listCtrlInstallments->AssignImageList(imageListInstallments, wxIMAGE_LIST_SMALL);

  wxListItem listItemInstallmentDate;
  listItemInstallmentDate.SetText(Lang::Get("Date"));
  listCtrlInstallments->InsertColumn(0, listItemInstallmentDate);
  listCtrlInstallments->SetColumnWidth(0, 100);

  wxListItem listItemInstallmentValue;
  listItemInstallmentValue.SetText(Lang::Get("Amount"));
  listItemInstallmentValue.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlInstallments->InsertColumn(1, listItemInstallmentValue);
  listCtrlInstallments->SetColumnWidth(1, 90);

  wxListItem listItemInstallmentCommited;
  listItemInstallmentCommited.SetText(Lang::Get("Paid"));
  listCtrlInstallments->InsertColumn(2, listItemInstallmentCommited);
  listCtrlInstallments->SetColumnWidth(2, 60);

  wxListItem listItemInstallmentPaymentWay;
  listItemInstallmentPaymentWay.SetText(Lang::Get("Payment way"));
  listCtrlInstallments->InsertColumn(3, listItemInstallmentPaymentWay);
  listCtrlInstallments->SetColumnWidth(3, 100);

  wxListItem listItemInstallmentBrief;
  listItemInstallmentBrief.SetText(Lang::Get("Description"));
  listCtrlInstallments->InsertColumn(4, listItemInstallmentBrief);
  listCtrlInstallments->SetColumnWidth(4, 150);


  // Informations about installments

  // Total paid

  wxFlexGridSizer *szgInstallmentsInfo = new wxFlexGridSizer(0, 3, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szInstallments->Add(szgInstallmentsInfo, 0, wxALL&(~wxBOTTOM), 5);

  label = new wxStaticText(panelInstallments, wxID_STATIC, Lang::Get("Paid amount") + wxT(": "));
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelInstallments, wxID_STATIC, wxT("$ "));
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountPaid = new wxStaticText(panelInstallments, wxID_STATIC, wxT("0,00"));
  szgInstallmentsInfo->Add(labelAmountPaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  // Installments not paid

  label = new wxStaticText(panelInstallments, wxID_STATIC, Lang::Get("Unpaid amount") + wxT(": "));
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelInstallments, wxID_STATIC, wxT("$ "));
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountInstallmentsNotPaid = new wxStaticText(panelInstallments, wxID_STATIC, wxT("0,00"));
  szgInstallmentsInfo->Add(labelAmountInstallmentsNotPaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgInstallmentsInfo->AddGrowableCol(2);


  // Notes tab

  wxPanel *panelNotes = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, 5);


  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1, 30), wxTE_MULTILINE);
  szNotes->Add(teditNotes, 1, wxALL | wxEXPAND, 1);


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


  teditAmountValue->Connect(wxEVT_KILL_FOCUS,
                            wxFocusEventHandler(PanelExpenseEdit::OnAmountValueKillFocus),
                            NULL,
                            this);

  teditInstallmentValue->Connect(wxEVT_KILL_FOCUS,
                                 wxFocusEventHandler(PanelExpenseEdit::OnInstallmentValueKillFocus),
                                 NULL,
                                 this);

  LoadAccountList();

  PaymentIntervalSelect();
}


void PanelExpenseEdit::Terminate()
{
  Close();

  Destroy();
}


int PanelExpenseEdit::LoadAccountList()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  DBQuery dbquery;

  choiceAccountOutput->Freeze();

  choiceAccountOutput->Clear();

  choiceAccountOutput->Append("Selecione", (wxClientData *) new ContainerIntData(0));

  wxString sql = "SELECT * "
                 "FROM " + DB::GetTableName("accounting.account") + " " +
                 "WHERE tipo_id = 'P' "
                 "AND conta_parent_id NOT IN (SELECT account_id FROM " + DB::GetTableName("accounting.account") + " "
                                             "WHERE conta_parent_id IS NOT NULL) "
                 "ORDER BY lower(nome)";

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas-contábeis."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("account_id")));

      choiceAccountOutput->Append(dbquery.GetFieldStrByName(index, "nome"), container);
    }
  }

  choiceAccountOutput->SetSelection(0);

  choiceAccountOutput->Thaw();

  return 0;
}


int PanelExpenseEdit::LoadData(int expenseId)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  DBQuery dbquery;

  std::vector<wxString> params;

  params.push_back(wxString::Format(wxT("%d"), expenseId));

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT to_char(register_dt, 'DD/MM/YYYY HH24:MI:SS') AS register_dt, ")
                 wxT("nome, tipo_id, expense_category_id, montante_valor, parcela_valor, ")
                 wxT("account_id, installment_periodicity, montante_fixo, quitada, pagamento_dia, ")
                 wxT("pagamento_mes, pagamento_ano, status ") 
          wxT("FROM ") + DB::GetTableName("financial.expense") + wxT(" ")
          wxT("WHERE expense_id = $1");
  }
  else {
    sql = wxT("SELECT strftime('%%d/%%m/%%Y', register_dt) AS register_dt, ")
                 wxT("nome, tipo_id, expense_category_id, montante_valor, parcela_valor, ")
                 wxT("account_id, installment_periodicity, montante_fixo, quitada, pagamento_dia, ")
                 wxT("pagamento_mes, pagamento_ano, status ") 
          wxT("FROM ") + DB::GetTableName("financial.expense") + wxT(" ")
          wxT("WHERE expense_id = $1");
  }

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre a despesa selecionada.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }

  if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("As informações sobre a despesa não foram encontradas."));

    return -1;
  }

  this->expenseId = expenseId;


  // Name

  teditName->SetValue(dbquery.GetFieldStrByName(0, wxT("nome")));

  //wxT("nome, tipo_id, montante_valor, quitada, pagamento_dia, pagamento_mes, pagamento_ano, status ")

  if (dbquery.GetFieldStrByName(0, wxT("tipo_id")).Cmp(wxT("F")) == 0) {
    radioBtnTypeFixed->SetValue(true);
    radioBtnTypeVariable->SetValue(false);
  }
  else {
    radioBtnTypeVariable->SetValue(true);
    radioBtnTypeFixed->SetValue(false);
  }

  // Account

  if (dbquery.GetFieldStrByName(0, wxT("account_id")).Length() > 0) {
    for (size_t index = 0; index < choiceAccountOutput->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("account_id")) == container->GetData()) {
        choiceAccountOutput->SetSelection(index);
      }
    }
  }

  // Payment date

  spinPaymentMonth->SetValue(dbquery.GetFieldIntByName(0, wxT("pagamento_mes")));

  spinPaymentYear->SetValue(dbquery.GetFieldIntByName(0, wxT("pagamento_ano")));

  OnDateChange();

  spinPaymentDay->SetValue(dbquery.GetFieldIntByName(0, wxT("pagamento_dia")));

  //wxMessageBox(dbquery.GetFieldStrByName(0, wxT("pagamento_ano")));

  // Amount value

  teditAmountValue->SetValue(Monetary::Normalize(Monetary::NumberNormalize(dbquery.GetFieldStrByName(0, wxT("montante_valor")))));

  // Amount is fixed or not

  if (dbquery.GetFieldStrByName(0, wxT("montante_fixo")).Cmp(wxT("1")) == 0) {
    checkNotFixedAmount->SetValue(false);
  }
  else {
    checkNotFixedAmount->SetValue(true);
  }

  // Installment value

  teditInstallmentValue->SetValue(Monetary::Normalize(Monetary::NumberNormalize(dbquery.GetFieldStrByName(0, wxT("parcela_valor")))));

  // Sub type

  for (unsigned int index = 0; index < choiceExpenseCategory->GetCount(); index++) {
    if (choiceExpenseCategory->GetClientObject(index)) {
      ContainerIntData *container = (ContainerIntData *) choiceExpenseCategory->GetClientObject(index);

      if (container->GetData() == (int) dbquery.GetFieldIntByName(0, wxT("expense_category_id"))) {
        choiceExpenseCategory->SetSelection(index);

        break;
      }
    }
  }

  messenger->Register(wxT("installment_periodicity = ") + dbquery.GetFieldStrByName(0, wxT("installment_periodicity")), LogMessage::MSG_DBG);

  // Payment interval

  for (unsigned int index = 0; index < choicePaymentInterval->GetCount(); index++) {
    if (choicePaymentInterval->GetClientObject(index)) {
      ContainerIntData *container = (ContainerIntData *) choicePaymentInterval->GetClientObject(index);

      // if (container->GetData() == (int) dbquery.GetFieldIntByName(0, wxT("installment_periodicity")))
      if (dbquery.GetFieldStrByName(0, wxT("installment_periodicity")).Cmp(wxString::Format(wxT("%c"), container->GetData())) == 0) {
        choicePaymentInterval->SetSelection(index);

        break;
      }
    }
  }

  // Paid or not

  if (dbquery.GetFieldStrByName(0, wxT("quitada")).Cmp(wxT("1")) == 0) {
    radioBtnPaid->SetValue(true);

    radioBtnNotPaid->SetValue(false);
  }
  else {
    radioBtnNotPaid->SetValue(true);

    radioBtnPaid->SetValue(false);
  }

  // Status

  if (dbquery.GetFieldStrByName(0, wxT("status")).Cmp(wxT("A")) == 0) {
    radioBtnStatusEnabled->SetValue(true);

    radioBtnStatusDisabled->SetValue(false);
  }
  else {
    radioBtnStatusDisabled->SetValue(true);

    radioBtnStatusEnabled->SetValue(false);
  }

  // Load notes

  DBQuery dbqueryNotes;

  wxString dbCommand = wxString::Format(wxT("SELECT observacao ")
                                        wxT("FROM ") + DB::GetTableName("financial.expense.notes") + wxT(" ")
                                        wxT("WHERE expense_id = %d"), this->expenseId);

  rc = dbqueryNotes.Execute(dbCommand);

  if (rc != 0) {
    wxMessageBox(wxT("Ocorreu erro ao tentar obter as observações sobre a despesa."));
  }
  else if (dbqueryNotes.GetRowsCount() > 0) {
    teditNotes->SetValue(dbqueryNotes.GetFieldStrByName(0, wxT("observacao")));
  }


  SetAmountAsNotFixed();

  LoadInstalmentsListing();

  PaymentIntervalSelect();


  return 0;
}


int PanelExpenseEdit::LoadCategories()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("PanelExpenseEdit::LoadCategories()", LogMessage::MSG_DBG);

  choiceExpenseCategory->Freeze();

  choiceExpenseCategory->Clear();

  choiceExpenseCategory->Append("", (wxClientData *) new ContainerIntData(0));

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


int PanelExpenseEdit::LoadInstalmentsListing(void)
{
  std::vector<wxString> params;

  listCtrlInstallments->Freeze();

  listCtrlInstallments->DeleteAllItems();

  DBQuery dbquery;

  double amountPaid = 0.0;
  double amountInstallmentsNotPaid = 0.0;

  wxString sql = "";

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT p.descricao, p.installment_id, p.expense_id, p.account_id, ")
          wxT("p.pagamento_forma, p.valor, p.quitada, ")
          wxT("to_char(p.pagamento_date, 'DD/MM/YYYY') AS pagamento_date, ")
          wxT("to_char(p.register_dt, 'DD/MM/YYYY HH24:MI:SS') AS register_dt ")
          wxT("FROM " + DB::GetTableName("financial.expense.payment.installment") + " p ") +
          wxString::Format(wxT("WHERE p.expense_id = %d "), this->expenseId) +
          wxT("ORDER BY p.pagamento_date DESC, p.pagamento_time DESC");
  }
  else {
    sql = wxT("SELECT p.descricao, p.installment_id, p.expense_id, p.account_id, ")
          wxT("p.pagamento_forma, p.valor, p.quitada, ")
          wxT("p.pagamento_date, ")
          wxT("strftime('%%d/%%m/%%Y', p.register_dt) AS register_dt ")
          wxT("FROM " + DB::GetTableName("financial.expense.payment.installment") + " p ") +
          wxString::Format(wxT("WHERE p.expense_id = %d "), this->expenseId) +
          wxT("ORDER BY p.pagamento_date DESC, p.pagamento_time DESC");
  }

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de parcelas de pagamento.\n\n")
                 wxT("Erro interno: ") + dbquery.GetMessageStr());
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      long itemIndex = listCtrlInstallments->GetItemCount();

      listCtrlInstallments->InsertItem(itemIndex, dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_date")), 0);

      listCtrlInstallments->SetItemData(itemIndex, dbquery.GetFieldLongByName(resultIndex, wxT("installment_id")));

      listCtrlInstallments->SetItem(resultIndex, 1, Monetary::Number2Monetary(dbquery.GetFieldStrByName(resultIndex, wxT("valor"))));

      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("1")) == 0) {
        listCtrlInstallments->SetItem(resultIndex, 2, wxT("Sim"));

        //GUI::ListSetRowBgColor(listCtrlInstallments, itemIndex);

        amountPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        listCtrlInstallments->SetItem(resultIndex, 2, wxT("Não"));

        //GUI::ListSetRowBgColor(listCtrlInstallments, itemIndex, GUI_COLOR_LIST_ITEM_WARNING);

        amountInstallmentsNotPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }

      wxString paymentWayStr = "";

      if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("V")) == 0) {
        paymentWayStr = wxT("À vista");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("D")) == 0) {
        paymentWayStr = wxT("Depósito");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("T")) == 0) {
        paymentWayStr = wxT("Transferência");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("B")) == 0) {
        paymentWayStr = wxT("C. Débito");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("C")) == 0) {
        paymentWayStr = wxT("C. Crédito");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("Q")) == 0) {
        paymentWayStr = wxT("Cheque");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("Q")) == 0) {
        paymentWayStr = wxT("Cheque");
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_forma")).Cmp(wxT("K")) == 0) {
        paymentWayStr = wxT("Vale");
      }
      else {
        paymentWayStr = wxT("Não informado");
      }

      listCtrlInstallments->SetItem(resultIndex, 3, paymentWayStr);

      listCtrlInstallments->SetItem(resultIndex, 4, dbquery.GetFieldStrByName(resultIndex, wxT("descricao")));
    }
  }

  listCtrlInstallments->Thaw();

  labelAmountPaid->SetLabel(Monetary::Number2Monetary(amountPaid));
  labelAmountPaid->Layout();
  labelAmountPaid->GetContainingSizer()->Layout();
  labelAmountPaid->GetParent()->Layout();

  labelAmountInstallmentsNotPaid->SetLabel(Monetary::Number2Monetary(amountInstallmentsNotPaid));
  labelAmountInstallmentsNotPaid->Layout();
  labelAmountInstallmentsNotPaid->GetContainingSizer()->Layout();
  labelAmountInstallmentsNotPaid->GetParent()->Layout();

  return 0;
}


void PanelExpenseEdit::OnSaveRequest(wxCommandEvent &event)
{
  if (teditName->GetValue().Length() < 1) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("É necessário informar o nome descritivo para a despesa."), LogMessage::MSG_INFO);

    return;
  }

  if (!radioBtnTypeFixed->GetValue() && !radioBtnTypeVariable->GetValue()) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("É necessário informar o tipo de despesa."), LogMessage::MSG_INFO);

    return;
  }

  if (!radioBtnStatusEnabled->GetValue() && !radioBtnStatusDisabled->GetValue()) {
    LogMessenger *messenger = LogMessenger::GetInstance();

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

  // Type

  fields.push_back(wxT("tipo_id"));

  if (radioBtnTypeFixed->GetValue()) {
    params.push_back(wxT("F"));
  }
  else {
    params.push_back(wxT("V"));
  }

  // Category

  fields.push_back(wxT("expense_category_id"));
  if (choiceExpenseCategory->GetSelection() >= 0 && choiceExpenseCategory->GetClientObject(choiceExpenseCategory->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceExpenseCategory->GetClientObject(choiceExpenseCategory->GetSelection());

    if (container->GetData() > 0) {
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    }
    else {
      params.push_back(wxEmptyString);
    }
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Account

  fields.push_back(wxT("account_id"));
  if (choiceAccountOutput->GetSelection() >= 0 && choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection());

    if (container->GetData() > 0) {
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    }
    else {
      params.push_back(wxEmptyString);
    }
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Payment date

  fields.push_back(wxT("pagamento_dia"));
  if (spinPaymentDay->IsEnabled()) {
    params.push_back(wxString::Format(wxT("%d"), spinPaymentDay->GetValue()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  fields.push_back(wxT("pagamento_mes"));
  if (spinPaymentMonth->IsEnabled()) {
    params.push_back(wxString::Format(wxT("%d"), spinPaymentMonth->GetValue()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  fields.push_back(wxT("pagamento_ano"));
  if (spinPaymentYear->IsEnabled()) {
    params.push_back(wxString::Format(wxT("%d"), spinPaymentYear->GetValue()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Expense amount value, fixed or not

  fields.push_back(wxT("montante_fixo"));
  if (checkNotFixedAmount->GetValue()) {
    params.push_back(wxT("0"));
  }
  else {
    params.push_back(wxT("1"));

    fields.push_back(wxT("montante_valor"));
    if (teditAmountValue->GetValue().Length() > 0) {
      params.push_back(Monetary::Monetary2Database(teditAmountValue->GetValue()));
    }
    else {
      params.push_back(wxEmptyString);
    }
  }

  // Installment value

  fields.push_back(wxT("parcela_valor"));
  if (teditInstallmentValue->GetValue().Length() > 0) {
    params.push_back(Monetary::Monetary2Database(teditInstallmentValue->GetValue()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Intallment period

  fields.push_back(wxT("installment_periodicity"));

  if (choicePaymentInterval->GetSelection() >= 0 && choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection());

    if (container->GetData() > 0) {
      params.push_back(wxString::Format(wxT("%c"), container->GetData()));
    }
    else {
      params.push_back(wxEmptyString);
    }
  }
  else {
    params.push_back(wxEmptyString);
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


  sql = DBUtils::MountSQLUpdate(DB::GetTableName("financial.expense"), fields, wxString::Format(wxT("expense_id = %d"), this->expenseId));

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

     messageStr = wxT("Ocorreu erro na modificação das informações sobre a despesa.\n\n")
                  wxT("Erro interno do banco:\n") + dbquery.GetMessageStr();
  }


  // Technical information

  if (!registeringError && teditNotes->GetValue().Length() > 0) {
    fields.clear();
    params.clear();

    wxString insertValuesStr = wxT("");

    // Technical information content

    fields.push_back(wxT("observacao"));
    params.push_back(teditNotes->GetValue());

    sql = DBUtils::MountSQLUpdate(wxT("financeiro.despesa_observacao"), fields, wxString::Format(wxT("expense_id = %d"), this->expenseId));

    //wxMessageBox(sql);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      messageStr = (wxT("Ocorreu erro na modificação das informações sobre a despesa.\n\n")
                    wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());

      registeringError = true;
    }
  }

  if (registeringError > 0) {
    wxMessageBox(messageStr);
  }
}


void PanelExpenseEdit::OnDeleteRequest(wxCommandEvent& event)
{
  int rc = wxMessageBox(wxT("Você confirma a exclusão das informações sobre a despesa?"),
                        wxT("Confirmação de exclusão de informações"),
                        wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, NULL);

  if (rc != wxYES) {
    return;
  }

  DBQuery dbquery;
  std::vector<wxString> params;

  // Delete products from invoice

  params.clear();

  params.push_back(wxString::Format(wxT("%d"), this->expenseId));

  rc = dbquery.Execute(wxT("DELETE FROM financeiro.despesa ")
                       wxT("WHERE expense_id = $1"), params);

  if (rc == 0 && dbquery.GetAffectedRowsCount() > 0) {
    wxMessageBox(wxT("As informações sobre a nota fiscal foram excluídas com sucesso."),
                 wxT("Informação"),
                 wxOK);

    wxString pageTitle = wxString::Format(Lang::Get("Expense") + wxT(" #%d"), this->expenseId);

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->PageRemove(pageTitle);
  }
  else {
    wxMessageBox(wxT("Não foi possível excluir as informações sobre a nota fiscal."),
                 wxT("Erro"),
                 wxOK | wxICON_ERROR);
  }


  event.Skip();
}


void PanelExpenseEdit::InstallmentAdd()
{
  if (notebook->GetSelection() != 1) {
    notebook->SetSelection(1);
  }

  ExpensePaymentAddDialog expensePaymentAddDialog(this);

  expensePaymentAddDialog.SetExpenseId(this->expenseId);

  expensePaymentAddDialog.ShowModal();

  LoadInstalmentsListing();
}


void PanelExpenseEdit::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  switch (event.GetKeyCode()) {
    // New installment
    case WXK_F2:

      InstallmentAdd();

      break;

    default:

      event.Skip();
  }
}


void PanelExpenseEdit::OnTaxMiscAddRequest(wxCommandEvent &event)
{
#if 0
  ProductAddTaxDialog productAddTaxDialog(this);

  productAddTaxDialog.ShowModal();

  if (productAddTaxDialog.IsCanceled()) {
    return;
  }

  // Add product to the products listing

  long itemIndex = listCtrlMiscTaxes->InsertItem(listCtrlMiscTaxes->GetItemCount(), productAddTaxDialog.GetTaxName());

  listCtrlMiscTaxes->SetItemData(itemIndex, productAddTaxDialog.GetTaxId());

  listCtrlMiscTaxes->SetItem(itemIndex, 1, productAddTaxDialog.GetTaxType());

  GUI::ListSetRowBgColor(listCtrlMiscTaxes, itemIndex);
#endif
}


void PanelExpenseEdit::OnTaxMiscDeleteRequest(wxCommandEvent &event)
{
  wxMessageBox(wxT("ProductEditDialog::OnTaxMiscDeleteRequest"));
}


int PanelExpenseEdit::OnDateChange()
{
  //long day;
  long month;
  long year;
 
  if (!spinPaymentDay || !spinPaymentMonth || !spinPaymentYear)
    return -1;

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


void PanelExpenseEdit::OnPaymentMonthSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelExpenseEdit::OnPaymentYearSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelExpenseEdit::OnPaymentIntervalSelect(wxCommandEvent &event)
{
  PaymentIntervalSelect();
}


void PanelExpenseEdit::PaymentIntervalSelect()
{
  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    teditAmountValue->Enable();

    checkNotFixedAmount->Disable();
  }
  else {
    checkNotFixedAmount->Enable();
  }

  SetAmountAsNotFixed();
}


void PanelExpenseEdit::SetAmountAsNotFixed()
{
  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    checkNotFixedAmount->SetValue(false);
  }

  if (checkNotFixedAmount->GetValue()) {
    teditAmountValue->Disable();
  }
  else {
    teditAmountValue->Enable();
  }
}


void PanelExpenseEdit::OnSetAmountAsNotFixed(wxCommandEvent &event)
{
  SetAmountAsNotFixed();
}


void PanelExpenseEdit::OnExpenseTypeChange(wxCommandEvent &event)
{
  LoadCategories();
}


void PanelExpenseEdit::OnAmountValueKillFocus(wxFocusEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditAmountValue != NULL && teditAmountValue->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditAmountValue->GetValue())) {
      MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

      messenger->Register(Lang::Get("The amount is not valid") + wxT("."), LogMessage::MSG_ERROR);

      mainFrame->SetStatusText(Lang::Get("The amount is not valid") + wxT("."));
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


void PanelExpenseEdit::OnInstallmentValueKillFocus(wxFocusEvent &event)
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
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    teditAmountValue->SetValue(teditInstallmentValue->GetValue());
  }
  else {
    if (teditAmountValue != NULL && teditAmountValue->GetValue().Length() > 0) {
      if (!Monetary::MonetaryIsValid(teditAmountValue->GetValue())) {
        MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

        mainFrame->SetStatusText(Lang::Get("The installment amount is not valid") + wxT("."));

        messenger->Register(Lang::Get("The installment amount is not valid") + wxT("."), LogMessage::MSG_ERROR);
      }
      else {
        teditAmountValue->SetValue(Monetary::Normalize(teditAmountValue->GetValue()));
      }
    }
  }

  event.Skip();
}


int PanelExpenseEdit::ValidateFields()
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

  if (teditInstallmentValue != NULL && teditInstallmentValue->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditInstallmentValue->GetValue())) {
      messenger->Register(Lang::Get("The amount is not valid") + wxT("."), LogMessage::MSG_ERROR);

      MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

      mainFrame->SetStatusText(Lang::Get("The amount is not valid") + wxT("."));
    }
    else {
      teditInstallmentValue->SetValue(Monetary::Normalize(teditInstallmentValue->GetValue()));
    }
  }

  return 0;
}


void PanelExpenseEdit::OnInstalmentAddRequest(wxCommandEvent &event)
{
  InstallmentAdd();

  LoadInstalmentsListing();
}


void PanelExpenseEdit::OnInstallmentsListItemEditDialogRequest(wxListEvent &event)
{
  wxListItem listItem = event.GetItem();

  ExpensePaymentEditDialog expensePaymentEditDialog(this);

  expensePaymentEditDialog.SetPaymentId(listItem.GetData());

  expensePaymentEditDialog.ShowModal();

  LoadInstalmentsListing();
}


BEGIN_EVENT_TABLE(PanelExpenseEdit, wxScrolledWindow)
  EVT_TOOL(DLG_EXPENSE_EDIT_TOOL_SAVE_ID, PanelExpenseEdit::OnSaveRequest)
  EVT_TOOL(DLG_EXPENSE_EDIT_TOOL_DELETE_ID, PanelExpenseEdit::OnDeleteRequest)

  EVT_TOOL(DLG_EXPENSE_EDIT_TOOL_TAXES_MISC_ADD_ID, PanelExpenseEdit::OnTaxMiscAddRequest)
  EVT_TOOL(DLG_EXPENSE_EDIT_TOOL_TAXES_MISC_DELETE_ID, PanelExpenseEdit::OnTaxMiscDeleteRequest)

  EVT_CHAR_HOOK(PanelExpenseEdit::OnCharHook)

  EVT_SPINCTRL(DLG_EXPENSE_EDIT_CHOICE_PAYMENT_MONTH_ID, PanelExpenseEdit::OnPaymentMonthSelect)
  EVT_SPINCTRL(DLG_EXPENSE_EDIT_CHOICE_PAYMENT_YEAR_ID, PanelExpenseEdit::OnPaymentYearSelect)

  EVT_CHOICE(DLG_EXPENSE_EDIT_CHOICE_PAYMENT_INTERVAL_ID, PanelExpenseEdit::OnPaymentIntervalSelect)

  EVT_CHECKBOX(DLG_EXPENSE_EDIT_CHOICE_AMOUNT_NOT_FIXED_ID, PanelExpenseEdit::OnSetAmountAsNotFixed)

  EVT_RADIOBUTTON(DLG_EXPENSE_EDIT_RADIO_EXPENSE_FIXED_ID, PanelExpenseEdit::OnExpenseTypeChange)
  EVT_RADIOBUTTON(DLG_EXPENSE_EDIT_RADIO_EXPENSE_VARIABLE_ID, PanelExpenseEdit::OnExpenseTypeChange)

  EVT_BUTTON(DLG_EXPENSE_EDIT_TOOL_INSTALLMENT_ADD_ID, PanelExpenseEdit::OnInstalmentAddRequest)

  EVT_LIST_ITEM_ACTIVATED(DLG_EXPENSE_EDIT_LISTCTRL_INSTALLMENTS_ID, PanelExpenseEdit::OnInstallmentsListItemEditDialogRequest)
END_EVENT_TABLE()