/**
 * About dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include "dlg_bank_list.hpp"
#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "app_version.hpp"
#include "pixmaps/close_16x16.xpm"
#include "pixmaps/bank_16x16.xpm"


BankListDialog::BankListDialog(wxWindow *parent):
  wxDialog(parent, wxID_ANY, "")
{
  SetTitle(Lang::Get("List of banks"));

  SetSize(wxSize(600, 500));

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(wxID_CANCEL, Lang::Get("Close"), wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  wxFlexGridSizer *szFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szTop->Add(szFields, 1, wxALL, 0);


  listCtrlBanks = new wxListCtrl(this,
                                 DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
                                 wxDefaultPosition,
                                 wxSize(5, 5),
                                 wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  szTop->Add(listCtrlBanks, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Assign an image list to the control

  wxImageList* imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(bank_16x16_xpm));
  listCtrlBanks->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColBankName;
  itemColBankName.SetText(Lang::Get("Bank"));
  listCtrlBanks->InsertColumn(0, itemColBankName);
  listCtrlBanks->SetColumnWidth(0, 300);

  wxListItem itemColAccountCountry;
  itemColAccountCountry.SetText(Lang::Get("Country"));
  listCtrlBanks->InsertColumn(1, itemColAccountCountry);
  listCtrlBanks->SetColumnWidth(1, 150);


  CenterOnParent();


  LoadList();
}


int BankListDialog::LoadList()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  listCtrlBanks->DeleteAllItems();

  DBQuery dbquery;
  DBQuery dbqueryPhone;
  std::vector<wxString> params;

  wxString sql = wxT("SELECT b.bank_id, b.nome, c.nome AS country_name ")
                 wxT("FROM ") + DB::GetTableName("bank") + wxT(" b ")
                 wxT("LEFT JOIN ") + DB::GetTableName("location.country") + wxT(" c ON (c.country_id = b.country_id) ")
                 wxT("ORDER BY b.nome, c.nome");

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Could not get the bank list."), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      listCtrlBanks->InsertItem(resultIndex, dbquery.GetFieldStrByName(resultIndex, wxT("nome")), 0);

      listCtrlBanks->SetItemData(resultIndex, dbquery.GetFieldLongByName(resultIndex, wxT("bank_id")));

      listCtrlBanks->SetItem(resultIndex, 1, Lang::Get(dbquery.GetFieldStrByName(resultIndex, wxT("country_name"))));
    }
  }

  return 0;
}


void BankListDialog::Terminate()
{
  Close();

  Destroy();
}


void BankListDialog::OnCloseRequest(wxCommandEvent& event)
{
  Terminate();
}


void BankListDialog::OnCharHook(wxKeyEvent& event)
{
  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Terminate();
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(BankListDialog, wxDialog)
  EVT_TOOL(wxID_CANCEL, BankListDialog::OnCloseRequest)
  EVT_CHAR_HOOK(BankListDialog::OnCharHook)
END_EVENT_TABLE()