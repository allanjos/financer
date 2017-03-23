/**
 *
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_EXPENSE_EDIT_HPP_
#define _PANEL_EXPENSE_EDIT_HPP_


#include "utils_include.hpp"


class PanelExpenseEdit: public wxScrolledWindow
{
  private:
    int expenseId;

    wxNotebook *notebook;

    wxAuiNotebook *nbookProductInfo;

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

    wxListCtrl *listCtrlInstallments;

    wxStaticText *labelAmountPaid;

    wxStaticText *labelAmountInstallmentsNotPaid;


    void OnSaveRequest(wxCommandEvent& event);

    void OnDeleteRequest(wxCommandEvent& event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnTaxMiscAddRequest(wxCommandEvent &event);

    void OnTaxMiscDeleteRequest(wxCommandEvent &event);

    void OnPaymentMonthSelect(wxSpinEvent &event);

    void OnPaymentYearSelect(wxSpinEvent &event);

    int OnDateChange();

    void PaymentIntervalSelect();

    void OnPaymentIntervalSelect(wxCommandEvent &event);

    void SetAmountAsNotFixed();

    void OnSetAmountAsNotFixed(wxCommandEvent &event);

    int LoadCategories();

    int LoadInstalmentsListing();

    void OnExpenseTypeChange(wxCommandEvent &event);

    int ValidateFields();

    void OnAmountValueKillFocus(wxFocusEvent &event);

    void OnInstallmentValueKillFocus(wxFocusEvent &event);

    void OnInstalmentAddRequest(wxCommandEvent &event);

    void OnInstallmentsListItemEditDialogRequest(wxListEvent &event);


    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelExpenseEdit(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Load data
     */
    int LoadData(int expenseId);

    int LoadAccountList();

    void InstallmentAdd();
};


#endif  /* _PANEL_EXPENSE_EDIT_HPP_ */