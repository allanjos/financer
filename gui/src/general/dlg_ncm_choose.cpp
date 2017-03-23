/**
 *
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 04/06/2009
 */


#include "utils_include.hpp"
#include "gui_controls.hpp"
#include "dlg_main.hpp"
#include "panel_ncm_main.hpp"
#include "dlg_ncm_choose.hpp"
#include "pixmaps/currency_2_16x16.xpm"
#include "pixmaps/nota_fiscal_32x32.xpm"
#include "pixmaps/select_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


NcmChooseDialog::NcmChooseDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxString(wxT("")),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxWANTS_CHARS)
{
  this->isCanceled = true;

  this->ncmId = -1;

  this->ncmCode = wxT("");

  this->ncmDescription = wxT("");

  SetSize(GUI::GetDefaultWindowSize(this));

  SetIcon(wxIcon(currency_2_16x16_xpm));


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);


  // Main title

  GUI::DrawTitle(this,
                 wxT("NCM - Procura"),
                 wxT("Procura por Código Fiscal das Operações e Prestações."),
                 wxBitmap(nota_fiscal_32x32_xpm));


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE|wxAUI_TB_TEXT|wxAUI_TB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_NCM_CHOOSE_BTN_SELECT_ID,
                   wxT(" Selecionar"),
                   wxBitmap(select_16x16_xpm), wxT("Confirmar escolha pelo NCM selecionado."));

  toolBar->AddTool(DLG_NCM_CHOOSE_BTN_CLOSE_ID,
                   wxT(" Fechar"),
                   wxBitmap(close_16x16_xpm), wxT("Fechar"));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);


  szTop->Add(new wxStaticLine(this), 0, wxALL|wxEXPAND|wxGROW, 0);



  treeCtrlListing = new wxTreeCtrl(this,
                                   DLG_NCM_CHOOSE_TREECTRL_LISTING_ID,
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   wxTR_DEFAULT_STYLE|wxTR_HAS_BUTTONS|wxTR_SINGLE);

  szTop->Add(treeCtrlListing, 1, wxALL|wxEXPAND|wxGROW|wxALIGN_RIGHT, 5);



  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szTop->Add(statusBar, 0, wxALL|wxGROW|wxEXPAND, 0);


  this->CenterOnParent();


  // Load taxes listing

  LoadList();
}


void NcmChooseDialog::Terminate()
{
  Close();

  Destroy();
}


void NcmChooseDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


int NcmChooseDialog::LoadList()
{
  treeCtrlListing->DeleteAllItems();

  wxTreeItemId treeItemRootId = treeCtrlListing->AddRoot(wxT("Categorias"));

  treeCtrlListing->ExpandAll();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT ncm_id, codigo, descricao FROM comercio.ncm WHERE ncm_parent_id IS NULL ORDER BY codigo"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de entradas NCM cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      // Type ID

      // Type name

      TreeItemDataNcm *treeItemDataNcm = new TreeItemDataNcm(dbquery.GetFieldIntByName(categoryIndex, wxT("ncm_id")),
                                                                dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")),
                                                                dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      wxTreeItemId treeItemId = treeCtrlListing->AppendItem(treeItemRootId,
                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      treeCtrlListing->SetItemData(treeItemId, treeItemDataNcm);

      // Load children

      LoadCategoryChildList(treeItemId);
    }
  }

  treeCtrlListing->ExpandAll();

  treeCtrlListing->SelectItem(treeCtrlListing->GetRootItem());

  return 0;
}


int NcmChooseDialog::LoadCategoryChildList(wxTreeItemId treeItemParentId)
{
  if (!treeItemParentId.IsOk() || treeCtrlListing->GetItemData(treeItemParentId) == NULL) {
    return -1;
  }

  int ncmId = ((TreeItemDataNcm *) treeCtrlListing->GetItemData(treeItemParentId))->GetNcmId();


  DBQuery dbquery;

  int rc = dbquery.Execute(wxString::Format(wxT("SELECT ncm_id, codigo, descricao FROM comercio.ncm WHERE ncm_parent_id = %d ORDER BY codigo"), ncmId));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de entradas NCM cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int categoryIndex = 0; categoryIndex < dbquery.GetRowsCount(); categoryIndex++) {
      TreeItemDataNcm *treeItemDataCategory = new TreeItemDataNcm(dbquery.GetFieldIntByName(categoryIndex, wxT("ncm_id")),
                                                                    dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")),
                                                                    dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      wxTreeItemId treeItemId = treeCtrlListing->AppendItem(treeItemParentId,
                                                            dbquery.GetFieldStrByName(categoryIndex, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(categoryIndex, wxT("descricao")));

      treeCtrlListing->SetItemData(treeItemId, treeItemDataCategory);

      // Load children

      LoadCategoryChildList(treeItemId);
    }
  }


  return 0;
}


void NcmChooseDialog::OnCharHook(wxKeyEvent& event)
{
  // <F2>
  if (event.GetKeyCode() == 341) {
    //LoadList();
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Terminate();
  }

  event.Skip();
}


void NcmChooseDialog::OnNcmChooseDialogRequest(wxCommandEvent &event)
{
  ItemSelect();

  this->isCanceled = false;

  Close();
}


bool NcmChooseDialog::IsCanceled()
{
  return isCanceled;
}


int NcmChooseDialog::ItemSelect()
{
  wxTreeItemId itemId = treeCtrlListing->GetSelection();

  if (!itemId.IsOk()) {
    return -1;
  }

  if (treeCtrlListing->GetItemData(itemId) == NULL) {
    return -1;
  }

  this->ncmId = ((TreeItemDataNcm *) treeCtrlListing->GetItemData(itemId))->GetNcmId();

  this->ncmCode = ((TreeItemDataNcm *) treeCtrlListing->GetItemData(itemId))->GetNcmCode();

  this->ncmDescription = ((TreeItemDataNcm *) treeCtrlListing->GetItemData(itemId))->GetNcmDescription();

  return 0;
}


int NcmChooseDialog::GetNcmId()
{
  return this->ncmId;
}


wxString NcmChooseDialog::GetNcmCode()
{
  return this->ncmCode;
}


wxString NcmChooseDialog::GetNcmDescription()
{
  return this->ncmDescription;
}


BEGIN_EVENT_TABLE(NcmChooseDialog, wxDialog)
  EVT_TOOL(DLG_NCM_CHOOSE_BTN_CLOSE_ID, NcmChooseDialog::OnCloseRequest)

  EVT_CHAR_HOOK(NcmChooseDialog::OnCharHook)

  EVT_TOOL(DLG_NCM_CHOOSE_BTN_SELECT_ID, NcmChooseDialog::OnNcmChooseDialogRequest)
END_EVENT_TABLE()
