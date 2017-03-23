/**
 *
 * \version 1.0
 * \since 08/19/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_monetary.hpp"
#include "utils_image.hpp"
#include "utils_config.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_calendar.hpp"
#include "panel_estate_new.hpp"
#include "dlg_ncm_choose.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "pixmaps/folder_bmp_btn.xpm"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"

PanelEstateNew::PanelEstateNew(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  SetAutoLayout(true);

  this->ncmId = -1;

  SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szRow;


  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_ESTATE_NEW_TOOL_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  szMargin->AddSpacer(5);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, (wxALL & (~wxTOP | ~wxBOTTOM)) | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  wxFont font;

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  // Name

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, Lang::Get("Name") + wxT(": "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szRow->Add(teditName, 1, wxALL | wxEXPAND, 1);

  szRow->AddSpacer(10);

  // Code

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Code") + wxT(": "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditCode = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120, -1));
  szRow->Add(teditCode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Notebook to separate information by category

  notebook = new wxNotebook(this, wxID_ANY);

  notebook->SetAutoLayout(true);

  szTop->Add(notebook, 1, wxGROW|wxALL, 1);



  // Panel to main information

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY);

  panelMain->SetAutoLayout(true);

  notebook->AddPage(panelMain, Lang::Get("General"), true);

  panelMain->SetScrollbars(1, 1, 20, 20);


  wxBoxSizer *szPanelMainMargin = new wxBoxSizer(wxVERTICAL);

  panelMain->SetSizer(szPanelMainMargin);


  wxBoxSizer *szPanelMain = new wxBoxSizer(wxVERTICAL);

  szPanelMainMargin->Add(szPanelMain, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  wxBoxSizer *szPanelMainColums = new wxBoxSizer(wxHORIZONTAL);
  szPanelMain->Add(szPanelMainColums, 1, wxALL | wxEXPAND | wxGROW, 10);


  // Main information tab - 1st column

  wxBoxSizer *szColumn_1 = new wxBoxSizer(wxVERTICAL);
  szPanelMainColums->Add(szColumn_1, 1, wxALL | wxEXPAND | wxGROW, 0);


  wxFlexGridSizer* szgMainFields = new wxFlexGridSizer(0, 2, 5, 5);

  szColumn_1->Add(szgMainFields, 0, wxALL | wxEXPAND | wxGROW, 0);

  // Data de aquisição

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Date of acquisition") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  teditAcquisitionDate = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditAcquisitionDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarAcquisitionDate = new wxBitmapButton(panelMain,
                                                  DLG_ESTATE_NEW_BTN_ACQUISITION_DATE_ID,
                                                  wxBitmap(calendar_bmp_btn_xpm),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
  szRow->Add(btnCalendarAcquisitionDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarAcquisitionDate->SetMinSize(wxSize(-1, teditAcquisitionDate->GetSize().GetHeight()));

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("          "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Active or not

  chkActive = new wxCheckBox(panelMain, wxID_ANY, Lang::Get("Enabled"));
  chkActive->SetValue(true);
  szRow->Add(chkActive, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Description

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Description") + wxT(": "), wxDefaultPosition, wxSize(-1, 70), 0);
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 1);

  teditDescription = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(1, 10), wxTE_MULTILINE);
  szgMainFields->Add(teditDescription, 1, wxALL | wxEXPAND | wxGROW, 1);

  // Type

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Type") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 1, wxALL | wxEXPAND, 0);

  cboxType = new wxComboBox(panelMain, wxID_ANY, wxEmptyString);
  szRow->Add(cboxType, 1, wxALL | wxEXPAND, 1);

  // Manufacturer

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Manufacturer") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxManufacturer = new wxComboBox(panelMain, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxManufacturer, 1, wxALL | wxEXPAND, 1);

  // Trademark

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Trademark") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxTrademark = new wxComboBox(panelMain, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxTrademark, 1, wxALL | wxEXPAND, 1);

  // Supplier

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Supplier") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxSupplier = new wxComboBox(panelMain, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxSupplier, 1, wxALL | wxEXPAND, 1);

  // Measure unit

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Measure unit") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxMeasureUnit = new wxComboBox(panelMain, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxMeasureUnit, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Warranty time

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Warranty time") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditTempoGarantia = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1), wxTE_RIGHT);
  szRow->Add(teditTempoGarantia, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditTempoGarantia->SetValidator(wxTextValidator(wxFILTER_DIGITS));

  cboxTempoGarantiaType = new wxChoice(panelMain, wxID_ANY);
  szRow->Add(cboxTempoGarantiaType, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxTempoGarantiaType->Append(wxT("days"), (wxClientData *) new ContainerIntData((int) 'D'));
  cboxTempoGarantiaType->Append(wxT("months"), (wxClientData *) new ContainerIntData((int) 'M'));
  cboxTempoGarantiaType->Append(wxT("years"), (wxClientData *) new ContainerIntData((int) 'Y'));

  cboxTempoGarantiaType->SetSelection(2);


  szgMainFields->AddGrowableCol(1);


  // Insurance

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Insurance"));

  GUI::FormatSectionLabel(label);

  szColumn_1->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);


  wxBoxSizer* szBoxSeguro = new wxBoxSizer(wxVERTICAL);
  szColumn_1->Add(szBoxSeguro, 0, wxALL | wxEXPAND | wxGROW, 0);

  wxFlexGridSizer* szgSeguroFields = new wxFlexGridSizer(0, 2, 5, 5);
  szBoxSeguro->Add(szgSeguroFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Insurance company

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Company") + wxT(": "));
  szgSeguroFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgSeguroFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 0);

  teditSeguroCompanhia = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1));
  szRow->Add(teditSeguroCompanhia, 1, wxALL | wxEXPAND, 1);

  // Apólice

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("Policy: "));
  szgSeguroFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgSeguroFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditSeguroApolice = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditSeguroApolice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Data de vencimento da apólice

  label = new wxStaticText(panelMain, wxID_STATIC, wxT("     ") + Lang::Get("Due date") + wxT(": "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditSeguroApoliceVencimentoDate = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditSeguroApoliceVencimentoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarApoliceVencimentoDate = new wxBitmapButton(panelMain,
                                               DLG_ESTATE_NEW_BTN_EXPIRY_DATE_ID,
                                               wxBitmap(calendar_bmp_btn_xpm),
                                               wxDefaultPosition,
                                               wxDefaultSize);
  szRow->Add(btnCalendarApoliceVencimentoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarApoliceVencimentoDate->SetMinSize(wxSize(-1, teditSeguroApoliceVencimentoDate->GetSize().GetHeight()));

  szgSeguroFields->AddGrowableCol(1);


  // Physical identification

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Physical identification"));

  GUI::FormatSectionLabel(label);

  szColumn_1->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);

  wxBoxSizer* szBoxPhysicalId = new wxBoxSizer(wxVERTICAL);
  szColumn_1->Add(szBoxPhysicalId, 0, wxALL | wxEXPAND | wxGROW, 0);

  wxFlexGridSizer* szgPhysicalIdFields = new wxFlexGridSizer(0, 2, 5, 5);
  szBoxPhysicalId->Add(szgPhysicalIdFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Número de série

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Series number") + wxT(": "));
  szgPhysicalIdFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgPhysicalIdFields->Add(szRow, 0, wxALL | wxEXPAND, 1);

  teditSerieNumber = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditSerieNumber, 1, wxALL | wxEXPAND, 1);

  // Bar code

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Bar code") + wxT(": "));
  szgPhysicalIdFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgPhysicalIdFields->Add(szRow, 0, wxALL | wxEXPAND, 0);

  teditBarCode = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditBarCode, 1, wxALL | wxEXPAND, 1);


  // Columns separator

  wxStaticLine *staticLine = new wxStaticLine(panelMain, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);

  szPanelMainColums->Add(staticLine, 0, wxALL | wxEXPAND | wxGROW, 15);


  // Main information tab - 2nd column

  wxBoxSizer *szColumn_2 = new wxBoxSizer(wxVERTICAL);
  szPanelMainColums->Add(szColumn_2, 1, wxALL | wxEXPAND | wxGROW, 10);


  // Dimensions

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Dimensions"));

  GUI::FormatSectionLabel(label);

  szColumn_2->Add(label, 0, wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);


  wxBoxSizer* szBoxDimensions = new wxBoxSizer(wxVERTICAL);
  szColumn_2->Add(szBoxDimensions, 0, wxALL | wxEXPAND | wxGROW, 0);

  wxFlexGridSizer* szgDimensionsFields = new wxFlexGridSizer(0, 2, 5, 5);
  szBoxDimensions->Add(szgDimensionsFields, 1, wxALL | wxEXPAND | wxGROW, 0);

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Length") + wxT(": "));
  szgDimensionsFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDimensionsFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditLength = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT(" ") + Lang::Get("meter") + wxT("(s)"));
  label->SetForegroundColour(GUI::COLOR_LABEL_INFORMATION);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Width

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Width") + wxT(": "));
  szgDimensionsFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDimensionsFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditWidth = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT(" ") + Lang::Get("meter") + wxT("(s)"));
  label->SetForegroundColour(GUI::COLOR_LABEL_INFORMATION);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Height

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Height") + wxT(": "));
  szgDimensionsFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDimensionsFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditHeight = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT(" ") + Lang::Get("meter") + wxT("(s)"));
  label->SetForegroundColour(GUI::COLOR_LABEL_INFORMATION);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Weight

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Weight"));

  GUI::FormatSectionLabel(label);

  szColumn_2->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);


  wxBoxSizer* szBoxWeight = new wxBoxSizer(wxVERTICAL);
  szColumn_2->Add(szBoxWeight, 0, wxALL|wxEXPAND|wxGROW, 0);

  wxFlexGridSizer* szgWeightFields = new wxFlexGridSizer(0, 2, 5, 5);
  szBoxWeight->Add(szgWeightFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Liquid weight

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Net weight") + wxT(": "));
  szgWeightFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgWeightFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditWeigthLiquid = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditWeigthLiquid, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT(" gram(s)"));
  label->SetForegroundColour(GUI::COLOR_LABEL_INFORMATION);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Brute weight

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Gross weight") + wxT(": "));
  szgWeightFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgWeightFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditWeigthBrute = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditWeigthBrute, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT(" gram(s)"));
  label->SetForegroundColour(GUI::COLOR_LABEL_INFORMATION);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szgWeightFields->AddGrowableCol(1);
  szgWeightFields->AddGrowableRow(1);


  // Accounting

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Accounting"));

  GUI::FormatSectionLabel(label);

  szColumn_2->Add(label, 0, wxTOP | wxBOTTOM | wxEXPAND, GUI::LABEL_MARGIN);


  wxBoxSizer* szBoxDepreciacao = new wxBoxSizer(wxVERTICAL);
  szColumn_2->Add(szBoxDepreciacao, 0, wxALL | wxEXPAND | wxGROW, 0);

  wxFlexGridSizer* szgDepreciacaoFields = new wxFlexGridSizer(0, 2, 5, 5);
  szBoxDepreciacao->Add(szgDepreciacaoFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Valor original do bem

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Original valor") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditValue = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Valor de mercado

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Market value") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditValueMarket = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditValueMarket, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Taxa de depreciação

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Depreciation rate") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditDepreciacaoTaxa = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditDepreciacaoTaxa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMain, wxID_STATIC, wxT(" %"));
  label->SetForegroundColour(GUI::COLOR_LABEL_INFORMATION);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Date

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Start of depreciation") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL | wxEXPAND | wxGROW, 1);

  teditDepreciacaoDate = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditDepreciacaoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarDepreciacaoDate = new wxBitmapButton(panelMain,
                                                  DLG_ESTATE_NEW_BTN_DEPRECIATION_BEGIN_DATE_ID,
                                                  wxBitmap(calendar_bmp_btn_xpm),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
  szRow->Add(btnCalendarDepreciacaoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarDepreciacaoDate->SetMinSize(wxSize(-1, teditDepreciacaoDate->GetSize().GetHeight()));

  // Acelerated depreciation

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Accelerated depreciation") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

  wxArrayString depreciacaoAceleradaValoresPreDefinidos;
  
  depreciacaoAceleradaValoresPreDefinidos.Add(wxT(""));
  depreciacaoAceleradaValoresPreDefinidos.Add(wxT("1,5 (2 turnos diários de operação, 16 h)"));
  depreciacaoAceleradaValoresPreDefinidos.Add(wxT("2 (3 turnos diários de operação, 24 h)"));

  choiceDepreciacaoAcelerada = new wxChoice(panelMain, wxID_ANY, wxDefaultPosition, wxSize(50, -1), depreciacaoAceleradaValoresPreDefinidos);
  szRow->Add(choiceDepreciacaoAcelerada, 1, wxALL | wxEXPAND, 1);

  // Taxa de depreciação

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Accumulated depreciation") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditDepreciacaoAcumulada = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1), wxTE_RIGHT);
  szRow->Add(teditDepreciacaoAcumulada, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Valor líquido do bem

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Book value") + wxT(": "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditLiquidValue = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1), wxTE_RIGHT);
  szRow->Add(teditLiquidValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  szgDepreciacaoFields->AddGrowableCol(1);


  // Picture information

  wxPanel *panelPicture = new wxPanel(notebook, wxID_ANY);

  notebook->AddPage(panelPicture, Lang::Get("Image"), false);


  wxBoxSizer* szPanelPictureMargin = new wxBoxSizer(wxVERTICAL);

  panelPicture->SetSizer(szPanelPictureMargin);


  wxBoxSizer* szPanelPicture = new wxBoxSizer(wxVERTICAL);

  szPanelPictureMargin->Add(szPanelPicture, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  // Picture information

  wxBoxSizer *szImageInfo = new wxBoxSizer(wxVERTICAL);
  szPanelPicture->Add(szImageInfo, 0, (wxALL & ~wxTOP)|wxEXPAND, 5);

  wxBoxSizer *szPictureFileChoose = new wxBoxSizer(wxHORIZONTAL);
  szImageInfo->Add(szPictureFileChoose, 0, wxALL|wxEXPAND, 0);

  // Controls for choosing the picture file

  label = new wxStaticText(panelPicture, wxID_STATIC, Lang::Get("File") + wxT(": "));
  szPictureFileChoose->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditPictureFilePath = new wxTextCtrl(panelPicture, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szPictureFileChoose->Add(teditPictureFilePath, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditPictureFilePath->Disable();

  wxBitmapButton *btnPictureFilePathChoose = new wxBitmapButton(panelPicture, DLG_ESTATE_NEW_BTN_CHOOSE_IMAGE_FILE_ID, wxBitmap(folder_bmp_btn_xpm));
  szPictureFileChoose->Add(btnPictureFilePathChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnPictureFilePathChoose->SetMinSize(wxSize(-1, teditPictureFilePath->GetSize().GetHeight()));

  // Picture file information

  wxFlexGridSizer *szPictureInfoFields = new wxFlexGridSizer(4, 4, 2);
  szImageInfo->Add(szPictureInfoFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // File size

  label = new wxStaticText(panelPicture, wxID_STATIC, Lang::Get("Type") + wxT(": "));
  szPictureInfoFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 0);

  wxTextCtrl *teditPictureType = new wxTextCtrl(panelPicture, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(5, -1));
  szPictureInfoFields->Add(teditPictureType, 1, wxALL | wxEXPAND, 0);

  teditPictureType->Disable();

  // File size

  label = new wxStaticText(panelPicture, wxID_STATIC, wxT("     ") + Lang::Get("Size") + wxT(": "));
  szPictureInfoFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 0);

  wxTextCtrl *teditPictureSize = new wxTextCtrl(panelPicture, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(5, -1));
  szPictureInfoFields->Add(teditPictureSize, 1, wxALL | wxEXPAND, 0);

  teditPictureSize->Disable();


  szPictureInfoFields->AddGrowableCol(1);
  szPictureInfoFields->AddGrowableCol(3);


  // Image picture

  wxBoxSizer *szImage = new wxBoxSizer(wxHORIZONTAL);
  szPanelPicture->Add(szImage, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  // Static bitmap

  staticBmpPicture = new wxStaticBitmap(panelPicture, wxID_STATIC, bmpPictureEmpty);
  szImage->Add(staticBmpPicture, 1, wxALL|wxEXPAND|wxGROW, 0);


  // Technical information tab

  wxScrolledWindow *panelTechnicalInfo = new wxScrolledWindow(notebook, wxID_ANY);

  notebook->AddPage(panelTechnicalInfo, Lang::Get("Additional information"), false);

  panelTechnicalInfo->SetScrollbars(1, 1, 20, 20);


  wxBoxSizer* szgTechnicalFields = new wxBoxSizer(wxHORIZONTAL);
  panelTechnicalInfo->SetSizer(szgTechnicalFields);

  teditTechnicalInfo = new wxTextCtrl(panelTechnicalInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1), wxTE_MULTILINE);
  szgTechnicalFields->Add(teditTechnicalInfo, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);


  // Top sizer ending configuration.

  //szTop->SetSizeHints(this);

  this->CenterOnParent();


  LoadGroupsListing();

  LoadMeasureListing();


  CenterOnParent();


  teditName->SetFocus();
}


void PanelEstateNew::Terminate()
{
  Close();

  Destroy();
}


int PanelEstateNew::LoadGroupsListing()
{
  cboxType->Freeze();

  cboxType->Clear();

  cboxType->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  cboxType->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT tipo_id, nome FROM produto.tipo ORDER BY LOWER(nome)"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Error on trying to load groups list.\n\n")
                        wxT("Database error: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("tipo_id")));

      cboxType->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxType->SetSelection(0);

  cboxType->Thaw();

  return 0;
}


int PanelEstateNew::LoadMeasureListing()
{
  cboxMeasureUnit->Freeze();

  cboxMeasureUnit->Clear();

  cboxMeasureUnit->Append(Lang::Get("Select"), (wxClientData *) new ContainerIntData(0));

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT * FROM medida.medida ORDER BY LOWER(nome)"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Could not get the measure units list.\n\n")
                        wxT("Database error: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("measure_id")));

      cboxMeasureUnit->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxMeasureUnit->SetStringSelection(wxT("Unidade"));

  cboxMeasureUnit->Thaw();

  return 0;
}


void PanelEstateNew::OnSaveRequest(wxCommandEvent &event)
{
  if (teditName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(wxT("É necessário informar o nome do produto."));

    return;
  }

  std::vector<wxString> params, fields;
  bool registeringError = false;
  wxString messageStr = wxT("");
  wxString sql = wxT("");

  // Name

  fields.push_back(wxT("nome"));
  params.push_back(teditName->GetValue());

  // Status

  fields.push_back(wxT("status"));

  if (chkActive->GetValue()) {
    params.push_back(wxT("A"));
  }
  else {
    params.push_back(wxT("I"));
  }

  /*
  // Physical or virtual

  fields.push_back(wxT("fisico"));

  if (radioBtnVirtual->GetValue()) {
    params.push_back(wxT("f"));
  }
  else {
    params.push_back(wxT("t"));
  }
  */

  // Series number

  if (teditSerieNumber->GetValue().Length() > 0) {
    fields.push_back(wxT("series_nr"));
    params.push_back(teditSerieNumber->GetValue());
  }

  // Code

  if (teditBarCode->GetValue().Length() > 0) {
    fields.push_back(wxT("barcode"));
    params.push_back(teditBarCode->GetValue());
  }

  // Description

  if (teditDescription->GetValue().Length() > 0) {
    fields.push_back(wxT("descricao"));
    params.push_back(teditDescription->GetValue());
  }

  /*
  // Type

  if (cboxType->GetSelection() >= 0 && cboxType->GetClientObject(cboxType->GetSelection()) != NULL) {
    fields.push_back(wxT("tipo_id"));
    params.push_back(wxString::Format(wxT("%d"), (int) cboxType->GetClientObject(cboxType->GetSelection())));
  }

  // Subtype

  if (cboxSubType->GetSelection() >= 0 && cboxSubType->GetClientObject(cboxSubType->GetSelection()) != NULL) {
    fields.push_back(wxT("subtipo_id"));
    params.push_back(wxString::Format(wxT("%d"), (int) cboxSubType->GetClientObject(cboxSubType->GetSelection())));
  }
  */

  // Manufacturer

  if (cboxManufacturer->GetSelection() >= 0 && cboxManufacturer->GetClientObject(cboxManufacturer->GetSelection())) {
    fields.push_back(wxT("fabricante_id"));

    ContainerIntData *container = (ContainerIntData *) cboxManufacturer->GetClientObject(cboxManufacturer->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }

  // Trademark

  if (cboxTrademark->GetSelection() >= 0 && cboxTrademark->GetClientObject(cboxTrademark->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxTrademark->GetClientObject(cboxTrademark->GetSelection());

    fields.push_back(wxT("marca_id"));

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }

  // Category

  if (teditCategoryId->GetValue().Length() > 0) {
    fields.push_back(wxT("categoria_id"));

    params.push_back(teditCategoryId->GetValue());
  }

  // Measure unit

  if (cboxMeasureUnit->GetSelection() >= 0 && cboxMeasureUnit->GetClientObject(cboxMeasureUnit->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxMeasureUnit->GetClientObject(cboxMeasureUnit->GetSelection());

    fields.push_back(wxT("measure_id"));

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }

  // Length

  if (teditLength->GetValue().Length() > 0) {
    fields.push_back(wxT("comprimento"));

    params.push_back(teditLength->GetValue());
  }

  // Width

  if (teditWidth->GetValue().Length() > 0) {
    fields.push_back(wxT("largura"));

    params.push_back(teditWidth->GetValue());
  }

  // Height

  if (teditHeight->GetValue().Length() > 0) {
    fields.push_back(wxT("altura"));

    params.push_back(teditHeight->GetValue());
  }

  // Liquid weight

  if (teditWeigthLiquid->GetValue().Length() > 0) {
    fields.push_back(wxT("net_weight"));

    params.push_back(teditWeigthLiquid->GetValue());
  }

  // Brute weight

  if (teditWeigthBrute->GetValue().Length() > 0) {
    fields.push_back(wxT("gross_weight"));

    params.push_back(teditWeigthBrute->GetValue());
  }

  // Original value

  if (teditValue->GetValue().Length() > 0) {
    fields.push_back(wxT("value_original"));

    params.push_back(teditValue->GetValue());
  }

  // Market value

  if (teditValueMarket->GetValue().Length() > 0) {
    fields.push_back(wxT("value_market"));

    params.push_back(teditValueMarket->GetValue());
  }

  // Depreciation percentage

  if (teditDepreciacaoTaxa->GetValue().Length() > 0) {
    fields.push_back(wxT("depreciation_rate"));

    params.push_back(teditDepreciacaoTaxa->GetValue());
  }

  // Depreciation begin date

  if (teditDepreciacaoDate->GetValue().Length() > 0) {
    fields.push_back(wxT("depreciation_start_date"));

    params.push_back(teditDepreciacaoDate->GetValue());
  }

  // Acumulated depreciation

  if (teditDepreciacaoAcumulada->GetValue().Length() > 0) {
    fields.push_back(wxT("depreciation_accumulated"));

    params.push_back(teditDepreciacaoAcumulada->GetValue());
  }

  // Accounting value

  if (teditLiquidValue->GetValue().Length() > 0) {
    fields.push_back(wxT("book_value"));

    params.push_back(teditLiquidValue->GetValue());
  }

  // Insurance

  if (teditSeguroCompanhia->GetValue().Length() > 0) {
    fields.push_back(wxT("insurance_company"));

    params.push_back(teditSeguroCompanhia->GetValue());
  }

  // Insurance policy

  if (teditSeguroApolice->GetValue().Length() > 0) {
    fields.push_back(wxT("insurance_policy"));

    params.push_back(teditSeguroApolice->GetValue());
  }

  // Insurance expiry date

  if (teditSeguroApoliceVencimentoDate->GetValue().Length() > 0) {
    fields.push_back(wxT("insurance_due_dt"));

    params.push_back(teditSeguroApoliceVencimentoDate->GetValue());
  }
return;

  DBQuery dbquery;


  // Init transaction

  DBTransaction::Begin();


  sql = DBUtils::MountSQLInsert(wxT("patrimonio.bem"), fields);

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messageStr = wxT("Erro interno do banco:\n") + dbquery.GetMessageStr();
  }


  // Get good id

  unsigned int goodId = -1;

  if (!registeringError) {
    goodId = DBUtils::GetSerialKey(wxT("patrimonio.bem"), wxT("estate_item_id"));

    if (goodId < 1) {
      registeringError = true;

      messageStr = wxT("Não foi possível obter a identificação do patrimônio cadastrado.\n\n")
                   wxT("Erro interno do banco:\n") + dbquery.GetMessageStr();
    }
  }


  // Technical information

  if (!registeringError && teditTechnicalInfo->GetValue().Length() > 0) {
    fields.clear();
    params.clear();

    wxString insertValuesStr = wxT("");

    // Item ID

    fields.push_back(wxT("estate_item_id"));
    params.push_back(wxString::Format(wxT("%ld"), goodId));

    // Technical information content

    fields.push_back(wxT("conteudo"));
    params.push_back(teditTechnicalInfo->GetValue());

    sql = DBUtils::MountSQLInsert(wxT("patrimonio.bem_informacao_tecnica"), fields);

    //wxMessageBox(sql);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      messageStr = (wxT("Erro no cadastro das informações técnicas sobre o bem patrimonial.\n\n")
                    wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());

      registeringError = true;
    }
  }


  // Picture
#if 0
  if (!registeringError) {
    if (teditPictureFilePath->GetValue().Length() > 0) {
      //char contentStr[strlen(teditLicitacaoNotes->GetValue().mb_str(wxConvUTF8)) + 1];

      // File descriptor

      char fileName[strlen((const char *) teditPictureFilePath->GetValue().mb_str(wxConvUTF8)) + 1];

      strcpy(fileName, (const char *) teditPictureFilePath->GetValue().mb_str(wxConvUTF8));

      wxFFile file(teditPictureFilePath->GetValue(), wxT("rb"));

      long fileLength = file.Length();

      file.Close();

      // Opening picture file

      FILE *picturefd = fopen(fileName, "rb");

      if (!picturefd) {
        fprintf(stderr, "Não foi possível abrir o arquivo de entrada.\n");

        exit(1);
      }

      // Getting the file content

      char fileContent[fileLength * 2 + 1];

      int readCount = fread((void *) fileContent, 1, fileLength * 2, picturefd);

      //wxMessageBox(wxString::Format(wxT("readCount: %d"), readCount));

      // Check picture type by extension

      char imageMimeType[50];

      wxRegEx regex;

      if (regex.Compile(wxT("\\.png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
          regex.Matches(teditPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/png");
      }
      else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(teditPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/jpeg");
      }
      else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(teditPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/gif");
      }
      else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(teditPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/bmp");
      }
      else {
        imageMimeType[0] = '\0';
      }


      PGresult *res;
      char sqlCommandAnsi[1024];
      char *paramValues[2];
      int paramLength[2];
      int paramFormats[2];

      sprintf(sqlCommandAnsi, "INSERT INTO patrimonio.bem_imagem (estate_item_id, file_mime, file_content) VALUES (%d, $1, $2)",
                              goodId);

      //wxMessageBox(wxString(sqlCommandAnsi, wxConvUTF8, strlen(sqlCommandAnsi)));

      paramValues[0] = imageMimeType;
      paramLength[0] = strlen(imageMimeType);
      paramFormats[0] = 0;

      paramValues[1] = fileContent;
      paramLength[1] = readCount;
      paramFormats[1] = 1;

      res = PQexecParams((PGconn *) DB::connection,
                         sqlCommandAnsi,
                         2, // Parameters count
                         NULL, // Parameter types
                         paramValues, // Parameter values
                         paramLength, // Parameter lenghts
                         paramFormats, // Parameter formats
                         0); // Binary results

      if (res == NULL) {
        if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
          wxMessageBox(wxT("Não foi possível cadastrar a imagem do bem patrimonial.\n\nErro interno:\n") +
                       wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
        }
      }
      else {
        ExecStatusType status = PQresultStatus((PGresult *) res); 

        if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
          if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
            wxMessageBox(wxT("Não foi possível cadastrar a imagem do bem patrimonial.\n\nErro interno:\n") +
                           wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
          }
        }
      }

    }
  }
#endif

  // Registering error

  if (registeringError) {
    DBTransaction::Rollback();

    wxMessageBox(wxString(wxT("Não foi possível cadastrar o bem patrimonial '")) + teditName->GetValue() + wxString(wxT("'.\n\n")) +
                 messageStr);
  }
  // Good is successfully registered
  else {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Bem patrimonial '") + teditName->GetValue() + wxT("' cadastrado com sucesso."), LogMessage::MSG_INFO);

    // End transaction

    DBTransaction::Commit();


    // Close panel

    wxString pageTitle = Lang::Get("New estate");

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->PageRemove(pageTitle);
  }
}


void PanelEstateNew::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // Exit <ESC>
  switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
      Close();
      break;
  }

  event.Skip();
}


void PanelEstateNew::OnChoosePictureFileRequest(wxCommandEvent &event)
{
  wxString caption = wxT("Selecione o arquivo de imagem ilustrativa");
  wxString wildcardOptions = wxT("Todos os arquivos de imagens (*.png;*.jpg;*.jpeg;*.bmp;*.gif)|*.png;*.jpg;*.jpeg;*.bmp;*.gif|")
                             wxT("PNG files (*.png)|*.png|")
                             wxT("JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|")
                             wxT("BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif");

  wxFileDialog fileDialog(this,
                          caption,
                          wxEmptyString,
                          wxEmptyString,
                          wildcardOptions,
                          wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR|wxFD_PREVIEW);

  if (fileDialog.ShowModal() == wxID_OK) {
    /*
    wxBitmapType bitmapType = wxBITMAP_TYPE_PNG;

    wxRegEx regex;

    if (regex.Compile(wxT("\\.png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(fileDialog.GetPath())) {
      bitmapType = wxBITMAP_TYPE_PNG;
    }
    else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(fileDialog.GetPath())) {
      bitmapType = wxBITMAP_TYPE_JPEG;
    }
    else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(fileDialog.GetPath())) {
      bitmapType = wxBITMAP_TYPE_GIF;
    }
    else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) && regex.Matches(fileDialog.GetPath())) {
      bitmapType = wxBITMAP_TYPE_BMP;
    }
    else {
      wxMessageBox(wxT("Tipo de arquivo não suportado."));

      return;
    }
    */

    if (!imagePicture.LoadFile(fileDialog.GetPath())) {
      wxMessageBox(wxT("O arquivo '") + fileDialog.GetPath() + wxT("' não pôde ser carregado."));

      return;
    }

    staticBmpPicture->GetContainingSizer()->Layout();
    staticBmpPicture->Layout();
    wxGetApp().GetTopWindow()->Layout();

    UtilsImage::Rescale(imagePicture, staticBmpPicture->GetSize().GetWidth(), staticBmpPicture->GetSize().GetHeight());

    wxBitmap bmpPicture(imagePicture);


    teditPictureFilePath->SetValue(fileDialog.GetPath());

    staticBmpPicture->SetBitmap(bmpPicture);

    staticBmpPicture->Layout();

    staticBmpPicture->GetContainingSizer()->Layout();

    wxGetApp().GetTopWindow()->Layout();

    staticBmpPicture->Refresh();
    staticBmpPicture->Update();

    Refresh();
    Update();
  }
}


int PanelEstateNew::LoadIcmsCstTables()
{
  DBQuery dbquery;

  // Load CST table A

  cboxIcmsCstA->Freeze();

  cboxIcmsCstA->Clear();

  int rc = dbquery.Execute(wxT("SELECT * FROM tributario.imposto_icms_cst_a ORDER BY lower(codigo)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de tipos de produtos.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    cboxIcmsCstA->Append(wxT(""), (void *) NULL);

    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("codigo")));

      wxString caption = dbquery.GetFieldStrByName(index, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(index, wxT("nome"));

      cboxIcmsCstA->Append(caption, container);
    }
  }

  cboxIcmsCstA->Thaw();

  // Load CST table B

  cboxIcmsCstB->Freeze();

  cboxIcmsCstB->Clear();

  rc = dbquery.Execute(wxT("SELECT * FROM tributario.imposto_icms_cst_b ORDER BY lower(codigo)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de tipos de produtos.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    cboxIcmsCstB->Append(wxT(""), (void *) NULL);

    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("codigo")));

      wxString caption = dbquery.GetFieldStrByName(index, wxT("codigo")) + wxT(" - ") + dbquery.GetFieldStrByName(index, wxT("nome"));

      cboxIcmsCstB->Append(caption, container);
    }
  }

  cboxIcmsCstB->Thaw();

  return 0;
}


void PanelEstateNew::OnNcmChooseDialogRequest(wxCommandEvent &event)
{
  NcmChooseDialog ncmChooseDialog(this);

  ncmChooseDialog.ShowModal();

  if (ncmChooseDialog.IsCanceled()) {
    return;
  }

  this->ncmId = ncmChooseDialog.GetNcmId();

  teditFiscalCodigoNCM->SetValue(ncmChooseDialog.GetNcmCode());

  teditFiscalNCMDescription->SetValue(ncmChooseDialog.GetNcmDescription());
}


void PanelEstateNew::OnAcquisitionDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditAcquisitionDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarAcquisitionDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarAcquisitionDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditAcquisitionDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                          calendarDialog.selectedMonth + wxT("/") + 
                          calendarDialog.selectedYear);
  }
}


void PanelEstateNew::OnExpiryDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditSeguroApoliceVencimentoDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarApoliceVencimentoDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarApoliceVencimentoDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditSeguroApoliceVencimentoDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                                               calendarDialog.selectedMonth + wxT("/") + 
                                               calendarDialog.selectedYear);
  }
}


void PanelEstateNew::OnDepreciationBeginDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditDepreciacaoDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnCalendarDepreciacaoDate->GetScreenPosition().x;
  calendarPosition.y = btnCalendarDepreciacaoDate->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditDepreciacaoDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                                   calendarDialog.selectedMonth + wxT("/") + 
                                   calendarDialog.selectedYear);
  }
}


void PanelEstateNew::OnCloseRequest(wxCloseEvent& event)
{
  event.Skip();
}


BEGIN_EVENT_TABLE(PanelEstateNew, wxScrolledWindow)
  EVT_TOOL(DLG_ESTATE_NEW_TOOL_SAVE_ID, PanelEstateNew::OnSaveRequest)

  EVT_KEY_UP(PanelEstateNew::OnCharHook)

  EVT_BUTTON(DLG_ESTATE_NEW_BTN_CHOOSE_IMAGE_FILE_ID, PanelEstateNew::OnChoosePictureFileRequest)
  EVT_BUTTON(DLG_ESTATE_NEW_BTN_NCM_CHOOSE_ID, PanelEstateNew::OnNcmChooseDialogRequest)
  EVT_BUTTON(DLG_ESTATE_NEW_BTN_ACQUISITION_DATE_ID, PanelEstateNew::OnAcquisitionDateChooseRequest)
  EVT_BUTTON(DLG_ESTATE_NEW_BTN_EXPIRY_DATE_ID, PanelEstateNew::OnExpiryDateChooseRequest)
  EVT_BUTTON(DLG_ESTATE_NEW_BTN_DEPRECIATION_BEGIN_DATE_ID, PanelEstateNew::OnDepreciationBeginDateChooseRequest)
  EVT_CLOSE(PanelEstateNew::OnCloseRequest)
END_EVENT_TABLE()
