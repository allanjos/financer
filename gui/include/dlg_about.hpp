/**
 * About dialog.
 *
 * \file  dlg_about.hpp
 * \version 1.0
 * \since 01/29/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_ABOUT_HPP_
#define _DLG_ABOUT_HPP_


#include "utils_include.hpp"


class AboutDialog: public wxDialog
{
  private:

    void OnEraseBackground(wxEraseEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    AboutDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    void OnRightClick(wxMouseEvent& event);
};


#endif  /* _DLG_ABOUT_HPP_ */