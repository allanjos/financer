/**
 * About dialog.
 *
 * \file  dlg_log_view.hpp
 * \version 1.0
 * \since 02/22/2012
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_LOG_VIEW_HPP_
#define _DLG_LOG_VIEW_HPP_


#include "utils_include.hpp"


/**
 * Log Viewer Dialog.
 */

class LogViewDialog : public wxDialog
{
  private:

    wxToolBar *toolBar;

    wxListCtrl *listCtrlLog;

    wxChoice *choiceEventType;

    wxTextCtrl *teditSearch;

    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    void OnCloseRequest(wxCommandEvent& event);

    void OnListViewItemActivated(wxListEvent& event);

    void OnSearchEnter(wxCommandEvent& event);

    void OnEventTypeChoice(wxCommandEvent& event);

    void OnClearLogRequest(wxCommandEvent& event);

    void OnRefreshRequest(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);

    void OnShow(wxShowEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:

    /**
     * Constructor and initializer.
     */
    LogViewDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadData();
};


#endif  /* _DLG_ABOUT_HPP_ */