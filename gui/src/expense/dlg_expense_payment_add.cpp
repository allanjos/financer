/**
 * Payment information.
 *
 * \version 1.0
 * \since 13/05/2009
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_tax.hpp"
#include "utils_config.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_expense_payment_add.hpp"
#include "dlg_calendar.hpp"
#include "app.hpp"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


ExpensePaymentAddDialog::ExpensePaymentAddDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           "Despesa - Cadastro de parcela de pagamento",
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxWANTS_CHARS | wxTAB_TRAVERSAL)
{
  this->expenseId = -1;

  SetAutoLayout(true);

  SetSize(800, 550);

  // Margin sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_EXPENSE_PAYMENT_ADD_TOOL_SAVE_ID,
                   " Cadastrar pagamento",
                   wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_EXPENSE_PAYMENT_ADD_TOOL_CLOSE_ID,
                   Lang::Get("Close"),
                   wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Expense name

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText* label = new wxStaticText(this, wxID_STATIC, "Despesa: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  choiceExpense = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceExpense, 1, wxALL | wxEXPAND, 1);

  szTop->AddSpacer(5);


  // Value

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, "Valor: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(Config::currencyList), Config::currencyList);
  cboxValueCurrency->SetStringSelection(Config::Get("currency"));
  szRow->Add(cboxValueCurrency, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency->Disable();

  teditValue = new wxTextCtrl(this, DLG_EXPENSE_PAYMENT_ADD_TEDIT_VALUE_ID, "", wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(25);

  // Date

  label = new wxStaticText(this, wxID_STATIC, "Data: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditPaymentDate = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditPaymentDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate = new wxBitmapButton(this,
                                              DLG_EXPENSE_PAYMENT_ADD_BTN_PAYMENT_DATE_ID,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szRow->Add(btnCalendarPaymentDate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate->SetMinSize(wxSize(-1, teditPaymentDate->GetSize().GetHeight()));

  szRow->AddSpacer(10);


  // Time

  label = new wxStaticText(this, wxID_STATIC, "Hora: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  spinCtrlTimeHour = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 23);
  szRow->Add(spinCtrlTimeHour, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(this, wxID_STATIC, ":");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  spinCtrlTimeMinutes = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 59);
  szRow->Add(spinCtrlTimeMinutes, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(25);


  // Paid or not

  label = new wxStaticText(this, wxID_STATIC, "Quitada: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  toggleBtnPaid = new wxToggleButton(this, DLG_EXPENSE_PAYMENT_ADD_RADIOBTN_PAID_ID, "Sim");
  szRow->Add(toggleBtnPaid, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);

  toggleBtnPaid->SetValue(true);



  // Description

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT("Descrição: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDescription = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditDescription, 1, wxALL | wxEXPAND, 1);


  // Account input

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *lblAccountParent = new wxStaticText(this,
                                                    wxID_STATIC,
                                                    wxT("Conta de saída: "),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(lblAccountParent, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  choiceAccountOutput = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceAccountOutput, 1, wxALL | wxEXPAND, 1);


  // Invoice

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *lblNotaFiscal = new wxStaticText(this,
                                                    wxID_STATIC,
                                                    "Nota fiscal: ",
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(lblNotaFiscal, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  cboxNotaFiscal = new wxChoice(this, wxID_ANY);
  szRow->Add(cboxNotaFiscal, 1, wxALL | wxEXPAND, 1);


  // Payment ways

  wxBoxSizer *szPaymentWaysColumns = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szPaymentWaysColumns, 1, wxALL | wxEXPAND | wxGROW, 1);

  wxSizer *szPaymentWays = new wxBoxSizer(wxVERTICAL);
  szPaymentWaysColumns->Add(szPaymentWays, 0, wxALL | wxALIGN_TOP, 2);

  label = new wxStaticText(this, wxID_STATIC, wxT("Forma de pagamento"));
  GUI::FormatSubSectionLabel(label);
  szPaymentWays->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  // Live cash

  radioBtnPaymentLiveCash = new wxRadioButton(this,
                                              DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_LIVECASH_ID,
                                              wxT("À vista"),
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxRB_GROUP);
  szPaymentWays->Add(radioBtnPaymentLiveCash, 0, wxALL, 5);

  radioBtnPaymentLiveCash->SetValue(true);

  // Deposit

  radioBtnPaymentDeposit = new wxRadioButton(this,
                                             DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ID,
                                             wxT("Depósito bancário"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentDeposit, 0, wxALL, 5);

  // Electronic transfer

  radioBtnPaymentTransfer = new wxRadioButton(this,
                                              DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_TRANSFER_ID,
                                              wxT("Transferência bancária"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentTransfer, 0, wxALL, 5);

  // Debit card

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCardDebit = new wxRadioButton(this,
                                               DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_DEBIT_ID,
                                               wxT("Cartão de débito"),
                                               wxDefaultPosition,
                                               wxDefaultSize);
  szRow->Add(radioBtnPaymentCardDebit, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Credit card

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCardCredit = new wxRadioButton(this,
                                                DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Cartão de crédito"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szRow->Add(radioBtnPaymentCardCredit, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Ticket

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentTicket = new wxRadioButton(this,
                                                DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Vale alimentação"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szRow->Add(radioBtnPaymentTicket, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Cheque

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCheque = new wxRadioButton(this,
                                            DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CHEQUE_ID,
                                            "Cheque",
                                            wxDefaultPosition,
                                            wxDefaultSize);
  szRow->Add(radioBtnPaymentCheque, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  // Information about payment way

  szBoxPaymentInfo = new wxBoxSizer(wxVERTICAL);
  szPaymentWaysColumns->Add(szBoxPaymentInfo, 1, wxALL | wxEXPAND | wxGROW, 0);


  // Deposit panel

  panelPaymentDeposit = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);

  panelPaymentDeposit->Show(false);

  wxBoxSizer *szPanelPaymentDeposit = new wxBoxSizer(wxVERTICAL);
  panelPaymentDeposit->SetSizer(szPanelPaymentDeposit);

  wxBoxSizer *szBoxDeposit = new wxBoxSizer(wxVERTICAL);
  szPanelPaymentDeposit->Add(szBoxDeposit, 1, wxALL | wxEXPAND | wxGROW, 10);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, wxT("Informações detalhadas sobre o pagamento via depósito bancário"));
  GUI::FormatSubSectionLabel(label);
  szBoxDeposit->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  wxFlexGridSizer* szgFields = new wxFlexGridSizer(0, 2, 0, 0);
  szBoxDeposit->Add(szgFields, 0, wxALL | wxEXPAND | wxGROW, 0);

  // Bank

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Banco: ");
  szgFields->Add(label, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositBankName = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
  szgFields->Add(teditDepositBankName, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Account

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Conta: ");
  szgFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditDepositAccount = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szgFields->Add(teditDepositAccount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Agency

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, wxT("        Agência: "));
  szgFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  teditDepositAgency = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
  szgFields->Add(teditDepositAgency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account type

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Tipo de conta: ");
  szgFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnDepositAccountCurrent = new wxRadioButton(panelPaymentDeposit,
                                                    DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnDepositAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnDepositAccountCurrent->SetValue(true);

  radioBtnDepositAccountSavings = new wxRadioButton(panelPaymentDeposit,
                                                    DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(radioBtnDepositAccountSavings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Depositor

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Depositante: ");
  szgFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditDepositDepositor = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(400, -1));
  szgFields->Add(teditDepositDepositor, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Transfer panel

  panelPaymentTransfer = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);

  panelPaymentTransfer->Show(false);

  wxBoxSizer *szBoxPanelTransfer = new wxBoxSizer(wxVERTICAL);
  panelPaymentTransfer->SetSizer(szBoxPanelTransfer);

  wxBoxSizer *szBoxTransfer = new wxBoxSizer(wxVERTICAL);
  szBoxPanelTransfer->Add(szBoxTransfer, 1, wxALL | wxEXPAND | wxGROW, 10);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, wxT("Informações detalhadas sobre o pagamento via transferência bancária"));
  GUI::FormatSubSectionLabel(label);
  szBoxTransfer->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  // Bank

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Banco do depositante: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditTransferBankName = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
  szRow->Add(teditTransferBankName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Conta do depositante: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditTransferAccount = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditTransferAccount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Agency

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, wxT("        Agência: "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditTransferAgency = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditTransferAgency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account type

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Tipo de conta: ");
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTransferAccountCurrent = new wxRadioButton(panelPaymentTransfer,
                                                    DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnTransferAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTransferAccountCurrent->SetValue(true);

  radioBtnTransferAccountSavings = new wxRadioButton(panelPaymentTransfer,
                                                    DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(radioBtnTransferAccountSavings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Transferer

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Nome do depositante: ");
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditTransferDepositor = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(400, -1));
  szRow->Add(teditTransferDepositor, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Debit card panel

  panelPaymentCardDebit = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);

  panelPaymentCardDebit->Show(false);

  wxBoxSizer *szBoxPanelCardDebit = new wxBoxSizer(wxVERTICAL);
  panelPaymentCardDebit->SetSizer(szBoxPanelCardDebit);

  wxBoxSizer *szBoxCardDebit = new wxBoxSizer(wxVERTICAL);
  szBoxPanelCardDebit->Add(szBoxCardDebit, 1, wxALL | wxEXPAND | wxGROW, 10);

  label = new wxStaticText(panelPaymentCardDebit, wxID_STATIC, wxT("Informações detalhadas sobre o pagamento com cartão de débito"));
  GUI::FormatSubSectionLabel(label);
  szBoxCardDebit->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);


  // Debit card mark

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardDebit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCardDebit, wxID_STATIC, wxT("Cartão: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  wxArrayString debitCardMarksList;

  debitCardMarksList.Add("VISA Eletron");
  debitCardMarksList.Add("Rede Shop");

  cboxDebitCardMark = new wxChoice(panelPaymentCardDebit, wxID_ANY, wxDefaultPosition, wxDefaultSize, debitCardMarksList);
  szRow->Add(cboxDebitCardMark, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  // Debit card number

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardDebit->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCardDebit, wxID_STATIC, wxT("Número do cartão: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDebitCardNumber = new wxTextCtrl(panelPaymentCardDebit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditDebitCardNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // Date

  label = new wxStaticText(panelPaymentCardDebit, wxID_STATIC, "Data de validade: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDebitCardValidityDate = new wxTextCtrl(panelPaymentCardDebit, wxID_ANY, "", wxDefaultPosition, wxSize(70, -1));
  szRow->Add(teditDebitCardValidityDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarDebitCardValidityDate = new wxBitmapButton(panelPaymentCardDebit,
                                              wxID_ANY,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szRow->Add(btnCalendarDebitCardValidityDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Card owner name

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardDebit->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCardDebit, wxID_STATIC, wxT("Nome impresso no cartão: "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditDebitCardOwnerName = new wxTextCtrl(panelPaymentCardDebit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditDebitCardOwnerName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Credit card panel

  panelPaymentCardCredit = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);

  panelPaymentCardCredit->Show(false);

  wxBoxSizer *szBoxPanelCardCredit = new wxBoxSizer(wxVERTICAL);
  panelPaymentCardCredit->SetSizer(szBoxPanelCardCredit);

  wxBoxSizer *szBoxCardCredit = new wxBoxSizer(wxVERTICAL);
  szBoxPanelCardCredit->Add(szBoxCardCredit, 1, wxALL | wxEXPAND | wxGROW, 10);

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, wxT("Informações detalhadas sobre o pagamento com cartão de crédito"));
  GUI::FormatSubSectionLabel(label);
  szBoxCardCredit->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  // Credit card mark

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardCredit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, wxT("Cartão: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  wxArrayString creditCardMarksList;

  creditCardMarksList.Add("VISA");
  creditCardMarksList.Add("Mastercard");

  cboxCreditCardMark = new wxChoice(panelPaymentCardCredit, wxID_ANY, wxDefaultPosition, wxDefaultSize, creditCardMarksList);
  szRow->Add(cboxCreditCardMark, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Credit card number

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardCredit->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, wxT("Número do cartão: "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditCreditCardNumber = new wxTextCtrl(panelPaymentCardCredit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditCreditCardNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // Date

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, "Data de validade: ");
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditCreditCardValidityDate = new wxTextCtrl(panelPaymentCardCredit, wxID_ANY, "", wxDefaultPosition, wxSize(70, -1));
  szRow->Add(teditCreditCardValidityDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarCreditCardValidityDate = new wxBitmapButton(panelPaymentCardCredit,
                                              wxID_ANY,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szRow->Add(btnCalendarCreditCardValidityDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Card owner name

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardCredit->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, wxT("Nome impresso no cartão: "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditCreditCardOwnerName = new wxTextCtrl(panelPaymentCardCredit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditCreditCardOwnerName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Credit card panel

  panelPaymentCheque = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);

  panelPaymentCheque->Show(false);

  wxBoxSizer *szBoxPanelCheque = new wxBoxSizer(wxVERTICAL);
  panelPaymentCheque->SetSizer(szBoxPanelCheque);

  wxBoxSizer *szBoxCheque = new wxBoxSizer(wxVERTICAL);
  szBoxPanelCheque->Add(szBoxCheque, 1, wxALL | wxEXPAND | wxGROW, 10);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, wxT("Informações detalhadas sobre o pagamento com cheque"));
  GUI::FormatSubSectionLabel(label);
  szBoxCheque->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCheque->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, "Banco: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  wxArrayString bankCardMarksList;

  bankCardMarksList.Add("Banco do Brasil");
  bankCardMarksList.Add("Banco Real");
  bankCardMarksList.Add("Bradesco");
  bankCardMarksList.Add("HSBC");
  bankCardMarksList.Add(wxT("Itaú"));
  bankCardMarksList.Add("Outro");

  cboxDebitCardMark = new wxChoice(panelPaymentCheque, wxID_ANY, wxDefaultPosition, wxDefaultSize, bankCardMarksList);
  szRow->Add(cboxDebitCardMark, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Cheque number

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCheque->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, wxT("Número do cheque: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditChequeNumber = new wxTextCtrl(panelPaymentCheque, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
  szRow->Add(teditChequeNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // Cheque owner

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCheque->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, wxT("Nome do proprietário do cheque: "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditChequeOwnerName = new wxTextCtrl(panelPaymentCheque, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditChequeOwnerName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Date

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCheque->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, "Data para saque: ");
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditChequeWithdrawalDate = new wxTextCtrl(panelPaymentCheque, wxID_ANY, "", wxDefaultPosition, wxSize(70, -1));
  szRow->Add(teditChequeWithdrawalDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarChequeWithdrawalDate = new wxBitmapButton(panelPaymentCheque,
                                                       wxID_ANY,
                                                       wxBitmap(calendar_bmp_btn_xpm),
                                                       wxDefaultPosition,
                                                       wxDefaultSize);
  szRow->Add(btnCalendarChequeWithdrawalDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szMargin->Add(statusBar, 0, wxALL|wxGROW|wxEXPAND, 0);


  //szTop->SetSizeHints(this);

  this->CenterOnParent();

  PaymentLiveCashSelect();

  LoadAccountList();

  LoadReceiptsList();


  // Payment default date. Current date

  wxDateTime dateTimeNow = wxDateTime::Now();

  teditPaymentDate->SetValue(dateTimeNow.Format("%d/%m/%Y"));


  spinCtrlTimeHour->SetValue(dateTimeNow.GetHour());
  spinCtrlTimeMinutes->SetValue(dateTimeNow.GetMinute());


  // Events

  teditValue->Connect(wxEVT_KILL_FOCUS,
                      wxFocusEventHandler(ExpensePaymentAddDialog::OnPaymentValueKillFocus),
                      NULL,
                      this);

  choiceExpense->Connect(wxEVT_CHOICE,
                         wxCommandEventHandler(ExpensePaymentAddDialog::OnExpenseChangeRequest),
                         NULL,
                         this);


  toggleBtnPaid->Connect(wxEVT_TOGGLEBUTTON,
                         wxCommandEventHandler(ExpensePaymentAddDialog::OnPaidChangeRequest),
                         NULL,
                         this);


  choiceExpense->SetFocus();


  Layout();
  Refresh();


  LoadExpenseList();
}


int ExpensePaymentAddDialog::LoadExpenseList()
{
  DBQuery dbquery;

  choiceExpense->Freeze();

  choiceExpense->Clear();

  choiceExpense->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  int rc = dbquery.Execute("SELECT d.* "
                           "FROM " + DB::GetTableName("financial.expense") + " d "
                           "WHERE d.status = 'A' "
                           "ORDER BY nome");

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de despesas cadastradas.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("expense_id")));

      choiceExpense->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  choiceExpense->SetSelection(0);

  choiceExpense->Thaw();

  return 0;
};


int ExpensePaymentAddDialog::LoadExpenseInfo()
{
  // Load expense information

  wxString sql = wxString::Format("SELECT d.expense_id, d.nome, d.montante_fixo, d.montante_valor, d.parcela_valor, d.account_id "
                                  "FROM " + DB::GetTableName("financial.expense") + " d "
                                  "WHERE d.expense_id = %ld ", this->expenseId);

  //wxMessageBox(sql);

  DBQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter informações sobre a despesa à qual a parcela está ligada.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("Não foram encontradas informações sobre a despesa à qual a parcela está ligada.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }

  // Account

  if (choiceAccountOutput->GetCount() > 0) {
    choiceAccountOutput->SetSelection(0);

    if (dbquery.GetFieldStrByName(0, wxT("account_id")).Length() > 0) {
      for (size_t index = 0; index < choiceAccountOutput->GetCount(); index++) {
        ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(index);

        if (dbquery.GetFieldIntByName(0, wxT("account_id")) == container->GetData()) {
          choiceAccountOutput->SetSelection(index);
        }
      }
    }
  }

  // Installment value

  if (dbquery.GetFieldStrByName(0, "parcela_valor").Length() > 0) {
    teditValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, "parcela_valor")));
  }
  else if (dbquery.GetFieldStrByName(0, "montante_valor").Length() > 0) {
    teditValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, "montante_valor")));
  }
  //else {
    //teditValue->SetValue("");
  //}

  teditValue->SetFocus();

  return 0;
}


void ExpensePaymentAddDialog::OnPaidChangeRequest(wxCommandEvent& event)
{
  if (toggleBtnPaid->GetValue()) {
    toggleBtnPaid->SetLabel(wxT("Sim"));
  }
  else {
    toggleBtnPaid->SetLabel(wxT("Não"));
  }
}


void ExpensePaymentAddDialog::OnExpenseChangeRequest(wxCommandEvent& event)
{
  ContainerIntData *container = (ContainerIntData *) choiceExpense->GetClientObject(choiceExpense->GetSelection());

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxString::Format("Despesa selecionada: %d", container->GetData()), LogMessage::MSG_DBG);

  if (container->GetData() <= 0) {
    return;
  }

  LoadExpenseInfo();
}


int ExpensePaymentAddDialog::LoadAccountList()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  DBQuery dbquery;

  choiceAccountOutput->Freeze();

  choiceAccountOutput->Clear();

  choiceAccountOutput->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  wxString dbCommand = "SELECT * "
                       "FROM " + DB::GetTableName("accounting.account") + " "
                       "WHERE tipo_id = 'P' "
                       "AND conta_parent_id NOT IN "
                           "(SELECT account_id FROM " + DB::GetTableName("accounting.account") + " "
                            "WHERE conta_parent_id IS NOT NULL) "
                       "ORDER BY lower(nome)";

  int rc = dbquery.Execute(dbCommand);

                           /* "SELECT * "
                           "FROM contabil.conta "
                           "WHERE tipo_id = 'A' AND conta_parent_id IS NULL "
                           "ORDER BY lower(nome)"); */

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas de saída cadastradas."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, "account_id"));

      choiceAccountOutput->Append(dbquery.GetFieldStrByName(index, "nome"), container);
    }
  }

  choiceAccountOutput->SetSelection(0);

  choiceAccountOutput->Thaw();

  return 0;
}


int ExpensePaymentAddDialog::LoadReceiptsList()
{
  DBQuery dbquery;

  cboxNotaFiscal->Freeze();

  cboxNotaFiscal->Clear();

  cboxNotaFiscal->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  cboxNotaFiscal->SetSelection(0);

  cboxNotaFiscal->Thaw();

  return 0;
}


bool ExpensePaymentAddDialog::SetExpenseId(long id)
{
  this->expenseId = id;

  if (choiceExpense->GetCount() > 0) {
    for (size_t index = 0; index < choiceExpense->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceExpense->GetClientObject(index);

      if (container && id == container->GetData()) {
        choiceExpense->SetSelection(index);
      }
    }
  }

  LoadExpenseInfo();

  return false;
}


void ExpensePaymentAddDialog::Terminate()
{
  Close();

  Destroy();
}


void ExpensePaymentAddDialog::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format("Key code: %d", event.GetKeyCode()));

  switch (event.GetKeyCode()) {
    // <F3> - Save / confirm
    case WXK_F3:

      SaveData();

      break;

    // <ESC> - Close
    case WXK_ESCAPE:

      Close();

      break;

    default:

      event.Skip();
  }
}


void ExpensePaymentAddDialog::OnPaymentLiveCashRequest(wxCommandEvent &event)
{
  PaymentLiveCashSelect();
}


void ExpensePaymentAddDialog::OnPaymentDepositRequest(wxCommandEvent &event)
{
  PaymentDepositSelect();
}


void ExpensePaymentAddDialog::OnPaymentTransferRequest(wxCommandEvent &event)
{
  PaymentTransferSelect();
}


void ExpensePaymentAddDialog::OnPaymentCardDebitRequest(wxCommandEvent &event)
{
  PaymentCardDebitSelect();
}


void ExpensePaymentAddDialog::OnPaymentCardCreditRequest(wxCommandEvent &event)
{
  PaymentCardCreditSelect();
}


void ExpensePaymentAddDialog::OnPaymentChequeRequest(wxCommandEvent &event)
{
  PaymentChequeSelect();
}



void ExpensePaymentAddDialog::PaymentLiveCashSelect()
{
  if (panelPaymentDeposit->IsShown()) {
    panelPaymentDeposit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentDeposit);
  }

  if (panelPaymentTransfer->IsShown()) {
    panelPaymentTransfer->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentTransfer);
  }

  if (panelPaymentCheque->IsShown()) {
    panelPaymentCheque->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCheque);
  }

  if (panelPaymentCardCredit->IsShown()) {
    panelPaymentCardCredit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardCredit);
  }

  if (panelPaymentCardDebit->IsShown()) {
    panelPaymentCardDebit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardDebit);
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentAddDialog::PaymentDepositSelect()
{
  if (panelPaymentTransfer->IsShown()) {
    panelPaymentTransfer->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentTransfer);
  }

  if (panelPaymentCheque->IsShown()) {
    panelPaymentCheque->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCheque);
  }

  if (panelPaymentCardCredit->IsShown()) {
    panelPaymentCardCredit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardCredit);
  }

  if (panelPaymentCardDebit->IsShown()) {
    panelPaymentCardDebit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardDebit);
  }

  if (!panelPaymentDeposit->IsShown()) {
    panelPaymentDeposit->Show(true);

    szBoxPaymentInfo->Detach(panelPaymentDeposit);

    szBoxPaymentInfo->Add(panelPaymentDeposit, 1, wxALL | wxEXPAND | wxGROW, 15);

    panelPaymentDeposit->Layout();

    szBoxPaymentInfo->Layout();

    panelPaymentDeposit->GetSizer()->Layout();

    panelPaymentDeposit->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentAddDialog::PaymentTransferSelect()
{
  if (panelPaymentDeposit->IsShown()) {
    panelPaymentDeposit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentDeposit);
  }

  if (panelPaymentCheque->IsShown()) {
    panelPaymentCheque->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCheque);
  }

  if (panelPaymentCardCredit->IsShown()) {
    panelPaymentCardCredit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardCredit);
  }

  if (panelPaymentCardDebit->IsShown()) {
    panelPaymentCardDebit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardDebit);
  }

  if (!panelPaymentTransfer->IsShown()) {
    panelPaymentTransfer->Show(true);

    szBoxPaymentInfo->Detach(panelPaymentTransfer);

    szBoxPaymentInfo->Add(panelPaymentTransfer, 1, wxALL | wxEXPAND | wxGROW, 15);

    panelPaymentTransfer->Layout();

    panelPaymentTransfer->GetSizer()->Layout();

    panelPaymentTransfer->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentAddDialog::PaymentCardDebitSelect()
{
  if (panelPaymentDeposit->IsShown()) {
    panelPaymentDeposit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentDeposit);
  }

  if (panelPaymentTransfer->IsShown()) {
    panelPaymentTransfer->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentTransfer);
  }

  if (panelPaymentCheque->IsShown()) {
    panelPaymentCheque->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCheque);
  }

  if (panelPaymentCardCredit->IsShown()) {
    panelPaymentCardCredit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardCredit);
  }

  if (!panelPaymentCardDebit->IsShown()) {
    panelPaymentCardDebit->Show(true);

    szBoxPaymentInfo->Detach(panelPaymentCardDebit);

    szBoxPaymentInfo->Add(panelPaymentCardDebit, 1, wxALL | wxEXPAND | wxGROW, 15);

    panelPaymentCardDebit->Layout();

    panelPaymentCardDebit->GetSizer()->Layout();

    panelPaymentCardDebit->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentAddDialog::PaymentCardCreditSelect()
{
  if (panelPaymentDeposit->IsShown()) {
    panelPaymentDeposit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentDeposit);
  }

  if (panelPaymentTransfer->IsShown()) {
    panelPaymentTransfer->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentTransfer);
  }

  if (panelPaymentCardDebit->IsShown()) {
    panelPaymentCardDebit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardDebit);
  }

  if (panelPaymentCheque->IsShown()) {
    panelPaymentCheque->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCheque);
  }

  if (!panelPaymentCardCredit->IsShown()) {
    panelPaymentCardCredit->Show(true);

    szBoxPaymentInfo->Detach(panelPaymentCardCredit);

    szBoxPaymentInfo->Add(panelPaymentCardCredit, 1, wxALL | wxEXPAND | wxGROW, 15);

    panelPaymentCardCredit->Layout();

    panelPaymentCardCredit->GetSizer()->Layout();

    panelPaymentCardCredit->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentAddDialog::PaymentChequeSelect()
{
  if (panelPaymentDeposit->IsShown()) {
    panelPaymentDeposit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentDeposit);
  }

  if (panelPaymentTransfer->IsShown()) {
    panelPaymentTransfer->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentTransfer);
  }

  if (panelPaymentCardDebit->IsShown()) {
    panelPaymentCardDebit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardDebit);
  }

  if (panelPaymentCardCredit->IsShown()) {
    panelPaymentCardCredit->Show(false);

    szBoxPaymentInfo->Detach(panelPaymentCardCredit);
  }

  if (!panelPaymentCheque->IsShown()) {
    panelPaymentCheque->Show(true);

    szBoxPaymentInfo->Detach(panelPaymentCheque);

    szBoxPaymentInfo->Add(panelPaymentCheque, 1, wxALL | wxEXPAND | wxGROW, 15);

    panelPaymentCheque->Layout();

    panelPaymentCheque->GetSizer()->Layout();

    panelPaymentCheque->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentAddDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


void ExpensePaymentAddDialog::OnSaveRequest(wxCommandEvent &event)
{
  SaveData();
}


int ExpensePaymentAddDialog::SaveData()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditValue->GetValue().Length() < 1) {
    wxMessageBox("Informe o valor do pagamento.");

    return -1;
  }

  if (toggleBtnPaid->GetValue() && teditPaymentDate->GetValue().Length() < 1) {
    wxMessageBox(wxT("Se o pagamento está quitado é necessário informar a data de quitação do mesmo."));

    return -1;
  }

  std::vector<wxString> params, fields, valueModifiers;
  DBQuery dbquery;
  
  params.clear();
  fields.clear();
  valueModifiers.clear();

  // Expense ID

  if (choiceExpense->GetSelection() >= 0 && choiceExpense->GetClientObject(choiceExpense->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceExpense->GetClientObject(choiceExpense->GetSelection());

    fields.push_back("expense_id");

    params.push_back(wxString::Format("%d", container->GetData()));

    valueModifiers.push_back("");
  }

  // Account ID

  if (choiceAccountOutput->GetSelection() >= 0 && choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection());

    fields.push_back("account_id");

    params.push_back(wxString::Format("%d", container->GetData()));

    valueModifiers.push_back("");
  }

  // Amount

  fields.push_back("valor");
  params.push_back(Monetary::Monetary2Database(teditValue->GetValue()));
  valueModifiers.push_back("");

  // Date

  if (teditPaymentDate->GetValue().Length() > 0) {
    fields.push_back("pagamento_date");

    if (DB::GetType() == DB_TYPE_POSTGRESQL) {
      params.push_back(teditPaymentDate->GetValue());
      valueModifiers.push_back("to_date($@, 'DD/MM/YYYY')");
    }
    else {
      wxDateTime dateTime;

      dateTime.ParseFormat(teditPaymentDate->GetValue(), wxT("%d/%m/%Y"));

      params.push_back(dateTime.Format("%Y-%m-%d"));
      valueModifiers.push_back("");
    }
  }

  // Time

  wxString paymentTimeStr = wxString::Format(wxT("%.2d:%.2d"), spinCtrlTimeHour->GetValue(), spinCtrlTimeMinutes->GetValue());

  fields.push_back("pagamento_time");
  params.push_back(paymentTimeStr);
  valueModifiers.push_back("");

  // Description

  if (teditDescription->GetValue().Length() > 0) {
    fields.push_back("descricao");
    params.push_back(teditDescription->GetValue());
    valueModifiers.push_back("");
  }

  // Paid?

  fields.push_back("quitada");

  if (toggleBtnPaid->GetValue()) {
    params.push_back("1");
  }
  else {
    params.push_back("0");
  }
  
  valueModifiers.push_back("");

  // Payment type

  fields.push_back("pagamento_forma");

  if (radioBtnPaymentLiveCash->GetValue()) {
    params.push_back("V");
  }
  else if (radioBtnPaymentDeposit->GetValue()) {
    params.push_back("D");
  }
  else if (radioBtnPaymentTransfer->GetValue()) {
    params.push_back("T");
  }
  else if (radioBtnPaymentCardDebit->GetValue()) {
    params.push_back("B");
  }
  else if (radioBtnPaymentCardCredit->GetValue()) {
    params.push_back("C");
  }
  else if (radioBtnPaymentTicket->GetValue()) {
    params.push_back("K");
  }
  else if (radioBtnPaymentCheque->GetValue()) {
    params.push_back("Q");
  }
  
  valueModifiers.push_back("");

  wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("financial.expense.payment.installment"), fields, valueModifiers);

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível cadastrar a parcela de pagamento para a despesa."), LogMessage::MSG_ERROR);

    messenger->Register(wxT("Database error: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);

    wxMessageBox(wxT("Não foi possível cadastrar a parcela de pagamento para a despesa."));
  }
  else {
    statusBar->SetStatusText(wxT("Parcela de pagamento adicionada à despesa."));

    Close();
  }

  return rc;
}


void ExpensePaymentAddDialog::OnPaymentDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditPaymentDate->GetValue(), "%d/%m/%Y");

  calendarPosition.x = btnCalendarPaymentDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarPaymentDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditPaymentDate->SetValue(calendarDialog.selectedDay + "/" + 
                               calendarDialog.selectedMonth + "/" + 
                               calendarDialog.selectedYear);
  }
}


void ExpensePaymentAddDialog::OnPayStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Enable();

  event.Skip();
}


void ExpensePaymentAddDialog::OnNotPaidStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Disable();

  event.Skip();
}


void ExpensePaymentAddDialog::OnPaymentValueKillFocus(wxFocusEvent &event)
{
  if (teditValue != NULL && teditValue->GetValue().Length() > 0) {
    if (!Monetary::MonetaryIsValid(teditValue->GetValue())) {
      statusBar->SetStatusText(wxT("O valor do pagamento não é válido."));
    }
    else {
      teditValue->SetValue(Monetary::Normalize(teditValue->GetValue()));

      statusBar->SetStatusText(wxT("O valor do pagamento foi convertido."));
    }
  }

  event.Skip();
}



BEGIN_EVENT_TABLE(ExpensePaymentAddDialog, wxDialog)
  EVT_TOOL(DLG_EXPENSE_PAYMENT_ADD_TOOL_SAVE_ID, ExpensePaymentAddDialog::OnSaveRequest)
  EVT_TOOL(DLG_EXPENSE_PAYMENT_ADD_TOOL_CLOSE_ID, ExpensePaymentAddDialog::OnCloseRequest)

  EVT_CHAR_HOOK(ExpensePaymentAddDialog::OnCharHook)

  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_LIVECASH_ID, ExpensePaymentAddDialog::OnPaymentLiveCashRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ID, ExpensePaymentAddDialog::OnPaymentDepositRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_TRANSFER_ID, ExpensePaymentAddDialog::OnPaymentTransferRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_DEBIT_ID, ExpensePaymentAddDialog::OnPaymentCardDebitRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID, ExpensePaymentAddDialog::OnPaymentCardCreditRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBUTTON_CHEQUE_ID, ExpensePaymentAddDialog::OnPaymentChequeRequest)

  EVT_BUTTON(DLG_EXPENSE_PAYMENT_ADD_BTN_PAYMENT_DATE_ID, ExpensePaymentAddDialog::OnPaymentDateChooseRequest)

  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBTN_PAID_ID, ExpensePaymentAddDialog::OnPayStateRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_ADD_RADIOBTN_NOT_PAID_ID, ExpensePaymentAddDialog::OnNotPaidStateRequest)
END_EVENT_TABLE()