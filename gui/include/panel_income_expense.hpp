/**
 * Supplier main dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_INCOME_EXPENSE_HPP_
#define _PANEL_INCOME_EXPENSE_HPP_

#include "utils_include.hpp"
#include "canvas_graph_income_expense.hpp"

class PanelExpenseNew;
class PanelExpenseEdit;
class PanelIncomeNew;
class PanelIncomeEdit;

class PanelIncomeExpense: public wxScrolledWindow
{
  private:

    PanelExpenseNew *panelExpenseNew;

    PanelExpenseEdit *panelExpenseEdit;

    PanelIncomeNew *panelIncomeNew;

    PanelIncomeEdit *panelIncomeEdit;

    bool isLoaded;

    wxSpinCtrl *spinnerYear;

    GraphIncomeExpenseCanvas *graphCanvas;

    wxTextCtrl *teditSearch;

    wxListCtrl *listCtrlIncomes;

    wxListCtrl *listCtrlExpenses;

    wxTextCtrl *teditTotalAcquisitionCost;

    wxTextCtrl *teditTotalAccountingValue;

    wxTextCtrl *teditTotalAcumulatedDepreciation;

    wxStaticText *labelAmountPaid;

    wxStaticText *labelAmountInstallmentsNotPaid;

    wxStaticText *labelExpensePaid;

    wxStaticText *labelResult;

    wxStaticText *labelResultName;

    wxStaticText *labelAmountPaidCurrentMonth;

    wxStaticText *labelAmountNotPaidCurrentMonth;

    wxStaticText *labelExpensePaidCurrentMonth;

    wxStaticText *labelResultCurrentMonth;

    wxStaticText *labelResultNameCurrentMonth;

    wxStaticText *labelAmountPaidCurrentDay;

    wxStaticText *labelAmountNotPaidCurrentDay;

    wxStaticText *labelExpensePaidCurrentDay;

    wxStaticText *labelResultCurrentDay;

    wxStaticText *labelResultNameCurrentDay;

    wxScrolledWindow *panelGraph;

    int LoadGeneralInfo();

    int LoadCurrentMonthInfo();

    int LoadCurrentDayInfo();

    void OnSpinnerGraphYearEvent(wxSpinEvent& event);

    void OnRefreshRequest(wxCommandEvent& event);

    void OnIncomesMainDialogRequest(wxCommandEvent& event);

    void OnIncomeNewDialogRequest(wxCommandEvent& event);

    void OnExpensesMainDialogRequest(wxCommandEvent& event);

    void OnExpenseNewDialogRequest(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);

    /*void OnKeyChar(wxKeyEvent& event);*/

    void OnIncomesListItemEditDialogRequest(wxListEvent& event);

    void OnExpensesListItemEditDialogRequest(wxListEvent& event);

    void OnDropDownToolbarIncome(wxAuiToolBarEvent& event);

    void OnDropDownToolbarExpense(wxAuiToolBarEvent& event);

    void OnExpenseKeyDown(wxListEvent& event);

    void OnSizeEvent(wxSizeEvent& event);

    void OnScrollEvent(wxScrollEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelIncomeExpense(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadData();

    int LoadIncomesListing();

    int LoadExpensesListing();
};

#endif  /* _PANEL_INCOME_EXPENSE_HPP_ */