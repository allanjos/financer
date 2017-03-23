/**
 * About dialog.
 *
 * \file  dlg_about.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef DLG_BANK_LIST_HPP
#define DLG_BANK_LIST_HPP


#include "utils_include.hpp"


class BankListDialog: public wxDialog
{
  private:

    wxListCtrl *listCtrlBanks;

    void OnCloseRequest(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    BankListDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadList();
};


#endif  /* DLG_BANK_LIST_HPP */