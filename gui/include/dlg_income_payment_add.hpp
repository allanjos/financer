/**
 * Payment information dialog
 *
 * \version 1.0
 * \since 13/05/2009
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_INCOME_PAYMENT_ADD_HPP_
#define _DLG_INCOME_PAYMENT_ADD_HPP_


#include "utils_include.hpp"
#include "utils_product.hpp"
#include "utils_tax.hpp"


class IncomePaymentAddDialog : public wxDialog
{
  private:
    long incomeId;

    wxTextCtrl *teditIncomeName;

    wxTextCtrl *teditValue;

    wxChoice *cboxValueCurrency;

    wxTextCtrl *teditPaymentDate;

    wxBitmapButton *btnCalendarPaymentDate;

    //wxCheckBox *checkboxPagamentoQuitado;

    wxRadioButton *radioBtnPaid;
    wxRadioButton *radioBtnNotPaid;

    wxTextCtrl *teditDescription;

    wxChoice *cboxNotaFiscal;

    // Payment ways

    wxRadioButton *radioBtnPaymentLiveCash;

    wxRadioButton *radioBtnPaymentTransfer;

    wxRadioButton *radioBtnPaymentDeposit;

    wxRadioButton *radioBtnPaymentCardDebit;

    wxRadioButton *radioBtnPaymentCardCredit;

    wxRadioButton *radioBtnPaymentTicket;

    wxRadioButton *radioBtnPaymentCheque;

    wxChoice *cboxDebitCardMark;

    // Deposit

    wxTextCtrl *teditDepositBankName;

    wxTextCtrl *teditDepositAgency;

    wxTextCtrl *teditDepositAccount;

    wxRadioButton *radioBtnDepositAccountCurrent;

    wxRadioButton *radioBtnDepositAccountSavings;

    wxTextCtrl *teditDepositDepositor;

    wxBitmapButton *btnCalendarDepositDate;

    wxTextCtrl *teditDepositTimeHour;

    wxTextCtrl *teditDepositTimeMinutes;

    // Transfer

    wxTextCtrl *teditTransferBankName;

    wxTextCtrl *teditTransferAgency;

    wxTextCtrl *teditTransferAccount;

    wxRadioButton *radioBtnTransferAccountCurrent;

    wxRadioButton *radioBtnTransferAccountSavings;

    wxTextCtrl *teditTransferDepositor;

    wxBitmapButton *btnCalendarTransferDate;

    wxTextCtrl *teditTransferTimeHour;

    wxTextCtrl *teditTransferTimeMinutes;

    // Debit card

    wxTextCtrl *teditDebitCardNumber;

    wxTextCtrl *teditDebitCardValidityDate;

    wxBitmapButton *btnCalendarDebitCardValidityDate;

    wxTextCtrl *teditDebitCardOwnerName;

    wxChoice *cboxCreditCardMark;

    wxTextCtrl *teditCreditCardNumber;

    wxTextCtrl *teditCreditCardValidityDate;

    wxBitmapButton *btnCalendarCreditCardValidityDate;

    wxTextCtrl *teditCreditCardOwnerName;

    wxTextCtrl *teditChequeNumber;

    wxTextCtrl *teditChequeOwnerName;

    wxBoxSizer *szBoxPaymentInfo;

    wxPanel *panelPaymentDeposit;

    wxPanel *panelPaymentCardDebit;

    wxPanel *panelPaymentTransfer;

    wxPanel *panelPaymentCardCredit;

    wxPanel *panelPaymentCheque;

    wxTextCtrl *teditChequeWithdrawalDate;

    wxBitmapButton *btnCalendarChequeWithdrawalDate;

    wxChoice *choiceAccountInput;

    /** Status bar for informative messages. */

    wxStatusBar *statusBar;

    void OnPayStateRequest(wxCommandEvent& event);

    void OnNotPaidStateRequest(wxCommandEvent& event);

    void OnSaveRequest(wxCommandEvent& event);

    void OnCloseRequest(wxCommandEvent& event);

    void OnPaymentDateChooseRequest(wxCommandEvent& event);

    void Terminate();

    void OnCharHook(wxKeyEvent& event);

    int LoadIncomeInfo();

    int LoadAccountList();

    int LoadReceiptsList();

    void OnPaymentLiveCashRequest(wxCommandEvent& event);

    void OnPaymentDepositRequest(wxCommandEvent& event);

    void OnPaymentTransferRequest(wxCommandEvent& event);

    void OnPaymentCardDebitRequest(wxCommandEvent& event);

    void OnPaymentCardCreditRequest(wxCommandEvent& event);

    void OnPaymentChequeRequest(wxCommandEvent& event);

    void PaymentLiveCashSelect();

    void PaymentDepositSelect();

    void PaymentTransferSelect();

    void PaymentCardDebitSelect();

    void PaymentCardCreditSelect();

    void PaymentChequeSelect();

    void OnPaymentValueKillFocus(wxFocusEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    IncomePaymentAddDialog(wxWindow *parent);

    bool SetIncomeId(long incomeId);

    int SaveData();
};


#endif  /* _DLG_INCOME_PAYMENT_ADD_HPP_ */
