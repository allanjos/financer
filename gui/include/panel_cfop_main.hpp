/**
 *
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 01/06/2009
 */

#ifndef _PANEL_CFOP_MAIN_HPP_
#define _PANEL_CFOP_MAIN_HPP_


#include "utils_include.hpp"


class TreeItemDataCfop: public wxTreeItemData
{
  private:

    int cfopId;

    wxString cfopCode;

    wxString cfopDescription;

  public:

    TreeItemDataCfop(const int& cfopId, wxString cfopCode = wxT(""), wxString cfopDescription = wxT(""));

    int GetCfopId();

    wxString GetCfopCode();

    wxString GetCfopDescription();
};


class PanelCfopMain: public wxScrolledWindow
{
  private:

    wxTreeCtrl *treeCtrlListing;

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * Deletes a cliente type entry.
     */
    void OnDeleteRequest(wxListEvent &event);

    int LoadCategoryChildList(wxTreeItemId treeItemParentId);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelCfopMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadList();
};


#endif  /* _PANEL_CFOP_MAIN_HPP_ */