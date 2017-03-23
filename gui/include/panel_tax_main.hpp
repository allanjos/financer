/**
 * Taxes main dialog.
 *
 * \file  dlg_tax_main.hpp
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 01/03/2009
 */

#ifndef _PANEL_TAX_MAIN_HPP_
#define _PANEL_TAX_MAIN_HPP_

#include "utils_include.hpp"

// Forward declarations

class PanelCfopMain;
class PanelNcmMain;

/**
 *
 */
class PanelTaxMain: public wxScrolledWindow
{
  private:
    PanelCfopMain *panelCfopMain;

    PanelNcmMain *panelNcmMain;

    wxListCtrl *listCtrlReportIcmsA;

    wxListCtrl *listCtrlReportIcmsB;

    void OnCfopMainDialogRequest(wxCommandEvent &event);

    void OnNcmMainDialogRequest(wxCommandEvent &event);

    void OnIpiMainDialogRequest(wxCommandEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelTaxMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadICMSTables();
};


#endif  /* _PANEL_TAX_MAIN_HPP_ */
