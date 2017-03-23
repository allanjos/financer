/**
 *
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 01/06/2009
 */

#ifndef _PANEL_NCM_MAIN_HPP_
#define _PANEL_NCM_MAIN_HPP_


#include "utils_include.hpp"


class TreeItemDataNcm: public wxTreeItemData
{
private:
  int ncmId;

  wxString ncmCode;

  wxString ncmDescription;

public:
  TreeItemDataNcm(const int& ncmId, wxString ncmCode = wxT(""), wxString ncmDescription = wxT(""));

  int GetNcmId();

  wxString GetNcmCode();

  wxString GetNcmDescription();
};


class PanelNcmMain: public wxScrolledWindow
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

    int LoadItemChildList(wxTreeItemId treeItemParentId);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelNcmMain(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadList();
};


#endif  /* _PANEL_NCM_MAIN_HPP_ */