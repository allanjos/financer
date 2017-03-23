/**
 * Estate main dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_ESTATE_MAIN_HPP_
#define _PANEL_ESTATE_MAIN_HPP_


#include "utils_include.hpp"

class PanelEstateNew;
class PanelEstateEdit;


class PanelEstateMain: public wxScrolledWindow
{
  private:

    PanelEstateNew *panelEstateNew;

    PanelEstateEdit *panelEstateEdit;

    wxTextCtrl *teditSearch;

    wxListCtrl *listCtrlReport;

    wxTextCtrl *teditTotalAcquisitionCost;

    wxTextCtrl *teditTotalAccountingValue;

    wxTextCtrl *teditTotalAcumulatedDepreciation;

    void OnRefreshRequest(wxCommandEvent &event);

    void OnNewDialogRequest(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);

    /*void OnKeyChar(wxKeyEvent& event);*/

    void OnKeyUp(wxKeyEvent& event);

    void OnListItemKeyDown(wxListEvent &event);

    void OnListItemEditDialogRequest(wxListEvent &event);

    void OnSearchRequest(wxCommandEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelEstateMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();


    int LoadData();
};


#endif  /* _PANEL_ESTATE_MAIN_HPP_ */
