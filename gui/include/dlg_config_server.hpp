/**
 *
 * \file  dlg_config.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author Allann Jones <alljols@yahoo.com>
 */

#ifndef _DLG_CONFIG_SERVER_HPP_
#define _DLG_CONFIG_SERVER_HPP_


#include "utils_include.hpp"


class ConfigServerDialog: public wxDialog
{
  private:
    wxChoice *cboxCurrentCurrency;

    wxChoice *cboxDateFormat;

    wxChoice *cboxNumberFormat;

    // Expenses

    /** Conta padrão para pagamento de despesas. */

    wxChoice *cboxAccountExpensePayment;

    /** Status bar for informative messages. */

    wxStatusBar *statusBar;

    void OnShow(wxShowEvent& event);

    /** Event table declaration - wxWidgets */

    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    ConfigServerDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Loads configuration data.
     */
    void LoadData();

    int LoadAccountList();

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Close interface request.
     */
    void OnCloseRequest(wxCommandEvent &event);

    void OnCharHook(wxKeyEvent& event);

    /**
     * Database reconnect request.
     */
    void OnDatabaseReconnectRequest(wxCommandEvent &event);
};


#endif  /* _DLG_CONFIG_SERVER_HPP_ */