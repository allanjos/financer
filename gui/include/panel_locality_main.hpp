/**
  * \file  dlg_locality_main.hpp
 * \version 1.0
 * \since 01/29/2007 10:55:20 AM AMT
 * \revision  none
 * \author: Allann Jones
 */

#ifndef _PANEL_LOCALITY_MAIN_HPP_
#define _PANEL_LOCALITY_MAIN_HPP_


#include "utils_include.hpp"


class PanelLocalityMain: public wxScrolledWindow
{
  private:

    wxTextCtrl *teditSearch;

    wxListCtrl* listCtrlReport;

    void OnCountryNewDialogRequest(wxCommandEvent& event);

    void OnKeyChar(wxKeyEvent& event);

    void OnKeyUp(wxKeyEvent& event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelLocalityMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();
};


#endif  /* _PANEL_LOCALITY_MAIN_HPP_ */
