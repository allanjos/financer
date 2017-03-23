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
#include "gui_controls.hpp"
#include "dlg_income_payment_edit.hpp"
#include "dlg_calendar.hpp"
#include "app.hpp"
//#include "pixmaps/product_32x32.xpm"
#include "pixmaps/cart_add_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


IncomePaymentEditDialog::IncomePaymentEditDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxT("Receita - Informações sobre parcela de pagamento"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxWANTS_CHARS|wxTAB_TRAVERSAL)
{
  this->paymentId = -1;

  SetAutoLayout(true);

  SetSize(800, 500);

  SetIcon(wxIcon(cart_add_16x16_xpm));

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  // Title

  /* GUI::DrawTitle(this,
                 wxT("Informações sobre parcela de pagamento de receita"),
                 wxT("Informações sobre parcela de pagamento cadastrada para a receita."),
                 wxBitmap(product_32x32_xpm)); */


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_INCOME_PAYMENT_EDIT_TOOL_SAVE_ID,
                   wxT(" Salvar modificações"),
                   wxBitmap(save_16x16_xpm),
                   wxT("Salvar informações sobre o pagamento."));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_INCOME_PAYMENT_EDIT_TOOL_DELETE_ID,
                   wxT(" Excluir parcela"),
                   wxBitmap(delete_16x16_xpm),
                   wxT("Excluir informações sobre a parcela de pagamento."));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_INCOME_PAYMENT_EDIT_TOOL_CLOSE_ID,
                   wxT(" Fechar"),
                   wxBitmap(close_16x16_xpm),
                   wxT("Fechar interface de cadastro de parcela de pagamento."));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND, GUI::WINDOW_MARGIN_SIZE);


  // Income name

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, "Receita: ");
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  choiceIncome = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceIncome, 1, wxALL | wxEXPAND, 1);

  // Installment value

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT("Valor: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(Config::currencyList), Config::currencyList);
  cboxValueCurrency->SetStringSelection(Config::Get("currency"));
  szRow->Add(cboxValueCurrency, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxValueCurrency->Disable();

  teditValue = new wxTextCtrl(this, DLG_INCOME_PAYMENT_EDIT_TEDIT_VALUE_ID, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(30);

  // Date

  label = new wxStaticText(this, wxID_STATIC, wxT("Data de pagamento: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditPaymentDate = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditPaymentDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate = new wxBitmapButton(this,
                                              DLG_INCOME_PAYMENT_EDIT_BTN_PAYMENT_DATE_ID,
                                              wxBitmap(calendar_bmp_btn_xpm),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szRow->Add(btnCalendarPaymentDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarPaymentDate->SetMinSize(wxSize(-1, teditPaymentDate->GetSize().GetHeight()));

  szRow->AddSpacer(30);


  // Paid or not

  label = new wxStaticText(this, wxID_STATIC, wxT("Pagamento quitado: "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnPaid = new wxRadioButton(this, DLG_INCOME_PAYMENT_EDIT_RADIOBTN_PAID_ID, wxT("Sim"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  radioBtnPaid->SetForegroundColour(*wxBLUE);
  szRow->Add(radioBtnPaid, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  radioBtnPaid->SetValue(true);

  radioBtnNotPaid = new wxRadioButton(this, DLG_INCOME_PAYMENT_EDIT_RADIOBTN_NOT_PAID_ID, wxT("Não"));
  radioBtnNotPaid->SetForegroundColour(*wxRED);
  szRow->Add(radioBtnNotPaid, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 20);

  // Account input

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *lblAccountParent = new wxStaticText(this,
                                                    wxID_STATIC,
                                                    wxT("Conta de entrada: "),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
  szRow->Add(lblAccountParent, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  choiceAccountInput = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceAccountInput, 1, wxALL | wxEXPAND, 1);


  // Invoice

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxGROW, 1);

  wxStaticText *lblNotaFiscal = new wxStaticText(this,
                                                    wxID_STATIC,
                                                    "Selecione a nota fiscal: ",
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
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
                                              DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_LIVECASH_ID,
                                              wxT("À vista"),
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxRB_GROUP);
  szPaymentWays->Add(radioBtnPaymentLiveCash, 0, wxALL, 5);

  radioBtnPaymentLiveCash->SetValue(true);

  // Deposit

  radioBtnPaymentDeposit = new wxRadioButton(this,
                                             DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ID,
                                             wxT("Depósito bancário"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentDeposit, 0, wxALL, 5);

  // Electronic transfer

  radioBtnPaymentTransfer = new wxRadioButton(this,
                                              DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_TRANSFER_ID,
                                              wxT("Transferência bancária"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentTransfer, 0, wxALL, 5);

  // Debit card

  radioBtnPaymentCardDebit = new wxRadioButton(this,
                                               DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_DEBIT_ID,
                                               wxT("Cartão de débito"),
                                               wxDefaultPosition,
                                               wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentCardDebit, 0, wxALL, 5);

  // Credit card

  radioBtnPaymentCardCredit = new wxRadioButton(this,
                                                DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Cartão de crédito"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentCardCredit, 0, wxALL, 5);

  // Ticket

  radioBtnPaymentTicket = new wxRadioButton(this,
                                                DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID,
                                                wxT("Vale alimentação"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentTicket, 0, wxALL, 5);

  // Cheque

  radioBtnPaymentCheque = new wxRadioButton(this,
                                            DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CHEQUE_ID,
                                            "Cheque",
                                            wxDefaultPosition,
                                            wxDefaultSize);
  szPaymentWays->Add(radioBtnPaymentCheque, 0, wxALL, 5);


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
                                                    DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnDepositAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnDepositAccountCurrent->SetValue(true);

  radioBtnDepositAccountSavings = new wxRadioButton(panelPaymentDeposit,
                                                    DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
                                                    wxT("Conta Poupança"),
                                                    wxDefaultPosition,
                                                    wxDefaultSize);
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
                                                    DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_CURRENT_ID,
                                                    "Conta Corrente",
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxRB_GROUP);
  szRow->Add(radioBtnTransferAccountCurrent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  radioBtnTransferAccountCurrent->SetValue(true);

  radioBtnTransferAccountSavings = new wxRadioButton(panelPaymentTransfer,
                                                    DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ACCOUNT_SAVINGS_ID,
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

  szMargin->Add(statusBar, 0, wxALL|wxGROW|wxEXPAND, 0);


  this->CenterOnParent();

  PaymentLiveCashSelect();

  LoadIncomeList();

  LoadAccountList();

  LoadReceiptsList();


  // Payment default date. Current date

  wxDateTime dateTimeNow = wxDateTime::Now();

  teditPaymentDate->SetValue(dateTimeNow.Format(wxT("%d/%m/%Y")));


  teditValue->Connect(wxEVT_KILL_FOCUS,
                      wxFocusEventHandler(IncomePaymentEditDialog::OnPaymentValueKillFocus),
                      NULL,
                      this);

  teditValue->SetFocus();
}


int IncomePaymentEditDialog::LoadIncomeList()
{
  DBQuery dbquery;

  choiceIncome->Freeze();

  choiceIncome->Clear();

  choiceIncome->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  int rc = dbquery.Execute(wxT("SELECT i.* ")
                           wxT("FROM ") + DB::GetTableName("financial.income") + wxT(" i ")
                           wxT("WHERE i.status = 'A' ")
                           wxT("ORDER BY i.nome"));

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de receitas cadastradas.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("income_id")));

      choiceIncome->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  choiceIncome->SetSelection(0);

  choiceIncome->Thaw();

  return 0;
}


int IncomePaymentEditDialog::LoadAccountList()
{
  DBQuery dbquery;

  choiceAccountInput->Freeze();

  choiceAccountInput->Clear();

  choiceAccountInput->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  int rc = dbquery.Execute("SELECT * "
                           "FROM " + DB::GetTableName("accounting.account") + " "
                           "WHERE tipo_id = 'A' "
                           "AND conta_parent_id NOT IN (SELECT account_id "
                                                       "FROM " + DB::GetTableName("accounting.account") + " "
                                                       "WHERE conta_parent_id IS NOT NULL) "
                           "ORDER BY lower(nome)");

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de contas de saída cadastradas."),
                 wxT("Erro"),
                 wxOK,
                 this);
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(resultIndex, wxT("account_id")));

      choiceAccountInput->Append(dbquery.GetFieldStrByName(resultIndex, wxT("nome")), container);
    }
  }

  choiceAccountInput->SetSelection(0);

  choiceAccountInput->Thaw();

  return 0;
}


int IncomePaymentEditDialog::LoadReceiptsList()
{
  DBQuery dbquery;

  cboxNotaFiscal->Freeze();

  cboxNotaFiscal->Clear();

  cboxNotaFiscal->Append(wxT("Selecione"), (wxClientData *) new ContainerIntData((int) 0));

  cboxNotaFiscal->SetSelection(0);

  cboxNotaFiscal->Thaw();

  return 0;
}


bool IncomePaymentEditDialog::SetPaymentId(long paymentId)
{
  // Load purchase information

  DBQuery dbquery;

  wxString sql = "";

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxString::Format("SELECT income_id, pagamento_forma, valor, quitada, account_id, descricao, "
                           "to_char(pagamento_date, 'DD/MM/YYYY') AS pagamento_date, "
                           "to_char(register_dt, 'DD/MM/YYYY HH24:MI:SS') AS register_dt "
                           "FROM " + DB::GetTableName("financial.income.payment.installment") + " p "
                           "WHERE p.installment_id = %ld ", paymentId);
  }
  else {
    sql = "SELECT income_id, pagamento_forma, valor, quitada, account_id, descricao, "
                 wxT("strftime('%d/%m/%Y', pagamento_date) AS pagamento_date, ")
                 "strftime('%d/%m/%Y %H:%M:%S', register_dt) AS register_dt "
          "FROM " + DB::GetTableName("financial.income.payment.installment") + " p " +
          wxString::Format("WHERE p.installment_id = %ld ", paymentId);
  }

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre a parcela de pagamento."),
                 wxT("Erro"),
                 wxOK,
                 this);

    return false;
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("Não foram encontradas informações sobre a parcela de pagamento solicitada."),
                 wxT("Erro"),
                 wxOK,
                 this);

    return false;
  }

  this->paymentId = paymentId;

  if (dbquery.GetFieldStrByName(0, wxT("income_id")).Length() > 0) {
    for (size_t index = 0; index < choiceIncome->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceIncome->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("income_id")) == container->GetData()) {
        choiceIncome->SetSelection(index);
      }
    }
  }

  if (dbquery.GetFieldStrByName(0, "account_id").Length() > 0) {
    for (size_t index = 0; index < choiceAccountInput->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("account_id")) == container->GetData()) {
        choiceAccountInput->SetSelection(index);
      }
    }
  }

  teditValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, "valor")));
  
  teditDescription->SetValue(dbquery.GetFieldStrByName(0, "descricao"));

  teditPaymentDate->SetValue(dbquery.GetFieldStrByName(0, "pagamento_date"));

  if (dbquery.GetFieldStrByName(0, "quitada").Cmp("1") == 0) {
    radioBtnPaid->SetValue(true);
    radioBtnNotPaid->SetValue(false);
  }
  else {
    radioBtnNotPaid->SetValue(true);
    radioBtnPaid->SetValue(false);
  }

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
  else if (dbquery.GetFieldStrByName(0, wxT("pagamento_forma")).Cmp(wxT("Q")) == 0) {
    radioBtnPaymentCheque->SetValue(true);

    PaymentChequeSelect();
  }

  return true;
}


void IncomePaymentEditDialog::Terminate()
{
  Close();

  Destroy();
}


void IncomePaymentEditDialog::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F3> - Save / confirm
  if (event.GetKeyCode() == 341) {
    wxMessageBox(wxT("Confirm"));
  }
  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


void IncomePaymentEditDialog::OnPaymentLiveCashRequest(wxCommandEvent &event)
{
  PaymentLiveCashSelect();
}


void IncomePaymentEditDialog::OnPaymentDepositRequest(wxCommandEvent &event)
{
  PaymentDepositSelect();
}


void IncomePaymentEditDialog::OnPaymentTransferRequest(wxCommandEvent &event)
{
  PaymentTransferSelect();
}


void IncomePaymentEditDialog::OnPaymentCardDebitRequest(wxCommandEvent &event)
{
  PaymentCardDebitSelect();
}


void IncomePaymentEditDialog::OnPaymentCardCreditRequest(wxCommandEvent &event)
{
  PaymentCardCreditSelect();
}


void IncomePaymentEditDialog::OnPaymentChequeRequest(wxCommandEvent &event)
{
  PaymentChequeSelect();
}



void IncomePaymentEditDialog::PaymentLiveCashSelect()
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


void IncomePaymentEditDialog::PaymentDepositSelect()
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


void IncomePaymentEditDialog::PaymentTransferSelect()
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


void IncomePaymentEditDialog::PaymentCardDebitSelect()
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


void IncomePaymentEditDialog::PaymentCardCreditSelect()
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


void IncomePaymentEditDialog::PaymentChequeSelect()
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


void IncomePaymentEditDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


void IncomePaymentEditDialog::OnSaveRequest(wxCommandEvent &event)
{
  if (teditValue->GetValue().Length() < 1) {
    wxMessageBox(wxT("Informe o valor do pagamento."),
                 wxT("Campo necessário"),
                 wxOK,
                 this);

    return;
  }

  if (radioBtnPaid->GetValue() && teditPaymentDate->GetValue().Length() < 1) {
    wxMessageBox(wxT("Se o pagamento está quitado é necessário informar a data de quitação do mesmo."),
                 wxT("Informação"),
                 wxOK,
                 this);

    return;
  }

  std::vector<wxString> params, fields, valueModifiers;
  DBQuery dbquery;

  fields.clear();
  params.clear();

  //fields.push_back(wxT("income_id"));
  //params.push_back(wxString::Format(wxT("%ld"), this->paymentId));

  fields.push_back(wxT("account_id"));
  if (choiceAccountInput->GetSelection() >= 0 && choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) choiceAccountInput->GetClientObject(choiceAccountInput->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

  fields.push_back(wxT("valor"));
  params.push_back(Monetary::Monetary2Database(teditValue->GetValue()));
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
    }
  }
  else {
    params.push_back(wxEmptyString);
    valueModifiers.push_back("");
  }

  fields.push_back("descricao");
  if (teditDescription->GetValue().Length() > 0) {
    params.push_back(teditDescription->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

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
  else {
    params.push_back(wxEmptyString);
  }
  valueModifiers.push_back("");

  wxString sql = DBUtils::MountSQLUpdate(DB::GetTableName("financial.income.payment.installment"),
                                         fields,
                                         wxString::Format(wxT("installment_id = %ld"), this->paymentId),
                                         valueModifiers);

  //wxMessageBox(sql);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível modificar as informações sobre a parcela de pagamento da receita."),
                 wxT("Erro"),
                 wxOK,
                 this);
  }
  else {
    statusBar->SetStatusText(wxT("As informações sobre a parcela de pagamento foram modificadas."));
  }
}


void IncomePaymentEditDialog::OnPaymentDateChooseRequest(wxCommandEvent &event)
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


void IncomePaymentEditDialog::OnPayStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Enable();
}


void IncomePaymentEditDialog::OnNotPaidStateRequest(wxCommandEvent &event)
{
  //teditPaymentDate->Disable();
}


void IncomePaymentEditDialog::OnPaymentValueKillFocus(wxFocusEvent &event)
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


void IncomePaymentEditDialog::OnDeleteRequest(wxCommandEvent &event) {
  if (wxYES != wxMessageBox(wxT("Você confirma a exclusão das informações sobre essa parcela de pagamento?"),
                            wxT("Confirmação de exclusão de informações sobre parcela de pagamento"),
                            wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION,
                            this)) {

    statusBar->SetStatusText(wxT("A exclusão das informações sobre a parcela de pagamento foi cancelada."));

    return;
  }

  wxString sqlCommand = wxString::Format(wxT("DELETE FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" WHERE installment_id = %d"),
                                         this->paymentId);

  DBQuery dbquery;

  int rc = dbquery.Execute(sqlCommand);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível excluir as informações sobre a parcela de pagamento."),
                 wxT("Erro"),
                 wxOK,
                 this);
  }
  else {
    Close();
  }
}


BEGIN_EVENT_TABLE(IncomePaymentEditDialog, wxDialog)
  EVT_TOOL(DLG_INCOME_PAYMENT_EDIT_TOOL_SAVE_ID, IncomePaymentEditDialog::OnSaveRequest)
  EVT_TOOL(DLG_INCOME_PAYMENT_EDIT_TOOL_CLOSE_ID, IncomePaymentEditDialog::OnCloseRequest)
  EVT_TOOL(DLG_INCOME_PAYMENT_EDIT_TOOL_DELETE_ID, IncomePaymentEditDialog::OnDeleteRequest)

  EVT_CHAR_HOOK(IncomePaymentEditDialog::OnCharHook)

  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_LIVECASH_ID, IncomePaymentEditDialog::OnPaymentLiveCashRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_DEPOSIT_ID, IncomePaymentEditDialog::OnPaymentDepositRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_TRANSFER_ID, IncomePaymentEditDialog::OnPaymentTransferRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_DEBIT_ID, IncomePaymentEditDialog::OnPaymentCardDebitRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CARD_CREDIT_ID, IncomePaymentEditDialog::OnPaymentCardCreditRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBUTTON_CHEQUE_ID, IncomePaymentEditDialog::OnPaymentChequeRequest)

  EVT_BUTTON(DLG_INCOME_PAYMENT_EDIT_BTN_PAYMENT_DATE_ID, IncomePaymentEditDialog::OnPaymentDateChooseRequest)

  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBTN_PAID_ID, IncomePaymentEditDialog::OnPayStateRequest)
  EVT_RADIOBUTTON(DLG_INCOME_PAYMENT_EDIT_RADIOBTN_NOT_PAID_ID, IncomePaymentEditDialog::OnNotPaidStateRequest)
END_EVENT_TABLE()
