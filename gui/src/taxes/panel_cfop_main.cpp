/**
 *
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 01/06/2009
 */


#include "utils_include.hpp"
#include "gui_controls.hpp"
#include "dlg_main.hpp"
#include "panel_cfop_main.hpp"


PanelCfopMain::PanelCfopMain(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  SetAutoLayout(true);

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);

  SetScrollbars(1, 1, 20, 20);

  treeCtrlListing = new wxTreeCtrl(this,
                                   DLG_PRODUCT_CATEGORY_MAIN_TREECTRL_LISTING_ID,
                                   wxDefaultPosition,
                                   wxSize(100, 100),
                                   wxTR_DEFAULT_STYLE | wxTR_HAS_BUTTONS | wxTR_SINGLE);

  szTop->Add(treeCtrlListing, 1, wxALL|wxEXPAND|wxGROW, 5);

  // Load taxes listing

  LoadList();
}


void PanelCfopMain::Terminate()
{
  Close();

  Destroy();
}


int PanelCfopMain::LoadList()
{
  treeCtrlListing->DeleteAllItems();

  wxTreeItemId treeItemRootId = treeCtrlListing->AddRoot(wxT("Categorias"));

  treeCtrlListing->ExpandAll();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT cfop_id, codigo, descricao FROM comercio.cfop WHERE cfop_parent_id IS NULL ORDER BY codigo"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de entradas CFOP cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      // Type ID

      // Type name

      TreeItemDataCfop *treeItemDataCategory = new TreeItemDataCfop(dbquery.GetFieldIntByName(categoryIndex, wxT("cfop_id")));

      wxTreeItemId treeItemId = treeCtrlListing->AppendItem(treeItemRootId,
                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      treeCtrlListing->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadCategoryChildList(treeItemId);
    }
  }

  treeCtrlListing->ExpandAll();

  treeCtrlListing->SelectItem(treeCtrlListing->GetRootItem());

  return 0;
}


int PanelCfopMain::LoadCategoryChildList(wxTreeItemId treeItemParentId)
{
  if (!treeItemParentId.IsOk() || treeCtrlListing->GetItemData(treeItemParentId) == NULL) {
    return -1;
  }

  int cfopId = ((TreeItemDataCfop *) treeCtrlListing->GetItemData(treeItemParentId))->GetCfopId();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxString::Format(wxT("SELECT cfop_id, codigo, descricao FROM comercio.cfop WHERE cfop_parent_id = %d ORDER BY codigo"), cfopId));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de entradas CFOP cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      TreeItemDataCfop *treeItemDataCategory = new TreeItemDataCfop(dbquery.GetFieldIntByName(categoryIndex, wxT("cfop_id")));

      wxTreeItemId treeItemId = treeCtrlListing->AppendItem(treeItemParentId,
                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      treeCtrlListing->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadCategoryChildList(treeItemId);
    }
  }


  return 0;
}


void PanelCfopMain::OnCharHook(wxKeyEvent& event)
{
  event.Skip();
}


TreeItemDataCfop::TreeItemDataCfop(const int& cfopId, wxString cfopCode, wxString cfopDescription)
{
  this->cfopId = cfopId;

  this->cfopCode = cfopCode;

  this->cfopDescription = cfopDescription;
}


int TreeItemDataCfop::GetCfopId() {
  return this->cfopId;
}


wxString TreeItemDataCfop::GetCfopCode(void) {
  return this->cfopCode;
}


wxString TreeItemDataCfop::GetCfopDescription(void) {
  return this->cfopDescription;
}


BEGIN_EVENT_TABLE(PanelCfopMain, wxScrolledWindow)
  EVT_CHAR_HOOK(PanelCfopMain::OnCharHook)
END_EVENT_TABLE()
