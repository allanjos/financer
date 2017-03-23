/**
 * Registering dialog - Header file.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_EXPENSE_NEW_HPP_
#define _PANEL_EXPENSE_NEW_HPP_


#include "utils_include.hpp"


class PanelExpenseNew: public wxScrolledWindow
{
  private:
    unsigned long expenseId;

    bool isCanceled;

    wxTextCtrl *teditName;

    wxRadioButton *radioBtnTypeFixed;
    wxRadioButton *radioBtnTypeVariable;

    wxChoice *choiceExpenseCategory;

    wxStaticText *labelPaymentDate;

    wxSpinCtrl *spinPaymentDay;
    wxSpinCtrl *spinPaymentMonth;
    wxSpinCtrl *spinPaymentYear;

    wxChoice *choicePaymentInterval;

    wxChoice *choiceAccountOutput;

    wxRadioButton *radioBtnStatusEnabled;
    wxRadioButton *radioBtnStatusDisabled;

    wxTextCtrl *teditAmountValue;

    wxCheckBox *checkNotFixedAmount;

    wxTextCtrl *teditInstallmentValue;

    wxRadioButton *radioBtnPaid;
    wxRadioButton *radioBtnNotPaid;

    wxTextCtrl *teditNotes;


    void OnSaveRequest(wxCommandEvent &event);

    void OnPaymentMonthSelect(wxSpinEvent &event);

    void OnPaymentYearSelect(wxSpinEvent &event);

    int OnDateChange();

    void OnPaymentIntervalSelect(wxCommandEvent &event);

    void SetAmountAsNotFixed();

    void OnSetAmountAsNotFixed(wxCommandEvent &event);

    int LoadCategories();

    void OnExpenseTypeChange(wxCommandEvent &event);

    void OnAmountValueKillFocus(wxFocusEvent &event);

    void OnInstallmentValueKillFocus(wxFocusEvent &event);

    void OnCloseRequest(wxCloseEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelExpenseNew(wxWindow *parent);

    unsigned long GetExpenseId();

    bool IsCanceled();

    int LoadAccountList();
};


#endif  /* _PANEL_EXPENSE_NEW_HPP_ */