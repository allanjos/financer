/**
 * Account editing dialog.
 *
 * \file  dlg_account_new.hpp
 * \version 1.0
 * \since 20/12/2008
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_ACCOUNT_EDIT_HPP_
#define _PANEL_ACCOUNT_EDIT_HPP_


#include "utils_include.hpp"
#include "utils_account.hpp"



class PanelAccountEdit : public wxScrolledWindow
{
  private:

    int accountId;

    int accountParentId;

    wxTextCtrl *teditName;

    wxTextCtrl *teditAccountCode;

    wxStaticText *lblAccountParent;

    wxTreeCtrl *treeCtrlAccounts;

    wxTextCtrl *teditDescription;

    wxRadioButton *radioBtnAccountTypeActive;

    wxRadioButton *radioBtnAccountTypePassive;

    wxRadioButton *radioBtnAccountTypeResult;

    wxChoice *cboxAccountSubType;

    wxTextCtrl *teditCreationDate;

    wxBitmapButton *btnCalendarBalanceDate;

    wxTextCtrl *teditMovementSearch;

    wxTextCtrl *teditMovementDate;

    wxButton *btnCalendarMovementAdd;

    wxBitmapButton *btnCalendarMovementDate;

    wxChoice *cboxMovementType;

    wxListCtrl *listCtrlMovement;

    wxTextCtrl *teditGeneralBalance;

    wxTextCtrl *teditNotes;

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent& event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnCreationDateChooseRequest(wxCommandEvent& event);

    void OnBalanceInitialDateChooseRequest(wxCommandEvent& event);

    void OnSetAsActiveAccount(wxCommandEvent& event);

    void OnSetAsPassiveAccount(wxCommandEvent& event);

    void OnSetAsOfResultsAccount(wxCommandEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:

    /**
     * Constructor and initializer.
     */
    PanelAccountEdit(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();


    bool LoadData(int accountId);


    int LoadAccountTypes();

    int LoadMovementTypes();

    int LoadAccountList();

    int LoadAccountChildList(wxTreeItemId treeItemParentId);

    /** Select a account from a treectrl. */

    int SelectAccountFromTreeCtrl(wxTreeCtrl *treeCtrl, wxTreeItemId treeItemId, int accountId);
};


#endif  /* _PANEL_ACCOUNT_EDIT_HPP_ */