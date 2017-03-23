/**
 * Payment information.
 *
 * \version 1.0
 * \since 07/01/2010
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_tax.hpp"
#include "utils_config.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_expense_payment_edit.hpp"
#include "dlg_calendar.hpp"
#include "app.hpp"
#include "pixmaps/delete_16x16.xpm"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


ExpensePaymentEditDialog::ExpensePaymentEditDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxT("Despesa - Informações sobre parcela de pagamento"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxWANTS_CHARS | wxTAB_TRAVERSAL)
{
  this->paymentId = -1;

  SetAutoLayout(true);

  SetSize(800, 550);

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

  toolBar->AddTool(DLG_EXPENSE_PAYMENT_EDIT_TOOL_SAVE_ID,
                   wxT(" Salvar modificações"),
                   wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_EXPENSE_PAYMENT_EDIT_TOOL_DELETE_ID,
                   wxT(" Excluir parcela"),
                   wxBitmap(delete_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_EXPENSE_PAYMENT_EDIT_TOOL_CLOSE_ID,
                   Lang::Get("Close"),
                   wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);


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
  szRow->Add(cboxValueCurrency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency->Disable();

  teditValue = new wxTextCtrl(this, DLG_EXPENSE_PAYMENT_EDIT_TEDIT_VALUE_ID, "", wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(25);

  // Date

  label = new wxStaticText(this, wxID_STATIC, "Data: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditPaymentDate = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditPaymentDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate = new wxBitmapButton(this,
                                              DLG_EXPENSE_PAYMENT_EDIT_BTN_PAYMENT_DATE_ID,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szRow->Add(btnCalendarPaymentDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate->SetMinSize(wxSize(-1, teditPaymentDate->GetSize().GetHeight()));

  szRow->AddSpacer(10);


  // Time

  label = new wxStaticText(this, wxID_STATIC, "Hora: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  spinCtrlTimeHour = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 23);
  szRow->Add(spinCtrlTimeHour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(this, wxID_STATIC, ":");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  spinCtrlTimeMinutes = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 59);
  szRow->Add(spinCtrlTimeMinutes, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(25);


  // Paid or not

  label = new wxStaticText(this, wxID_STATIC, "Quitada: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  toggleBtnPaid = new wxToggleButton(this, DLG_EXPENSE_PAYMENT_EDIT_RADIOBTN_PAID_ID, "Sim");
  szRow->Add(toggleBtnPaid, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 10);

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
                                              DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_LIVECASH_ID,
                                              wxT("À vista"),
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxRB_GROUP);
  szPaymentWays->Add(radioBtnPaymentLiveCash, 0, wxALL, 5);

  radioBtnPaymentLiveCash->SetValue(true);

  // Deposit

  radioBtnPaymentDeposit = new wxRadioButton(this,
                                             DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ID,
                                             wxT("Depósito bancário"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentDeposit, 0, wxALL, 5);

  // Electronic transfer

  radioBtnPaymentTransfer = new wxRadioButton(this,
                                              DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_TRANSFER_ID,
                                              wxT("Transferência bancária"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentTransfer, 0, wxALL, 5);

  // Debit card

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCardDebit = new wxRadioButton(this,
                                               DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_DEBIT_ID,
                                               wxT("Cartão de débito"),
                                               wxDefaultPosition,
                                               wxDefaultSize);
  szRow->Add(radioBtnPaymentCardDebit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Credit card

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCardCredit = new wxRadioButton(this,
                                                DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Cartão de crédito"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szRow->Add(radioBtnPaymentCardCredit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Ticket

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentTicket = new wxRadioButton(this,
                                                DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Vale alimentação"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szRow->Add(radioBtnPaymentTicket, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Cheque

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCheque = new wxRadioButton(this,
                                            DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CHEQUE_ID,
                                            "Cheque",
                                            wxDefaultPosition,
                                            wxDefaultSize);
  szRow->Add(radioBtnPaymentCheque, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


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
  szBoxDeposit->Add(szgFields, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Bank

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Banco: ");
  szgFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  teditDepositBankName = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
  szgFields->Add(teditDepositBankName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Conta: ");
  szgFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  teditDepositAccount = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szgFields->Add(teditDepositAccount, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

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
                                                    DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnDepositAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnDepositAccountCurrent->SetValue(true);

  radioBtnDepositAccountSavings = new wxRadioButton(panelPaymentDeposit,
                                                    DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(radioBtnDepositAccountSavings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Depositor

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Depositante: ");
  szgFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

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
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

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
  szRow->Add(teditTransferAccount, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Agency

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, wxT("        Agência: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditTransferAgency = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditTransferAgency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account type

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Tipo de conta: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTransferAccountCurrent = new wxRadioButton(panelPaymentTransfer,
                                                    DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnTransferAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTransferAccountCurrent->SetValue(true);

  radioBtnTransferAccountSavings = new wxRadioButton(panelPaymentTransfer,
                                                    DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(radioBtnTransferAccountSavings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Transferer

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Nome do depositante: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

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
  szRow->Add(cboxDebitCardMark, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Debit card number

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCardDebit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

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
  szBoxCardDebit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCardDebit, wxID_STATIC, wxT("Nome impresso no cartão: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

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
  szBoxCardCredit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, wxT("Número do cartão: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditCreditCardNumber = new wxTextCtrl(panelPaymentCardCredit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditCreditCardNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // Date

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, "Data de validade: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

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
  szBoxCardCredit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCardCredit, wxID_STATIC, wxT("Nome impresso no cartão: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

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
  szBoxCheque->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, wxT("Número do cheque: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditChequeNumber = new wxTextCtrl(panelPaymentCheque, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
  szRow->Add(teditChequeNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // Cheque owner

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCheque->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, wxT("Nome do proprietário do cheque: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditChequeOwnerName = new wxTextCtrl(panelPaymentCheque, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditChequeOwnerName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Date

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxCheque->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentCheque, wxID_STATIC, "Data para saque: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

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

  szMargin->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);


  this->CenterOnParent();


  PaymentLiveCashSelect();

  // Load fields data

  LoadAccountList();

  LoadExpenseList();

  LoadReceiptsList();


  // Payment default date. Current date

  wxDateTime dateTimeNow = wxDateTime::Now();

  teditPaymentDate->SetValue(dateTimeNow.Format(wxT("%d/%m/%Y")));


  teditValue->Connect(wxEVT_KILL_FOCUS,
                      wxFocusEventHandler(ExpensePaymentEditDialog::OnPaymentValueKillFocus),
                      NULL,
                      this);

  toggleBtnPaid->Connect(wxEVT_TOGGLEBUTTON,
                         wxCommandEventHandler(ExpensePaymentEditDialog::OnPaidChangeRequest),
                         NULL,
                         this);
}


void ExpensePaymentEditDialog::OnPaidChangeRequest(wxCommandEvent& event)
{
  if (toggleBtnPaid->GetValue()) {
    toggleBtnPaid->SetLabel(wxT("Sim"));
  }
  else {
    toggleBtnPaid->SetLabel(wxT("Não"));
  }
}


int ExpensePaymentEditDialog::LoadAccountList()
{
  DBQuery dbquery;

  choiceAccountOutput->Freeze();

  choiceAccountOutput->Clear();

  choiceAccountOutput->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  int rc = dbquery.Execute(wxT("SELECT * ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" ")
                           wxT("WHERE tipo_id = 'P' ")
                           wxT("AND conta_parent_id NOT IN (SELECT account_id ")
                                                       wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" ")
                                                       wxT("WHERE conta_parent_id IS NOT NULL) ")
                           wxT("ORDER BY lower(nome)"));

                           /* wxT("SELECT * ")
                           wxT("FROM contabil.conta ")
                           wxT("WHERE tipo_id = 'A' AND conta_parent_id IS NULL ")
                           wxT("ORDER BY lower(nome)")); */

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de contas de saída cadastradas."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("account_id")));

      choiceAccountOutput->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  choiceAccountOutput->SetSelection(0);

  choiceAccountOutput->Thaw();

  return 0;
}


int ExpensePaymentEditDialog::LoadReceiptsList()
{
  DBQuery dbquery;

  cboxNotaFiscal->Freeze();

  cboxNotaFiscal->Clear();

  cboxNotaFiscal->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  cboxNotaFiscal->SetSelection(0);

  cboxNotaFiscal->Thaw();

  return 0;
}


int ExpensePaymentEditDialog::LoadExpenseList()
{
  DBQuery dbquery;

  choiceExpense->Freeze();

  choiceExpense->Clear();

  choiceExpense->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  int rc = dbquery.Execute(wxT("SELECT d.* ")
                           wxT("FROM ") + DB::GetTableName("financial.expense") + wxT(" d ")
                           wxT("WHERE d.status = 'A' ")
                           wxT("ORDER BY nome"));

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


bool ExpensePaymentEditDialog::SetPaymentId(long paymentId)
{
  // Load purchase information

  DBQuery dbquery;
  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxString::Format(wxT("SELECT expense_id, pagamento_forma, valor, quitada, account_id, descricao, ")
                           wxT("to_char(pagamento_date, 'DD/MM/YYYY') AS pagamento_date, ")
                           wxT("EXTRACT(HOUR FROM pagamento_time) as payment_hr, EXTRACT(MINUTE FROM pagamento_time) as payment_min, ")
                           wxT("to_char(register_dt, 'DD/MM/YYYY HH24:MI:SS') AS register_dt ")
                           wxT("FROM " + DB::GetTableName("financial.expense.payment.installment") + " p ")
                           wxT("WHERE p.installment_id = %ld "), paymentId);
  }
  else {
    sql = wxT("SELECT expense_id, pagamento_forma, valor, quitada, account_id, descricao, ")
          wxT("strftime('%d/%m/%Y', pagamento_date) AS pagamento_date, ")
          wxT("strftime('%H', pagamento_time) as payment_hr, strftime('%M', pagamento_time) as payment_min, ")
          wxT("strftime('%d/%m/%Y %H:%M:%S', register_dt) AS register_dt ")
          wxT("FROM " + DB::GetTableName("financial.expense.payment.installment") + " p ") +
          wxString::Format(wxT("WHERE p.installment_id = %ld "), paymentId);
  }

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    statusBar->SetStatusText(wxT("Não foi possível obter as informações sobre a parcela de pagamento."));

    messenger->Register(wxT("Database error: ") + dbquery.GetMessageStr(), LogMessage::MSG_DBG);

    return false;
  }
  else if (dbquery.GetRowsCount() < 1) {
    statusBar->SetStatusText(wxT("Não foram encontradas informações sobre a parcela de pagamento solicitada."));

    return false;
  }

  this->paymentId = paymentId;

  if (dbquery.GetFieldStrByName(0, wxT("expense_id")).Length() > 0) {
    for (size_t index = 0; index < choiceExpense->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceExpense->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("expense_id")) == container->GetData()) {
        choiceExpense->SetSelection(index);
      }
    }
  }

  if (dbquery.GetFieldStrByName(0, wxT("account_id")).Length() > 0) {
    for (size_t index = 0; index < choiceAccountOutput->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("account_id")) == container->GetData()) {
        choiceAccountOutput->SetSelection(index);
      }
    }
  }

  teditValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, wxT("valor"))));

  teditDescription->SetValue(dbquery.GetFieldStrByName(0, wxT("descricao")));

  teditPaymentDate->SetValue(dbquery.GetFieldStrByName(0, wxT("pagamento_date")));

  spinCtrlTimeHour->SetValue(dbquery.GetFieldIntByName(0, wxT("payment_hr")));

  spinCtrlTimeMinutes->SetValue(dbquery.GetFieldIntByName(0, wxT("payment_min")));

  if (dbquery.GetFieldStrByName(0, wxT("quitada")).Cmp(wxT("1")) == 0) {
    toggleBtnPaid->SetValue(true);
  }
  else {
    toggleBtnPaid->SetValue(false);
  }

  wxCommandEvent evt(wxEVT_TOGGLEBUTTON, wxID_ANY);

  toggleBtnPaid->GetEventHandler()->ProcessEvent(evt);

  if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("V")) == 0) {
    radioBtnPaymentLiveCash->SetValue(true);

    PaymentLiveCashSelect();
  }
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("D")) == 0) {
    radioBtnPaymentDeposit->SetValue(true);

    PaymentDepositSelect();
  }
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("T")) == 0) {
    radioBtnPaymentTransfer->SetValue(true);

    PaymentTransferSelect();
  }
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("B")) == 0) {
    radioBtnPaymentCardDebit->SetValue(true);

    PaymentCardDebitSelect();
  }
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("C")) == 0) {
    radioBtnPaymentCardCredit->SetValue(true);

    PaymentCardCreditSelect();
  }
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("K")) == 0) {
    radioBtnPaymentTicket->SetValue(true);

    PaymentCardCreditSelect();
  }
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("Q")) == 0) {
    radioBtnPaymentCheque->SetValue(true);

    PaymentChequeSelect();
  }

  return true;
}


void ExpensePaymentEditDialog::Terminate()
{
  Close();

  Destroy();
}


void ExpensePaymentEditDialog::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F3> - Save / confirm
  if (event.GetKeyCode() == 341) {
    SaveData();
  }
  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


void ExpensePaymentEditDialog::OnPaymentLiveCashRequest(wxCommandEvent &event)
{
  PaymentLiveCashSelect();
}


void ExpensePaymentEditDialog::OnPaymentDepositRequest(wxCommandEvent &event)
{
  PaymentDepositSelect();
}


void ExpensePaymentEditDialog::OnPaymentTransferRequest(wxCommandEvent &event)
{
  PaymentTransferSelect();
}


void ExpensePaymentEditDialog::OnPaymentCardDebitRequest(wxCommandEvent &event)
{
  PaymentCardDebitSelect();
}


void ExpensePaymentEditDialog::OnPaymentCardCreditRequest(wxCommandEvent &event)
{
  PaymentCardCreditSelect();
}


void ExpensePaymentEditDialog::OnPaymentChequeRequest(wxCommandEvent &event)
{
  PaymentChequeSelect();
}



void ExpensePaymentEditDialog::PaymentLiveCashSelect()
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


void ExpensePaymentEditDialog::PaymentDepositSelect()
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

    szBoxPaymentInfo->Add(panelPaymentDeposit, 0, wxALL|wxEXPAND|wxGROW|wxALIGN_TOP, 1);

    panelPaymentDeposit->Layout();

    panelPaymentDeposit->GetSizer()->Layout();

    panelPaymentDeposit->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentEditDialog::PaymentTransferSelect()
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

    szBoxPaymentInfo->Add(panelPaymentTransfer, 0, wxALL|wxEXPAND|wxGROW|wxALIGN_TOP, 1);

    panelPaymentTransfer->Layout();

    panelPaymentTransfer->GetSizer()->Layout();

    panelPaymentTransfer->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentEditDialog::PaymentCardDebitSelect()
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

    szBoxPaymentInfo->Add(panelPaymentCardDebit, 0, wxALL|wxEXPAND|wxGROW|wxALIGN_TOP, 1);

    panelPaymentCardDebit->Layout();

    panelPaymentCardDebit->GetSizer()->Layout();

    panelPaymentCardDebit->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentEditDialog::PaymentCardCreditSelect()
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

    szBoxPaymentInfo->Add(panelPaymentCardCredit, 0, wxALL|wxEXPAND|wxGROW|wxALIGN_TOP, 1);

    panelPaymentCardCredit->Layout();

    panelPaymentCardCredit->GetSizer()->Layout();

    panelPaymentCardCredit->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentEditDialog::PaymentChequeSelect()
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

    szBoxPaymentInfo->Add(panelPaymentCheque, 0, wxALL|wxEXPAND|wxGROW|wxALIGN_TOP, 1);

    panelPaymentCheque->Layout();

    panelPaymentCheque->GetSizer()->Layout();

    panelPaymentCheque->GetContainingSizer()->Layout();
  }

  szBoxPaymentInfo->Layout();

  Layout();

  wxGetApp().GetTopWindow()->Layout();
}


void ExpensePaymentEditDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


int ExpensePaymentEditDialog::SaveData()
{
  if (teditValue->GetValue().Length() < 1) {
    wxMessageBox(wxT("Informe o valor do pagamento."));

    return -1;
  }

  if (toggleBtnPaid->GetValue() && teditPaymentDate->GetValue().Length() < 1) {
    wxMessageBox(wxT("Se o pagamento está quitado é necessário informar a data de quitação do mesmo."));

    return -1;
  }

  std::vector<wxString> params, fields, valueModifiers;
  DBQuery dbquery;

  fields.clear();
  params.clear();

  //fields.push_back(wxT("expense_id"));
  //params.push_back(wxString::Format(wxT("%ld"), this->paymentId));

  fields.push_back(wxT("account_id"));
  if (choiceAccountOutput->GetSelection() >= 0 && choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountOutput->GetClientObject(choiceAccountOutput->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  valueModifiers.push_back("");

  fields.push_back(wxT("expense_id"));
  if (choiceExpense->GetSelection() >= 0 && choiceExpense->GetClientObject(choiceExpense->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceExpense->GetClientObject(choiceExpense->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  valueModifiers.push_back("");

  params.push_back(teditDescription->GetValue());
  fields.push_back(wxT("descricao"));
  valueModifiers.push_back("");

  params.push_back(Monetary::Monetary2Database(teditValue->GetValue()));
  fields.push_back(wxT("valor"));
  valueModifiers.push_back("");

  fields.push_back(wxT("pagamento_date"));

  if (teditPaymentDate->GetValue().Length() > 0) {
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
  else {
    params.push_back(wxEmptyString);
    valueModifiers.push_back("");
  }

  // Time

  wxString paymentTimeStr = wxString::Format(wxT("%.2d:%.2d"), spinCtrlTimeHour->GetValue(), spinCtrlTimeMinutes->GetValue());

  fields.push_back("pagamento_time");
  params.push_back(paymentTimeStr);
  valueModifiers.push_back("");

  fields.push_back(wxT("quitada"));
  if (toggleBtnPaid->GetValue()) {
    params.push_back(wxT("1"));
  }
  else {
    params.push_back(wxT("0"));
  }
  valueModifiers.push_back("");

  fields.push_back(wxT("pagamento_forma"));
  if (radioBtnPaymentLiveCash->GetValue()) {
    params.push_back(wxT("V"));
  }
  else if (radioBtnPaymentDeposit->GetValue()) {
    params.push_back(wxT("D"));
  }
  else if (radioBtnPaymentTransfer->GetValue()) {
    params.push_back(wxT("T"));
  }
  else if (radioBtnPaymentCardDebit->GetValue()) {
    params.push_back(wxT("B"));
  }
  else if (radioBtnPaymentCardCredit->GetValue()) {
    params.push_back(wxT("C"));
  }
  else if (radioBtnPaymentTicket->GetValue()) {
    params.push_back(wxT("K"));
  }
  else if (radioBtnPaymentCheque->GetValue()) {
    params.push_back(wxT("Q"));
  }
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

  wxString sql = DBUtils::MountSQLUpdate(DB::GetTableName("financial.expense.payment.installment"),
                                         fields,
                                         wxString::Format(wxT("installment_id = %ld"), this->paymentId),
                                         valueModifiers);

  //wxMessageBox(sql);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível cadastrar a parcela de pagamento para a despesa.\n\n")
                 wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());
  }
  else {
    statusBar->SetStatusText(wxT("As infomações sobre a parcela de pagamento foram modificadas."));
  }

  return 0;
}


void ExpensePaymentEditDialog::OnSaveRequest(wxCommandEvent &event)
{
  SaveData();
}


void ExpensePaymentEditDialog::OnPaymentDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditPaymentDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarPaymentDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarPaymentDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditPaymentDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                               calendarDialog.selectedMonth + wxT("/") + 
                               calendarDialog.selectedYear);
  }
}


void ExpensePaymentEditDialog::OnPayStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Enable();

  event.Skip();
}


void ExpensePaymentEditDialog::OnNotPaidStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Disable();

  event.Skip();
}


void ExpensePaymentEditDialog::OnPaymentValueKillFocus(wxFocusEvent &event)
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
}


void ExpensePaymentEditDialog::OnDeleteRequest(wxCommandEvent &event) {
  if (wxYES != wxMessageBox(wxT("Você confirma a exclusão das informações sobre essa parcela de pagamento?"),
                            wxT("Confirmação de exclusão de informações sobre parcela de pagamento"),
                            wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION, NULL)) {

    statusBar->SetStatusText(wxT("A exclusão das informações sobre a parcela de pagamento foi cancelada."));

    return;
  }

  wxString sqlCommand = wxString::Format(wxT("DELETE FROM " + DB::GetTableName("financial.expense.payment.installment") + " WHERE installment_id = %d"),
                                         this->paymentId);

  DBQuery dbquery;

  int rc = dbquery.Execute(sqlCommand);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível excluir as informações sobre a parcela de pagamento.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }
  else {
    Close();
  }
}


BEGIN_EVENT_TABLE(ExpensePaymentEditDialog, wxDialog)
  EVT_TOOL(DLG_EXPENSE_PAYMENT_EDIT_TOOL_SAVE_ID, ExpensePaymentEditDialog::OnSaveRequest)
  EVT_TOOL(DLG_EXPENSE_PAYMENT_EDIT_TOOL_CLOSE_ID, ExpensePaymentEditDialog::OnCloseRequest)
  EVT_TOOL(DLG_EXPENSE_PAYMENT_EDIT_TOOL_DELETE_ID, ExpensePaymentEditDialog::OnDeleteRequest)

  EVT_CHAR_HOOK(ExpensePaymentEditDialog::OnCharHook)

  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_LIVECASH_ID, ExpensePaymentEditDialog::OnPaymentLiveCashRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ID, ExpensePaymentEditDialog::OnPaymentDepositRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_TRANSFER_ID, ExpensePaymentEditDialog::OnPaymentTransferRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_DEBIT_ID, ExpensePaymentEditDialog::OnPaymentCardDebitRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID, ExpensePaymentEditDialog::OnPaymentCardCreditRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBUTTON_CHEQUE_ID, ExpensePaymentEditDialog::OnPaymentChequeRequest)

  EVT_BUTTON(DLG_EXPENSE_PAYMENT_EDIT_BTN_PAYMENT_DATE_ID, ExpensePaymentEditDialog::OnPaymentDateChooseRequest)

  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBTN_PAID_ID, ExpensePaymentEditDialog::OnPayStateRequest)
  EVT_RADIOBUTTON(DLG_EXPENSE_PAYMENT_EDIT_RADIOBTN_NOT_PAID_ID, ExpensePaymentEditDialog::OnNotPaidStateRequest)
END_EVENT_TABLE()
