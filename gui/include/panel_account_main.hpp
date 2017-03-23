/**
 * \brief Accounts main dialog.
 *
 * \file  dlg_account_main.hpp
 * \version 1.0
 * \since 14/11/2008
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_ACCOUNT_MAIN_HPP_
#define _PANEL_ACCOUNT_MAIN_HPP_


#include "utils_include.hpp"
#include "model_account_data.hpp"
#include "renderer_data_account.hpp"
#include "panel_account_edit.hpp"
#include "panel_account_new.hpp"


class PanelAccountEdit;


class PanelAccountMain : public wxScrolledWindow
{
  private:
  
    PanelAccountNew *panelAccountNew;

    PanelAccountEdit *panelAccountEdit;

    wxTextCtrl *teditSearch;

    wxDataViewCtrl *dataViewAccounts;

    wxObjectDataPtr<AccountDataModel> dataViewModelAccounts;

    void OnRefreshRequest(wxCommandEvent &event);

    void OnCharHook(wxKeyEvent& event);

    void OnNewDialogRequest(wxCommandEvent& event);

    void OnKeyChar(wxKeyEvent& event);

    void OnKeyUp(wxKeyEvent& event);

    void OnAccountDataViewItemActivated(wxDataViewEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelAccountMain(wxWindow *parent);


    ~PanelAccountMain();


    /**
     * Closes and terminates the dialog.
     */
    void Terminate();


    void ExpandAccountDataViewCtrl(AccountDataModelNode *dataViewItem);


    int LoadList();
};


#endif  /* _PANEL_ACCOUNT_MAIN_HPP_ */