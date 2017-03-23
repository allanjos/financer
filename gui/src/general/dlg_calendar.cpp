/**
 * Calendar dialog.
 *
 * \file dlg_calendar.cpp
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"


CalendarDialog::CalendarDialog(wxWindow *parent, wxPoint position, wxDateTime dateTime):
  wxDialog(parent, wxID_ANY, wxString(Lang::Get("Calendar")), position)
{
  hasSelection = false;

  this->dateTime = dateTime;

  SetSize(550, 300);


  this->SetBackgroundColour(*wxWHITE);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);


  calendarCtrl = new wxCalendarCtrl(this,
                                    DLG_CALENDAR_CALENDAR_CTRL_ID,
                                    dateTime,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxCAL_MONDAY_FIRST |
                                    wxCAL_SHOW_HOLIDAYS |
                                    wxRAISED_BORDER);

  szTop->Add(calendarCtrl, 0, wxALIGN_CENTER|wxALL, 5);


  szTop->SetSizeHints(this);
}


void CalendarDialog::Terminate()
{
  Close();

  statusBar->Destroy();

  Destroy();
}


void CalendarDialog::OnCharHook(wxKeyEvent& event)
{
  // Exit <ESC>
  switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
      Close();
      break;
  }

  event.Skip();
}


void CalendarDialog::OnCalendar(wxCalendarEvent& event)
{
  wxDateTime dateTime = event.GetDate();

  this->selectedDay = dateTime.Format(wxT("%d"));
  this->selectedMonth = dateTime.Format(wxT("%m"));
  this->selectedYear = dateTime.Format(wxT("%Y"));

  this->hasSelection = true;

  Close();

  //wxLogMessage(wxT("Selected %s from calendar"), dateTime.Format(wxT("%d/%m/%Y")).c_str());
}


BEGIN_EVENT_TABLE(CalendarDialog, wxDialog)
  EVT_CHAR_HOOK(CalendarDialog::OnCharHook)
  EVT_CALENDAR(DLG_CALENDAR_CALENDAR_CTRL_ID, CalendarDialog::OnCalendar)
END_EVENT_TABLE()
