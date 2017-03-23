/**
 *
 * \file  dlg_config.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author Allann Jones <alljols@yahoo.com>
 */

#ifndef _DLG_CONFIG_HPP_
#define _DLG_CONFIG_HPP_


#include "utils_include.hpp"


class BankAccountBalanceUpdate : public wxDialog
{
  private:

    int accountId;

    wxStaticText *labelOwnerName;

    wxStaticText *labelAccountType;

    wxStaticText *labelBankName;

    wxStaticText *labelAccountNumber;

    wxStaticText *labelAccountAgency;

    wxTextCtrl *teditNewBalance;

    wxTextCtrl *teditBalanceDate;

    wxBitmapButton *btnCalendarBalanceDate;

    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent& event);

    void OnBalanceDateChooseRequest(wxCommandEvent& event);

    /**
     * Close interface request.
     */
    void OnCloseRequest(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);

    void OnBalanceValueKillFocus(wxFocusEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    BankAccountBalanceUpdate(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Loads configuration data.
     */
    int LoadData(int id);
};


#endif  /* _DLG_CONFIG_HPP_ */
