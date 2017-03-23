/**
 * Payment information dialog
 *
 * \version 1.0
 * \since 13/05/2009
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_EXPENSE_PAYMENT_ADD_HPP_
#define _DLG_EXPENSE_PAYMENT_ADD_HPP_


#include "utils_include.hpp"


class ExpensePaymentAddDialog : public wxDialog
{
  private:
    long expenseId;

    //wxStaticText *textExpenseName;
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

    // Transfer

    wxTextCtrl *teditTransferBankName;

    wxTextCtrl *teditTransferAgency;

    wxTextCtrl *teditTransferAccount;

    wxRadioButton *radioBtnTransferAccountCurrent;

    wxRadioButton *radioBtnTransferAccountSavings;

    wxTextCtrl *teditTransferDepositor;

    wxBitmapButton *btnCalendarTransferDate;

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

    void OnPaidChangeRequest(wxCommandEvent& event);

    void OnExpenseChangeRequest(wxCommandEvent& event);

    void OnPayStateRequest(wxCommandEvent& event);

    void OnNotPaidStateRequest(wxCommandEvent& event);

    void OnSaveRequest(wxCommandEvent& event);

    void OnCloseRequest(wxCommandEvent& event);

    void OnPaymentDateChooseRequest(wxCommandEvent& event);

    void Terminate();

    void OnCharHook(wxKeyEvent& event);

    int LoadExpenseList();

    int LoadExpenseInfo();

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

    void OnPaymentValueKillFocus(wxFocusEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    ExpensePaymentAddDialog(wxWindow *parent);

    bool SetExpenseId(long id);

    int SaveData();
};


#endif  /* _DLG_EXPENSE_PAYMENT_ADD_HPP_ */