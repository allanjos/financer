/**
 * New bank account.
 *
 * \file  panel_person_new.hpp
 * \version 1.0
 * \since 05/25/2013
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_BANK_ACCOUNT_NEW_HPP_
#define _PANEL_BANK_ACCOUNT_NEW_HPP_


#include "utils_include.hpp"


class PanelBankAccountNew: public wxScrolledWindow
{
  private:

    wxFlexGridSizer *szgMainFields;

    wxStaticText *labelOwnerName;

    wxTextCtrl *teditOwnerName;

    wxChoice *choiceType;

    wxChoice *choiceBank;

    wxTextCtrl *teditNumber;

    wxTextCtrl *teditAgency;

    wxTextCtrl *teditCreationDate;

    wxBitmapButton *btnCreationDateChoose;

    wxChoice *choiceAccountingAccount;

    wxTextCtrl *teditBalance;

    wxTextCtrl *teditNotes;

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    void OnCloseRequest(wxCommandEvent& event);

    /**
     * Key hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnBalanceKillFocus(wxFocusEvent &event);

    void OnCreationDateChooseRequest(wxCommandEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelBankAccountNew(wxWindow *parent);

    /**
     * Loads banks listing.
     */
    int LoadBankList();

    /**
     * Loads accounting accounts.
     */
    int LoadAccountList();
};


#endif  /* _PANEL_BANK_ACCOUNT_NEW_HPP_ */