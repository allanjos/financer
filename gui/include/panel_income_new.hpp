/**
 * Registering dialog - Header file.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_INCOME_NEW_HPP_
#define _PANEL_INCOME_NEW_HPP_


#include "utils_include.hpp"


class PanelIncomeNew: public wxScrolledWindow
{
  private:

    unsigned long expenseId;

    bool isCanceled;

    wxTextCtrl *teditName;

    wxChoice *choiceIncomeCategory;

    wxChoice *choiceWorkType;

    wxStaticText *labelPaymentDate;

    wxSpinCtrl *spinPaymentDay;
    wxSpinCtrl *spinPaymentMonth;
    wxSpinCtrl *spinPaymentYear;

    wxChoice *choicePaymentInterval;

    wxRadioButton *radioBtnStatusEnabled;
    wxRadioButton *radioBtnStatusDisabled;

    wxChoice *choiceAccountInput;

    wxTextCtrl *teditAmountValue;

    wxCheckBox *checkNotFixedAmount;

    wxTextCtrl *teditInstallmentValue;

    wxRadioButton *radioBtnPaid;
    wxRadioButton *radioBtnNotPaid;

    wxTextCtrl *teditNotes;

    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnPaymentMonthSelect(wxSpinEvent &event);

    void OnPaymentYearSelect(wxSpinEvent &event);

    int OnDateChange();

    void OnPaymentIntervalSelect(wxCommandEvent &event);

    void SetAmountAsNotFixed();

    void OnSetAmountAsNotFixed(wxCommandEvent &event);

    int LoadIncomeCategories();

    int LoadWorkTypes();

    void OnExpenseTypeChange(wxCommandEvent &event);

    void OnAmountValueKillFocus(wxFocusEvent &event);

    void OnInstallmentValueKillFocus(wxFocusEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:

    /**
     * Constructor and initializer.
     */
    PanelIncomeNew(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    unsigned long GetExpenseId();

    bool IsCanceled();

    int LoadAccountList();
};


#endif  /* _PANEL_INCOME_NEW_HPP_ */
