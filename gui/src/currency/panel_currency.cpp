/**
 *
 * \version 1.0
 * \since 08/22/2007
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "utils_config.hpp"
#include "panel_currency.hpp"
#include "pixmaps/currency_16x16.xpm"


PanelCurrency::PanelCurrency(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS)
{
  SetAutoLayout(true);

  SetSize(wxSize(400, 600));

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxEXPAND, 0);

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, Lang::Get("From") + wxT(": "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  wxChoice *choiceCurrencyFrom = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceCurrencyFrom, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT("  ") + Lang::Get("to") + wxT(":  "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER, 1);

  wxChoice *choiceCurrencyTo = new wxChoice(this, wxID_ANY);
  szRow->Add(choiceCurrencyTo, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Quotation rate

  label = new wxStaticText(this, wxID_STATIC, wxT("  ") + Lang::Get("Quotation") + wxT(":  "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  wxTextCtrl *teditQuotation = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditQuotation, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  wxButton *buttonQuotationAdd = new wxButton(this, wxID_ANY, Lang::Get("Add quotation"));
  szRow->Add(buttonQuotationAdd, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);


  szTop->AddSpacer(10);


  // Currency quotations

  listCtrlQuotations = new wxListCtrl(this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);

  // Assign an image list to the control

  wxImageList* imageList = new wxImageList(16, 16);

  imageList->Add(wxIcon(currency_16x16_xpm));


  listCtrlQuotations->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColName;
  itemColName.SetText(Lang::Get("From"));
  listCtrlQuotations->InsertColumn(0, itemColName);
  listCtrlQuotations->SetColumnWidth(0, 200);

  wxListItem itemColType;
  itemColType.SetText(Lang::Get("To"));
  listCtrlQuotations->InsertColumn(1, itemColType);
  listCtrlQuotations->SetColumnWidth(1, 200);

  wxListItem itemColPhoneCom;
  itemColPhoneCom.SetText(Lang::Get("Quotation"));
  listCtrlQuotations->InsertColumn(2, itemColPhoneCom);
  listCtrlQuotations->SetColumnWidth(2, 100);

  szTop->Add(listCtrlQuotations, 1, wxALL | wxEXPAND | wxGROW, 0);
}


void PanelCurrency::Terminate()
{
  Close();

  Destroy();
}


void PanelCurrency::OnSaveRequest(wxCommandEvent &event)
{
  wxMessageBox(wxT("PanelCurrency::OnSaveRequest"));
}


void PanelCurrency::OnCharHook(wxKeyEvent& event)
{
  // Exit <ESC>
  switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
      Terminate();

      break;
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(PanelCurrency, wxScrolledWindow)
  EVT_CHAR_HOOK(PanelCurrency::OnCharHook)
END_EVENT_TABLE()