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
#include "utils_product_category.hpp"
#include "gui_controls.hpp"
#include "panel_estate_edit.hpp"
#include "dlg_calendar.hpp"
//#include "dlg_product_search.hpp"
//#include "dlg_product_tax_add.hpp"
#include "dlg_product_category_choose.hpp"
#include "dlg_ncm_choose.hpp"
#include "app.hpp"

#include "pixmaps/folder_bmp_btn.xpm"
#include "pixmaps/image_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"
#include "pixmaps/technical_info_16x16.xpm"
#include "pixmaps/calendar_bmp_btn.xpm"
#include "pixmaps/save_16x16.xpm"


PanelEstateEdit::PanelEstateEdit(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  this->goodId = -1;

  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);


  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  wxBoxSizer *szRow;


  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_ESTATE_EDIT_TOOL_SAVE_ID, wxT(" Salvar"), wxBitmap(save_16x16_xpm), wxT("Salvar informações sobre o produto"));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_ESTATE_EDIT_TOOL_DELETE_ID, wxT(" Excluir"), wxBitmap(delete_16x16_xpm), wxT("Excluir informações sobre o produto."));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL|wxEXPAND|wxGROW, 0);

  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  // Product code

  wxFont font;

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxGROW, 5);

  // Name

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, wxT("Nome: "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  teditName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szRow->Add(teditName, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Code

  label = new wxStaticText(this, wxID_STATIC, wxT("     Código: "));
  font = label->GetFont();
  font.Bold();
  label->SetFont(font);
  szRow->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  teditCode = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120, -1));
  szRow->Add(teditCode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Notebook to separate product information by category

  nbookProductInfo= new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, (wxAUI_NB_TOP|wxNO_BORDER) & ~(wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_CLOSE_ON_ALL_TABS));

  nbookProductInfo->SetAutoLayout(true);

  //nbookProductInfo->SetArtProvider(new wxAuiSimpleTabArt);

  //szTop->Add(nbookProductInfo, 0, 0, 0);
  szTop->Add(nbookProductInfo, 1, wxGROW|wxALL, 1);


  // Panel to main product information
  wxScrolledWindow *panelMainInfo = new wxScrolledWindow(nbookProductInfo, wxID_ANY);

  nbookProductInfo->AddPage(panelMainInfo, wxT("Principal"), true);

  panelMainInfo->SetScrollbars(1, 1, 20, 20);


  wxBoxSizer *szMainInfoMargin = new wxBoxSizer(wxVERTICAL);
  panelMainInfo->SetSizer(szMainInfoMargin);

  wxGridSizer *szMainInfoColumns = new wxGridSizer(0, 2, 5, 5);
  szMainInfoMargin->Add(szMainInfoColumns, 1, wxALL|wxEXPAND|wxGROW, 5);

  // Main information tab - 1nd column

  wxBoxSizer *szFirstColumn = new wxBoxSizer(wxVERTICAL);
  szMainInfoColumns->Add(szFirstColumn, 1, wxALL|wxEXPAND|wxGROW, 0);


  wxFlexGridSizer* szgMainFields = new wxFlexGridSizer(0, 2, 0, 0);
  szFirstColumn->Add(szgMainFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Data de aquisição

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Data de aquisição: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxGROW, 1);

  teditAcquisitionDate = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(70, -1));
  szRow->Add(teditAcquisitionDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarAcquisitionDate = new wxBitmapButton(panelMainInfo,
                                                  DLG_ESTATE_EDIT_BTN_ACQUISITION_DATE_ID,
                                                  wxBitmap(calendar_bmp_btn_xpm),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
  szRow->Add(btnCalendarAcquisitionDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("          "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Active or not

  chkProductActive = new wxCheckBox(panelMainInfo, wxID_ANY, wxT("Ativo"));
  chkProductActive->SetValue(true);
  szRow->Add(chkProductActive, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Product description

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Descrição: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 1);

  teditProductDescription = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(1, 10), wxTE_MULTILINE);
  szgMainFields->Add(teditProductDescription, 1, wxALL|wxEXPAND|wxGROW|wxALIGN_CENTER_VERTICAL, 1);

  // Product type

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Tipo: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0);

  cboxProductType = new wxComboBox(panelMainInfo, DLG_ESTATE_EDIT_CHOICE_PRODUCT_TYPE_ID, wxEmptyString);
  szRow->Add(cboxProductType, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Product manufacturer

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Fabricante: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxProductManufacturer = new wxComboBox(panelMainInfo, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxProductManufacturer, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Product trademark

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Marca: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxProductTrademark = new wxComboBox(panelMainInfo, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxProductTrademark, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Product manufacturer

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Fornecedor: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxProductSupplier = new wxComboBox(panelMainInfo, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxProductSupplier, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Product measure unit

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Unidade de medida: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  cboxProductMeasureUnit = new wxComboBox(panelMainInfo, wxID_ANY, wxEmptyString);
  szgMainFields->Add(cboxProductMeasureUnit, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Tempo de garantia

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Tempo de garantia: "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductTempoGarantia = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1), wxTE_RIGHT);
  szRow->Add(teditProductTempoGarantia, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditProductTempoGarantia->SetValidator(wxTextValidator(wxFILTER_DIGITS));

  cboxProductTempoGarantiaType = new wxChoice(panelMainInfo, wxID_ANY);
  szRow->Add(cboxProductTempoGarantiaType, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxProductTempoGarantiaType->Append(wxT("dias"), (void *) 'D');
  cboxProductTempoGarantiaType->Append(wxT("meses"), (void *) 'M');
  cboxProductTempoGarantiaType->Append(wxT("anos"), (void *) 'Y');


  szgMainFields->AddGrowableCol(1);
  szgMainFields->AddGrowableRow(1);

  // Seguro

  wxStaticBox* staticBoxSeguro = new wxStaticBox(panelMainInfo, wxID_ANY, wxT(" Seguro "));
  wxStaticBoxSizer* szBoxSeguro = new wxStaticBoxSizer(staticBoxSeguro, wxVERTICAL);
  szFirstColumn->Add(szBoxSeguro, 0, wxALL|wxEXPAND|wxGROW, 0);

  wxFlexGridSizer* szgSeguroFields = new wxFlexGridSizer(0, 2, 1, 1);
  szBoxSeguro->Add(szgSeguroFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Companhia de seguros

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Companhia: "));
  szgSeguroFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgSeguroFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxGROW, 0);

  teditSeguroCompanhia = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1));
  szRow->Add(teditSeguroCompanhia, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Apólice

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Apólice: "));
  szgSeguroFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgSeguroFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditSeguroApolice = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditSeguroApolice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Data de vencimento da apólice

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("     Vencimento: "));
  szRow->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  teditSeguroApoliceVencimentoDate = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditSeguroApoliceVencimentoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarApoliceVencimentoDate = new wxBitmapButton(panelMainInfo,
                                                        DLG_ESTATE_EDIT_BTN_EXPIRY_DATE_ID,
                                                        wxBitmap(calendar_bmp_btn_xpm),
                                                        wxDefaultPosition,
                                                        wxDefaultSize);
  szRow->Add(btnCalendarApoliceVencimentoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szgSeguroFields->AddGrowableCol(1);


  // Main information tab - 2nd column

  wxBoxSizer *sz2ndColumn = new wxBoxSizer(wxVERTICAL);
  szMainInfoColumns->Add(sz2ndColumn, 1, wxALL|wxEXPAND, 0);


  // Physical identification

  wxStaticBox* staticBoxPhysicalId = new wxStaticBox(panelMainInfo, wxID_ANY, wxT(" Identificação física "));
  wxStaticBoxSizer* szBoxPhysicalId = new wxStaticBoxSizer(staticBoxPhysicalId, wxVERTICAL);
  sz2ndColumn->Add(szBoxPhysicalId, 0, wxALL|wxEXPAND|wxGROW, 0);

  wxFlexGridSizer* szgPhysicalIdFields = new wxFlexGridSizer(0, 2, 1, 1);
  szBoxPhysicalId->Add(szgPhysicalIdFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Número de série

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Número de série: "));
  szgPhysicalIdFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgPhysicalIdFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  teditSerieNumber = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditSerieNumber, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Bar code

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Código de barras: "));
  szgPhysicalIdFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgPhysicalIdFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0);

  teditBarCode = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szRow->Add(teditBarCode, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Dimensions

  wxStaticBox* staticBoxDimensions = new wxStaticBox(panelMainInfo, wxID_ANY, wxT(" Dimensões "));
  wxStaticBoxSizer* szBoxDimensions = new wxStaticBoxSizer(staticBoxDimensions, wxVERTICAL);
  sz2ndColumn->Add(szBoxDimensions, 0, wxALL|wxEXPAND|wxGROW, 0);

  wxFlexGridSizer* szgDimensionsFields = new wxFlexGridSizer(0, 2, 1, 1);
  szBoxDimensions->Add(szgDimensionsFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Comprimento: "));
  szgDimensionsFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDimensionsFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductLength = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT(" metro(s)"));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Width

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Largura: "));
  szgDimensionsFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDimensionsFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductWidth = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT(" metro(s)"));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Height

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Altura: "));
  szgDimensionsFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDimensionsFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductHeight = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT(" metro(s)"));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Weight

  wxStaticBox* staticBoxWeight = new wxStaticBox(panelMainInfo, wxID_ANY, wxT(" Peso "));
  wxStaticBoxSizer* szBoxWeight = new wxStaticBoxSizer(staticBoxWeight, wxVERTICAL);
  sz2ndColumn->Add(szBoxWeight, 0, wxALL|wxEXPAND|wxGROW, 0);

  wxFlexGridSizer* szgWeightFields = new wxFlexGridSizer(0, 2, 1, 1);
  szBoxWeight->Add(szgWeightFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Liquid weight

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Peso Líquido: "));
  szgWeightFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgWeightFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductWeigthLiquid = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductWeigthLiquid, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT(" grama(s)"));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Brute weight

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Peso Bruto: "));
  szgWeightFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgWeightFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductWeigthBrute = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductWeigthBrute, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT(" grama(s)"));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szgWeightFields->AddGrowableCol(1);
  szgWeightFields->AddGrowableRow(1);


  // Accounting

  wxStaticBox* staticBoxDepreciacao = new wxStaticBox(panelMainInfo, wxID_ANY, wxT(" Contabilidade "));
  wxStaticBoxSizer* szBoxDepreciacao = new wxStaticBoxSizer(staticBoxDepreciacao, wxVERTICAL);
  sz2ndColumn->Add(szBoxDepreciacao, 0, wxALL|wxEXPAND|wxGROW, 0);

  wxFlexGridSizer* szgDepreciacaoFields = new wxFlexGridSizer(0, 2, 1, 1);
  szBoxDepreciacao->Add(szgDepreciacaoFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Valor original do bem

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Valor original: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductValue = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Valor de mercado

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Valor de mercado: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductValueMarket = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductValueMarket, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Taxa de depreciação

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Taxa de depreciação: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditDepreciacaoTaxa = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditDepreciacaoTaxa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT(" %"));
  label->SetForegroundColour(*wxBLUE);
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Date

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Início da depreciação: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxGROW, 1);

  teditDepreciacaoDate = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(70, -1));
  szRow->Add(teditDepreciacaoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnCalendarDepreciacaoDate = new wxBitmapButton(panelMainInfo,
                                                  DLG_ESTATE_EDIT_BTN_DEPRECIATION_BEGIN_DATE_ID,
                                                  wxBitmap(calendar_bmp_btn_xpm),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
  szRow->Add(btnCalendarDepreciacaoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Acelerated depreciation

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Depreciação acelerada: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0);

  wxArrayString depreciacaoAceleradaValoresPreDefinidos;
  
  depreciacaoAceleradaValoresPreDefinidos.Add(wxT(""));
  depreciacaoAceleradaValoresPreDefinidos.Add(wxT("1,5 (2 turnos diários de operação, 16 h)"));
  depreciacaoAceleradaValoresPreDefinidos.Add(wxT("2 (3 turnos diários de operação, 24 h)"));

  choiceDepreciacaoAcelerada = new wxChoice(panelMainInfo, wxID_ANY, wxDefaultPosition, wxSize(100, -1), depreciacaoAceleradaValoresPreDefinidos);
  szRow->Add(choiceDepreciacaoAcelerada, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);

  // Taxa de depreciação

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Depreciação acumulada: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditDepreciacaoAcumulada = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditDepreciacaoAcumulada, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Valor líquido do bem

  label = new wxStaticText(panelMainInfo, wxID_STATIC, wxT("Valor contábil: "));
  szgDepreciacaoFields->Add(label, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgDepreciacaoFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditProductLiquidValue = new wxTextCtrl(panelMainInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), wxTE_RIGHT);
  szRow->Add(teditProductLiquidValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  szgDepreciacaoFields->AddGrowableCol(1);


  // Categoria

  wxStaticBox* staticBoxCategory = new wxStaticBox(panelMainInfo, wxID_ANY, wxT(" Categoria "));
  wxStaticBoxSizer* szBoxCategory = new wxStaticBoxSizer(staticBoxCategory, wxHORIZONTAL);
  szMainInfoMargin->Add(szBoxCategory, 0, (wxALL & ~wxTOP)|wxEXPAND|wxGROW, 5);

  // Category

  wxBitmapButton *btnProductCategoryChoose = new wxBitmapButton(panelMainInfo, DLG_ESTATE_EDIT_BTN_CHOOSE_CATEGORY_ID, wxBitmap(folder_bmp_btn_xpm));
  btnProductCategoryChoose->SetToolTip(wxT("Selecionar a categoria do produto."));
  szBoxCategory->Add(btnProductCategoryChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 1);

  teditCategoryId = new wxTextCtrl(panelMainInfo, wxID_STATIC, wxT(""), wxDefaultPosition, wxSize(55, -1), wxTE_READONLY);
  szBoxCategory->Add(teditCategoryId, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditCategoryName = new wxTextCtrl(panelMainInfo, wxID_STATIC, wxT("Não definido."), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
  teditCategoryName->SetForegroundColour(*wxBLUE);
  szBoxCategory->Add(teditCategoryName, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 1);


  // Picture information

  wxPanel *panelPictureInfo = new wxPanel(nbookProductInfo, wxID_ANY);

  nbookProductInfo->AddPage(panelPictureInfo, wxT("Imagem ilustrativa"), false, wxBitmap(image_16x16_xpm));


  wxBoxSizer* szPanelPicture = new wxBoxSizer(wxVERTICAL);
  panelPictureInfo->SetSizer(szPanelPicture);


  // Picture information

  wxStaticBox *staticBoxProductImageInfo = new wxStaticBox(panelPictureInfo, wxID_ANY, wxT("Informações sobre a imagem"));
  wxStaticBoxSizer *szProductImageInfo = new wxStaticBoxSizer(staticBoxProductImageInfo, wxVERTICAL);
  szPanelPicture->Add(szProductImageInfo, 0, (wxALL & ~wxTOP)|wxEXPAND, 5);

  wxBoxSizer *szPictureFileChoose = new wxBoxSizer(wxHORIZONTAL);
  szProductImageInfo->Add(szPictureFileChoose, 0, wxALL|wxEXPAND, 0);

  // Controls for choosing the picture file

  label = new wxStaticText(panelPictureInfo, wxID_STATIC, wxT("Arquivo: "));
  szPictureFileChoose->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditProductPictureFilePath = new wxTextCtrl(panelPictureInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(10, -1));
  szPictureFileChoose->Add(teditProductPictureFilePath, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditProductPictureFilePath->Disable();

  wxBitmapButton *btnProductPictureFilePathChoose = new wxBitmapButton(panelPictureInfo, DLG_ESTATE_EDIT_BTN_CHOOSE_IMAGE_FILE_ID, wxBitmap(folder_bmp_btn_xpm));
  btnProductPictureFilePathChoose->SetToolTip(wxT("Selecionar arquivo de imagem ilustrativa para o produto."));
  szPictureFileChoose->Add(btnProductPictureFilePathChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Picture file information

  wxFlexGridSizer *szPictureInfoFields = new wxFlexGridSizer(4, 4, 2);
  szProductImageInfo->Add(szPictureInfoFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // File size

  label = new wxStaticText(panelPictureInfo, wxID_STATIC, wxT("Tipo (MIME): "));
  szPictureInfoFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 0);

  wxTextCtrl *teditProductPictureType = new wxTextCtrl(panelPictureInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(5, -1));
  szPictureInfoFields->Add(teditProductPictureType, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0);

  teditProductPictureType->Disable();

  // File size

  label = new wxStaticText(panelPictureInfo, wxID_STATIC, wxT("     Tamanho: "));
  szPictureInfoFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 0);

  wxTextCtrl *teditProductPictureSize = new wxTextCtrl(panelPictureInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(5, -1));
  szPictureInfoFields->Add(teditProductPictureSize, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0);

  teditProductPictureSize->Disable();


  szPictureInfoFields->AddGrowableCol(1);
  szPictureInfoFields->AddGrowableCol(3);


  // Image picture

  wxStaticBox *staticBoxProductImage = new wxStaticBox(panelPictureInfo, wxID_ANY, wxT("Imagem"));
  wxStaticBoxSizer *szProductImage = new wxStaticBoxSizer(staticBoxProductImage, wxHORIZONTAL);
  szPanelPicture->Add(szProductImage, 1, (wxALL & ~wxTOP)|wxEXPAND, 5);


  // Static bitmap

  staticBmpProductPicture = new wxStaticBitmap(panelPictureInfo, wxID_STATIC, bmpProductPictureEmpty);
  szProductImage->Add(staticBmpProductPicture, 1, wxALL|wxEXPAND|wxGROW, 0);


  // Technical information tab

  wxScrolledWindow *panelTechnicalInfo = new wxScrolledWindow(nbookProductInfo, wxID_ANY);

  nbookProductInfo->AddPage(panelTechnicalInfo, wxT("Informações técnicas"), false, wxBitmap(technical_info_16x16_xpm));

  panelTechnicalInfo->SetScrollbars(1, 1, 20, 20);


  wxBoxSizer* szgTechnicalFields = new wxBoxSizer(wxHORIZONTAL);
  panelTechnicalInfo->SetSizer(szgTechnicalFields);

  teditTechnicalInfo = new wxTextCtrl(panelTechnicalInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
  szgTechnicalFields->Add(teditTechnicalInfo, 1, wxALL|wxEXPAND|wxGROW|wxALIGN_CENTER_VERTICAL, 5);


  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

  szTop->Add(statusBar, 0, wxALL|wxGROW|wxEXPAND, 0);


  szTop->Layout();


  // Top sizer ending configuration.

  //szTop->SetSizeHints(this);

  LoadGroupsListing();

  LoadManufacturersListing();

  LoadTrademarksListing();

  LoadMeasureListing();

  //LoadSuppliersListing();

  panelMainInfo->Layout();
  szMainInfoMargin->Layout();
  nbookProductInfo->Layout();
  Layout();
  GetSizer()->Layout();
  wxGetApp().GetTopWindow()->Layout();

  teditName->SetFocus();
}


void PanelEstateEdit::Terminate()
{
  Close();

  statusBar->Destroy();

  Destroy();
}


int PanelEstateEdit::LoadData(int goodId)
{
  DBQuery dbquery;

  std::vector<wxString> params;

  params.push_back(wxString::Format(wxT("%d"), goodId));

  wxString sql = "";

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT nome, series_nr, barcode, rfid_serial, net_weight, gross_weight, measure_id, tipo_id, subtipo_id, categoria_id, comprimento, largura, altura, fisico, marca_id, fabricante_id, to_char(acquisition_date, 'DD/MM/YYYY') AS acquisition_date, warranty_months, value_original, value_market, depreciation_rate, to_char(depreciation_start_date, 'DD/MM/YYYY') AS depreciation_start_date, book_value, descricao, status, depreciation_accumulated, codigo, insurance_company, insurance_policy, to_char(insurance_due_dt, 'DD/MM/YYYY') AS insurance_due_dt ")
          wxT("FROM patrimonio.bem b ")
          wxT("WHERE b.estate_item_id = $1");
  }
  else {
    sql = wxT("SELECT nome, series_nr, barcode, rfid_serial, net_weight, gross_weight, measure_id, tipo_id, subtipo_id, categoria_id, comprimento, largura, altura, fisico, marca_id, fabricante_id, to_char(acquisition_date, 'DD/MM/YYYY') AS acquisition_date, warranty_months, value_original, value_market, depreciation_rate, to_char(depreciation_start_date, 'DD/MM/YYYY') AS depreciation_start_date, book_value, descricao, status, depreciation_accumulated, codigo, insurance_company, insurance_policy, insurance_due_dt ")
          wxT("FROM patrimonio.bem b ")
          wxT("WHERE b.estate_item_id = $1");
  }

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre o imposto selecionado.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }

  if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("Produto não encontrado."));

    return -1;
  }

  this->goodId = goodId;

  // Name

  teditName->SetValue(dbquery.GetFieldStrByName(0, wxT("nome")));

  // Code

  teditCode->SetValue(dbquery.GetFieldStrByName(0, wxT("codigo")));

  // Series number

  teditSerieNumber->SetValue(dbquery.GetFieldStrByName(0, wxT("series_nr")));

  // Bar code

  teditBarCode->SetValue(dbquery.GetFieldStrByName(0, wxT("barcode")));

  // Acquisition date

  teditAcquisitionDate->SetValue(dbquery.GetFieldStrByName(0, wxT("acquisition_date")));

  // Length

  teditProductLength->SetValue(dbquery.GetFieldStrByName(0, wxT("comprimento")));

  // Width

  teditProductWidth->SetValue(dbquery.GetFieldStrByName(0, wxT("largura")));

  // Height

  teditProductHeight->SetValue(dbquery.GetFieldStrByName(0, wxT("altura")));

  // Brute weight

  teditProductWeigthBrute->SetValue(dbquery.GetFieldStrByName(0, wxT("gross_weight")));

  // Liquid weight

  teditProductWeigthLiquid->SetValue(dbquery.GetFieldStrByName(0, wxT("net_weight")));

  // Original value

  teditProductValue->SetValue(dbquery.GetFieldStrByName(0, wxT("value_original")));

  // Market value

  teditProductValueMarket->SetValue(dbquery.GetFieldStrByName(0, wxT("value_market")));

  // Deprecation percentage

  teditDepreciacaoTaxa->SetValue(dbquery.GetFieldStrByName(0, wxT("depreciation_rate")));

  // Deprecation begin date

  teditDepreciacaoDate->SetValue(dbquery.GetFieldStrByName(0, wxT("depreciation_start_date")));

  // Accounting value

  teditProductLiquidValue->SetValue(dbquery.GetFieldStrByName(0, wxT("book_value")));

  // Acumulated depreciation

  teditDepreciacaoAcumulada->SetValue(dbquery.GetFieldStrByName(0, wxT("depreciation_accumulated")));

  // Description

  teditProductDescription->SetValue(dbquery.GetFieldStrByName(0, wxT("descricao")));

  // Insurance company

  teditSeguroCompanhia->SetValue(dbquery.GetFieldStrByName(0, wxT("insurance_company")));

  // Insurance policy

  teditSeguroApolice->SetValue(dbquery.GetFieldStrByName(0, wxT("insurance_policy")));

  // Insurance expiry date

  teditSeguroApoliceVencimentoDate->SetValue(dbquery.GetFieldStrByName(0, wxT("insurance_due_dt")));

  // Status

  if (dbquery.GetFieldStrByName(0, wxT("status")).Cmp(wxT("A")) == 0) {
    chkProductActive->SetValue(true);
  }
  else {
    chkProductActive->SetValue(false);
  }

  /*
  // Type

  for (size_t typeIndex = 0; typeIndex < cboxProductType->GetCount(); typeIndex++) {
    if (dbquery.GetFieldIntByName(0, wxT("tipo_id")) == (int) cboxProductType->GetClientData(typeIndex)) {
      cboxProductType->SetSelection(typeIndex);
      break;
    }
  }

  // Subtype

  LoadSubTypesListing();

  for (size_t subTypeIndex = 0; subTypeIndex < cboxProductSubType->GetCount(); subTypeIndex++) {
    if (dbquery.GetFieldIntByName(0, wxT("subtipo_id")) == (int) cboxProductSubType->GetClientData(subTypeIndex)) {
      cboxProductSubType->SetSelection(subTypeIndex);
      break;
    }
  }

  // Manufacturer

  for (size_t manufacturerIndex = 0; manufacturerIndex < cboxProductManufacturer->GetCount(); manufacturerIndex++) {
    if (dbquery.GetFieldIntByName(0, wxT("fabricante_id")) == (int) cboxProductManufacturer->GetClientData(manufacturerIndex)) {
      cboxProductManufacturer->SetSelection(manufacturerIndex);
      break;
    }
  }


  // Trademark

  for (size_t trademarkIndex = 0; trademarkIndex < cboxProductTrademark->GetCount(); trademarkIndex++) {
    if (dbquery.GetFieldIntByName(0, wxT("marca_id")) == (int) cboxProductTrademark->GetClientData(trademarkIndex)) {
      cboxProductTrademark->SetSelection(trademarkIndex);
      break;
    }
  }
  */

  // Category

  if (dbquery.GetFieldLongByName(0, wxT("categoria_id")) > 0) {
    teditCategoryId->SetValue(dbquery.GetFieldStrByName(0, wxT("categoria_id")));

    teditCategoryName->SetValue(ProductCategoryUtils::GetCompoundName(dbquery.GetFieldLongByName(0, wxT("categoria_id"))));
  }

  // Measure unit

  for (size_t index = 0; index < cboxProductMeasureUnit->GetCount(); index++) {
    if (cboxProductMeasureUnit->GetClientObject(index)) {
      ContainerIntData *container = (ContainerIntData *) cboxProductMeasureUnit->GetClientObject(index);

      if (dbquery.GetFieldIntByName(0, wxT("measure_id")) == container->GetData()) {
        cboxProductMeasureUnit->SetSelection(index);

        break;
      }
    }
  }

  // Technical information

  sql = wxT("SELECT conteudo ")
        wxT("FROM patrimonio.bem_informacao_tecnica ")
        wxT("WHERE estate_item_id = ") + wxString::Format(wxT("%d"), this->goodId);

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Erro na consulta das informações técnicas sobre o bem.\n\n")
                 wxT("Erro interno do banco:\n") + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    teditTechnicalInfo->SetValue(dbquery.GetFieldStrByName(0, wxT("conteudo")));
  }


#if 0
  // Product picture

  sql = wxT("SELECT * ")
        wxT("FROM patrimonio.bem_imagem ")
        wxT("WHERE estate_item_id = ") + wxString::Format(wxT("%d"), this->goodId);

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Erro na consulta da imagem do bem.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    size_t fieldValueLength;

    unsigned char *fileContent = PQunescapeBytea((const unsigned char *) dbquery.GetFieldAnsiStrByName(0, wxT("file_content")), &fieldValueLength);

    /*
    wxBitmapType bitmapType = wxBITMAP_TYPE_PNG;

    wxRegEx regex;

    if (regex.Compile(wxT("/png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
        regex.Matches(dbquery.GetFieldStrByName(0, wxT("file_mime")))) {
      bitmapType = wxBITMAP_TYPE_PNG;
    }
    else if (regex.Compile(wxT("(/jpg|/jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
             regex.Matches(dbquery.GetFieldStrByName(0, wxT("file_mime")))) {
      bitmapType = wxBITMAP_TYPE_JPEG;
    }
    else if (regex.Compile(wxT("/gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
             regex.Matches(dbquery.GetFieldStrByName(0, wxT("file_mime")))) {
      bitmapType = wxBITMAP_TYPE_GIF;
    }
    else if (regex.Compile(wxT("/bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
             regex.Matches(dbquery.GetFieldStrByName(0, wxT("file_mime")))) {
      bitmapType = wxBITMAP_TYPE_BMP;
    }
    */

    // Load picture stream in a memory stream

    wxMemoryInputStream stream(fileContent, fieldValueLength);

    // Load memory stream in a image object

    imageProductPicture.LoadFile(stream);

    UtilsImage::Rescale(imageProductPicture, staticBmpProductPicture->GetSize().GetWidth(), staticBmpProductPicture->GetSize().GetHeight());

    // Show picture in a bitmap object, if it's valid

    if (imageProductPicture.IsOk() && imageProductPicture.GetWidth() > 0 && imageProductPicture.GetHeight() > 0) {
      wxBitmap bmpProductPicture(imageProductPicture);

      staticBmpProductPicture->SetBitmap(bmpProductPicture);

      staticBmpProductPicture->Layout();

      staticBmpProductPicture->GetContainingSizer()->Layout();

      wxGetApp().GetTopWindow()->Layout();

      staticBmpProductPicture->Refresh();
      staticBmpProductPicture->Update();

      Refresh();
      Update();
    }
  }

#endif


  return 0;
}


int PanelEstateEdit::LoadGroupsListing()
{
  cboxProductType->Freeze();

  cboxProductType->Clear();

  cboxProductType->Append(wxT(""), (void *) NULL);

  cboxProductType->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT tipo_id, nome FROM produto.tipo ORDER BY LOWER(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de tipos de produtos.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("tipo_id")));

      cboxProductType->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxProductType->GetContainingSizer()->Layout();

  cboxProductType->Thaw();

  return 0;
}


int PanelEstateEdit::LoadManufacturersListing()
{
  cboxProductManufacturer->Freeze();

  cboxProductManufacturer->Clear();

  cboxProductManufacturer->Append(wxT(""), (void *) NULL);

  cboxProductManufacturer->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT fabricante_id, nome FROM produto.fabricante ORDER BY LOWER(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de fabricantes de produtos.\n\n")) + 
                 wxString(wxT("Erro interno do banco:\n")) + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("fabricante_id")));

      cboxProductManufacturer->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxProductManufacturer->GetContainingSizer()->Layout();

  cboxProductManufacturer->Thaw();

  return 0;
}


int PanelEstateEdit::LoadTrademarksListing()
{
  cboxProductTrademark->Freeze();

  cboxProductTrademark->Clear();

  cboxProductTrademark->Append(wxT(""), (void *) NULL);

  cboxProductTrademark->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT * FROM produto.marca ORDER BY LOWER(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de marcas de produtos.")) + 
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("marca_id")));

      cboxProductTrademark->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxProductTrademark->GetContainingSizer()->Layout();

  cboxProductTrademark->Thaw();

  return 0;
}


int PanelEstateEdit::LoadMeasureListing()
{
  cboxProductMeasureUnit->Freeze();

  cboxProductMeasureUnit->Clear();

  cboxProductMeasureUnit->Append(wxT(""), (void *) NULL);

  cboxProductMeasureUnit->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT * FROM medida.medida ORDER BY LOWER(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de medidas.")) + 
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("measure_id")));

      cboxProductMeasureUnit->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxProductMeasureUnit->GetContainingSizer()->Layout();

  cboxProductMeasureUnit->Thaw();

  return 0;
}


int PanelEstateEdit::LoadSuppliersListing()
{
  cboxProductSupplier->Freeze();

  cboxProductSupplier->Clear();

  cboxProductSupplier->Append(wxT(""), (void *) NULL);

  cboxProductSupplier->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT * FROM fornecedor.fornecedor ORDER BY lower(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de fornecedores.\n\n")) + 
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("fornecedor_id")));

      cboxProductSupplier->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxProductSupplier->Thaw();

  return 0;
}


int PanelEstateEdit::LoadEnterprisesListing()
{
  cboxProductEnterprise->Freeze();

  cboxProductEnterprise->Clear();

  cboxProductEnterprise->Append(wxT(""), (void *) NULL);

  cboxProductEnterprise->SetSelection(0);

  DBQuery dbquery;

  int rc = dbquery.Execute(wxT("SELECT * FROM empresa.empresa ORDER BY lower(nome)"));

  if (rc != 0) {
    wxMessageBox(wxString(wxT("Não foi possível obter a listagem de empresas.\n\n")) + 
                 wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());
  }
  else {
    for (int index = 0; index < dbquery.GetRowsCount(); index++) {
      ContainerIntData *container = new ContainerIntData(dbquery.GetFieldIntByName(index, wxT("enterprise_id")));

      cboxProductEnterprise->Append(dbquery.GetFieldStrByName(index, wxT("nome")), container);
    }
  }

  cboxProductEnterprise->Thaw();

  return 0;
}


void PanelEstateEdit::OnSaveRequest(wxCommandEvent &event)
{
  if (teditName->GetValue().Length() < 1) {
    statusBar->SetStatusText(wxT("É necessário informar o nome do produto."));
    return;
  }


  std::vector<wxString> params, fields;
  bool registeringError = false;
  wxString messageStr = wxT("");
  wxString sql = wxT("");

  // Product name

  fields.push_back(wxT("nome"));
  params.push_back(teditName->GetValue());

  // Status

  fields.push_back(wxT("status"));

  if (chkProductActive->GetValue()) {
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

  // Product code

  fields.push_back(wxT("codigo"));

  if (teditCode->GetValue().Length() > 0) {
    params.push_back(teditCode->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Series number

  fields.push_back(wxT("series_nr"));

  if (teditSerieNumber->GetValue().Length() > 0) {
    params.push_back(teditSerieNumber->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Bar code

  fields.push_back(wxT("barcode"));

  if (teditBarCode->GetValue().Length() > 0) {
    params.push_back(teditBarCode->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Description

  fields.push_back(wxT("descricao"));

  if (teditProductDescription->GetValue().Length() > 0) {
    params.push_back(teditProductDescription->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  /*
  // Type

  if (cboxProductType->GetSelection() >= 0 && cboxProductType->GetClientData(cboxProductType->GetSelection()) != NULL) {
    fields.push_back(wxT("tipo_id"));
    params.push_back(wxString::Format(wxT("%d"), (int) cboxProductType->GetClientData(cboxProductType->GetSelection())));
  }

  // Subtype

  if (cboxProductSubType->GetSelection() >= 0 && cboxProductSubType->GetClientData(cboxProductSubType->GetSelection()) != NULL) {
    fields.push_back(wxT("subtipo_id"));
    params.push_back(wxString::Format(wxT("%d"), (int) cboxProductSubType->GetClientData(cboxProductSubType->GetSelection())));
  }
  */

  // Manufacturer

  fields.push_back(wxT("fabricante_id"));

  if (cboxProductManufacturer->GetSelection() >= 0 && cboxProductManufacturer->GetClientObject(cboxProductManufacturer->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) cboxProductManufacturer->GetClientObject(cboxProductManufacturer->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Trademark

  fields.push_back(wxT("marca_id"));

  if (cboxProductTrademark->GetSelection() >= 0 && cboxProductTrademark->GetClientObject(cboxProductTrademark->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) cboxProductTrademark->GetClientObject(cboxProductTrademark->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), (int) container->GetData()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Category

  fields.push_back(wxT("categoria_id"));

  if (teditCategoryId->GetValue().Length() > 0) {
    params.push_back(teditCategoryId->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Measure unit

  fields.push_back(wxT("measure_id"));

  if (cboxProductMeasureUnit->GetSelection() >= 0 && cboxProductMeasureUnit->GetClientObject(cboxProductMeasureUnit->GetSelection()) != NULL) {
    ContainerIntData *container = (ContainerIntData *) cboxProductMeasureUnit->GetClientObject(cboxProductMeasureUnit->GetSelection());

    params.push_back(wxString::Format(wxT("%d"), container->GetData()));
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Length

  fields.push_back(wxT("comprimento"));

  if (teditProductLength->GetValue().Length() > 0) {
    params.push_back(teditProductLength->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Width

  fields.push_back(wxT("largura"));

  if (teditProductWidth->GetValue().Length() > 0) {
    params.push_back(teditProductWidth->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Height

  fields.push_back(wxT("altura"));

  if (teditProductHeight->GetValue().Length() > 0) {
    params.push_back(teditProductHeight->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Liquid weight

  fields.push_back(wxT("net_weight"));

  if (teditProductWeigthLiquid->GetValue().Length() > 0) {
    params.push_back(teditProductWeigthLiquid->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Brute weight

  fields.push_back(wxT("gross_weight"));

  if (teditProductWeigthBrute->GetValue().Length() > 0) {
    params.push_back(teditProductWeigthBrute->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Original value

  fields.push_back(wxT("value_original"));

  if (teditProductValue->GetValue().Length() > 0) {
    params.push_back(teditProductValue->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Market value

  fields.push_back(wxT("value_market"));

  if (teditProductValueMarket->GetValue().Length() > 0) {
    params.push_back(teditProductValueMarket->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Depreciation percentage

  fields.push_back(wxT("depreciation_rate"));

  if (teditDepreciacaoTaxa->GetValue().Length() > 0) {
    params.push_back(teditDepreciacaoTaxa->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Depreciation begin date

  fields.push_back(wxT("depreciation_start_date"));

  if (teditDepreciacaoDate->GetValue().Length() > 0) {
    params.push_back(teditDepreciacaoDate->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Accounting value

  fields.push_back(wxT("book_value"));

  if (teditProductLiquidValue->GetValue().Length() > 0) {
    params.push_back(teditProductLiquidValue->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Insurance

  fields.push_back(wxT("insurance_company"));

  if (teditSeguroCompanhia->GetValue().Length() > 0) {
    params.push_back(teditSeguroCompanhia->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Insurance policy

  fields.push_back(wxT("insurance_policy"));

  if (teditSeguroApolice->GetValue().Length() > 0) {
    params.push_back(teditSeguroApolice->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }

  // Insurance expiry date

  fields.push_back(wxT("insurance_due_dt"));

  if (teditSeguroApoliceVencimentoDate->GetValue().Length() > 0) {
    params.push_back(teditSeguroApoliceVencimentoDate->GetValue());
  }
  else {
    params.push_back(wxEmptyString);
  }


  DBQuery dbquery;


  // Init transaction

  sql = DBUtils::MountSQLUpdate(wxT("patrimonio.bem"),
                                fields,
                                wxString::Format(wxT("estate_item_id = %d"), this->goodId));

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messageStr = wxT("Erro interno do banco:\n") + dbquery.GetMessageStr();
  }


  // Technical information

  if (!registeringError && teditTechnicalInfo->GetValue().Length() > 0) {
    bool hasNotes = false;

    // Checking notes existence

    rc = dbquery.Execute(wxString::Format(wxT("SELECT COUNT(*) FROM patrimonio.bem_informacao_tecnica WHERE estate_item_id = %d"), goodId));

    if (rc != 0) {
      hasNotes = false;
    }
    else {
      if (dbquery.GetFieldLong(0, 0) > 0) {
        hasNotes = true;
      }
    }


    fields.clear();
    params.clear();

    wxString insertValuesStr = wxT("");

    // Technical information content

    fields.push_back(wxT("conteudo"));
    params.push_back(teditTechnicalInfo->GetValue());

    if (hasNotes) {
      sql = DBUtils::MountSQLUpdate(wxT("patrimonio.bem_informacao_tecnica"),
                                    fields,
                                    wxString::Format(wxT("estate_item_id = %d"), this->goodId));
    }
    else {
      fields.push_back(wxT("estate_item_id"));
      params.push_back(wxString::Format(wxT("%d"), this->goodId));

      sql = DBUtils::MountSQLInsert(wxT("patrimonio.bem_informacao_tecnica"), fields);
    }

    //wxMessageBox(sql);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      messageStr = (wxT("Erro na modificação das informações técnicas sobre o bem.\n\n")
                    wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

      registeringError = true;
    }
  }


  // Picture
#if 0
  if (!registeringError) {
    if (teditProductPictureFilePath->GetValue().Length() > 0) {

      bool hasPicture = false;

      // Checking picture existence

      rc = dbquery.Execute(wxString::Format(wxT("SELECT COUNT(*) FROM patrimonio.bem_imagem WHERE estate_item_id = %d"), goodId));

      if (rc != 0) {
        hasPicture = false;
      }
      else {
        if (dbquery.GetFieldLong(0, 0) > 0) {
          hasPicture = true;
        }
      }

      // File descriptor

      char fileName[strlen((const char *) teditProductPictureFilePath->GetValue().mb_str(wxConvUTF8)) + 1];

      strcpy(fileName, (const char *) teditProductPictureFilePath->GetValue().mb_str(wxConvUTF8));

      wxFFile file(teditProductPictureFilePath->GetValue(), wxT("rb"));

      long fileLength = file.Length();

      file.Close();

      // Opening picture file

      FILE *picturefd = fopen(fileName, "rb");

      if (!picturefd) {
        fprintf(stderr, "Não foi possível abrir o arquivo entrada.\n");

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
          regex.Matches(teditProductPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/png");
      }
      else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(teditProductPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/jpeg");
      }
      else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(teditProductPictureFilePath->GetValue())) {
        strcpy(imageMimeType, "image/gif");
      }
      else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(teditProductPictureFilePath->GetValue())) {
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

      if (hasPicture) {
        sprintf(sqlCommandAnsi, "UPDATE patrimonio.bem_imagem SET file_mime=$1, file_content=$2 WHERE estate_item_id=%d",
                                goodId);
      }
      else {
        sprintf(sqlCommandAnsi, "INSERT INTO patrimonio.bem_imagem (estate_item_id, file_mime, file_content) VALUES (%d, $1, $2)",
                                goodId);
      }

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
          wxMessageBox(wxT("Não foi possível cadastrar a imagem do bem.\n\nErro interno:\n") +
                       wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
        }
      }
      else {
        ExecStatusType status = PQresultStatus((PGresult *) res); 

        if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
          if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
            wxMessageBox(wxT("Não foi possível cadastrar a imagem do bem.\n\nErro interno:\n") +
                         wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
          }
        }
      }

    }
  }
#endif


  // Good registering error

  if (registeringError) {
    wxMessageBox(wxString(wxT("Não foi possível modificar as informações sobre o bem '")) +
                 teditName->GetValue() + wxString(wxT("'.\n\n")) + messageStr);
  }
  // Product is successfully registered
  else {
    statusBar->SetStatusText(wxT("As informações sobre o bem '") + teditName->GetValue() + wxT("' foram modificadas."));
  }
}


void PanelEstateEdit::OnProductTypeChoose(wxCommandEvent &event)
{
  //LoadSubTypesListing();
}


void PanelEstateEdit::OnCharHook(wxKeyEvent& event)
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


void PanelEstateEdit::OnChoosePictureFileRequest(wxCommandEvent &event)
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

    if (!imageProductPicture.LoadFile(fileDialog.GetPath())) {
      wxMessageBox(wxT("O arquivo '") + fileDialog.GetPath() + wxT("' não pôde ser carregado."));

      return;
    }

    staticBmpProductPicture->GetContainingSizer()->Layout();
    staticBmpProductPicture->Layout();
    wxGetApp().GetTopWindow()->Layout();

    UtilsImage::Rescale(imageProductPicture, staticBmpProductPicture->GetSize().GetWidth(), staticBmpProductPicture->GetSize().GetHeight());

    wxBitmap bmpProductPicture(imageProductPicture);


    teditProductPictureFilePath->SetValue(fileDialog.GetPath());

    staticBmpProductPicture->SetBitmap(bmpProductPicture);

    staticBmpProductPicture->Layout();

    staticBmpProductPicture->GetContainingSizer()->Layout();

    wxGetApp().GetTopWindow()->Layout();

    staticBmpProductPicture->Refresh();
    staticBmpProductPicture->Update();

    Refresh();
    Update();
  }
}


/*
int PanelEstateEdit::LoadProductInfo(wxString goodId, wxString quantityStr)
{
  wxMessageBox(wxT("PanelEstateEdit::LoadProductInfo"));

  wxString sql = wxString::Format(wxT("SELECT produto.*, tipo.nome AS tipo_nome, venda.preco, moeda.sigla as preco_moeda ")
                                            wxT("FROM produto.produto ")
                                            wxT("LEFT JOIN produto.tipo ON (produto.tipo_id = tipo.tipo_id) ")
                                            wxT("LEFT JOIN produto.venda ON (produto.produto_id = venda.produto_id) ")
                                            wxT("LEFT JOIN monetario.moeda ON (venda.preco_moeda_id = moeda.currency_id) ")
                                            wxT("WHERE produto.produto_id = ") + goodId);

  DBQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre o produto pesquisado.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());

    return -1;
  }
  else if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("O produto pesquisado não foi encontrado."));

    return -1;
  }
  else {
    double quantityValue;

    quantityStr.ToDouble(&quantityValue);

    // Product descriptive name

    long itemIndex = listCtrlComposition->InsertItem(listCtrlComposition->GetItemCount(),
                                                     dbquery.GetFieldStrByName(0, wxT("produto_id")));

    listCtrlComposition->SetItemData(itemIndex, dbquery.GetFieldLongByName(0, wxT("produto_id")));

    // Product name

    listCtrlComposition->SetItem(itemIndex, 1, dbquery.GetFieldStrByName(0, wxT("nome")));

    // Product quantity

    listCtrlComposition->SetItem(itemIndex, 2, quantityStr);

    // Colour line

    GUI::ListSetRowBgColor(listCtrlComposition, itemIndex);
  }

  return 0;
}
*/


/*
void PanelEstateEdit::OnProductComponentSearchRequest(wxCommandEvent &event)
{
  ProductSearchDialog productSearchDialog(this);

  productSearchDialog.ShowModal();

  if (productSearchDialog.IsCanceled()) {
    return;
  }

  if (productSearchDialog.GetProductId() == -1) {
    return;
  }

  teditComponentAddProductCode->SetValue(wxString::Format(wxT("%ld"), productSearchDialog.GetProductId()));

  teditComponentAddProductName->SetValue(productSearchDialog.GetProductName());
}


void PanelEstateEdit::OnAddProductToCompositionRequest(wxCommandEvent &event)
{
  wxMessageBox(wxT("PanelEstateEdit::OnAddProductToCompositionRequest"));

  double quantity = 0.0;
 
  if (teditComponentAddProductQuantity->GetValue().Length() > 0) {
    teditComponentAddProductQuantity->GetValue().ToDouble(&quantity);
  }

  if (quantity == 0.0) {
    wxMessageBox(wxT("A quantidade do produto a ser adicionado deve ser maior que zero."));

    return;
  }

  LoadProductInfo(teditComponentAddProductCode->GetValue(), teditComponentAddProductQuantity->GetValue());
}
*/


void PanelEstateEdit::OnCategoryChooseRequest(wxCommandEvent &event)
{
  ProductCategoryChooseDialog productCategoryChooseDialog(this);

  productCategoryChooseDialog.ShowModal();

  if (productCategoryChooseDialog.IsCanceled()) {
    return;
  }

  teditCategoryId->SetValue(wxString::Format(wxT("%ld"), productCategoryChooseDialog.GetCategoryId()));

  teditCategoryName->SetValue(productCategoryChooseDialog.GetCategoryCompoundName());
}


void PanelEstateEdit::OnDeleteRequest(wxCommandEvent &event)
{
  wxMessageBox(wxT("PanelEstateEdit::OnDeleteRequest"));
}


void PanelEstateEdit::OnAcquisitionDateChooseRequest(wxCommandEvent &event)
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


void PanelEstateEdit::OnExpiryDateChooseRequest(wxCommandEvent &event)
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


void PanelEstateEdit::OnDepreciationBeginDateChooseRequest(wxCommandEvent &event)
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


BEGIN_EVENT_TABLE(PanelEstateEdit, wxScrolledWindow)
  EVT_TOOL(DLG_ESTATE_EDIT_TOOL_SAVE_ID, PanelEstateEdit::OnSaveRequest)
  EVT_TOOL(DLG_ESTATE_EDIT_TOOL_DELETE_ID, PanelEstateEdit::OnDeleteRequest)

  EVT_CHOICE(DLG_ESTATE_EDIT_CHOICE_PRODUCT_TYPE_ID, PanelEstateEdit::OnProductTypeChoose)

  EVT_KEY_UP(PanelEstateEdit::OnCharHook)

  EVT_BUTTON(DLG_ESTATE_EDIT_BTN_CHOOSE_IMAGE_FILE_ID, PanelEstateEdit::OnChoosePictureFileRequest)
  //EVT_BUTTON(DLG_ESTATE_EDIT_BTN_PRODUCT_COMPONENT_SELECT_ID, PanelEstateEdit::OnProductComponentSearchRequest)
  //EVT_BUTTON(DLG_ESTATE_EDIT_BTN_COMPOSITION_PRODUCT_ADD_ID, PanelEstateEdit::OnAddProductToCompositionRequest)
  EVT_BUTTON(DLG_ESTATE_EDIT_BTN_CHOOSE_CATEGORY_ID, PanelEstateEdit::OnCategoryChooseRequest)
  EVT_BUTTON(DLG_ESTATE_EDIT_BTN_ACQUISITION_DATE_ID, PanelEstateEdit::OnAcquisitionDateChooseRequest)
  EVT_BUTTON(DLG_ESTATE_EDIT_BTN_EXPIRY_DATE_ID, PanelEstateEdit::OnExpiryDateChooseRequest)
  EVT_BUTTON(DLG_ESTATE_EDIT_BTN_DEPRECIATION_BEGIN_DATE_ID, PanelEstateEdit::OnDepreciationBeginDateChooseRequest)
END_EVENT_TABLE()
