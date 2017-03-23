/**
 * Help dialog.
 *
 * \file  dlg_help.hpp
 * \version 1.1
 * \since 2007-2013
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_GUIDE_HPP_
#define _PANEL_GUIDE_HPP_


#include "utils_include.hpp"
#include "wx/html/htmlwin.h"
#include "wx/html/helpctrl.h"


class PanelGuide: public wxScrolledWindow
{
  private:
    wxHtmlHelpController advancedHtmlHelp;

    wxHtmlHelpController embeddedHtmlHelp;

    wxHtmlHelpWindow* embeddedHelpWindow;

    void OnResize(wxSizeEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelGuide(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     *
     */
    wxHtmlHelpController& GetAdvancedHtmlHelpController() { return advancedHtmlHelp; } 
};


#endif  /* _PANEL_GUIDE_HPP_ */
