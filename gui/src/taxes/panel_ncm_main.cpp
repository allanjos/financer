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
#include "app.hpp"
#include "panel_ncm_main.hpp"


PanelNcmMain::PanelNcmMain(wxWindow *parent):
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

  szTop->Add(treeCtrlListing, 1, wxALL | wxEXPAND | wxGROW, 5);

  // Load taxes listing

  LoadList();

  treeCtrlListing->Layout();
  szTop->Layout();
  Layout();
  wxGetApp().GetTopWindow()->Layout();
}


void PanelNcmMain::Terminate()
{
  Close();

  Destroy();
}


int PanelNcmMain::LoadList()
{
  treeCtrlListing->DeleteAllItems();

  wxTreeItemId treeItemRootId = treeCtrlListing->AddRoot(wxT("Categorias"));

  treeCtrlListing->ExpandAll();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT ncm_id, codigo, descricao FROM comercio.ncm WHERE ncm_parent_id IS NULL ORDER BY codigo"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de entradas CFOP cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      // Type ID

      // Type name

      TreeItemDataNcm *treeItemDataCategory = new TreeItemDataNcm(dbquery.GetFieldIntByName(categoryIndex, wxT("ncm_id")));

      wxTreeItemId treeItemId = treeCtrlListing->AppendItem(treeItemRootId,
                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      treeCtrlListing->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadItemChildList(treeItemId);
    }
  }

  treeCtrlListing->ExpandAll();

  treeCtrlListing->SelectItem(treeCtrlListing->GetRootItem());

  return 0;
}


int PanelNcmMain::LoadItemChildList(wxTreeItemId treeItemParentId)
{
  if (!treeItemParentId.IsOk() || treeCtrlListing->GetItemData(treeItemParentId) == NULL) {
    return -1;
  }

  int ncmId = ((TreeItemDataNcm *) treeCtrlListing->GetItemData(treeItemParentId))->GetNcmId();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxString::Format(wxT("SELECT ncm_id, codigo, descricao FROM comercio.ncm WHERE ncm_parent_id = %d ORDER BY codigo"), ncmId));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de entradas CFOP cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      TreeItemDataNcm *treeItemDataCategory = new TreeItemDataNcm(dbquery.GetFieldIntByName(categoryIndex, wxT("ncm_id")));

      wxTreeItemId treeItemId = treeCtrlListing->AppendItem(treeItemParentId,
                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      treeCtrlListing->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadItemChildList(treeItemId);
    }
  }


  return 0;
}


void PanelNcmMain::OnCharHook(wxKeyEvent& event)
{
  event.Skip();
}


TreeItemDataNcm::TreeItemDataNcm(const int& ncmId, wxString ncmCode, wxString ncmDescription) {
  this->ncmId = ncmId;

  this->ncmCode = ncmCode;

  this->ncmDescription = ncmDescription;
}


int TreeItemDataNcm::GetNcmId(void) {
  return this->ncmId;
}


wxString TreeItemDataNcm::GetNcmCode(void) {
  return this->ncmCode;
}


wxString TreeItemDataNcm::GetNcmDescription(void) {
  return this->ncmDescription;
}


BEGIN_EVENT_TABLE(PanelNcmMain, wxScrolledWindow)
  EVT_CHAR_HOOK(PanelNcmMain::OnCharHook)
END_EVENT_TABLE()
