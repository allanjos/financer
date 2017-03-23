/**
 * Calendar dialog.
 *
 * \file  dlg_calendar.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_CALENDAR_HPP_
#define _DLG_CALENDAR_HPP_


#include "utils_include.hpp"


class CalendarDialog : public wxDialog
{
  private:
    wxCalendarCtrl *calendarCtrl;

    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;


    /**
     * About list control.
     */
    wxListCtrl* lcAboutList;

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    wxString selectedDay;

    wxString selectedMonth;

    wxString selectedYear;

    wxDateTime dateTime;

    bool hasSelection;

    /**
     * Constructor and initializer.
     */
    CalendarDialog(wxWindow *parent, wxPoint position = wxDefaultPosition, wxDateTime dateTime = wxDefaultDateTime);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    void OnCalendar(wxCalendarEvent& event);
};


#endif  /* _DLG_CALENDAR_HPP_ */
