/**
 * Account registering dialog.
 *
 * \file  dlg_account_new.hpp
 * \version 1.0
 * \since 22/10/2008
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_ACCOUNT_NEW_HPP_
#define _PANEL_ACCOUNT_NEW_HPP_


#include "utils_include.hpp"
#include "utils_account.hpp"



class PanelAccountNew : public wxScrolledWindow
{
  private:
    wxTextCtrl *teditName;

    wxTextCtrl *teditAccountCode;

    wxStaticText *lblAccountParent;

    wxTreeCtrl *treeCtrlAccounts;

    wxTextCtrl *teditDescription;

    wxRadioButton *radioBtnAccountTypeActive;

    wxRadioButton *radioBtnAccountTypePassive;

    wxRadioButton *radioBtnAccountTypeResult;

    wxChoice *cboxAccountSubType;

    wxChoice *cboxBalanceInitialCurrency;

    //wxChoice *cboxAccountCurrency;

    wxTextCtrl *teditCreationDate;

    wxBitmapButton *btnCalendarBalanceDate;

    wxTextCtrl *teditNotes;

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnBalanceInitialDateChooseRequest(wxCommandEvent &event);

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
    PanelAccountNew(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();


    int LoadAccountTypes();

    int LoadAccountList();

    int LoadAccountChildList(wxTreeItemId treeItemParentId);
};


#endif  /* _PANEL_ACCOUNT_NEW_HPP_ */
