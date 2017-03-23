/**
 * People main dialog.
 *
 * \file  dlg_product_new.cpp
 * \version 1.0
 * \since 08/19/2007 10:50:12 AM AMT
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_database.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
//#include "dlg_client_new.hpp"
#include "panel_person_main.hpp"
#include "panel_person_edit.hpp"
#include "panel_person_new.hpp"
#include "pixmaps/search_16x16.xpm"
#include "pixmaps/customer_16x16.xpm"
#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/person_add_16x16.xpm"


PanelPersonMain::PanelPersonMain(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  // Main sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(PANEL_PERSON_MAIN_TOOL_NEW_ID,
                   Lang::Get("New"),
                   wxBitmap(person_add_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_PERSON_MAIN_TOOL_REFRESH_ID,
                   Lang::Get("Refresh"),
                   wxBitmap(refresh_16x16_xpm));

  toolBar->AddSeparator();

  // Toolbar - Search controls

  toolBar->AddControl(new wxStaticBitmap(toolBar, wxID_ANY, wxBitmap(search_16x16_xpm)));

  wxTextCtrl *teditSearch = new wxTextCtrl(toolBar, wxID_ANY, wxT(""));

  teditSearch->SetMinSize(wxSize(150, 23));

  toolBar->AddControl(teditSearch);


  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL|wxEXPAND|wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // People/contacts listing

  listCtrlReport = new wxListCtrl(this,
                                  PANEL_PERSON_MAIN_LISTCTRL_CLIENT_LIST_ID,
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  // Assign an image list to the control

  wxImageList* imageList = new wxImageList(16, 16);

  imageList->Add(wxIcon(client_16x16_xpm));

  listCtrlReport->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColName;
  itemColName.SetText(Lang::Get("Contact"));
  listCtrlReport->InsertColumn(0, itemColName);
  listCtrlReport->SetColumnWidth(0, 200);

  wxListItem itemColType;
  itemColType.SetText(Lang::Get("Type"));
  listCtrlReport->InsertColumn(1, itemColType);
  listCtrlReport->SetColumnWidth(1, 100);

  wxListItem itemColPhoneCom;
  itemColPhoneCom.SetText(Lang::Get("Phone"));
  listCtrlReport->InsertColumn(2, itemColPhoneCom);
  listCtrlReport->SetColumnWidth(2, 105);

  wxListItem itemColEmail;
  itemColEmail.SetText(Lang::Get("E-mail"));
  listCtrlReport->InsertColumn(3, itemColEmail);
  listCtrlReport->SetColumnWidth(3, 200);

  szTop->Add(listCtrlReport, 1, wxALL | wxEXPAND | wxGROW, 0);


  // Load list

  LoadData();


  listCtrlReport->SetFocus();
}


void PanelPersonMain::Terminate()
{
  Close();

  Destroy();
}


int PanelPersonMain::LoadData(void)
{
  listCtrlReport->Freeze();

  listCtrlReport->DeleteAllItems();

  DBQuery dbquery;
  DBQuery dbqueryPhone;
  std::vector<wxString> params;

  int rc = dbquery.Execute(wxT("SELECT p.*, t.nome AS tipo_nome ")
                           wxT("FROM ") + DB::GetTableName("person") + wxT(" p ") +
                           wxT("LEFT JOIN " + DB::GetTableName("person.type") + " t ON (p.tipo_id = t.tipo_id) ")
                           wxT("ORDER BY lower(p.nome)"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Erro na obtenção da lista de pessoas cadastradas."), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlReport->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("nome")), 0);

      listCtrlReport->SetItemData(resultIndex, dbquery.GetFieldLongByName(resultIndex, wxT("person_id")));

      listCtrlReport->SetItem(resultIndex, 1, dbquery.GetFieldStrByName(resultIndex, wxT("tipo_nome")));

      if (dbquery.GetFieldStrByName(resultIndex, wxT("email")).Length() > 0) {
        listCtrlReport->SetItem(resultIndex, 3, dbquery.GetFieldStrByName(resultIndex, wxT("email")));
      }
      else if (dbquery.GetFieldStrByName(resultIndex, wxT("email_2")).Length() > 0) {
        listCtrlReport->SetItem(resultIndex, 3, dbquery.GetFieldStrByName(resultIndex, wxT("email_2")));
      }

      // Phone

      params.clear();

      params.push_back(dbquery.GetFieldStrByName(resultIndex, wxT("person_id")));

      rc = dbqueryPhone.Execute(wxT("SELECT * ")
                                wxT("FROM pessoa.telefone ")
                                wxT("WHERE person_id = $1 ")
                                wxT("AND numero != '' AND numero IS NOT NULL"), params);

      if (rc != 0) {
        LogMessenger *messenger = LogMessenger::GetInstance();

        messenger->Register(wxT("Erro na obtenção do telefone da pessoa."), LogMessage::MSG_ERROR);
      }
      else if (dbqueryPhone.GetRowsCount() > 0) {
        listCtrlReport->SetItem(resultIndex, 2, dbqueryPhone.GetFieldStrByName(0, wxT("numero")));
      }
    }
  }

  listCtrlReport->Thaw();

  return 0;
}


/*
void PanelPersonMain::OnPersonTypeMainDialogRequest(wxCommandEvent& event)
{
  PersonTypeMainDialog clientTypeMainDialog(this);

  clientTypeMainDialog.ShowModal();
}
*/


void PanelPersonMain::OnKeyChar(wxKeyEvent& event)
{
  // CTRL-N - New client
  if (event.ControlDown() && event.GetKeyCode() == 14) {
    /*
    PersonNewDialog clientNewDialog(this);

    clientNewDialog.ShowModal();
    */
  }
  // CTRL-P - Search
  else if (event.ControlDown() && event.GetKeyCode() == 16) {
    teditSearch->SetFocus();
  }

  event.Skip();
}

void PanelPersonMain::OnKeyUp(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("%d"), event.GetKeyCode()));

  if (event.GetKeyCode() == 315 || event.GetKeyCode() == 317) {
    listCtrlReport->SetFocus();

    listCtrlReport->GetEventHandler()->ProcessEvent(event);
  }

  event.Skip();
}


void PanelPersonMain::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == 341) {
    /*
    PersonNewDialog clientNewDialog(this);

    clientNewDialog.ShowModal();
    */

    LoadData();
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


void PanelPersonMain::OnPersonNewDialogRequest(wxCommandEvent& event)
{
  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString caption = Lang::Get("New contact");

  PanelPersonNew *panelPersonNew;

  mainFrame->PageCreate(&panelPersonNew, caption, true);
}


void PanelPersonMain::OnListItemKeyDown(wxListEvent &event)
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

    wxString clientName = listItem.GetText();

    if (wxYES == wxMessageBox(wxT("Você confirma a exclusão do registro do pessoa '") + clientName + wxT("'?"),
                              wxT("Confirmação de exclusão de registro de pessoa"),
                              wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION, NULL)) {

      DBQuery dbquery;

      listCtrlReport->DeleteAllItems();

      int rc = dbquery.Execute(wxString::Format("DELETE FROM pessoa.pessoa WHERE person_id = %ld",
                               listCtrlReport->GetItemData(event.GetIndex())));

      if (rc != 0) {
        wxMessageBox(wxT("Não foi possível excluir as informações sobre a pessoa selecionada."),
                     wxT("Erro"),
                     wxOK|wxICON_ERROR);
      }
      else {
        listCtrlReport->DeleteItem(event.GetIndex());

        wxMessageBox(wxT("As informações sobre o pessoa '") + clientName + wxT("' foram excluídas."));
      }
    }
  }
}


void PanelPersonMain::OnPersonDataViewItemActivated(wxListEvent& event)
{
  wxListItem listItem = event.GetItem();

  int data = listItem.GetData();

  //wxMessageBox(wxString::Format(wxT("data = %d"), data));

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString caption = Lang::Get("Contact") + wxT(" #") + wxString::Format("%d", data);

  mainFrame->PageCreate(&panelPersonEdit, caption, true);

  panelPersonEdit->LoadData(listItem.GetData());
}


void PanelPersonMain::OnRefreshRequest(wxCommandEvent& event)
{
  LoadData();
}


BEGIN_EVENT_TABLE(PanelPersonMain, wxScrolledWindow)
  EVT_CHAR(PanelPersonMain::OnKeyChar)
  EVT_KEY_DOWN(PanelPersonMain::OnKeyUp)
  EVT_TOOL(PANEL_PERSON_MAIN_TOOL_NEW_ID, PanelPersonMain::OnPersonNewDialogRequest)
  EVT_TOOL(PANEL_PERSON_MAIN_TOOL_REFRESH_ID, PanelPersonMain::OnRefreshRequest)
  //EVT_TOOL(PANEL_PERSON_MAIN_TOOL_TYPE_MAIN_ID, PanelPersonMain::OnPersonTypeMainDialogRequest)
  EVT_CHAR_HOOK(PanelPersonMain::OnCharHook)
  EVT_LIST_KEY_DOWN(PANEL_PERSON_MAIN_LISTCTRL_CLIENT_LIST_ID, PanelPersonMain::OnListItemKeyDown)
  EVT_LIST_ITEM_ACTIVATED(PANEL_PERSON_MAIN_LISTCTRL_CLIENT_LIST_ID, PanelPersonMain::OnPersonDataViewItemActivated)
END_EVENT_TABLE()