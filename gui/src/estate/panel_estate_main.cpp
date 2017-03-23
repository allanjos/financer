/**
 * State main dialog.
 *
 * \version 1.0
 * \since 24/09/2009 01:06
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_parser_xml.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "panel_estate_main.hpp"
#include "panel_estate_new.hpp"
#include "panel_estate_edit.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "pixmaps/search_16x13.xpm"
#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/empresa_16x16.xpm"
#include "pixmaps/customer_16x16.xpm"
#include "pixmaps/save_16x16.xpm"


PanelEstateMain::PanelEstateMain(wxWindow *parent):
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

  toolBar->AddTool(PANEL_ESTATE_MAIN_BTN_SUPPLIER_NEW_ID, Lang::Get("New"), wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_ESTATE_MAIN_BTN_REFRESH_ID, Lang::Get("Refresh"), wxBitmap(refresh_16x16_xpm));

  // Toolbar - Search controls

  toolBar->AddSeparator();

  // Search controls

  teditSearch = new wxTextCtrl(toolBar, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1), wxBORDER_SIMPLE);

  toolBar->AddControl(teditSearch);

  wxBitmapButton *btnSearch = new wxBitmapButton(this, PANEL_ESTATE_MAIN_BTN_SEARCH_ID, wxBitmap(search_16x13_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxBU_EXACTFIT);

  btnSearch->SetMinSize(wxSize(-1, teditSearch->GetSize().GetHeight()));

  toolBar->AddControl(btnSearch);

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Search

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, Lang::Get("Estates"));
  szTop->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  wxBoxSizer *szListing = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szListing, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Supplier listing

  listCtrlReport = new wxListCtrl(this,
                                  PANEL_ESTATE_MAIN_LISTCTRL_ID,
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  szListing->Add(listCtrlReport, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Assign an image list to the control

  wxImageList* imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(client_16x16_xpm));
  imageList->Add(wxIcon(empresa_16x16_xpm));
  listCtrlReport->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColId;
  itemColId.SetText(Lang::Get("ID"));
  listCtrlReport->InsertColumn(0, itemColId);
  listCtrlReport->SetColumnWidth(0, 50);

  wxListItem itemColName;
  itemColName.SetText(Lang::Get("Good"));
  listCtrlReport->InsertColumn(1, itemColName);
  listCtrlReport->SetColumnWidth(1, 250);

  wxListItem itemColType;
  itemColType.SetText(Lang::Get("Code"));
  listCtrlReport->InsertColumn(2, itemColType);
  listCtrlReport->SetColumnWidth(2, 100);

  wxListItem itemColClassification;
  itemColClassification.SetText(Lang::Get("Series number"));
  listCtrlReport->InsertColumn(3, itemColClassification);
  listCtrlReport->SetColumnWidth(3, 100);

  wxListItem itemColContact;
  itemColContact.SetText(Lang::Get("Type"));
  listCtrlReport->InsertColumn(4, itemColContact);
  listCtrlReport->SetColumnWidth(4, 120);

  wxListItem itemColValue;
  itemColValue.SetAlign(wxLIST_FORMAT_RIGHT);
  itemColValue.SetText(Lang::Get("Book value"));
  listCtrlReport->InsertColumn(5, itemColValue);
  listCtrlReport->SetColumnWidth(5, 100);


  // Accounting

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Values"));
  szTop->Add(label, 0, wxTOP | wxBOTTOM, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  wxBoxSizer *szValues = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szValues, 0, wxALL | wxEXPAND, 0);

  wxGridSizer *szValuesFields = new wxGridSizer(0, 3, 5, 5);
  szValues->Add(szValuesFields, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

  // Total accounting value

  wxBoxSizer *szRow = new wxBoxSizer(wxVERTICAL);
  szValuesFields->Add(szRow, 0, wxALL | wxEXPAND, 0);

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Purchase cost") + wxT(": "));
  szRow->Add(label, 0, wxALL, 1);

  teditTotalAcquisitionCost = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(70, -1), wxTE_RIGHT|wxTE_READONLY);
  szRow->Add(teditTotalAcquisitionCost, 0, wxALL | wxEXPAND, 1);

  // Acumulated depreciation

  szRow = new wxBoxSizer(wxVERTICAL);
  szValuesFields->Add(szRow, 0, wxALL|wxEXPAND, 0);

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Accumulated depreciation") + wxT(": "));
  szRow->Add(label, 0, wxALL, 1);

  teditTotalAcumulatedDepreciation = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(70, -1), wxTE_RIGHT|wxTE_READONLY);
  szRow->Add(teditTotalAcumulatedDepreciation, 0, wxALL|wxEXPAND, 1);

  // Total accounting value

  szRow = new wxBoxSizer(wxVERTICAL);
  szValuesFields->Add(szRow, 0, wxALL|wxEXPAND, 0);

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Book value") + wxT(": "));
  szRow->Add(label, 0, wxALL, 1);

  teditTotalAccountingValue = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(70, -1), wxTE_RIGHT|wxTE_READONLY);
  szRow->Add(teditTotalAccountingValue, 0, wxALL|wxEXPAND, 1);


  szTop->Layout();

  listCtrlReport->SetFocus();
}


void PanelEstateMain::Terminate()
{
  Close();

  Destroy();
}


void PanelEstateMain::OnNewDialogRequest(wxCommandEvent& event)
{
  /*
  PatrimonioBensNewDialog patrimonioBensNewDialog(this);

  patrimonioBensNewDialog.ShowModal();
  */

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageCreate(&panelEstateNew, Lang::Get("New estate"), true);

  //LoadData();
}

/*
void PanelEstateMain::OnKeyChar(wxKeyEvent& event)
{
  // CTRL-N - New supplier
  if (event.ControlDown() && event.GetKeyCode() == 14) {
    SupplierNewDialog supplierNewDialog(this);

    supplierNewDialog.ShowModal();
  }
  // CTRL-P - Search
  else if (event.ControlDown() && event.GetKeyCode() == 16) {
    teditSearch->SetFocus();
  }

  event.Skip();
}
*/

void PanelEstateMain::OnKeyUp(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("%d"), event.GetKeyCode()));

  if (event.GetKeyCode() == 315 || event.GetKeyCode() == 317) {
    listCtrlReport->SetFocus();

    listCtrlReport->GetEventHandler()->ProcessEvent(event);
  }

  event.Skip();
}


void PanelEstateMain::OnRefreshRequest(wxCommandEvent &event)
{
  LoadData();
}


int PanelEstateMain::LoadData()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  listCtrlReport->Freeze();

  listCtrlReport->DeleteAllItems();

  DBQuery dbquery;
  std::vector<wxString> params;
  int rc;

  wxString sql = wxT("SELECT b.* ")
                 wxT("FROM ") + DB::GetTableName("estate.item") + wxT(" b ");

  sql += wxT(" ORDER BY b.nome");

  messenger->Register(sql, LogMessage::MSG_DBG);

  if (params.size() > 0) {
    rc = dbquery.Execute(sql, params);
  }
  else {
    rc = dbquery.Execute(sql);
  }

  if (rc != 0) {
    messenger->Register(wxT("Could not get the estates listing. ") +
                        Lang::Get("Internal error") + wxT(": ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlReport->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("estate_item_id")), 0);

      listCtrlReport->SetItemData(resultIndex, dbquery.GetFieldLongByName(resultIndex, wxT("estate_item_id")));

      listCtrlReport->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("nome")));

      listCtrlReport->SetItem(resultIndex, 2, dbquery.GetFieldStrByName(resultIndex, wxT("codigo")));

      listCtrlReport->SetItem(resultIndex, 3, dbquery.GetFieldStrByName(resultIndex, wxT("series_nr")));

      listCtrlReport->SetItem(resultIndex, 5, Monetary::Number2Monetary(dbquery.GetFieldStrByName(resultIndex, wxT("book_value"))));

      /*
      if (dbquery.GetFieldLongByName(resultIndex, wxT("tipo")) == 1) {
        listCtrlReport->SetItem(resultIndex, 3, wxT("Pessoa"));

        listCtrlReport->SetItemImage(resultIndex, 0);
      }
      else if (dbquery.GetFieldLongByName(resultIndex, wxT("tipo")) == 2) {
        listCtrlReport->SetItem(resultIndex, 3, wxT("Empresa"));

        listCtrlReport->SetItemImage(resultIndex, 1);
      }
      else {
        listCtrlReport->SetItem(resultIndex, 3, wxT("Não especificado"));

        listCtrlReport->SetItemImage(resultIndex, -1);
      }
      */
    }
  }

  listCtrlReport->Thaw();

  // Load values

  rc = dbquery.Execute(wxT("SELECT SUM(book_value) AS total_contabil, SUM(value_original) AS custo_aquisicao ")
                       wxT("FROM") + DB::GetTableName("estate.item") + wxT(" b "));

  if (rc != 0) {
    messenger->Register(wxT("Could not get the total amount of estates. ") +
                        Lang::Get("Internal error") + wxT(": ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    teditTotalAcquisitionCost->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, wxT("custo_aquisicao"))));

    teditTotalAccountingValue->SetValue(Monetary::Number2Monetary(dbquery.GetFieldStrByName(0, wxT("total_contabil"))));
  }

  return 0;
}


void PanelEstateMain::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == 341) {
    /*
    PatrimonioBensNewDialog patrimonioBensNewDialog(this);

    patrimonioBensNewDialog.SetAutoLayout(true);
    patrimonioBensNewDialog.Layout();

    patrimonioBensNewDialog.ShowModal();
    */

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->PageCreate(&panelEstateNew, Lang::Get("New estate"), true);

    LoadData();
  }

  event.Skip();
}


void PanelEstateMain::OnListItemKeyDown(wxListEvent &event)
{
  if (event.GetIndex() < 0) {
    return;
  }

  if (event.GetKeyCode() == WXK_DELETE) {
    wxListItem listItem;

    listItem.SetId(event.GetIndex());
    listItem.SetColumn(1);
    listItem.SetMask(wxLIST_MASK_TEXT);
    
    listCtrlReport->GetItem(listItem);

    wxString supplierName = listItem.GetText();

    if (wxYES == wxMessageBox(wxT("Você confirma a exclusão das informações sobre o bem selecionado?"),
                              wxT("Confirmação de exclusão de informações sobre bem"),
                              wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION, NULL)) {

      DBQuery dbquery;

      wxString sqlCommand = wxString::Format(wxT("DELETE FROM patrimonio.bem WHERE estate_item_id = %d"),
                                             listCtrlReport->GetItemData(event.GetIndex()));

      int rc = dbquery.Execute(sqlCommand);

      if (rc != 0) {
        wxMessageBox(wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

        wxMessageBox(wxT("Não foi possível excluir as informações do cliente selecionado."),
                     wxT("Erro"),
                     wxOK|wxICON_ERROR);
      }
      else {
        //listCtrlReport->DeleteItem(event.GetIndex());

        wxMessageBox(wxT("As informações sobre o bem '") + supplierName + wxT("' foram excluídas."));

        LoadData();
      }
    }
  }

  event.Skip();
}


void PanelEstateMain::OnListItemEditDialogRequest(wxListEvent &event)
{
  /*
  wxListItem listItem = event.GetItem();

  PatrimonioBensEditDialog patrimonioBensEditDialog(this);

  patrimonioBensEditDialog.LoadData(listItem.GetData());

  patrimonioBensEditDialog.SetAutoLayout(true);

  patrimonioBensEditDialog.Layout();

  patrimonioBensEditDialog.ShowModal();


  LoadData();
  */

  wxListItem listItem = event.GetItem();

  int data = listItem.GetData();

  //wxMessageBox(wxString::Format(wxT("data = %d"), data));

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString caption = Lang::Get("Estate") + wxT(" ") + wxString::Format("%d", data);

  mainFrame->PageCreate(&panelEstateEdit, caption, true);

  panelEstateEdit->LoadData(listItem.GetData());
}


void PanelEstateMain::OnSearchRequest(wxCommandEvent& event)
{
  LoadData();
}


BEGIN_EVENT_TABLE(PanelEstateMain, wxScrolledWindow)
  //EVT_CHAR(PanelEstateMain::OnKeyChar)
  EVT_KEY_DOWN(PanelEstateMain::OnKeyUp)
  EVT_TOOL(PANEL_ESTATE_MAIN_BTN_SUPPLIER_NEW_ID, PanelEstateMain::OnNewDialogRequest)
  EVT_TOOL(PANEL_ESTATE_MAIN_BTN_REFRESH_ID, PanelEstateMain::OnRefreshRequest)
  EVT_KEY_DOWN(PanelEstateMain::OnCharHook)
  EVT_LIST_KEY_DOWN(PANEL_ESTATE_MAIN_LISTCTRL_ID, PanelEstateMain::OnListItemKeyDown)
  EVT_LIST_ITEM_ACTIVATED(PANEL_ESTATE_MAIN_LISTCTRL_ID, PanelEstateMain::OnListItemEditDialogRequest)
  EVT_BUTTON(PANEL_ESTATE_MAIN_BTN_SEARCH_ID, PanelEstateMain::OnSearchRequest)
END_EVENT_TABLE()