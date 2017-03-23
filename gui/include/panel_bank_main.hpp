/**
 * Supplier main dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_BANK_MAIN_HPP_
#define _PANEL_BANK_MAIN_HPP_

#include "utils_include.hpp"

class PanelBankMain: public wxScrolledWindow
{
  private:

    bool isLoaded;

    wxListCtrl *listCtrlAccounts;

    wxListCtrl *listCtrlMovement;

    void OnNewDialogRequest(wxCommandEvent& event);

    void OnRefreshRequest(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);

    void OnBankAccountListItemEditDialogRequest(wxListEvent& event);

    void OnBankListDialogRequest(wxCommandEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelBankMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadData();

    int LoadAccounts();

    int LoadMovements();
};

#endif  /* _PANEL_BANK_MAIN_HPP_ */