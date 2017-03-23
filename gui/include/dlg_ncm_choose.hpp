/**
 *
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 04/06/2009
 */

#ifndef _DLG_NCM_CHOOSE_HPP_
#define _DLG_NCM_CHOOSE_HPP_

#include "utils_include.hpp"


class NcmChooseDialog : public wxDialog
{
  private:
    int ncmId;

    wxString ncmCode;

    wxString ncmDescription;

    bool isCanceled;

    wxTreeCtrl *treeCtrlListing;
    
    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    void OnCloseRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * Deletes a cliente type entry.
     */
    void OnDeleteRequest(wxListEvent &event);

    int LoadCategoryChildList(wxTreeItemId treeItemParentId);

    void OnNcmChooseDialogRequest(wxCommandEvent &event);

    int ItemSelect();

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    NcmChooseDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadList();

    bool IsCanceled();

    int GetNcmId();

    wxString GetNcmCode();

    wxString GetNcmDescription();
};


#endif  /* _DLG_NCM_CHOOSE_HPP_ */
