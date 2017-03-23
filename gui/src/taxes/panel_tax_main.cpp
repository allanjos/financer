/**
 * Taxes main dialog.
 *
 * \version 1.0
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 * \since 01/03/2009
 */

#include "utils_include.hpp"
#include "gui_controls.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "panel_tax_main.hpp"
#include "panel_cfop_main.hpp"
#include "panel_ncm_main.hpp"
#include "pixmaps/currency_2_16x16.xpm"


PanelTaxMain::PanelTaxMain(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  SetAutoLayout(true);


  SetScrollbars(1, 1, 20, 20);


  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_TAX_MAIN_BTN_CFOP_ID,
                   wxT("CFOP"),
                   wxBitmap(currency_2_16x16_xpm),
                   wxT("Código Fiscal das Operações e Prestações."));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_TAX_MAIN_BTN_NCM_ID,
                   wxT("NCM"),
                   wxBitmap(currency_2_16x16_xpm),
                   wxT("Nomenclatura Comum do Mercosul."));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_TAX_MAIN_BTN_IPI_ID,
                   wxT("IPI"),
                   wxBitmap(currency_2_16x16_xpm),
                   wxT("Imposto sobre produtos industrializados."));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Label - ICMS

  wxStaticText *label = new wxStaticText(this, wxID_ANY, wxT("ICMS"));
  szTop->Add(label, 0, wxBOTTOM, 10);

  wxFont font = label->GetFont();
  font.MakeBold();
  font.SetPointSize(font.GetPointSize() + 1);
  label->SetFont(font);


  wxBoxSizer* szICMS = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szICMS, 1, wxALL|wxEXPAND, 5);

  // ICMS - Table A

  wxBoxSizer* szIcmsA = new wxBoxSizer(wxVERTICAL);
  szICMS->Add(szIcmsA, 1, (wxALL & ~wxTOP) | wxEXPAND, 1);

  label = new wxStaticText(this, wxID_ANY, wxT("Tabela A - Origem da mercadoria"));
  szIcmsA->Add(label, 0, wxBOTTOM, GUI_CONTROL_MARGIN_SIZE);

  font = label->GetFont();
  font.MakeBold();
  label->SetFont(font);

  listCtrlReportIcmsA = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(100, -1), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);
  szIcmsA->Add(listCtrlReportIcmsA, 1, wxRIGHT | wxEXPAND, GUI_CONTROL_MARGIN_SIZE);

  // Code

  wxListItem itemColIcmsTable1Id;
  itemColIcmsTable1Id.SetText(wxT("Código"));
  itemColIcmsTable1Id.SetAlign(wxLIST_FORMAT_CENTRE);
  listCtrlReportIcmsA->InsertColumn(0, itemColIcmsTable1Id);
  listCtrlReportIcmsA->SetColumnWidth(0, 60);

  // Name

  wxListItem itemColIcmsTable1Name;
  itemColIcmsTable1Name.SetText(wxT("Nome"));
  listCtrlReportIcmsA->InsertColumn(1, itemColIcmsTable1Name);
  listCtrlReportIcmsA->SetColumnWidth(1, 270);


  // ICMS - Table B

  wxBoxSizer* szIcmsB = new wxBoxSizer(wxVERTICAL);
  szICMS->Add(szIcmsB, 1, (wxALL & ~wxTOP)|wxEXPAND, 1);

  label = new wxStaticText(this, wxID_ANY, wxT("Tabela B - Tributada pelo ICMS"));
  szIcmsB->Add(label, 0, wxBOTTOM, GUI_CONTROL_MARGIN_SIZE);

  font = label->GetFont();
  font.MakeBold();
  label->SetFont(font);

  listCtrlReportIcmsB = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(100, -1), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);
  szIcmsB->Add(listCtrlReportIcmsB, 1, wxEXPAND, 0);

  // Code

  wxListItem itemColIcmsTable2Id;
  itemColIcmsTable2Id.SetText(wxT("Código"));
  itemColIcmsTable2Id.SetAlign(wxLIST_FORMAT_CENTRE);
  listCtrlReportIcmsB->InsertColumn(0, itemColIcmsTable2Id);
  listCtrlReportIcmsB->SetColumnWidth(0, 60);

  // Name

  wxListItem itemColIcmsTable2Name;
  itemColIcmsTable2Name.SetText(wxT("Nome"));
  listCtrlReportIcmsB->InsertColumn(1, itemColIcmsTable2Name);
  listCtrlReportIcmsB->SetColumnWidth(1, 350);

  // Load ICMS table A

  LoadICMSTables();
}


void PanelTaxMain::Terminate()
{
  Close();

  Destroy();
}


int PanelTaxMain::LoadICMSTables()
{
  int resultIndex;
  DBQuery dbquery;

  // ICMS table A

  listCtrlReportIcmsA->DeleteAllItems();

  int rc = dbquery.Execute(wxT("SELECT * FROM tributario.imposto_icms_cst_a ORDER BY lower(codigo)"));

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de tributos.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }

  if (dbquery.GetRowsCount() > 0) {
    for (resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlReportIcmsA->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("codigo")));

      listCtrlReportIcmsA->SetItemData(resultIndex, dbquery.GetFieldIntByName(resultIndex, wxT("codigo")));

      listCtrlReportIcmsA->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("nome")));

      GUI::ListSetRowBgColor(listCtrlReportIcmsA, resultIndex);
    }
  }

  // ICMS table B

  listCtrlReportIcmsB->DeleteAllItems();

  rc = dbquery.Execute(wxT("SELECT * FROM tributario.imposto_icms_cst_b ORDER BY lower(codigo)"));

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter a listagem de tributos.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }

  if (dbquery.GetRowsCount() > 0) {
    for (resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlReportIcmsB->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("codigo")));

      listCtrlReportIcmsB->SetItemData(resultIndex, dbquery.GetFieldIntByName(resultIndex, wxT("codigo")));

      listCtrlReportIcmsB->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("nome")));

      GUI::ListSetRowBgColor(listCtrlReportIcmsB, resultIndex);
    }
  }

  return 0;
}


void PanelTaxMain::OnCfopMainDialogRequest(wxCommandEvent &event)
{
  wxString pageTitle = wxT("CFOP");

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageCreate(&panelCfopMain, pageTitle);
}


void PanelTaxMain::OnNcmMainDialogRequest(wxCommandEvent &event)
{
  wxString pageTitle = wxT("NCM");

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageCreate(&panelNcmMain, pageTitle);
}


void PanelTaxMain::OnIpiMainDialogRequest(wxCommandEvent &event)
{
  wxMessageBox(wxT("PanelTaxMain::OnIpiMainDialogRequest"));
  //IpiMainDialog ipiMainDialog(this);

  //ipiMainDialog.ShowModal();
}


BEGIN_EVENT_TABLE(PanelTaxMain, wxScrolledWindow)
  EVT_TOOL(DLG_TAX_MAIN_BTN_CFOP_ID, PanelTaxMain::OnCfopMainDialogRequest)
  EVT_TOOL(DLG_TAX_MAIN_BTN_NCM_ID, PanelTaxMain::OnNcmMainDialogRequest)
  EVT_TOOL(DLG_TAX_MAIN_BTN_IPI_ID, PanelTaxMain::OnIpiMainDialogRequest)
END_EVENT_TABLE()
