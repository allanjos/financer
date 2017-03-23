/**
 * Product category choose dialog.
 *
 * \version 1.0
 * \since 21/06/2009
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _DLG_PRODUCT_CATEGORY_CHOOSE_HPP_
#define _DLG_PRODUCT_CATEGORY_CHOOSE_HPP_


#include "utils_include.hpp"


class TreeItemDataCategory: public wxTreeItemData
{
  private:

    long categoryId;

    wxString categoryName;

  public:

    TreeItemDataCategory(const int categoryId, wxString name = wxT("")) {
      this->categoryId = categoryId;
      this->categoryName = name;
    }

    int GetCategoryId(void) {
      return this->categoryId;
    }

    wxString GetCategoryName(void) {
      return this->categoryName;
    }
};


class ProductCategoryChooseDialog : public wxDialog
{
  private:

    long categoryId;

    wxString categoryName;

    wxString categoryCompoundName;

    bool isCanceled;

    /**
     * Main listing tree.
     */
    wxTreeCtrl *treeCtrlCategories;

    void OnCharHook(wxKeyEvent& event);

    void OnTreeItemActivatedRequest(wxTreeEvent &event);

    /**
     * Close interface request.
     */
    void OnCloseRequest(wxCommandEvent &event);

    void OnSelectRequest(wxCommandEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    ProductCategoryChooseDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    bool IsCanceled();

    /**
     * Load product's types listing.
     */
    int LoadList();

    int LoadCategoryChildList(wxTreeItemId treeItemParentId);

    int CategorySelect();

    long GetCategoryId();

    wxString GetCategoryName();

    wxString GetCategoryCompoundName();
};


#endif  /* _DLG_PRODUCT_CATEGORY_CHOOSE_HPP_ */