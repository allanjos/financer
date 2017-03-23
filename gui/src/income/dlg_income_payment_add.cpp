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
#include "dlg_income_payment_add.hpp"
#include "dlg_calendar.hpp"
#include "app.hpp"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


IncomePaymentAddDialog::IncomePaymentAddDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxT("Receita - Adição de parcela de pagamento"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxWANTS_CHARS | wxTAB_TRAVERSAL)
{
  this->incomeId = -1;

  SetAutoLayout(true);

  SetSize(800, 500);

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

  toolBar->AddTool(DLG_INCOME_PAYMENT_ADD_TOOL_SAVE_ID,
                   wxT(" Cadastrar pagamento"),
                   wxBitmap(save_16x16_xpm),
                   wxT("Salvar informações sobre o pagamento."));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_INCOME_PAYMENT_ADD_TOOL_CLOSE_ID,
                   Lang::Get("Close"),
                   wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szMargin->AddSpacer(5);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND, GUI::WINDOW_MARGIN_SIZE);

  // Income name

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText* label = new wxStaticText(this, wxID_STATIC, "Receita: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditIncomeName = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1), wxTE_READONLY);
  szRow->Add(teditIncomeName, 1, wxALL | wxEXPAND, 1);

  // Installment value

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT("Valor: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(Config::currencyList), Config::currencyList);
  cboxValueCurrency->SetStringSelection(Config::Get("currency"));
  szRow->Add(cboxValueCurrency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency->Disable();

  teditValue = new wxTextCtrl(this, DLG_INCOME_PAYMENT_ADD_TEDIT_VALUE_ID, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(30);

  // Date

  label = new wxStaticText(this, wxID_STATIC, wxT("Data de pagamento: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditPaymentDate = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditPaymentDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate = new wxBitmapButton(this,
                                              DLG_INCOME_PAYMENT_ADD_BTN_PAYMENT_DATE_ID,
                                              wxBitmap(calendar_bmp_btn_xpm));
  szRow->Add(btnCalendarPaymentDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate->SetMinSize(wxSize(-1, teditPaymentDate->GetSize().GetHeight()));

  szRow->AddSpacer(30);


  // Paid or not

  label = new wxStaticText(this, wxID_STATIC, wxT("Pagamento quitado: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnPaid = new wxRadioButton(this, DLG_INCOME_PAYMENT_ADD_RADIOBTN_PAID_ID, wxT("Sim"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnPaid->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnPaid, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnPaid->SetValue(true);

  radioBtnNotPaid = new wxRadioButton(this, DLG_INCOME_PAYMENT_ADD_RADIOBTN_NOT_PAID_ID, wxT("Não"));
  radioBtnNotPaid->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnNotPaid, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  // Account input

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *lblAccountParent = new wxStaticText(this,
                                                    wxID_STATIC,
                                                    wxT("Conta de entrada: "));
  szRow->Add(lblAccountParent, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  choiceAccountInput = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceAccountInput, 1, wxALL | wxEXPAND, 1);


  // Invoice

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *lblNotaFiscal = new wxStaticText(this,
                                                    wxID_STATIC,
                                                    "Selecione a nota fiscal: ");
  szRow->Add(lblNotaFiscal, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  cboxNotaFiscal = new wxChoice(this, wxID_ANY);
  szRow->Add(cboxNotaFiscal, 1, wxALL | wxEXPAND, 1);


  // Description

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT("Descrição: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDescription = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditDescription, 1, wxALL | wxEXPAND, 1);


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
                                              DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_LIVECASH_ID,
                                              wxT("À vista"),
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxRB_GROUP);
  szPaymentWays->Add(radioBtnPaymentLiveCash, 0, wxALL, 5);

  radioBtnPaymentLiveCash->SetValue(true);

  // Deposit

  radioBtnPaymentDeposit = new wxRadioButton(this,
                                             DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ID,
                                             wxT("Depósito bancário"));
  szPaymentWays->Add(radioBtnPaymentDeposit, 0, wxALL, 5);

  // Electronic transfer

  radioBtnPaymentTransfer = new wxRadioButton(this,
                                              DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_TRANSFER_ID,
                                              wxT("Transferência bancária"));
  szPaymentWays->Add(radioBtnPaymentTransfer, 0, wxALL, 5);

  // Debit card

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCardDebit = new wxRadioButton(this,
                                               DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_DEBIT_ID,
                                               wxT("Cartão de débito"));
  szRow->Add(radioBtnPaymentCardDebit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Credit card

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCardCredit = new wxRadioButton(this,
                                                DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Cartão de crédito"));
  szRow->Add(radioBtnPaymentCardCredit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Ticket

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentTicket = new wxRadioButton(this,
                                                DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Vale alimentação"));
  szRow->Add(radioBtnPaymentTicket, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Cheque

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szPaymentWays->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 5);

  radioBtnPaymentCheque = new wxRadioButton(this,
                                            DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CHEQUE_ID,
                                            "Cheque");
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

  // Bank

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxDeposit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Banco: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositBankName = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
  szRow->Add(teditDepositBankName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxDeposit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Conta: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositAccount = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditDepositAccount, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Agency

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, wxT("        Agência: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositAgency = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditDepositAgency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Account type

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxDeposit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Tipo de conta: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnDepositAccountCurrent = new wxRadioButton(panelPaymentDeposit,
                                                    DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnDepositAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnDepositAccountCurrent->SetValue(true);

  radioBtnDepositAccountSavings = new wxRadioButton(panelPaymentDeposit,
                                                    DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"));
  szRow->Add(radioBtnDepositAccountSavings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Depositor

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxDeposit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Depositante: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositDepositor = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(400, -1));
  szRow->Add(teditDepositDepositor, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Time

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxDeposit->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, "Hora: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositTimeHour = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(30, -1));
  szRow->Add(teditDepositTimeHour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelPaymentDeposit, wxID_STATIC, " : ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditDepositTimeMinutes = new wxTextCtrl(panelPaymentDeposit, wxID_ANY, "", wxDefaultPosition, wxSize(30, -1));
  szRow->Add(teditDepositTimeMinutes, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


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
                                                    DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnTransferAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTransferAccountCurrent->SetValue(true);

  radioBtnTransferAccountSavings = new wxRadioButton(panelPaymentTransfer,
                                                    DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"));
  szRow->Add(radioBtnTransferAccountSavings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Transferer

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Nome do depositante: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditTransferDepositor = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(400, -1));
  szRow->Add(teditTransferDepositor, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Time

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szBoxTransfer->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, "Hora: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditTransferTimeHour = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(30, -1));
  szRow->Add(teditTransferTimeHour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelPaymentTransfer, wxID_STATIC, " : ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditTransferTimeMinutes = new wxTextCtrl(panelPaymentTransfer, wxID_ANY, "", wxDefaultPosition, wxSize(30, -1));
  szRow->Add(teditTransferTimeMinutes, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


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
                                              wxBitmap(calendar_bmp_btn_xpm));
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
                                              wxBitmap(calendar_bmp_btn_xpm));
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
                                                       wxBitmap(calendar_bmp_btn_xpm));
  szRow->Add(btnCalendarChequeWithdrawalDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szMargin->Add(statusBar, 0, wxALL|wxGROW|wxEXPAND, 0);


  //szMargin->SetSizeHints(this);

  this->CenterOnParent();

  PaymentLiveCashSelect();

  LoadAccountList();

  LoadReceiptsList();


  // Payment default date. Current date

  wxDateTime dateTimeNow = wxDateTime::Now();

  teditPaymentDate->SetValue(dateTimeNow.Format(wxT("%d/%m/%Y")));


  teditValue->Connect(wxEVT_KILL_FOCUS,
                      wxFocusEventHandler(IncomePaymentAddDialog::OnPaymentValueKillFocus),
                      NULL,
                      this);

  teditValue->SetFocus();
}


int IncomePaymentAddDialog::LoadIncomeInfo()
{
  // Load income information

  wxString sql = wxString::Format(wxT("SELECT r.nome, r.montante_fixo, r.montante_valor, r.parcela_valor, r.account_id ")
                                  wxT("FROM ") + DB::GetTableName("financial.income") + wxT(" r ")
                                  wxT("WHERE r.income_id = %ld "), this->incomeId);

  //wxMessageBox(sql);

  DBQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter informações sobre a receita à qual a parcela está ligada.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());

    return -1;
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxString(wxT("Não foram encontradas informações sobre a receita à qual a parcela está ligada.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());

    return -1;
  }

  // Income name

  teditIncomeName->SetValue(dbquery.GetFieldStrByName(0, wxT("nome")));

  // Account

  if (dbquery.GetFieldStrByName(0, wxT("account_id")).Length() > 0) {
    for (size_t accountIndex = 0; accountIndex < choiceAccountInput->GetCount(); accountIndex++) {
      ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(accountIndex);

      if (dbquery.GetFieldIntByName(0, wxT("account_id")) == container->GetData()) {
        choiceAccountInput->SetSelection(accountIndex);
      }
    }
  }

  // Installment value

  if (dbquery.GetFieldStrByName(0, wxT("parcela_valor")).Length() > 0) {
    teditValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, wxT("parcela_valor"))));
  }
  else if (dbquery.GetFieldStrByName(0, wxT("montante_valor")).Length() > 0) {
    teditValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, wxT("montante_valor"))));
  }

  return 0;
}


int IncomePaymentAddDialog::LoadAccountList()
{
  DBQuery dbquery;

  choiceAccountInput->Freeze();

  choiceAccountInput->Clear();

  choiceAccountInput->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  int rc = dbquery.Execute(wxT("SELECT * ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" ")
                           wxT("WHERE tipo_id = 'A' ")
                           wxT("AND conta_parent_id NOT IN (SELECT account_id ")
                                                       wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" ")
                                                       wxT("WHERE conta_parent_id IS NOT NULL) ")
                           wxT("ORDER BY lower(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de contas de saída cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("account_id")));

      choiceAccountInput->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  choiceAccountInput->SetSelection(0);

  choiceAccountInput->Thaw();

  return 0;
}


int IncomePaymentAddDialog::LoadReceiptsList()
{
  DBQuery dbquery;

  cboxNotaFiscal->Freeze();

  cboxNotaFiscal->Clear();

  cboxNotaFiscal->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  cboxNotaFiscal->SetSelection(0);

  cboxNotaFiscal->Thaw();

  return 0;
}


bool IncomePaymentAddDialog::SetIncomeId(long incomeId)
{
  // Load purchase information

  DBQuery dbquery;

  wxString sql = wxString::Format(wxT("SELECT r.* ")
                                  wxT("FROM ") + DB::GetTableName("financial.income") + wxT(" r ")
                                  wxT("WHERE r.income_id = %ld "), incomeId);

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de contas de saída cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());

    return false;
  }
  else if (dbquery.GetRowsCount() > 0) {
    this->incomeId = incomeId;

    LoadIncomeInfo();

    return true;
  }

  return false;
}


void IncomePaymentAddDialog::Terminate()
{
  Close();

  Destroy();
}


void IncomePaymentAddDialog::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // Save / confirm
  switch (event.GetKeyCode()) {
    case WXK_F3:
      SaveData();

      break;

    // Exit <ESC>
    case WXK_ESCAPE:
      Close();

      break;
  }

  event.Skip();
}


void IncomePaymentAddDialog::OnPaymentLiveCashRequest(wxCommandEvent &event)
{
  PaymentLiveCashSelect();
}


void IncomePaymentAddDialog::OnPaymentDepositRequest(wxCommandEvent &event)
{
  PaymentDepositSelect();
}


void IncomePaymentAddDialog::OnPaymentTransferRequest(wxCommandEvent &event)
{
  PaymentTransferSelect();
}


void IncomePaymentAddDialog::OnPaymentCardDebitRequest(wxCommandEvent &event)
{
  PaymentCardDebitSelect();
}


void IncomePaymentAddDialog::OnPaymentCardCreditRequest(wxCommandEvent &event)
{
  PaymentCardCreditSelect();
}


void IncomePaymentAddDialog::OnPaymentChequeRequest(wxCommandEvent &event)
{
  PaymentChequeSelect();
}



void IncomePaymentAddDialog::PaymentLiveCashSelect()
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


void IncomePaymentAddDialog::PaymentDepositSelect()
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


void IncomePaymentAddDialog::PaymentTransferSelect()
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


void IncomePaymentAddDialog::PaymentCardDebitSelect()
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


void IncomePaymentAddDialog::PaymentCardCreditSelect()
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


void IncomePaymentAddDialog::PaymentChequeSelect()
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


void IncomePaymentAddDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


int IncomePaymentAddDialog::SaveData()
{
  if (teditValue->GetValue().Length() < 1) {
    wxMessageBox(wxT("Informe o valor do pagamento."));

    return -1;
  }

  if (radioBtnPaid->GetValue() && teditPaymentDate->GetValue().Length() < 1) {
    wxMessageBox(wxT("Se o pagamento está quitado é necessário informar a data de quitação do mesmo."));

    return -1;
  }

  std::vector<wxString> params, fields, valueModifiers;
  DBQuery dbquery;

  fields.clear();
  params.clear();
  valueModifiers.clear();

  fields.push_back(wxT("income_id"));
  params.push_back(wxString::Format(wxT("%ld"), this->incomeId));
  valueModifiers.push_back("");

  if (choiceAccountInput->GetSelection() >= 0 && choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("account_id"));
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
      valueModifiers.push_back("");
    }
  }

  fields.push_back(wxT("valor"));
  params.push_back(Monetary::Monetary2Database(teditValue->GetValue()));
  valueModifiers.push_back("");

  if (teditDescription->GetValue().Length() > 0) {
    fields.push_back(wxT("descricao"));
    params.push_back(teditDescription->GetValue());
    valueModifiers.push_back("");
  }

  if (teditPaymentDate->GetValue().Length() > 0) {
    fields.push_back(wxT("pagamento_date"));

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

  fields.push_back(wxT("quitada"));
  if (radioBtnPaid->GetValue()) {
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
  else if (radioBtnPaymentCheque->GetValue()) {
    params.push_back(wxT("Q"));
  }
  valueModifiers.push_back("");

  LogMessenger *messenger = LogMessenger::GetInstance();

  wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("financial.income.payment.installment"), fields, valueModifiers);

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível cadastrar a parcela de pagamento para a receita."));

    messenger->Register(wxT("DB: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    statusBar->SetStatusText(wxT("Parcela de pagamento adicionada à receita."));

    Close();
  }

  return 0;
}


void IncomePaymentAddDialog::OnSaveRequest(wxCommandEvent &event)
{
  SaveData();
}


void IncomePaymentAddDialog::OnPaymentDateChooseRequest(wxCommandEvent &event)
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


void IncomePaymentAddDialog::OnPayStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Enable();
}


void IncomePaymentAddDialog::OnNotPaidStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Disable();
}


void IncomePaymentAddDialog::OnPaymentValueKillFocus(wxFocusEvent &event)
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


BEGIN_EVENT_TABLE(IncomePaymentAddDialog, wxDialog)
  EVT_TOOL(DLG_INCOME_PAYMENT_ADD_TOOL_SAVE_ID, IncomePaymentAddDialog::OnSaveRequest)
  EVT_TOOL(DLG_INCOME_PAYMENT_ADD_TOOL_CLOSE_ID, IncomePaymentAddDialog::OnCloseRequest)

  EVT_CHAR_HOOK(IncomePaymentAddDialog::OnCharHook)

  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_LIVECASH_ID, IncomePaymentAddDialog::OnPaymentLiveCashRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_DEPOSIT_ID, IncomePaymentAddDialog::OnPaymentDepositRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_TRANSFER_ID, IncomePaymentAddDialog::OnPaymentTransferRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_DEBIT_ID, IncomePaymentAddDialog::OnPaymentCardDebitRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CARD_CREDIT_ID, IncomePaymentAddDialog::OnPaymentCardCreditRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBUTTON_CHEQUE_ID, IncomePaymentAddDialog::OnPaymentChequeRequest)

  EVT_BUTTON(DLG_INCOME_PAYMENT_ADD_BTN_PAYMENT_DATE_ID, IncomePaymentAddDialog::OnPaymentDateChooseRequest)

  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBTN_PAID_ID, IncomePaymentAddDialog::OnPayStateRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_ADD_RADIOBTN_NOT_PAID_ID, IncomePaymentAddDialog::OnNotPaidStateRequest)
END_EVENT_TABLE()