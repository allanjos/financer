/**
 * New bank account.
 *
 * \file  panel_person_new.hpp
 * \version 1.0
 * \since 05/25/2013
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_BANK_ACCOUNT_EDIT_HPP_
#define _PANEL_BANK_ACCOUNT_EDIT_HPP_


#include "utils_include.hpp"


class PanelBankAccountEdit: public wxScrolledWindow
{
  private:

    int accountId;

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

    wxTextCtrl *teditNotes;

    wxListCtrl *listCtrlMovement;

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent& event);

    void OnDeleteRequest(wxCommandEvent& event);

    void OnCloseRequest(wxCommandEvent& event);

    void OnUpdateBalanceRequest(wxCommandEvent& event);

    /**
     * Key hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnCreationDateChooseRequest(wxCommandEvent& event);

    /**
     * Loads banks listing.
     */
    int LoadBankList();

    /**
     * Loads accounting accounts.
     */
    int LoadAccountList();

    int LoadMovements();

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelBankAccountEdit(wxWindow *parent);

    int LoadData(int id);
};


#endif  /* _PANEL_BANK_ACCOUNT_EDIT_HPP_ */