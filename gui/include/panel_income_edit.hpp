/**
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_INCOME_EDIT_HPP_
#define _PANEL_INCOME_EDIT_HPP_


#include "utils_include.hpp"


class PanelIncomeEdit : public wxScrolledWindow
{
  private:
    int incomeId;

    wxAuiNotebook *nbookProductInfo;

    wxTextCtrl *teditName;

    wxRadioButton *radioBtnTypeFixe;
    wxRadioButton *radioBtnTypeVariable;

    wxChoice *choiceIncomeCategory;

    wxChoice *choiceWorkType;

    wxStaticText *labelPaymentDate;

    wxSpinCtrl *spinPaymentDay;
    wxSpinCtrl *spinPaymentMonth;
    wxSpinCtrl *spinPaymentYear;

    wxChoice *choicePaymentInterval;

    wxChoice *choiceAccountInput;

    wxRadioButton *radioBtnStatusEnabled;
    wxRadioButton *radioBtnStatusDisabled;

    wxTextCtrl *teditAmountValue;

    wxCheckBox *checkNotFixedAmount;

    wxTextCtrl *teditInstallmentValue;

    wxRadioButton *radioBtnPaid;
    wxRadioButton *radioBtnNotPaid;

    wxTextCtrl *teditNotes;

    wxListCtrl *listCtrlInstallments;

    wxStaticText *labelAmountPaid;

    wxStaticText *labelAmountInstallmentsNotPaid;

    wxListCtrl *listCtrlReceipt;

    /** Status bar for informative messages. */

    wxStatusBar *statusBar;


    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnTaxMiscAddRequest(wxCommandEvent &event);

    void OnTaxMiscDeleteRequest(wxCommandEvent &event);

    void OnDeleteRequest(wxCommandEvent &event);

    void OnPaymentMonthSelect(wxSpinEvent &event);

    void OnPaymentYearSelect(wxSpinEvent &event);

    int OnDateChange();

    void OnPaymentIntervalSelect(wxCommandEvent &event);

    void SetAmountAsNotFixed();

    void OnSetAmountAsNotFixed(wxCommandEvent &event);

    int LoadIncomeCategories();

    int LoadWorkTypes();

    int LoadInstalmentsListing();

    void OnExpenseTypeChange(wxCommandEvent &event);

    int ValidateFields();

    void OnAmountValueKillFocus(wxFocusEvent &event);

    void OnInstallmentValueKillFocus(wxFocusEvent &event);

    void OnInstallmentAddRequest(wxCommandEvent &event);

    void OnInstallmentsListItemEditDialogRequest(wxListEvent &event);


    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelIncomeEdit(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Load data
     */
    int LoadData(int incomeId);

    void InstallmentAdd();

    int LoadAccountList();
};


#endif  /* _PANEL_INCOME_EDIT_HPP_ */