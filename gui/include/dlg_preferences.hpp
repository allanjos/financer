/**
 *
 * \file  dlg_config.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author Allann Jones <alljols@yahoo.com>
 */

#ifndef DLG_CONFIG_HPP
#define DLG_CONFIG_HPP


#include "utils_include.hpp"


class PreferencesDialog : public wxDialog
{
  private:

    wxBoxSizer *szDatabaseInfo;

    wxChoice *cboxDbManager;

    wxChoice *cboxLanguage;

    wxTextCtrl *teditServerIP;

    wxTextCtrl *teditServerDbPort;

    wxTextCtrl *teditServerDbName;

    wxChoice *cboxServerDbSSLMode;

    wxTextCtrl *teditServerDbLogin;

    wxTextCtrl *teditServerDbPasswd;

    wxTextCtrl *teditServerDbFilePath;

    wxTextCtrl *teditBudgetValidity;

    wxChoice *cboxBudgetValidityTimeType;

    wxTextCtrl *teditSaleComissaoPercent;

    wxXmlDocument doc;

    wxString paramValueConnectiontype;

    wxFlexGridSizer *szgDbPostgreSQLFields;

    wxFlexGridSizer *szgDbSQLiteFields;

    wxCheckBox *checkBoxSetupOnInit;

    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    /**
     * Process the XML stream loaded from the configuration file.
     */
    void ConfigFileProcess(wxXmlNode *node);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PreferencesDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    void LoadLanguages();

    /**
     * Loads configuration data.
     */
    int LoadData();

    int ModifyContent();

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Close interface request.
     */
    void OnCloseRequest(wxCommandEvent &event);

    void OnCharHook(wxKeyEvent& event);

    void OnDatabaseTypeRequest(wxCommandEvent& event);

    void UpdateDbFields();

    /**
     * Database reconnect request.
     */
    void OnDatabaseReconnectRequest(wxCommandEvent &event);
};


#endif  /* DLG_CONFIG_HPP */