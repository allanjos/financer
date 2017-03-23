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
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "panel_income_edit.hpp"
#include "dlg_income_payment_add.hpp"
#include "dlg_income_payment_edit.hpp"
#include "dlg_main.hpp"
#include "pixmaps/income_16x16.xpm"
#include "pixmaps/add_16x16.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/invoice_16x16.xpm"

PanelIncomeEdit::PanelIncomeEdit(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  incomeId = -1;
  spinPaymentDay = NULL;
  spinPaymentMonth = NULL;
  spinPaymentYear = NULL;

  SetAutoLayout(true);

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

  toolBar->AddTool(DLG_INCOME_EDIT_TOOL_SAVE_ID, wxT(" Salvar"), wxBitmap(save_16x16_xpm), wxT("Salvar informações sobre a nova receita."));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL|wxEXPAND|wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Tittle

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 5);


  wxFont font;

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, wxT("Título: "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szRow->Add(teditName, 1, wxALL | wxEXPAND, 1);


  // Notebook to separate product information by category

  wxNotebook *notebook= new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxEXPAND|wxALL|wxGROW, 1);


  // Main info

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxHSCROLL|wxVSCROLL);

  panelMain->SetScrollbars(1, 1, 20, 20);

  notebook->AddPage(panelMain, wxT("Informações gerais"), true);

  wxBoxSizer *szMarginMain = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMarginMain);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMarginMain->Add(szMain, 1, wxALL|wxEXPAND|wxGROW, 5);


  wxFlexGridSizer *szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szgMainFields, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Income category

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Categoria: "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  choiceIncomeCategory = new wxChoice(panelMain, DLG_INCOME_EDIT_CHOICE_EXPENSE_CATEGORY_ID);
  szgMainFields->Add(choiceIncomeCategory, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Work type

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Regime de trabalho: "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  choiceWorkType = new wxChoice(panelMain, DLG_EXPENSE_NEW_CHOICE_EXPENSE_CATEGORY_ID);
  szgMainFields->Add(choiceWorkType, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Payment date

  labelPaymentDate = new wxStaticText(panelMain, wxID_STATIC, wxT("Data de pagamento: "));
  szgMainFields->Add(labelPaymentDate, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentDay = new wxSpinCtrl(panelMain, DLG_INCOME_EDIT_CHOICE_PAYMENT_YEAR_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 31, 1);
  szRow->Add(spinPaymentDay, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentMonth = new wxSpinCtrl(panelMain, DLG_INCOME_EDIT_CHOICE_PAYMENT_YEAR_ID, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 12, 1);
  szRow->Add(spinPaymentMonth, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  spinPaymentYear = new wxSpinCtrl(panelMain, DLG_INCOME_EDIT_CHOICE_PAYMENT_YEAR_ID, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 1850, 3000, 2009);
  szRow->Add(spinPaymentYear, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("    * Ou data inicial de pagamento se a receita possui múltiplas parcelas de pagamento."));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Payment period

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Periodicidade de pagamento: "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  choicePaymentInterval = new wxChoice(panelMain, DLG_INCOME_EDIT_CHOICE_PAYMENT_INTERVAL_ID);
  szgMainFields->Add(choicePaymentInterval, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  choicePaymentInterval->Append(wxT("Parcela única"), (wxClientData *) new ContainerIntData((int) 'U'));
  choicePaymentInterval->Append(wxT("Diário"), (wxClientData *) new ContainerIntData((int) 'D'));
  choicePaymentInterval->Append(wxT("Semanal"), (wxClientData *) new ContainerIntData((int) 'S'));
  choicePaymentInterval->Append(wxT("Quinzenal"), (wxClientData *) new ContainerIntData((int) 'Q'));
  choicePaymentInterval->Append(wxT("Mensal"), (wxClientData *) new ContainerIntData((int) 'M'));
  choicePaymentInterval->Append(wxT("Semestral"), (wxClientData *) new ContainerIntData((int) 'W'));
  choicePaymentInterval->Append(wxT("Anual"), (wxClientData *) new ContainerIntData((int) 'A'));

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

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Valor do montante: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("R$ "));
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditAmountValue = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditAmountValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  checkNotFixedAmount = new wxCheckBox(panelMain, DLG_INCOME_EDIT_CHOICE_AMOUNT_NOT_FIXED_ID, wxT("A receita não possui montante fixo pois possui múltiplas parcelas de pagamento e período indefinido."));
  szRow->Add(checkNotFixedAmount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Expense installment value

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Valor da(s) parcela(s): "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("R$ "));
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditInstallmentValue = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditInstallmentValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Expense is paid

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Quitada: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnPaid = new wxRadioButton(panelMain, wxID_ANY, wxT("Sim"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnPaid->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnPaid, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnNotPaid = new wxRadioButton(panelMain, wxID_ANY, wxT("Não"));
  radioBtnNotPaid->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnNotPaid, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  radioBtnNotPaid->SetValue(true);

  // Expense status

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Status: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusEnabled = new wxRadioButton(panelMain, wxID_ANY, wxT("Ativa"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnStatusEnabled->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnStatusEnabled, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnStatusDisabled = new wxRadioButton(panelMain, wxID_ANY, wxT("Inativa"));
  radioBtnStatusDisabled->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnStatusDisabled, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);


  // Installments tab

  wxPanel *panelInstallments = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelInstallments, wxT("Parcelas de pagamento"), false);

  wxBoxSizer *szMarginInstallments = new wxBoxSizer(wxVERTICAL);
  panelInstallments->SetSizer(szMarginInstallments);

  wxBoxSizer *szInstallments = new wxBoxSizer(wxVERTICAL);
  szMarginInstallments->Add(szInstallments, 1, wxALL|wxEXPAND|wxGROW, 5);

  // Button for add installment

  wxButton *buttonInstallmentAdd = new wxButton(panelInstallments, DLG_INCOME_EDIT_TOOL_INSTALLMENT_ADD_ID, Lang::Get("Add installment"));
  szInstallments->Add(buttonInstallmentAdd, 0, wxALL | wxALIGN_RIGHT, 5);

  listCtrlInstallments = new wxListCtrl(panelInstallments,
                                        DLG_INCOME_EDIT_LISTCTRL_INSTALLMENTS_ID,
                                        wxDefaultPosition,
                                        wxSize(-1, 30),
                                        wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);

  szInstallments->Add(listCtrlInstallments, 1, wxALL|wxEXPAND|wxGROW, 5);

  wxImageList* imageListInstallments = new wxImageList(16, 16);
  imageListInstallments->Add(wxIcon(income_16x16_xpm));
  listCtrlInstallments->AssignImageList(imageListInstallments, wxIMAGE_LIST_SMALL);

  wxListItem listItemInstallmentDate;
  listItemInstallmentDate.SetText(wxT("Data"));
  listCtrlInstallments->InsertColumn(0, listItemInstallmentDate);
  listCtrlInstallments->SetColumnWidth(0, 110);

  wxListItem listItemInstallmentValue;
  listItemInstallmentValue.SetText(wxT("Valor"));
  listItemInstallmentValue.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlInstallments->InsertColumn(1, listItemInstallmentValue);
  listCtrlInstallments->SetColumnWidth(1, 90);

  wxListItem listItemInstallmentCommited;
  listItemInstallmentCommited.SetText(wxT("Quitada"));
  listCtrlInstallments->InsertColumn(2, listItemInstallmentCommited);
  listCtrlInstallments->SetColumnWidth(2, 60);

  wxListItem listItemInstallmentPaymentWay;
  listItemInstallmentPaymentWay.SetText(wxT("Forma"));
  listCtrlInstallments->InsertColumn(3, listItemInstallmentPaymentWay);
  listCtrlInstallments->SetColumnWidth(3, 100);

  wxListItem listItemInstallmentReceipt;
  listItemInstallmentReceipt.SetText(wxT("Nota Fiscal"));
  listCtrlInstallments->InsertColumn(4, listItemInstallmentReceipt);
  listCtrlInstallments->SetColumnWidth(4, 150);


  // Informations about installments

  // Total paid

  wxFlexGridSizer *szgInstallmentsInfo = new wxFlexGridSizer(0, 3, 1, 1);
  szInstallments->Add(szgInstallmentsInfo, 0, wxALL&(~wxBOTTOM), 5);

  label = new wxStaticText(panelInstallments, wxID_STATIC, wxT("Montante quitado: "));
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelInstallments, wxID_STATIC, wxT("R$ "));
  label->SetForegroundColour(*wxBLUE);
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountPaid = new wxStaticText(panelInstallments, wxID_STATIC, wxT("0,00"));
  labelAmountPaid->SetForegroundColour(*wxBLUE);
  szgInstallmentsInfo->Add(labelAmountPaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  // Installments not paid

  label = new wxStaticText(panelInstallments, wxID_STATIC, wxT("Parcelas não quitadas: "));
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelInstallments, wxID_STATIC, wxT("R$ "));
  label->SetForegroundColour(*wxBLUE);
  szgInstallmentsInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountInstallmentsNotPaid = new wxStaticText(panelInstallments, wxID_STATIC, wxT("0,00"));
  labelAmountInstallmentsNotPaid->SetForegroundColour(*wxBLUE);
  szgInstallmentsInfo->Add(labelAmountInstallmentsNotPaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgInstallmentsInfo->AddGrowableCol(2);


  // Receipts tab

  wxPanel *panelReceipts = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelReceipts, wxT("Notas fiscais"), false);

  wxBoxSizer *szMarginReceipts = new wxBoxSizer(wxVERTICAL);
  panelReceipts->SetSizer(szMarginReceipts);

  wxBoxSizer *szReceipts = new wxBoxSizer(wxVERTICAL);
  szMarginReceipts->Add(szReceipts, 1, wxALL | wxEXPAND | wxGROW, 5);


  // Toolbar das notas fiscais

  wxAuiToolBar *toolBarNotasFiscais = new wxAuiToolBar(panelReceipts,
                                                       wxID_ANY,
                                                       wxDefaultPosition,
                                                       wxDefaultSize,
                                                       wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);
                                                       //wxAUI_TB_DEFAULT_STYLE|wxAUI_TB_TEXT|wxAUI_TB_HORZ_TEXT|wxAUI_TB_PLAIN_BACKGROUND);

  toolBarNotasFiscais->SetToolBitmapSize(wxSize(16, 16));

  toolBarNotasFiscais->AddTool(wxID_ANY, wxT(" Adicionar nota fiscal"), wxBitmap(add_16x16_xpm), wxT("Adicionar nota fiscal à compra."));

  toolBarNotasFiscais->Realize();

  szReceipts->Add(toolBarNotasFiscais, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Listagem de notas fiscais

  listCtrlReceipt = new wxListCtrl(panelReceipts,
                                   wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(-1, 30),
                                   wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);

  szReceipts->Add(listCtrlReceipt, 1, wxALL | wxEXPAND | wxGROW, 5);

  wxImageList* imageListReceipt = new wxImageList(16, 16);
  imageListReceipt->Add(wxIcon(invoice_16x16_xpm));
  listCtrlReceipt->AssignImageList(imageListReceipt, wxIMAGE_LIST_SMALL);

  wxListItem listItemReceiptDate;
  listItemReceiptDate.SetText(wxT("Data"));
  listCtrlReceipt->InsertColumn(0, listItemReceiptDate);
  listCtrlReceipt->SetColumnWidth(0, 80);

  wxListItem listItemReceiptNumber;
  listItemReceiptNumber.SetText(wxT("Número"));
  listCtrlReceipt->InsertColumn(1, listItemReceiptNumber);
  listCtrlReceipt->SetColumnWidth(1, 100);

  wxListItem listItemReceiptSeries;
  listItemReceiptSeries.SetText(wxT("Série"));
  listCtrlReceipt->InsertColumn(2, listItemReceiptSeries);
  listCtrlReceipt->SetColumnWidth(2, 90);

  wxListItem listItemReceiptValue;
  listItemReceiptValue.SetText(wxT("Valor"));
  listCtrlReceipt->InsertColumn(3, listItemReceiptValue);
  listCtrlReceipt->SetColumnWidth(3, 100);


  // Notes tab

  wxPanel *panelNotes = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelNotes, wxT("Observações"), false);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, 5);


  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1, 30), wxTE_MULTILINE);
  szNotes->Add(teditNotes, 1, wxALL | wxEXPAND, 1);


  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szMargin->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);


  //szTop->SetSizeHints(this);

  LoadIncomeCategories();

  LoadWorkTypes();

  LoadAccountList();


  teditName->SetFocus();


  teditAmountValue->Connect(wxEVT_KILL_FOCUS,
                            wxFocusEventHandler(PanelIncomeEdit::OnAmountValueKillFocus),
                            NULL,
                            this);

  teditInstallmentValue->Connect(wxEVT_KILL_FOCUS,
                                 wxFocusEventHandler(PanelIncomeEdit::OnInstallmentValueKillFocus),
                                 NULL,
                                 this);
}


void PanelIncomeEdit::Terminate()
{
  Close();

  Destroy();
}


int PanelIncomeEdit::LoadAccountList()
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


int PanelIncomeEdit::LoadData(int incomeId)
{
  DBQuery dbquery;

  std::vector<wxString> params;

  params.push_back(wxString::Format(wxT("%d"), incomeId));

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT to_char(register_dt, 'DD/MM/YYYY HH24:MI:SS') AS register_dt, ")
                 wxT("nome, regime_trabalho_id, income_category_id, montante_valor, account_id, ")
                 wxT("parcela_valor, installment_periodicity, montante_fixo, quitada, ")
                 wxT("pagamento_dia, pagamento_mes, pagamento_ano, status ")
          wxT("FROM ") + DB::GetTableName("financial.income") + " "
          wxT("WHERE income_id = $1");
  }
  else {
    sql = wxT("SELECT strftime('%%d/%%m/%%Y', register_dt) AS register_dt, ")
                 wxT("nome, regime_trabalho_id, income_category_id, montante_valor, account_id, ")
                 wxT("parcela_valor, installment_periodicity, montante_fixo, quitada, ")
                 wxT("pagamento_dia, pagamento_mes, pagamento_ano, status ")
          wxT("FROM ") + DB::GetTableName("financial.income") + " "
          wxT("WHERE income_id = $1");
  }

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre a receita selecionada.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }

  if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("As informações sobre a receita não foram encontradas."));

    return -1;
  }

  this->incomeId = incomeId;


  // Name

  teditName->SetValue(dbquery.GetFieldStrByName(0, wxT("nome")));

  // Account

  if (dbquery.GetFieldStrByName(0, wxT("account_id")).Length() > 0) {
    for (size_t index = 0; index < choiceAccountInput->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("account_id")) == container->GetData()) {
        choiceAccountInput->SetSelection(index);
      }
    }
  }

  // Payment date

  spinPaymentMonth->SetValue(dbquery.GetFieldIntByName(0, wxT("pagamento_mes")));

  spinPaymentYear->SetValue(dbquery.GetFieldIntByName(0, wxT("pagamento_ano")));

  OnDateChange();

  spinPaymentDay->SetValue(dbquery.GetFieldIntByName(0, wxT("pagamento_dia")));

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

  // Income category

  for (unsigned int index = 0; index < choiceIncomeCategory->GetCount(); index++) {
    if (choiceIncomeCategory->GetClientObject(index)) {
      ContainerIntData *container = (ContainerIntData *) choiceIncomeCategory->GetClientObject(index);

      if (container->GetData() == dbquery.GetFieldIntByName(0, wxT("income_category_id"))) {
        choiceIncomeCategory->SetSelection(index);

        break;
      }
    }
  }

  // Work type

  for (unsigned int index = 0; index < choiceWorkType->GetCount(); index++) {
    if (choiceWorkType->GetClientObject(index)) {
      ContainerIntData *container = (ContainerIntData *) choiceWorkType->GetClientObject(index);

      if (container->GetData() == (int) dbquery.GetFieldIntByName(0, wxT("regime_trabalho_id"))) {
        choiceWorkType->SetSelection(index);

        break;
      }
    }
  }

  // Payment interval

  for (unsigned int index = 0; index < choicePaymentInterval->GetCount(); index++) {
    if (choicePaymentInterval->GetClientObject(index)) {
      if (dbquery.GetFieldStrByName(0, wxT("installment_periodicity")).Length() > 0) {
        ContainerIntData *container = (ContainerIntData *) choicePaymentInterval->GetClientObject(index);

        if (container->GetData() == dbquery.GetFieldAnsiStrByName(0, wxT("installment_periodicity"))[0]) {
          choicePaymentInterval->SetSelection(index);

          break;
        }
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
                                        wxT("FROM ") + DB::GetTableName("financial.income.notes") + wxT(" ")
                                        wxT("WHERE income_id = %d"), this->incomeId);

  rc = dbqueryNotes.Execute(dbCommand);

  if (rc != 0) {
    wxMessageBox(wxT("Ocorreu erro ao tentar obter as observações sobre a receita."));
  }
  else if (dbqueryNotes.GetRowsCount() > 0) {
    teditNotes->SetValue(dbqueryNotes.GetFieldStrByName(0, wxT("observacao")));
  }


  SetAmountAsNotFixed();


  LoadInstalmentsListing();


  return 0;
}


int PanelIncomeEdit::LoadIncomeCategories()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  choiceIncomeCategory->Freeze();

  choiceIncomeCategory->Clear();

  choiceIncomeCategory->Append("Selecione", (wxClientData *) new ContainerIntData(0));

  DBQuery dbquery;

  wxString sql = wxT("SELECT income_category_id, nome ")
                 wxT("FROM ") + DB::GetTableName("financial.income.category") + wxT(" ")
                 wxT("WHERE status = 'A' ")
                 wxT("ORDER BY nome");

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as categorias de receita."));

    messenger->Register(wxT("Erro interno do banco: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("Não há categorias de receitas cadastradas. É necessário cadastrar categorias de receitas."));
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("income_category_id")));

      choiceIncomeCategory->Append(dbquery.GetFieldStrByName(index, "nome"), container);
    }
  }

  choiceIncomeCategory->SetSelection(0);

  choiceIncomeCategory->Thaw();

  return 0;
}


int PanelIncomeEdit::LoadWorkTypes()
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


int PanelIncomeEdit::LoadInstalmentsListing(void)
{
  std::vector<wxString> params;

  listCtrlInstallments->Freeze();

  listCtrlInstallments->DeleteAllItems();

  DBQuery dbquery;

  double amountPaid = 0.0;
  double amountInstallmentsNotPaid = 0.0;

  wxString sql = "";

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, to_char(p.pagamento_date, 'DD/MM/YYYY') AS pagamento_date, ")
          wxT("p.pagamento_forma, p.valor, p.quitada, ")
          wxT("to_char(p.register_dt, 'DD/MM/YYYY HH24:MI:SS') AS register_dt ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ") +
          wxString::Format(wxT("WHERE p.income_id = %d "), this->incomeId) +
          wxT("ORDER BY p.pagamento_date DESC");
  }
  else {
    sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, p.pagamento_date, ")
          wxT("p.pagamento_forma, p.valor, p.quitada, ")
          wxT("strftime('%%d/%%m/%%Y', p.register_dt) AS register_dt ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ") +
          wxString::Format(wxT("WHERE p.income_id = %d "), this->incomeId) +
          wxT("ORDER BY p.pagamento_date DESC");
  }

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de parcelas de pagamento.\n\n")
                 wxT("Erro interno: ") + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      long itemIndex = listCtrlInstallments->GetItemCount();

      listCtrlInstallments->InsertItem(itemIndex, dbquery.GetFieldStrByName(index, wxT("pagamento_date")), 0);

      listCtrlInstallments->SetItemData(itemIndex, dbquery.GetFieldLongByName(index, wxT("installment_id")));

      listCtrlInstallments->SetItem(index, 1, Monetary::Number2Monetary(dbquery.GetFieldStrByName(index, wxT("valor"))));

      if (dbquery.GetFieldStrByName(index, wxT("quitada")).Cmp(wxT("1")) == 0) {
        listCtrlInstallments->SetItem(index, 2, wxT("Sim"));

        //GUI::ListSetRowBgColor(listCtrlInstallments, itemIndex);

        amountPaid += dbquery.GetFieldDoubleByName(index, wxT("valor"));
      }
      else {
        listCtrlInstallments->SetItem(index, 2, wxT("Não"));

        //GUI::ListSetRowBgColor(listCtrlInstallments, itemIndex, GUI_COLOR_LIST_ITEM_WARNING);

        amountInstallmentsNotPaid += dbquery.GetFieldDoubleByName(index, wxT("valor"));
      }

      if (dbquery.GetFieldStrByName(index, wxT("pagamento_forma")).Cmp(wxT("V")) == 0) {
        listCtrlInstallments->SetItem(index, 3, wxT("À vista"));
      }
      else if (dbquery.GetFieldStrByName(index, wxT("pagamento_forma")).Cmp(wxT("D")) == 0) {
        listCtrlInstallments->SetItem(index, 3, wxT("Depósito"));
      }
      else if (dbquery.GetFieldStrByName(index, wxT("pagamento_forma")).Cmp(wxT("T")) == 0) {
        listCtrlInstallments->SetItem(index, 3, wxT("Transferência"));
      }
      else if (dbquery.GetFieldStrByName(index, wxT("pagamento_forma")).Cmp(wxT("B")) == 0) {
        listCtrlInstallments->SetItem(index, 3, wxT("C. Débito"));
      }
      else if (dbquery.GetFieldStrByName(index, wxT("pagamento_forma")).Cmp(wxT("C")) == 0) {
        listCtrlInstallments->SetItem(index, 3, wxT("C. Crédito"));
      }
      else if (dbquery.GetFieldStrByName(index, wxT("pagamento_forma")).Cmp(wxT("Q")) == 0) {
        listCtrlInstallments->SetItem(index, 3, wxT("Cheque"));
      }
      else {
        listCtrlInstallments->SetItem(index, 3, wxT("Não informado"));
      }
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


void PanelIncomeEdit::OnSaveRequest(wxCommandEvent &event)
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

  fields.push_back(wxT("income_category_id"));
  if (choiceIncomeCategory->GetSelection() >= 0 && choiceIncomeCategory->GetClientObject(choiceIncomeCategory->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceIncomeCategory->GetClientObject(choiceIncomeCategory->GetSelection());

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

  fields.push_back(wxT("regime_trabalho_id"));
  if (choiceWorkType->GetSelection() >= 0 && choiceWorkType->GetClientObject(choiceWorkType->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceWorkType->GetClientObject(choiceWorkType->GetSelection());

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

  // Account

  fields.push_back(wxT("account_id"));
  if (choiceAccountInput->GetSelection() >= 0 && choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection());

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


  sql = DBUtils::MountSQLUpdate(DB::GetTableName("financial.income"), fields, wxString::Format(wxT("income_id = %d"), this->incomeId));

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

     messageStr = wxT("Ocorreu erro na modificação das informações sobre a receita.\n\n")
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

    sql = DBUtils::MountSQLUpdate(DB::GetTableName("financial.income.notes"), fields, wxString::Format(wxT("income_id = %d"), this->incomeId));

    //wxMessageBox(sql);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      messageStr = (wxT("Ocorreu erro na modificação das informações sobre a receita.\n\n")
                    wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());

      registeringError = true;
    }
  }

  if (registeringError > 0) {
    wxMessageBox(messageStr);
  }
}


void PanelIncomeEdit::OnCharHook(wxKeyEvent& event)
{
  switch (event.GetKeyCode()) {
    // New installment
    case WXK_F2:
      InstallmentAdd();

      break;
  }

  event.Skip();
}


void PanelIncomeEdit::OnTaxMiscAddRequest(wxCommandEvent &event)
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


void PanelIncomeEdit::OnTaxMiscDeleteRequest(wxCommandEvent &event)
{
  wxMessageBox(wxT("ProductEditDialog::OnTaxMiscDeleteRequest"));
}


int PanelIncomeEdit::OnDateChange()
{
  long month, year;
 
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


void PanelIncomeEdit::OnPaymentMonthSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelIncomeEdit::OnPaymentYearSelect(wxSpinEvent &event)
{
  OnDateChange();
}


void PanelIncomeEdit::OnPaymentIntervalSelect(wxCommandEvent &event)
{
  if (choicePaymentInterval->GetSelection() >= 0 &&
      choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()) &&
      ((ContainerIntData *) choicePaymentInterval->GetClientObject(choicePaymentInterval->GetSelection()))->GetData() == (int) 'U') {
    teditAmountValue->Enable();
  }

  SetAmountAsNotFixed();
}


void PanelIncomeEdit::SetAmountAsNotFixed()
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


void PanelIncomeEdit::OnSetAmountAsNotFixed(wxCommandEvent &event)
{
  SetAmountAsNotFixed();
}


void PanelIncomeEdit::OnExpenseTypeChange(wxCommandEvent &event)
{
  LoadIncomeCategories();
}


void PanelIncomeEdit::OnAmountValueKillFocus(wxFocusEvent &event)
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


void PanelIncomeEdit::OnInstallmentValueKillFocus(wxFocusEvent &event)
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
        messenger->Register(Lang::Get("The amount is not valid") + wxT("."), LogMessage::MSG_ERROR);

        MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

        mainFrame->SetStatusText(Lang::Get("The amount is not valid") + wxT("."));
      }
      else {
        teditAmountValue->SetValue(Monetary::Normalize(teditAmountValue->GetValue()));
      }
    }
  }

  event.Skip();
}


int PanelIncomeEdit::ValidateFields()
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
      MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

      mainFrame->SetStatusText(Lang::Get("The installment amount is not valid") + wxT("."));

      messenger->Register(Lang::Get("The installment amount is not valid") + wxT("."), LogMessage::MSG_ERROR);
    }
    else {
      teditInstallmentValue->SetValue(Monetary::Normalize(teditInstallmentValue->GetValue()));
    }
  }

  return 0;
}


void PanelIncomeEdit::InstallmentAdd()
{
  IncomePaymentAddDialog incomePaymentAddDialog(this);

  incomePaymentAddDialog.SetIncomeId(this->incomeId);

  incomePaymentAddDialog.ShowModal();

  LoadInstalmentsListing();
}


void PanelIncomeEdit::OnInstallmentAddRequest(wxCommandEvent &event)
{
  InstallmentAdd();
}


void PanelIncomeEdit::OnInstallmentsListItemEditDialogRequest(wxListEvent &event)
{
  wxListItem listItem = event.GetItem();

  IncomePaymentEditDialog incomePaymentEditDialog(this);

  incomePaymentEditDialog.SetPaymentId(listItem.GetData());

  incomePaymentEditDialog.ShowModal();

  LoadInstalmentsListing();
}


BEGIN_EVENT_TABLE(PanelIncomeEdit, wxScrolledWindow)
  EVT_TOOL(DLG_INCOME_EDIT_TOOL_SAVE_ID, PanelIncomeEdit::OnSaveRequest)
  EVT_TOOL(DLG_INCOME_EDIT_TOOL_TAXES_MISC_ADD_ID, PanelIncomeEdit::OnTaxMiscAddRequest)
  EVT_TOOL(DLG_INCOME_EDIT_TOOL_TAXES_MISC_DELETE_ID, PanelIncomeEdit::OnTaxMiscDeleteRequest)

  EVT_CHAR_HOOK(PanelIncomeEdit::OnCharHook)

  EVT_SPINCTRL(DLG_INCOME_EDIT_CHOICE_PAYMENT_MONTH_ID, PanelIncomeEdit::OnPaymentMonthSelect)
  EVT_SPINCTRL(DLG_INCOME_EDIT_CHOICE_PAYMENT_YEAR_ID, PanelIncomeEdit::OnPaymentYearSelect)

  EVT_CHOICE(DLG_INCOME_EDIT_CHOICE_PAYMENT_INTERVAL_ID, PanelIncomeEdit::OnPaymentIntervalSelect)

  EVT_CHECKBOX(DLG_INCOME_EDIT_CHOICE_AMOUNT_NOT_FIXED_ID, PanelIncomeEdit::OnSetAmountAsNotFixed)

  EVT_RADIOBUTTON(DLG_INCOME_EDIT_RADIO_EXPENSE_FIXED_ID, PanelIncomeEdit::OnExpenseTypeChange)
  EVT_RADIOBUTTON(DLG_INCOME_EDIT_RADIO_EXPENSE_VARIABLE_ID, PanelIncomeEdit::OnExpenseTypeChange)

  EVT_BUTTON(DLG_INCOME_EDIT_TOOL_INSTALLMENT_ADD_ID, PanelIncomeEdit::OnInstallmentAddRequest)

  EVT_LIST_ITEM_ACTIVATED(DLG_INCOME_EDIT_LISTCTRL_INSTALLMENTS_ID, PanelIncomeEdit::OnInstallmentsListItemEditDialogRequest)
END_EVENT_TABLE()
