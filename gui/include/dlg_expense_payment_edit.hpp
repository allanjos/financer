/**
 * Payment information dialog
 *
 * \version 1.0
 * \since 07/01/2010
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_EXPENSE_PAYMENT_EDIT_HPP_
#define _DLG_EXPENSE_PAYMENT_EDIT_HPP_


#include "utils_include.hpp"


class ExpensePaymentEditDialog : public wxDialog
{
  private:

    long paymentId;

    wxChoice *choiceExpense;

    wxTextCtrl *teditValue;

    wxChoice *cboxValueCurrency;

    wxTextCtrl *teditPaymentDate;

    wxBitmapButton *btnCalendarPaymentDate;

    wxSpinCtrl *spinCtrlTimeHour;

    wxSpinCtrl *spinCtrlTimeMinutes;

    wxToggleButton *toggleBtnPaid;

    wxTextCtrl *teditDescription;

    wxChoice *cboxNotaFiscal;

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

    wxChoice *choiceAccountOutput;

    /** Status bar for informative messages. */

    wxStatusBar *statusBar;

    void OnPayStateRequest(wxCommandEvent &event);

    void OnNotPaidStateRequest(wxCommandEvent &event);

    void OnSaveRequest(wxCommandEvent &event);

    void OnCloseRequest(wxCommandEvent &event);

    void OnPaymentDateChooseRequest(wxCommandEvent &event);

    void Terminate();

    void OnCharHook(wxKeyEvent& event);

    void OnPaymentLiveCashRequest(wxCommandEvent &event);

    void OnPaymentDepositRequest(wxCommandEvent &event);

    void OnPaymentTransferRequest(wxCommandEvent &event);

    void OnPaymentCardDebitRequest(wxCommandEvent &event);

    void OnPaymentCardCreditRequest(wxCommandEvent &event);

    void OnPaymentChequeRequest(wxCommandEvent &event);

    void PaymentLiveCashSelect();

    void PaymentDepositSelect();

    void PaymentTransferSelect();

    void PaymentCardDebitSelect();

    void PaymentCardCreditSelect();

    void PaymentChequeSelect();

    void OnPaymentValueKillFocus(wxFocusEvent &event);

    void OnDeleteRequest(wxCommandEvent &event);

    void OnPaidChangeRequest(wxCommandEvent& event);

    int LoadAccountList();

    int LoadExpenseList();

    int LoadReceiptsList();

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    ExpensePaymentEditDialog(wxWindow *parent);

    bool SetPaymentId(long paymentId);

    int SaveData();
};


#endif  /* _DLG_EXPENSE_PAYMENT_EDIT_HPP_ */
