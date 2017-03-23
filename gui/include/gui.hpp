/**
 * Functions for graphical user interface.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


/** \defgroup GUI Graphical User Interface */
/**@{*/

#ifndef _GUI_HPP_
#define _GUI_HPP_


#include "gui_lib.hpp"


/**
 * Alignment for a GUI label in a form.
 */
#define GUI_FORM_LABEL_ALIGNMENT wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL

/**
 * Alignment for a generic GUI control in a form.
 */
#define GUI_FORM_CONTROL_ALIGNMENT wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL


/**
 * Grid checkbox item - Enable and disable values.
 */
//#ifdef WIN32
  #define GRID_CHECKBOX_VALUE_TRUE  wxT("1")
  #define GRID_CHECKBOX_VALUE_FALSE wxEmptyString
/*
#else
  #define GRID_CHECKBOX_VALUE_TRUE  wxT("0")
  #define GRID_CHECKBOX_VALUE_FALSE wxEmptyString
#endif
*/


#define GUI_COLOR_LIST_ITEM_ODD wxColour(0xf8, 0xf8, 0xf8)

#define GUI_COLOR_LIST_ITEM_EVEN wxColour(0xff, 0xff, 0xff)

#define GUI_COLOR_LIST_ITEM_WARNING wxColour(0xff, 0xd5, 0xd5)

#define GUI_COLOR_LIST_ITEM_DISABLED wxColour(0xdd, 0xdd, 0xdd)

#define GUI_COLOR_FONT_DISABLED wxColour(0x99, 0x99, 0x99)

#define GUI_COLOR_LABEL_MONETARY wxColor(0x17, 0x68, 0x28)

#define GUI_COLOR_LABEL wxColor(0x55, 0x55, 0x55)

#define GUI_COLOR_LABEL_INCOME *wxBLUE

#define GUI_COLOR_LABEL_EXPENSE *wxRED


#define GUI_AUI_NOTEBOOK_STYLE wxAUI_NB_TOP & ~(wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_CLOSE_ON_ALL_TABS)

#define GUI_CONTROL_MARGIN_SIZE 5


class GUI {
  public:

    static const int FORM_MARGIN;

    static const int FORM_INDENT_WIDTH;

    static const int FIELDS_SPACING_HORIZONTAL;
    static const int FIELDS_SPACING_VERTICAL;

    static const int FIELDS_SPACING_SMALLER_HORIZONTAL;
    static const int FIELDS_SPACING_SMALLER_VERTICAL;

    static const int WINDOW_MARGIN_SIZE;

    static const int LABEL_MARGIN;

    static const wxColor COLOR_LABEL_INFORMATION;

    static int ListSetRowBgColor(wxGrid *grid, int row);

    static int ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex);

    static int ListSetRowBgColor(wxGrid *grid, int row, wxColour color);

    static int ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, wxColour color);

    static int ListSetRowBgColor(wxGrid *grid, int row, int state);

    static int ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, int state);

    static wxString ListGetItemText(wxListCtrl *listCtrl, int line, int col);

    static int DrawTitle(wxDialog *windowParent, const wxString &title, const wxString &subTitle, wxBitmap bmpTitle = wxNullBitmap);

    //static int DrawTitle(wxDialog *windowParent, const wxChar title[], const wxChar subTitle[], wxBitmap bmpTitle = wxNullBitmap);

    static wxSize GetDefaultWindowSize(wxWindow *window);

    static void FormatSectionLabel(wxStaticText *label);

    static void FormatSubSectionLabel(wxStaticText *label);
};


class ContainerIntData: public wxClientData
{
  private:
    int value;

  public:
    ContainerIntData(int value);

    int GetData();
};


#endif /* _GUI_HPP_ */

/**@}*/