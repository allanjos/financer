/**
 * Taxes main dialog.
 *
 * \file  dlg_tax_main.hpp
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 01/03/2009
 */

#ifndef _DLG_TAX_MAIN_HPP_
#define _DLG_TAX_MAIN_HPP_


#include "utils_include.hpp"


class TaxMainDialog : public wxDialog
{
  private:
    wxListCtrl *listCtrlReport;

    wxListCtrl *listCtrlReportIcmsA;

    wxListCtrl *listCtrlReportIcmsB;
    
    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    void OnCloseRequest(wxCommandEvent &event);

    /**
     *
     */
    void OnTaxMiscNewDialogRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * Deletes a cliente type entry.
     */
    void OnDeleteRequest(wxListEvent &event);

    /**
     * Client type editing.
     */
    void OnEditDialogRequest(wxListEvent &event);

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
    TaxMainDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadList();

    int LoadICMSTables();
};


#endif  /* _DLG_CLIENT_TYPE_MAIN_HPP_ */
