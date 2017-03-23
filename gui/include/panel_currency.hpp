/**
 *
 * \file  dlg_conf_currency.hpp
 * \version 1.0
 * \since 01/29/2007 10:55:20 AM AMT
 * \revision  none
 * \author Allann Jones <alljols@yahoo.com>
 */

#ifndef _PANEL_CURRENCY_HPP_
#define _PANEL_CURRENCY_HPP_


#include "utils_include.hpp"


class PanelCurrency: public wxScrolledWindow
{
  private:

    wxListCtrl *listCtrlQuotations;

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelCurrency(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);
};


#endif  /* _PANEL_CURRENCY_HPP_ */
