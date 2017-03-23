/**
 * People main dialog.
 *
 * \file  dlg_client_main.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_PERSON_MAIN_HPP_
#define _PANEL_PERSON_MAIN_HPP_


#include "utils_include.hpp"


class PanelPersonEdit;


class PanelPersonMain: public wxScrolledWindow
{
  private:

    PanelPersonEdit *panelPersonEdit;

    wxTextCtrl *teditSearch;

    wxListCtrl* listCtrlReport;

    void OnPersonNewDialogRequest(wxCommandEvent& event);

    void OnRefreshRequest(wxCommandEvent& event);

    //void OnPersonTypeMainDialogRequest(wxCommandEvent& event);

    void OnKeyChar(wxKeyEvent& event);

    void OnKeyUp(wxKeyEvent& event);

    void OnPersonDataViewItemActivated(wxListEvent& event);

    /**
     * Key hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnListItemKeyDown(wxListEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelPersonMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Loads client list.
     */
    int LoadData();
};


#endif  /* _PANEL_PERSON_MAIN_HPP_ */