/**
 * About dialog.
 *
 * \file  dlg_log_view.hpp
 * \version 1.0
 * \since 02/22/2012
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_LOG_VIEW_ENTRY_HPP_
#define _DLG_LOG_VIEW_ENTRY_HPP_


#include "utils_include.hpp"


/**
 * Log Viewer Dialog.
 */

class LogEntryViewDialog : public wxDialog
{
  private:

    wxStaticText *labelTypeName;

    wxTextCtrl *teditMessage;

    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    void OnCloseRequest(wxCommandEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:

    /**
     * Constructor and initializer.
     */
    LogEntryViewDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadData(int id);
};


#endif  /* _DLG_LOG_VIEW_ENTRY_HPP_ */