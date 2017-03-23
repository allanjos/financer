/**
 * Product category choose dialog.
 *
 * \version 1.0
 * \since 21/06/2009
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "gui_controls.hpp"
#include "dlg_product_category_choose.hpp"
#include "dlg_main.hpp"
#include "pixmaps/select_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


ProductCategoryChooseDialog::ProductCategoryChooseDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxT("Seleção de categoria de produto"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX)
{
  this->categoryId = -1;
  this->categoryName = wxT("");
  this->categoryCompoundName = wxT("");
  this->isCanceled = false;

  SetAutoLayout(true);

  SetSize(wxSize(780, 550));


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);


  // Title

  /*GUI::DrawTitle(this,
                 wxT("Seleção de categoria de produto"),
                 wxT("Seleção de categoria de produto."),
                 wxBitmap(product_32x32_xpm));*/

  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_PRODUCT_CATEGORY_CHOOSE_BTN_SELECT_ID, wxT(" Selecionar"), wxBitmap(select_16x16_xpm), wxT("Confirmar seleção de categoria de produto."));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_PRODUCT_CATEGORY_CHOOSE_TOOL_CLOSE_ID, wxT(" Fechar"), wxBitmap(close_16x16_xpm), wxT("Fechar"));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szTop->Add(new wxStaticLine(this), 0, wxALL|wxEXPAND|wxGROW, 0);


  treeCtrlCategories = new wxTreeCtrl(this,
                                      DLG_PRODUCT_CATEGORY_MAIN_TREECTRL_LISTING_ID,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTR_DEFAULT_STYLE | wxTR_HAS_BUTTONS | wxTR_SINGLE);

  szTop->Add(treeCtrlCategories, 1, wxALL | wxEXPAND | wxGROW | wxALIGN_RIGHT, 5);

  //wxTreeItemId treeItemRootId = treeCtrlCategories->AddRoot(wxT("Categorias"));


  this->CenterOnParent();


  LoadList();
}


void ProductCategoryChooseDialog::Terminate()
{
  Close();

  Destroy();
}


int ProductCategoryChooseDialog::LoadList()
{
  treeCtrlCategories->DeleteAllItems();

  wxTreeItemId treeItemRootId = treeCtrlCategories->AddRoot(wxT("Categorias"));

  treeCtrlCategories->ExpandAll();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT categoria_id, nome FROM produto.categoria WHERE categoria_parent_id IS NULL ORDER BY nome"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de categorias de produtos cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      // Type ID

      // Type name

      TreeItemDataCategory *treeItemDataCategory = new TreeItemDataCategory(dbquery.GetFieldIntByName(categoryIndex, wxT("categoria_id")));

      wxTreeItemId treeItemId = treeCtrlCategories->AppendItem(treeItemRootId,
                                                               dbquery.GetFieldStrByName(categoryIndex, wxT("nome")));

      treeCtrlCategories->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadCategoryChildList(treeItemId);
    }
  }

  treeCtrlCategories->ExpandAll();

  treeCtrlCategories->SelectItem(treeCtrlCategories->GetRootItem());

  return 0;
}


int ProductCategoryChooseDialog::LoadCategoryChildList(wxTreeItemId treeItemParentId)
{
  if (!treeItemParentId.IsOk() || treeCtrlCategories->GetItemData(treeItemParentId) == NULL) {
    return -1;
  }

  int categoryId = ((TreeItemDataCategory *) treeCtrlCategories->GetItemData(treeItemParentId))->GetCategoryId();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxString::Format(wxT("SELECT categoria_id, nome FROM produto.categoria WHERE categoria_parent_id = %d ORDER BY nome"), categoryId));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de categorias de produtos cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      TreeItemDataCategory *treeItemDataCategory = new TreeItemDataCategory(dbquery.GetFieldIntByName(categoryIndex, wxT("categoria_id")),
                                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("nome")));

      wxTreeItemId treeItemId = treeCtrlCategories->AppendItem(treeItemParentId,
                                                               dbquery.GetFieldStrByName(categoryIndex, wxT("nome")));

      treeCtrlCategories->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadCategoryChildList(treeItemId);
    }
  }


  return 0;
}


void ProductCategoryChooseDialog::OnCloseRequest(wxCommandEvent &event)
{
  this->isCanceled = true;

  Terminate();
}


bool ProductCategoryChooseDialog::IsCanceled()
{
  return this->isCanceled;
}


void ProductCategoryChooseDialog::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == 341) {
    //ProductTypeNewDialog productTypeNewDialog(this);

    //productTypeNewDialog.ShowModal();

    //LoadList();
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


int ProductCategoryChooseDialog::CategorySelect()
{
  wxTreeItemId itemId = treeCtrlCategories->GetSelection();

  if (!itemId.IsOk()) {
    return -1;
  }


  this->categoryId = ((TreeItemDataCategory *) treeCtrlCategories->GetItemData(itemId))->GetCategoryId();

  this->categoryName = ((TreeItemDataCategory *) treeCtrlCategories->GetItemData(itemId))->GetCategoryName();

  this->categoryCompoundName = wxT("");

  DBQuery dbquery;

  long categoryIdCurrent = this->categoryId;

  while (1) {
    int rc = dbquery.Execute(wxString::Format(wxT("SELECT categoria_id, categoria_parent_id, nome ")
                                              wxT("FROM produto.categoria ")
                                              wxT("WHERE categoria_id = %ld"), categoryIdCurrent));

    if (rc != 0) {
      wxMessageBox(wxString(wxT("Não foi possível obter a listagem de categorias de produtos cadastradas.\n\n")) +
                   wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());

      break;
    }
    else if (dbquery.GetRowsCount() > 0) {
      if (this->categoryCompoundName.Length() > 0) {
        this->categoryCompoundName = dbquery.GetFieldStrByName(0, wxT("nome")) + wxT(" / ") + this->categoryCompoundName;
      }
      else {
        this->categoryCompoundName = dbquery.GetFieldStrByName(0, wxT("nome"));
      }

      categoryIdCurrent = dbquery.GetFieldLongByName(0, wxT("categoria_parent_id"));

      if (dbquery.GetFieldLongByName(0, wxT("categoria_parent_id")) < 1) {
        break;
      }
    }
  }


  Close();

  return 0;
}


void ProductCategoryChooseDialog::OnSelectRequest(wxCommandEvent &event)
{
  CategorySelect();
}


void ProductCategoryChooseDialog::OnTreeItemActivatedRequest(wxTreeEvent &event)
{
  CategorySelect();
}


long ProductCategoryChooseDialog::GetCategoryId()
{
  return this->categoryId;
}


wxString ProductCategoryChooseDialog::GetCategoryName()
{
  return this->categoryName;
}


wxString ProductCategoryChooseDialog::GetCategoryCompoundName()
{
  return this->categoryCompoundName;
}



BEGIN_EVENT_TABLE(ProductCategoryChooseDialog, wxDialog)
  EVT_TOOL(DLG_PRODUCT_CATEGORY_CHOOSE_TOOL_CLOSE_ID, ProductCategoryChooseDialog::OnCloseRequest)
  EVT_TOOL(DLG_PRODUCT_CATEGORY_CHOOSE_BTN_SELECT_ID, ProductCategoryChooseDialog::OnSelectRequest)

  EVT_CHAR_HOOK(ProductCategoryChooseDialog::OnCharHook)

  EVT_TREE_ITEM_ACTIVATED(DLG_PRODUCT_CATEGORY_MAIN_TREECTRL_LISTING_ID, ProductCategoryChooseDialog::OnTreeItemActivatedRequest)
END_EVENT_TABLE()
