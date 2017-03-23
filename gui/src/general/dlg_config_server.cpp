/**
 *
 * \file  dlg_conf_currency.cpp
 * \version 1.0
 * \since 08/22/2007 00:50:55 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "gui_controls.hpp"
#include "utils_monetary.hpp"
#include "utils_lang.hpp"
#include "utils_number.hpp"
#include "utils_datetime.hpp"
#include "utils_config.hpp"
#include "dlg_config_server.hpp"
#include "dlg_main.hpp"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


ConfigServerDialog::ConfigServerDialog(wxWindow *parent):
  wxDialog(parent, wxID_ANY, Lang::Get("Configuration"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxFULL_REPAINT_ON_RESIZE)
{
  SetSize(wxSize(750, 550));

  SetAutoLayout(true);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);


  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_CONFIG_SERVER_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_CONFIG_SERVER_TOOL_CLOSE_ID, Lang::Get("Close"), wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szTop->AddSpacer(3);


  // Notebook

  wxNotebook *nbookConfig= new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, GUI_AUI_NOTEBOOK_STYLE);

  szTop->Add(nbookConfig, 1, wxALL | wxGROW | wxEXPAND, 0);


  // General configuration

  wxScrolledWindow *panelConfigGeneral = new wxScrolledWindow(nbookConfig, wxID_ANY);

  panelConfigGeneral->SetAutoLayout(true);

  panelConfigGeneral->SetScrollbars(1, 1, 20, 20);

  nbookConfig->AddPage(panelConfigGeneral, Lang::Get("General"), true);

  wxBoxSizer *szConfigGeneralMargin = new wxBoxSizer(wxVERTICAL);

  panelConfigGeneral->SetSizer(szConfigGeneralMargin);

  wxFlexGridSizer* szConfigGeneralFields = new wxFlexGridSizer(0, 2, 2, 2);
  szConfigGeneralMargin->Add(szConfigGeneralFields, 1, wxALL | wxEXPAND, 10);

  // Currency

  wxStaticText *label = new wxStaticText(panelConfigGeneral, wxID_STATIC, Lang::Get("Currency") + wxT(": "));
  szConfigGeneralFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  cboxCurrentCurrency = new wxChoice(panelConfigGeneral,
                                     wxID_ANY);
  szConfigGeneralFields->Add(cboxCurrentCurrency, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Date format

  label = new wxStaticText(panelConfigGeneral, wxID_STATIC, Lang::Get("Date format") + wxT(": "));
  szConfigGeneralFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  cboxDateFormat = new wxChoice(panelConfigGeneral, wxID_ANY);
  szConfigGeneralFields->Add(cboxDateFormat, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxDateFormat->Append(Lang::Get("MM/DD/YYYY"), (wxClientData *) new ContainerIntData(DT::DATE_ENG_FORMAT));
  cboxDateFormat->Append(Lang::Get("DD/MM/YYYY"), (wxClientData *) new ContainerIntData(DT::DATE_BRA_FORMAT));

  cboxDateFormat->SetSelection(0);

  // Date format

  label = new wxStaticText(panelConfigGeneral, wxID_STATIC, Lang::Get("Amount format") + wxT(": "));
  szConfigGeneralFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  cboxNumberFormat = new wxChoice(panelConfigGeneral, wxID_ANY);
  szConfigGeneralFields->Add(cboxNumberFormat, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxNumberFormat->Append(Lang::Get("9,999.99"), (wxClientData *) new ContainerIntData(Number::NUMBER_ENG_FORMAT));
  cboxNumberFormat->Append(Lang::Get("9.999,99"), (wxClientData *) new ContainerIntData(Number::NUMBER_BRA_FORMAT));

  cboxNumberFormat->SetSelection(0);


  // Expense configuration panel

  wxScrolledWindow *panelConfigExpense = new wxScrolledWindow(nbookConfig, wxID_ANY);

  panelConfigExpense->SetAutoLayout(true);

  panelConfigExpense->SetScrollbars(1, 1, 20, 20);

  nbookConfig->AddPage(panelConfigExpense, Lang::Get("Expenses"), false);

  wxBoxSizer *szMarginExpenseConfig = new wxBoxSizer(wxHORIZONTAL);

  panelConfigExpense->SetSizer(szMarginExpenseConfig);

  // Margin

  wxBoxSizer *szPanelConfigExpense = new wxBoxSizer(wxVERTICAL);
  szMarginExpenseConfig->Add(szPanelConfigExpense, 1, wxALL | wxEXPAND | wxGROW, 10);


  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szPanelConfigExpense->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  // Conta padrão para quitação de compras

  label = new wxStaticText(panelConfigExpense,
                           wxID_STATIC,
                           Lang::Get("Default ledger account for expenses payment") + wxT(": "),
                           wxDefaultPosition,
                           wxDefaultSize);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxAccountExpensePayment = new wxChoice(panelConfigExpense, wxID_ANY);
  szRow->Add(cboxAccountExpensePayment, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  cboxAccountExpensePayment->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  cboxAccountExpensePayment->SetSelection(0);



  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szTop->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);


  // Top sizer ending configuration.

  this->CenterOnParent();

  LoadData();
}


void ConfigServerDialog::Terminate()
{
  Close();

  Destroy();
}


void ConfigServerDialog::LoadData()
{
  if (!DB::IsConnected()) {
    return;
  }

  LoadAccountList();

  Monetary::LoadCurrencyList(cboxCurrentCurrency);


  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT c.* ")
                           wxT("FROM ") + DB::GetTableName("configuration") + wxT(" c"));

  if (rc != 0) {
    wxMessageBox(Lang::Get("Cannot get the current system configuration") + wxT("."));

    return;
  }
  else if (dbquery.GetRowsCount() > 0) {
    // Default currency

    for (unsigned int index = 0; index < cboxCurrentCurrency->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) cboxCurrentCurrency->GetClientObject(index);

      if (container && container->GetData() == dbquery.GetFieldIntByName(0, wxT("currency_id"))) {
        cboxCurrentCurrency->SetSelection(index);
      }
    }

    // Date format

    for (unsigned int index = 0; index < cboxDateFormat->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) cboxDateFormat->GetClientObject(index);

      if (container && container->GetData() == dbquery.GetFieldIntByName(0, wxT("date_format_id"))) {
        cboxDateFormat->SetSelection(index);
      }
    }

    // Number format

    for (unsigned int index = 0; index < cboxNumberFormat->GetCount(); index++) {
      ContainerIntData *container = (ContainerIntData *) cboxNumberFormat->GetClientObject(index);

      if (container && container->GetData() == dbquery.GetFieldIntByName(0, wxT("number_format_id"))) {
        cboxNumberFormat->SetSelection(index);
      }
    }
  }
}


int ConfigServerDialog::LoadAccountList()
{
  if (!DB::IsConnected()) {
    return -1;
  }

  DBQuery dbquery;

  cboxAccountExpensePayment->Freeze();

  cboxAccountExpensePayment->Clear();

  cboxAccountExpensePayment->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  int rc = dbquery.Execute(wxT("SELECT * ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" ") +
                           wxT("WHERE tipo_id = 'P' ")
                           wxT("AND conta_parent_id NOT IN (SELECT account_id ")
                                                       wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" ")
                                                       wxT("WHERE conta_parent_id IS NOT NULL) ")
                           wxT("ORDER BY lower(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de contas de saída cadastradas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("account_id")));

      cboxAccountExpensePayment->Append(dbquery.GetFieldStrByName(index, wxT("nome")), (wxClientData *) container);
    }
  }

  cboxAccountExpensePayment->SetSelection(0);

  cboxAccountExpensePayment->Thaw();

  return 0;
}



void ConfigServerDialog::OnSaveRequest(wxCommandEvent &event)
{
  std::vector<wxString> fields, params, paramTypes;
  DBQuery dbquery;

  // Check if there is a saved configuration

  wxString sql = wxT("SELECT COUNT(*) AS cnt ")
                 wxT("FROM ") + DB::GetTableName("configuration");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar se a configuração existente.\n\n")
                 wxT("Erro interno: ") + dbquery.GetMessageStr());

    return;
  }

  int configurationExists = dbquery.GetFieldIntByName(0, wxT("cnt"));

  fields.push_back(wxT("currency_id"));
  paramTypes.push_back(wxT("int"));
  if (cboxCurrentCurrency->GetClientObject(cboxCurrentCurrency->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxCurrentCurrency->GetClientObject(cboxCurrentCurrency->GetSelection());

    if (container->GetData() != 0)
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    else
      params.push_back(wxEmptyString);
  }
  else {
    params.push_back(wxEmptyString);
  }

  fields.push_back(wxT("date_format_id"));
  paramTypes.push_back(wxT("int"));
  if (cboxDateFormat->GetClientObject(cboxDateFormat->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxDateFormat->GetClientObject(cboxDateFormat->GetSelection());

    if (container->GetData() != 0)
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    else
      params.push_back(wxEmptyString);
  }
  else {
    params.push_back(wxEmptyString);
  }

  fields.push_back(wxT("number_format_id"));
  paramTypes.push_back(wxT("int"));
  if (cboxNumberFormat->GetClientObject(cboxNumberFormat->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxNumberFormat->GetClientObject(cboxNumberFormat->GetSelection());

    if (container->GetData() != 0)
      params.push_back(wxString::Format(wxT("%d"), container->GetData()));
    else
      params.push_back(wxEmptyString);
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Update configuration information

  if (configurationExists) {
    sql = DBUtils::MountSQLUpdate(DB::GetTableName("configuration"), fields);
  }
  // Insert configuration information
  else {
    sql = DBUtils::MountSQLInsert(DB::GetTableName("configuration"), fields);
  }

  rc = dbquery.Execute(sql, params, paramTypes);

  if (rc != 0) {
    wxMessageBox(wxT("Ocorreu erro na tentativa de salvar as informações editadas."));
  }
  else {
    statusBar->SetStatusText(wxT("Informações salvas."));
  }

  Config::LoadServer();
}


void ConfigServerDialog::OnCloseRequest(wxCommandEvent &event)
{
  Terminate();
}


void ConfigServerDialog::OnCharHook(wxKeyEvent& event)
{
  // Exit <ESC>
  switch (event.GetKeyCode()) {
    case WXK_ESCAPE:

      Terminate();

      break;

    default:

      event.Skip();
  }
}


BEGIN_EVENT_TABLE(ConfigServerDialog, wxDialog)
  EVT_TOOL(DLG_CONFIG_SERVER_TOOL_SAVE_ID, ConfigServerDialog::OnSaveRequest)
  EVT_TOOL(DLG_CONFIG_SERVER_TOOL_CLOSE_ID, ConfigServerDialog::OnCloseRequest)
  EVT_CHAR_HOOK(ConfigServerDialog::OnCharHook)
END_EVENT_TABLE()