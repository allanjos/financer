/**
 * Person editing dialog.
 *
 * \version 1.0
 * \since 08/19/2007 10:50:12 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_image.hpp"
#include "utils_config.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "panel_person_main.hpp"
#include "panel_person_edit.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "dlg_calendar.hpp"
#include "pixmaps/search_16x13.xpm"
#include "pixmaps/calendar_16x16.xpm"
//#include "pixmaps/sale_16x16.xpm"
//#include "pixmaps/client_16x16.xpm"
//#include "pixmaps/purchase_16x16.xpm"
//#include "pixmaps/client_32x32.xpm"
#include "pixmaps/photo.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"
#include "pixmaps/image_16x16.xpm"


PanelPersonEdit::PanelPersonEdit(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  this->personId = -1;
  this->customerId = -1;
  this->enterpriseId = -1;

  this->photoFilePath = wxT("");

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

  toolBar->AddTool(PANEL_PERSON_EDIT_BTN_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(PANEL_PERSON_EDIT_BTN_DELETE_ID, Lang::Get("Delete"), wxBitmap(delete_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Two columns - Photo and information

  wxBoxSizer *szInfoColumns = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szInfoColumns, 1, wxEXPAND | wxALL | wxGROW, 0);


  // Client picture

  wxScrolledWindow *panelPicture = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(130, -1));
  szInfoColumns->Add(panelPicture, 0, wxRIGHT | wxEXPAND | wxGROW, 0);

  panelPicture->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer* szPicture = new wxBoxSizer(wxVERTICAL);
  panelPicture->SetSizer(szPicture);

  // Control to modify photo

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szPicture->Add(szRow, 0, wxALL, 1);

  wxBitmapButton *btnPictureChose = new wxBitmapButton(panelPicture,
                                                       PANEL_PERSON_EDIT_BTN_PHOTO_CHOOSE_ID,
                                                       wxBitmap(image_16x16_xpm));
  szRow->Add(btnPictureChose, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnPictureChose->SetToolTip(Lang::Get("Select a picture"));

  szPicture->Add(new wxStaticLine(panelPicture), 0, wxALL | wxEXPAND | wxGROW, 0);

  szPicture->AddSpacer(5);


  // Static bitmap

  wxBitmap bmpPicture(photo_xpm);

  bmpPicture.SetWidth(120);

  staticBmpPicture = new wxStaticBitmap(panelPicture, wxID_STATIC, bmpPicture);
  szPicture->Add(staticBmpPicture, 0, wxALL | wxALIGN_TOP, 0);

  szPicture->Layout();


  // Right information rows

  wxBoxSizer *szRightRows = new wxBoxSizer(wxVERTICAL);
  szInfoColumns->Add(szRightRows, 1, wxEXPAND | wxBOTTOM | wxGROW, 5);

  // Person Name

  szFullName = new wxBoxSizer(wxHORIZONTAL);
  szRightRows->Add(szFullName, 0, wxEXPAND | wxALL, 0);

  labelFullName = new wxStaticText(this, wxID_STATIC, wxT("Nome: "));
  wxFont font = labelFullName->GetFont();
  font.Bold();
  labelFullName->SetFont(font);
  szFullName->Add(labelFullName, 0, wxALIGN_CENTER_VERTICAL, 0);

  teditFullName = new wxTextCtrl(this, wxID_ANY, wxT(""));
  szFullName->Add(teditFullName, 1, wxEXPAND, 0);


  szRow = new wxBoxSizer(wxHORIZONTAL);
  szRightRows->Add(szRow, 0, wxTOP | wxBOTTOM | wxALIGN_RIGHT, 10);

  wxCheckBox *checkBoxCustomer = new wxCheckBox(this, wxID_ANY, Lang::Get("Customer"));
  szRow->Add(checkBoxCustomer, 0, wxALL | wxEXPAND, 0);

  wxCheckBox *checkBoxSupplier = new wxCheckBox(this, wxID_ANY, Lang::Get("Supplier"));
  szRow->Add(checkBoxSupplier, 0, wxLEFT | wxEXPAND, 20);

  wxCheckBox *checkBoxEmployee = new wxCheckBox(this, wxID_ANY, Lang::Get("Employee"));
  szRow->Add(checkBoxEmployee, 0, wxLEFT | wxEXPAND, 20);

  // Notebook

  //wxAuiNotebook *notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, (wxAUI_NB_TOP|wxNO_BORDER) & ~(wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_CLOSE_ON_ALL_TABS));
  wxNotebook *notebook = new wxNotebook(this, wxID_ANY);

  szRightRows->Add(notebook, 1, wxEXPAND | wxALL | wxGROW, 0);


  //notebook->SetArtProvider(new wxAuiSimpleTabArt);


  // Main info

  wxScrolledWindow *panelMain = new wxScrolledWindow(notebook, wxID_ANY);

  notebook->AddPage(panelMain, Lang::Get("General"), false);

  panelMain->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szMarginAddress = new wxBoxSizer(wxVERTICAL);
  panelMain->SetSizer(szMarginAddress);

  wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);
  szMarginAddress->Add(szMain, 0, wxALL|wxEXPAND|wxGROW, GUI::FORM_MARGIN);


  // Fields

  szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szMain->Add(szgMainFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Natureza da Pessoa/instituição fisica/jurídica

  wxStaticText *label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Person nature") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxGROW, 1);

  choicePersonNature = new wxChoice(panelMain, PANEL_PERSON_EDIT_CHOICE_PERSON_TYPE_ID);
  szRow->Add(choicePersonNature, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  choicePersonNature->Append(Lang::Get("Natural person"), (wxClientData *) new ContainerIntData((int) 'F'));
  choicePersonNature->Append(Lang::Get("Legal person"), (wxClientData *) new ContainerIntData((int) 'J'));

  choicePersonNature->SetSelection(0);

  szRow->AddSpacer(10);

  // CNPJ / CPF

  lblCpfCnpj = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Tax Corporation Registration") + wxT(": "));
  szRow->Add(lblCpfCnpj, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditCPF_CNPJ = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200, -1));
  szRow->Add(teditCPF_CNPJ, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditCPF_CNPJ->SetMaxLength(30);

  // Fantasy name

  labelFantasyName = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Trading name") + wxT(": "));
  szgMainFields->Add(labelFantasyName, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  teditFantasyName = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(450, -1));
  szgMainFields->Add(teditFantasyName, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 1);

  // Inscrição estadual

  labelInscricaoEstadual = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("State registration") + wxT(": "));
  szgMainFields->Add(labelInscricaoEstadual, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxGROW, 1);

  teditInscricaoEstadual = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200, -1));
  szRow->Add(teditInscricaoEstadual, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szgMainFields->AddGrowableCol(1);


  // RG

  wxBoxSizer* szRG = new wxBoxSizer(wxVERTICAL);
  szMain->Add(szRG, 0, wxALL | wxEXPAND | wxGROW, 0);

  label = new wxStaticText(panelMain, wxID_ANY, Lang::Get("Identity document"));
  szRG->Add(label, 0, wxTOP | wxBOTTOM, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  wxFlexGridSizer *szGridRG = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szRG->Add(szGridRG, 1, wxLEFT | wxBOTTOM | wxEXPAND | wxGROW, 0);

  // RG número

  labelRgNumber = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Number") + wxT(": "));
  szGridRG->Add(labelRgNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szGridRG->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 0);

  teditRgNumber = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120, -1));
  szRow->Add(teditRgNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // RG órgão

  labelRgOrgao = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Institution") + wxT(": "));
  szRow->Add(labelRgOrgao, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditRgOrgao = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200, -1));
  szRow->Add(teditRgOrgao, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow->AddSpacer(10);

  // RG data de expedição

  labelRgExpedicaoDate = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Expedition") + wxT(": "));
  szRow->Add(labelRgExpedicaoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditRGExpedicaoDate = new wxTextCtrl(panelMain, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditRGExpedicaoDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnRgDateChoose = new wxBitmapButton(panelMain,
                                       PANEL_PERSON_EDIT_TEDIT_RG_DATE_ID,
                                       wxBitmap(calendar_16x16_xpm));
  szRow->Add(btnRgDateChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnRgDateChoose->SetMinSize(wxSize(-1, teditRGExpedicaoDate->GetSize().GetHeight()));

  // Country

  labelRgCountry = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Country") + wxT(": "));
  szGridRG->Add(labelRgCountry, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditRgCountry = new wxTextCtrl(panelMain, PANEL_PERSON_EDIT_CHOICE_RG_COUNTRY_ID);
  szGridRG->Add(teditRgCountry, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 1);

  // Registered state

  labelRgFederalState = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("State") + wxT(": "));
  szGridRG->Add(labelRgFederalState, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szGridRG->Add(szRow, 1, wxALL | wxEXPAND, 0);

  teditRgUF = new wxTextCtrl(panelMain, PANEL_PERSON_EDIT_CHOICE_IDENT_STATE_ID);
  szRow->Add(teditRgUF, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Registered city

  labelRgCity = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("City") + wxT(": "));
  szGridRG->Add(labelRgCity, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szGridRG->Add(szRow, 1, wxALL | wxEXPAND, 0);

  teditRgCity = new wxTextCtrl(panelMain, PANEL_PERSON_EDIT_CHOICE_RG_CITY_ID);
  szRow->Add(teditRgCity, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  szGridRG->AddGrowableCol(1);


  // Born date

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Date of birth") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  teditBornDate = new wxTextCtrl(panelMain, PANEL_PERSON_EDIT_TEDIT_BORN_DATE_ID, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditBornDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnBornDateChoose = new wxBitmapButton(panelMain,
                                         PANEL_PERSON_EDIT_BUTTON_BORN_DATE_CHOOSE_ID,
                                         wxBitmap(calendar_16x16_xpm));
  szRow->Add(btnBornDateChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnBornDateChoose->SetMinSize(wxSize(-1, teditBornDate->GetSize().GetHeight()));

  szRow->AddSpacer(50);


  // Tipo

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Gender") + wxT(": "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxSex = new wxChoice(panelMain, wxID_ANY);
  szRow->Add(cboxSex, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxSex->Append(wxT("Não especificado"), (wxClientData *) new ContainerIntData((int) 0));
  cboxSex->Append(wxT("Masculino"), (wxClientData *) new ContainerIntData((int) 'M'));
  cboxSex->Append(wxT("Feminino"), (wxClientData *) new ContainerIntData((int) 'F'));

  cboxSex->SetSelection(0);


  // Contact information tab

  wxScrolledWindow *panelContact = new wxScrolledWindow(notebook, wxID_ANY);

  notebook->AddPage(panelContact, Lang::Get("Contact"), false);

  panelContact->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szMarginContact = new wxBoxSizer(wxVERTICAL);
  panelContact->SetSizer(szMarginContact);

  wxBoxSizer *szContact = new wxBoxSizer(wxVERTICAL);
  szMarginContact->Add(szContact, 1, wxALL | wxEXPAND | wxGROW, GUI::FORM_MARGIN);

  // Home phone

  wxBoxSizer* szContactHome = new wxBoxSizer(wxVERTICAL);
  szContact->Add(szContactHome, 0, wxALL|wxEXPAND|wxGROW, 0);

  label = new wxStaticText(panelContact, wxID_ANY, Lang::Get("Phone"));
  szContactHome->Add(label, 0, wxBOTTOM, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  // Phone fields

  wxFlexGridSizer *szgPhoneFields = new wxFlexGridSizer(0, 5, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szContactHome->Add(szgPhoneFields, 1, wxLEFT | wxBOTTOM | wxEXPAND | wxGROW, 0);

  // Phone 01

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Number") + wxT(": "));
  szgPhoneFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditPhone01 = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgPhoneFields->Add(teditPhone01, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  // Speak with

  label = new wxStaticText(panelContact, wxID_STATIC, wxT("     ") + Lang::Get("Talk to") + wxT(": "));
  szgPhoneFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditPhoneContactName01 = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgPhoneFields->Add(teditPhoneContactName01, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  // Phone type

  choicePhoneType01 = new wxChoice(panelContact, wxID_ANY);
  szgPhoneFields->Add(choicePhoneType01, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  choicePhoneType01->Append(Lang::Get("Personal"), (wxClientData *) new ContainerIntData((int) 'P'));
  choicePhoneType01->Append(Lang::Get("Mobile"), (wxClientData *) new ContainerIntData((int) 'M'));
  choicePhoneType01->Append(Lang::Get("Professional"), (wxClientData *) new ContainerIntData((int) 'P'));
  choicePhoneType01->Append(Lang::Get("Fax"), (wxClientData *) new ContainerIntData((int) 'F'));

  choicePhoneType01->SetSelection(0);

  // Phone 02

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Number") + wxT(": "));
  szgPhoneFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditPhone02 = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgPhoneFields->Add(teditPhone02, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  // Speak with

  label = new wxStaticText(panelContact, wxID_STATIC, wxT("     ") + Lang::Get("Talk to") + wxT(": "));
  szgPhoneFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  teditPhoneContactName02 = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1));
  szgPhoneFields->Add(teditPhoneContactName02, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  // Phone type

  choicePhoneType02 = new wxChoice(panelContact, wxID_ANY);
  szgPhoneFields->Add(choicePhoneType02, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);

  choicePhoneType02->Append(Lang::Get("Personal"), (wxClientData *) new ContainerIntData((int) 'P'));
  choicePhoneType02->Append(Lang::Get("Mobile"), (wxClientData *) new ContainerIntData((int) 'M'));
  choicePhoneType02->Append(Lang::Get("Professional"), (wxClientData *) new ContainerIntData((int) 'P'));
  choicePhoneType02->Append(Lang::Get("Fax"), (wxClientData *) new ContainerIntData((int) 'F'));

  choicePhoneType02->SetSelection(2);


  // Web contact

  wxBoxSizer* szContactWeb = new wxBoxSizer(wxVERTICAL);
  szContact->Add(szContactWeb, 0, wxALL | wxEXPAND | wxGROW, 0);

  label = new wxStaticText(panelContact, wxID_ANY, Lang::Get("Web"));
  szContactWeb->Add(label, 0, wxTOP | wxBOTTOM, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  // E-mail

  wxFlexGridSizer *szgWebFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szContactWeb->Add(szgWebFields, 1, wxBOTTOM | wxEXPAND | wxGROW, GUI::FIELDS_SPACING_VERTICAL);

  //szRow = new wxBoxSizer(wxHORIZONTAL);
  //szContactWeb->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 0);

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("E-mail") + wxT(": "));
  szgWebFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditEmail = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300, -1));
  szgWebFields->Add(teditEmail, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Home page

  //szRow = new wxBoxSizer(wxHORIZONTAL);
  //szContactWeb->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 0);

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Home page") + wxT(": "));
  szgWebFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditHomePage = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300, -1));
  szgWebFields->Add(teditHomePage, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szgWebFields->AddGrowableCol(1);


  // Address

  wxBoxSizer* szAddress = new wxBoxSizer(wxVERTICAL);
  szContact->Add(szAddress, 0, wxALL | wxEXPAND | wxGROW, 0);

  label = new wxStaticText(panelContact, wxID_ANY, Lang::Get("Address"));
  szAddress->Add(label, 0, wxTOP | wxBOTTOM, GUI::LABEL_MARGIN);

  GUI::FormatSectionLabel(label);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szAddress->Add(szRow, 1, wxBOTTOM | wxEXPAND | wxGROW, GUI::FIELDS_SPACING_VERTICAL);

  // Location fields

  wxFlexGridSizer *szgLocationFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szAddress->Add(szgLocationFields, 0, wxBOTTOM | wxEXPAND | wxGROW, 0);

  // CEP

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Postal code") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgLocationFields->Add(szRow, 0, wxALL | wxEXPAND, 0);

  teditAddressCEP = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditAddressCEP, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Search button

  wxBitmapButton *btnAddressSearchFromCEP = new wxBitmapButton(panelContact,
                                                               wxID_ANY,
                                                               wxBitmap(search_16x13_xpm),
                                                               wxDefaultPosition,
                                                               wxDefaultSize);
  szRow->Add(btnAddressSearchFromCEP, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnAddressSearchFromCEP->SetMinSize(wxSize(-1, teditAddressCEP->GetSize().GetHeight()));

  // Street

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Street") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgLocationFields->Add(szRow, 0, wxALL | wxEXPAND, 0);

  teditAddressStreet = new wxTextCtrl(panelContact, wxID_ANY, wxT(""));
  szRow->Add(teditAddressStreet, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Number

  label = new wxStaticText(panelContact, wxID_STATIC, wxT("     ") + Lang::Get("Number") + wxT(": "));
  szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  teditAddressNumber = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1));
  szRow->Add(teditAddressNumber, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  // Complement

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Complement") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditAddressComplement = new wxTextCtrl(panelContact, wxID_ANY, wxT(""));
  szgLocationFields->Add(teditAddressComplement, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Country

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Country") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditAddressCountry = new wxTextCtrl(panelContact, PANEL_PERSON_EDIT_CHOICE_ADDRESS_COUNTRY_ID);
  szgLocationFields->Add(teditAddressCountry, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);

  // Registered state

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("State") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgLocationFields->Add(szRow, 1, wxALL | wxEXPAND, 0);

  teditAddressState = new wxTextCtrl(panelContact, PANEL_PERSON_EDIT_CHOICE_ADDRESS_UF_ID);
  szRow->Add(teditAddressState, 1, wxALL | wxEXPAND, 1);

  // Registered city

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("City") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgLocationFields->Add(szRow, 1, wxALL | wxEXPAND, 0);

  teditAddressCity = new wxTextCtrl(panelContact, PANEL_PERSON_EDIT_CHOICE_ADDRESS_CITY_ID);
  szRow->Add(teditAddressCity, 1, wxALL | wxEXPAND, 1);

  // Quarter

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("District") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditAddressDistrict = new wxTextCtrl(panelContact, wxID_ANY, wxT(""));
  szgLocationFields->Add(teditAddressDistrict, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 1);


  szgLocationFields->AddGrowableCol(1);


  // Notes about person

  wxScrolledWindow *panelNotes = new wxScrolledWindow(notebook, wxID_ANY);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  panelNotes->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szNotes->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Notes field

  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szRow->Add(teditNotes, 1, wxALL | wxEXPAND | wxGROW, 1);

  this->CenterOnParent();


  SetAsPerson(true);
}


void PanelPersonEdit::Terminate()
{
  Close();

  Destroy();
}


int PanelPersonEdit::LoadData(int personId)
{
  this->personId = personId;

  DBQuery dbquery;
  DBQuery dbqueryPerson;

  std::vector<wxString> params;

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();
  LogMessenger *messenger = LogMessenger::GetInstance();

  // Person information

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT p.nome, p.tipo_id, identity_number, to_char(identity_dt, 'DD/MM/YYYY') AS identity_dt, identity_institution, ")
          wxT("identity_country, identity_state, identity_city, security_social_number, trading_name, state_registration, ")
          wxT("to_char(birth_date, 'DD/MM/YYYY') AS birth_date, ")
          wxT("email, email_2, homepage, gender ")
          wxT("FROM pessoa.pessoa p ")
          wxT("WHERE p.person_id = $1");
  }
  else {
    sql = wxT("SELECT p.nome, p.tipo_id, identity_number, identity_dt, identity_institution, ")
          wxT("identity_country, identity_state, identity_city, security_social_number, trading_name, state_registration, ")
          wxT("birth_date, ")
          wxT("email, email_2, homepage, gender ")
          wxT("FROM ") + DB::GetTableName("person") +  wxT(" p ")
          wxT("WHERE p.person_id = ?");
  }

  params.push_back(wxString::Format(wxT("%d"), personId));

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbqueryPerson.Execute(sql, params);

  if (rc != 0) {

    mainFrame->SetStatusText(Lang::Get("Could not get information about the contact") + wxT("."));

    messenger->Register(Lang::Get("Could not get information about the contact") + wxT(".\n\n")
                        wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else if (dbqueryPerson.GetRowsCount() < 1) {
    mainFrame->SetStatusText(wxT("O registro de informações pessoais não foi encontrado."));
  }
  else {
    teditFullName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("nome")));

    teditBornDate->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("birth_date")));

    if (dbqueryPerson.GetFieldStrByName(0, wxT("gender")).Length() > 0) {
      for (size_t index = 0; index < cboxSex->GetCount(); index++) {
        ContainerIntData *container = (ContainerIntData *) cboxSex->GetClientObject(index);

        //wxMessageBox(wxString::Format(wxT("%c == %d"), dbqueryPerson.GetFieldAnsiStrByName(0, wxT("gender"))[0], container->GetData()));

        if (dbqueryPerson.GetFieldAnsiStrByName(0, wxT("gender"))[0] == container->GetData()) {
          cboxSex->SetSelection(index);

          break;
        }
      }
    }

    if (dbqueryPerson.GetFieldStrByName(0, wxT("tipo_id")).Length() > 0) {
      for (size_t index = 0; index < choicePersonNature->GetCount(); index++) {
        ContainerIntData *container = (ContainerIntData *) choicePersonNature->GetClientObject(index);

        if (dbqueryPerson.GetFieldAnsiStrByName(0, wxT("tipo_id"))[0] == container->GetData()) {
          choicePersonNature->SetSelection(index);

          PersonTypeChange();

          break;
        }
      }
    }

    teditCPF_CNPJ->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("security_social_number")));

    teditRgNumber->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("identity_number")));

    teditRGExpedicaoDate->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("identity_dt")));

    teditRgOrgao->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("identity_institution")));

    teditRgCountry->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("identity_country")));

    teditRgUF->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("identity_state")));

    teditRgCity->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("identity_city")));

    teditEmail->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("email")));

    teditFantasyName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("trading_name")));

    teditHomePage->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("homepage")));

    teditInscricaoEstadual->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("state_registration")));
  }


  /*
  // Address information

  params.clear();

  sql = wxT("SELECT e.* ")
        wxT("FROM pessoa.endereco e ")
        wxT("WHERE e.person_id = $1");

  params.push_back(wxString::Format(wxT("%d"), personId));

  rc = dbqueryPerson.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar o registro de informações pessoais sobre o cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
  }
  else if (dbqueryPerson.GetRowsCount() > 0) {
    teditAddressCEP->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("cep")));

    teditAddressStreet->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("rua")));

    teditAddressNumber->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("endr_numero")));

    teditAddressComplement->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("endr_complemento")));

    teditAddressDistrict->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("bairro")));

    teditAddressCountry->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("country_id")));

    teditAddressState->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("state")));

    teditAddressCity->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("city")));
  }
  */


  /*
  // Personal phone

  params.clear();

  sql = wxT("SELECT e.* ")
        wxT("FROM pessoa.telefone e ")
        wxT("WHERE e.person_id = $1 AND tipo = 'P'");

  params.push_back(wxString::Format(wxT("%d"), personId));

  rc = dbqueryPerson.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar o telefone pessoal do cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
  }
  else if (dbqueryPerson.GetRowsCount() > 0) {
    teditPhoneHome->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("numero")));

    teditPhoneHomeContactName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("contact_name")));
  }


  // Comercial phone

  params.clear();

  sql = wxT("SELECT e.* ")
        wxT("FROM pessoa.telefone e ")
        wxT("WHERE e.person_id = $1 AND tipo = 'C'");

  params.push_back(wxString::Format(wxT("%d"), personId));

  rc = dbqueryPerson.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar o telefone comercial do cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
  }
  else if (dbqueryPerson.GetRowsCount() > 0) {
    teditPhoneBusiness->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("numero")));

    teditPhoneBusinessRamal->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("ramal")));

    teditPhoneBusinessContactName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("contact_name")));
  }


  // Fax

  params.clear();

  sql = wxT("SELECT e.* ")
        wxT("FROM pessoa.telefone e ")
        wxT("WHERE e.person_id = $1 AND tipo = 'F'");

  params.push_back(wxString::Format(wxT("%d"), personId));

  rc = dbqueryPerson.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar o fax do cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
  }
  else if (dbqueryPerson.GetRowsCount() > 0) {
    teditPhoneFax->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("numero")));

    teditPhoneFaxRamal->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("ramal")));
  }
  */

#if 0
  // Person picture

  sql = wxT("SELECT * ")
        wxT("FROM pessoa.imagem ")
        wxT("WHERE person_id = ") + wxString::Format(wxT("%d"), this->personId);

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("Get picture command: ") + sql, LogMessage::MSG_DBG);

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    wxMessageBox(wxT("Erro na consulta da fotografia do cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    size_t fieldValueLength;

    unsigned char *fileContent = PQunescapeBytea((const unsigned char *) dbquery.GetFieldAnsiStrByName(0, wxT("file_content")), &fieldValueLength);

    // Load picture stream in a memory stream

    wxMemoryInputStream stream(fileContent, fieldValueLength);

    // Load memory stream in a image object

    imagePhoto.LoadFile(stream);

    UtilsImage::Rescale(imagePhoto, 110, 300);

    // Show picture in a bitmap object, if it's valid

    staticBmpPicture->Show(false);

    if (imagePhoto.IsOk() && imagePhoto.GetWidth() > 0 && imagePhoto.GetHeight() > 0) {
      wxBitmap bmpProductPicture(imagePhoto);

      staticBmpPicture->Freeze();

      staticBmpPicture->SetBitmap(bmpProductPicture);

      staticBmpPicture->Layout();

      staticBmpPicture->Thaw();

      staticBmpPicture->GetContainingSizer()->Layout();

      wxGetApp().GetTopWindow()->Layout();

      staticBmpPicture->Refresh();
      staticBmpPicture->Update();
    }

    staticBmpPicture->Show(true);
  }
#endif

  /*
  wxString sql = wxT("SELECT c.* ")
                 wxT("FROM cliente.cliente c ")
                 wxT("WHERE c.cliente_id = $1");

  int rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível obter as informações sobre o cliente selecionado."));

    Thaw();

    return -1;
  }

  if (dbquery.GetRowsCount() < 1) {
    wxMessageBox(wxT("Cliente não encontrado."));

    Thaw();

    Close();

    return -1;
  }

  this->personId = personId;

  // Type

  for (size_t typeIndex = 0; typeIndex < cboxClientType->GetCount(); typeIndex++) {
    if (dbquery.GetFieldIntByName(0, wxT("tipo_id")) == (int) cboxClientType->GetClientData(typeIndex)) {
      cboxClientType->SetSelection(typeIndex);
    }
  }

  // Notes about client

  params.clear();

  sql = wxT("SELECT conteudo ")
        wxT("FROM cliente.observacao ")
        wxT("WHERE cliente_id = $1");

  params.push_back(wxString::Format(wxT("%d"), personId));

  rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar observações sobre o cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }
  else if (dbquery.GetRowsCount() > 0) {
    teditNotes->SetValue(dbquery.GetFieldStrByName(0, wxT("conteudo")));
  }

  // Verifies if the client is a person

  params.clear();

  sql = wxT("SELECT cp.* ")
        wxT("FROM cliente.pessoa cp ")
        wxT("WHERE cp.cliente_id = $1");

  params.push_back(wxString::Format(wxT("%d"), personId));

  rc = dbqueryPerson.Execute(sql, params);

  if (rc != 0) {
    wxMessageBox(wxT("Não foi possível verificar se o cliente é uma pessoa.\n\n")
                 wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
  }
  else if (dbqueryPerson.GetRowsCount() > 0) {
    //statusBar->SetStatusText(wxT("O cliente é uma pessoa."), 1);

    this->_personId = dbqueryPerson.GetFieldLongByName(0, wxT("person_id"));

    SetAsPerson(true);
  }
  //else {
  //  statusBar->SetStatusText(wxT("O cliente não é uma pessoa."), 1);
  //}


  // If client is not a person, verifies if it is a enterprise

  if (this->_personId < 1) {
    params.clear();

    DBQuery dbqueryEnterprise;

    sql = wxT("SELECT ce.* ")
          wxT("FROM cliente.empresa ce ")
          wxT("WHERE ce.cliente_id = $1");

    params.push_back(wxString::Format(wxT("%d"), personId));

    rc = dbqueryEnterprise.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar se o cliente é uma empresa.\n\n")
                   wxT("Erro interno do banco: ") + dbqueryEnterprise.GetMessageStr());
    }
    else if (dbqueryEnterprise.GetRowsCount() > 0) {
      //statusBar->SetStatusText(wxT("O cliente é uma empresa."), 1);

      this->enterpriseId = dbqueryEnterprise.GetFieldLongByName(0, wxT("enterprise_id"));

      SetAsPerson(false);
    }
  }
  */

  /*
  else if (this->enterpriseId > 0) {
    // Main person information

    params.clear();

    sql = wxT("SELECT e.nome, e.trading_name, e.cnpj, e.corporate_name, e.state_registration, ")
          wxT("email, homepage ")
          wxT("FROM empresa.empresas e ")
          wxT("WHERE e.enterprise_id = $1");

    params.push_back(wxString::Format(wxT("%d"), enterpriseId));

    rc = dbqueryPerson.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar o registro de informações pessoais sobre o cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
    }
    else if (dbqueryPerson.GetRowsCount() < 1) {
      wxMessageBox(wxT("O registro de informações pessoais não foi encontrado."));
    }
    else {
      teditFullName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("nome")));

      teditFantasyName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("trading_name")));

      teditCPF_CNPJ->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("cnpj")));

      teditInscricaoEstadual->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("state_registration")));

      teditEmail->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("email")));

      teditHomePage->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("homepage")));
    }


    // Address information

    params.clear();

    sql = wxT("SELECT e.* ")
          wxT("FROM empresa.empresa_endereco e ")
          wxT("WHERE e.enterprise_id = $1");

    params.push_back(wxString::Format(wxT("%d"), enterpriseId));

    rc = dbqueryPerson.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar o registro de informações sobre o endereço do cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
    }
    else if (dbqueryPerson.GetRowsCount() > 0) {
      teditAddressCEP->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("cep")));

      teditAddressStreet->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("rua")));

      teditAddressNumber->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("endr_numero")));

      teditAddressComplement->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("endr_complemento")));

      teditAddressDistrict->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("bairro")));

      teditAddressCountry->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("country_id")));

      teditAddressState->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("state")));

      teditAddressCity->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("city")));
    }


    // Personal phone

    params.clear();

    sql = wxT("SELECT e.* ")
          wxT("FROM empresa.empresa_telefone e ")
          wxT("WHERE e.enterprise_id = $1 AND tipo = 'P'");

    params.push_back(wxString::Format(wxT("%d"), enterpriseId));

    rc = dbqueryPerson.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar o telefone pessoal do cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
    }
    else if (dbqueryPerson.GetRowsCount() > 0) {
      teditPhoneHome->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("numero")));

      teditPhoneHomeContactName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("contact_name")));
    }


    // Comercial phone

    params.clear();

    sql = wxT("SELECT e.* ")
          wxT("FROM empresa.empresa_telefone e ")
          wxT("WHERE e.enterprise_id = $1 AND tipo = 'C'");

    params.push_back(wxString::Format(wxT("%d"), enterpriseId));

    rc = dbqueryPerson.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar o telefone comercial do cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
    }
    else if (dbqueryPerson.GetRowsCount() > 0) {
      teditPhoneBusiness->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("numero")));

      teditPhoneBusinessRamal->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("ramal")));

      teditPhoneBusinessContactName->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("contact_name")));
    }


    // Fax

    params.clear();

    sql = wxT("SELECT e.* ")
          wxT("FROM empresa.empresa_telefone e ")
          wxT("WHERE e.enterprise_id = $1 AND tipo = 'F'");

    params.push_back(wxString::Format(wxT("%d"), enterpriseId));

    rc = dbqueryPerson.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar o fax do cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbqueryPerson.GetMessageStr());
    }
    else if (dbqueryPerson.GetRowsCount() > 0) {
      teditPhoneFax->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("numero")));

      teditPhoneFaxRamal->SetValue(dbqueryPerson.GetFieldStrByName(0, wxT("ramal")));
    }


    // Client picture

    sql = wxT("SELECT * ")
          wxT("FROM empresa.empresa_imagem ")
          wxT("WHERE enterprise_id = ") + wxString::Format(wxT("%d"), this->enterpriseId);

    rc = dbquery.Execute(sql);

    if (rc != 0) {
      wxMessageBox(wxT("Erro na consulta da fotografia do cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
    else if (dbquery.GetRowsCount() > 0) {
      size_t fieldValueLength;

      unsigned char *fileContent = PQunescapeBytea((const unsigned char *) dbquery.GetFieldAnsiStrByName(0, wxT("file_content")), &fieldValueLength);

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

      // Load picture stream in a memory stream

      wxMemoryInputStream stream(fileContent, fieldValueLength);

      // Load memory stream in a image object

      imagePhoto.LoadFile(stream);

      UtilsImage::Rescale(imagePhoto, 120, 500);

      // Show picture in a bitmap object, if it's valid

      if (imagePhoto.IsOk() && imagePhoto.GetWidth() > 0 && imagePhoto.GetHeight() > 0) {
        wxBitmap bmpProductPicture(imagePhoto);

        staticBmpPicture->SetBitmap(bmpProductPicture);

        staticBmpPicture->Layout();

        staticBmpPicture->GetContainingSizer()->Layout();

        wxGetApp().GetTopWindow()->Layout();

        staticBmpPicture->Refresh();
        staticBmpPicture->Update();

        Refresh();
        Update();
      }
    }
  }
  */

  return 0;
}


void PanelPersonEdit::OnSaveRequest(wxCommandEvent &event)
{
  SaveData();
}


int PanelPersonEdit::SaveData()
{
  if (teditFullName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(Lang::Get("The highlighted fields are required") + wxT("."));

    return -1;
  }

  DBQuery dbquery;
  std::vector<wxString> fields, params, valueModifiers;
  bool registeringError = false;
  wxString errorMessageStr = wxT("");
  wxString sql = wxT("");
  int rc;

  /*
  // Client information

  fields.clear();
  params.clear();

  fields.push_back(wxT("tipo_id"));
  params.push_back(wxString::Format(wxT("%d"), (int) cboxClientType->GetClientData(cboxClientType->GetSelection())));

  sql = DBUtils::MountSQLUpdate(wxT("cliente.cliente"),
                                fields,
                                wxString::Format(wxT("cliente_id = %d"), this->personId));

  rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    wxMessageBox(wxT("Ocorreu erro na modificação das informações sobre o cliente.\n\n")
                 wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
  }

  // Client notes

  // Notes

  params.clear();

  params.push_back(wxString::Format(wxT("%d"), this->personId));

  sql = wxT("SELECT COUNT(*) FROM cliente.observacao WHERE cliente_id = $1");

  rc = dbquery.Execute(sql, params);

  fields.clear();
  params.clear();

  fields.push_back(wxT("conteudo"));
  params.push_back(teditNotes->GetValue());

  if (dbquery.GetFieldInt(0, 0) < 1) {
    fields.push_back(wxT("cliente_id"));
    params.push_back(wxString::Format(wxT("%d"), this->personId));

    wxString sql = DBUtils::MountSQLInsert(wxT("cliente.observacao"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      wxMessageBox(wxT("Ocorreu erro no cadastro das observações sobre o cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }
  else {
    wxString sql = DBUtils::MountSQLUpdate(wxT("cliente.observacao"),
                                           fields,
                                           wxString::Format(wxT("cliente_id = %d"), this->personId));

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      wxMessageBox(wxT("Ocorreu erro na modificação das informações sobre as observações sobre o cliente.\n\n")
                   wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }


  fields.clear();
  params.clear();
  */


  // Contact name

  fields.push_back(wxT("nome"));
  params.push_back(teditFullName->GetValue());
  valueModifiers.push_back("");

  // Update person main information

  if (choicePersonNature->GetClientObject(choicePersonNature->GetSelection())) {
    fields.push_back(wxT("tipo_id"));

    ContainerIntData *container = (ContainerIntData *) choicePersonNature->GetClientObject(choicePersonNature->GetSelection());

    params.push_back(wxString::Format(wxT("%c"), container->GetData()));

    valueModifiers.push_back("");
  }

  if (teditRgNumber->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_number"));

    params.push_back(teditRgNumber->GetValue());

    valueModifiers.push_back("");
  }

  if (teditRgOrgao->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_institution"));

    params.push_back(teditRgOrgao->GetValue());

    valueModifiers.push_back("");
  }

  if (teditRGExpedicaoDate->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_dt"));

    params.push_back(teditRGExpedicaoDate->GetValue());

    valueModifiers.push_back("");
  }

  if (teditRgCity->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_city"));

    params.push_back(teditRgCity->GetValue());

    valueModifiers.push_back("");
  }

  if (teditRgUF->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_state"));

    params.push_back(teditRgUF->GetValue());

    valueModifiers.push_back("");
  }

  if (teditRgCountry->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_country"));

    params.push_back(teditRgCountry->GetValue());

    valueModifiers.push_back("");
  }

  if (teditCPF_CNPJ->GetValue().Length() > 0) {
    fields.push_back(wxT("security_social_number"));

    params.push_back(teditCPF_CNPJ->GetValue());

    valueModifiers.push_back("");
  }

  if (teditFantasyName->GetValue().Length() > 0) {
    fields.push_back(wxT("trading_name"));

    params.push_back(teditFantasyName->GetValue());

    valueModifiers.push_back("");
  }

  if (teditInscricaoEstadual->GetValue().Length() > 0) {
    fields.push_back(wxT("state_registration"));

    params.push_back(teditInscricaoEstadual->GetValue());

    valueModifiers.push_back("");
  }

  if (teditBornDate->GetValue().Length() > 0) {
    fields.push_back(wxT("birth_date"));

    params.push_back(teditBornDate->GetValue());

    //valueModifiers.push_back("to_date($@, 'DD/MM/YYYY')");
  }

  if (cboxSex->GetClientObject(cboxSex->GetSelection())) {
    ContainerIntData *container = (ContainerIntData *) cboxSex->GetClientObject(cboxSex->GetSelection());

    if (container->GetData() > 0) {
      fields.push_back(wxT("gender"));

      params.push_back(wxString::Format(wxT("%c"), container->GetData()));

      valueModifiers.push_back("");
    }
  }

  if (teditEmail->GetValue().Length() > 0) {
    fields.push_back(wxT("email"));

    params.push_back(teditEmail->GetValue());

    valueModifiers.push_back("");
  }
 
  /*
  if (teditEmail02->GetValue().Length() > 0) {
    fields.push_back(wxT("email_2"));
    params.push_back(teditEmail02->GetValue());
  }
  */
 
  if (teditHomePage->GetValue().Length() > 0) {
    fields.push_back(wxT("homepage"));

    params.push_back(teditHomePage->GetValue());

    valueModifiers.push_back("");
  }

  sql = DBUtils::MountSQLUpdate(DB::GetTableName("person"),
                                fields,
                                wxT("person_id = ") + wxString::Format(wxT("%d"), this->personId),
                                valueModifiers);

  //wxMessageBox(sql);

  rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    errorMessageStr += wxT("Ocorreu erro na atualização das informações principais sobre a pessoa.\n")
                       wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
  }


  // Person picture
#if 0
  if (photoFilePath.Length() > 0) {
    params.clear();

    params.push_back(wxString::Format(wxT("%d"), this->personId));

    sql = wxT("SELECT COUNT(*) FROM pessoa.imagem WHERE person_id = $1");

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      wxMessageBox(wxT("Não foi possível verificar a imagem da pessoa."));
    }
    else if (dbquery.GetFieldInt(0, 0) < 1) {
      char fileName[strlen((const char *) photoFilePath.mb_str(wxConvUTF8)) + 1];

      strcpy(fileName, (const char *) photoFilePath.mb_str(wxConvUTF8));

      wxFFile file(photoFilePath, wxT("rb"));

      long fileLength = file.Length();

      file.Close();

      // Opening picture file

      FILE *picturefd = fopen(fileName, "rb");

      if (!picturefd) {
        fprintf(stderr, "Não foi possível abrir o arquivo com a fotografia do cliente.\n");

        exit(1);
      }

      // Getting the file content

      char *fileContent;

      fileContent = (char *) malloc((fileLength * 2 + 1) * sizeof(char));

      int readCount = fread((void *) fileContent, 1, fileLength * 2, picturefd);

      // Check picture type by extension

      char imageMimeType[50];

      wxRegEx regex;

      if (regex.Compile(wxT("\\.png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
          regex.Matches(photoFilePath)) {
        strcpy(imageMimeType, "image/png");
      }
      else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(photoFilePath)) {
        strcpy(imageMimeType, "image/jpeg");
      }
      else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(photoFilePath)) {
        strcpy(imageMimeType, "image/gif");
      }
      else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(photoFilePath)) {
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

      sprintf(sqlCommandAnsi, "INSERT INTO pessoa.imagem (person_id, file_mime, file_content) VALUES (%d, $1, $2)",
                              personId);

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
          wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                       wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
        }
      }
      else {
        ExecStatusType status = PQresultStatus((PGresult *) res); 

        if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
          if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
            wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                         wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
          }
        }
      }

      free(fileContent);
    }
    else {
      char fileName[strlen((const char *) photoFilePath.mb_str(wxConvUTF8)) + 1];

      strcpy(fileName, (const char *) photoFilePath.mb_str(wxConvUTF8));

      wxFFile file(photoFilePath, wxT("rb"));

      long fileLength = file.Length();

      file.Close();

      // Opening picture file

      FILE *picturefd = fopen(fileName, "rb");

      if (!picturefd) {
        fprintf(stderr, "Não foi possível abrir o arquivo com a fotografia do cliente.\n");

        exit(1);
      }

      // Getting the file content

      char *fileContent;

      fileContent = (char *) malloc((fileLength * 2 + 1) * sizeof(char));

      int readCount = fread((void *) fileContent, 1, fileLength * 2, picturefd);

      //wxMessageBox(wxString::Format(wxT("length: %d, readCount: %d"), fileLength, readCount));

      // Check picture type by extension

      char imageMimeType[50];

      wxRegEx regex;

      if (regex.Compile(wxT("\\.png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
          regex.Matches(photoFilePath)) {
        strcpy(imageMimeType, "image/png");
      }
      else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(photoFilePath)) {
        strcpy(imageMimeType, "image/jpeg");
      }
      else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(photoFilePath)) {
        strcpy(imageMimeType, "image/gif");
      }
      else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
               regex.Matches(photoFilePath)) {
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

      sprintf(sqlCommandAnsi, "UPDATE pessoa.imagem SET file_mime = $1, file_content = $2 WHERE person_id = %d",
                              personId);

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
          wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                       wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
        }
      }
      else {
        ExecStatusType status = PQresultStatus((PGresult *) res); 

        if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
          if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
            wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                         wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
          }
        }
      }

      free(fileContent);
    }
  }
#endif


#if 0
  // Inserting person address

  fields.clear();
  params.clear();

  fields.push_back(wxT("person_id"));
  params.push_back(wxString::Format(wxT("%d"), this->personId));

  sql = wxT("SELECT COUNT(*) FROM pessoa.endereco WHERE person_id = $1");

  rc = dbquery.Execute(sql, params);

  if (dbquery.GetFieldInt(0, 0) < 1) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%d"), this->personId));

    if (teditAddressCEP->GetValue().Length() > 0) {
      fields.push_back(wxT("cep"));
      params.push_back(teditAddressCEP->GetValue());
    }

    if (teditAddressStreet->GetValue().Length() > 0) {
      fields.push_back(wxT("rua"));
      params.push_back(teditAddressStreet->GetValue());
    }

    if (teditAddressNumber->GetValue().Length() > 0) {
      fields.push_back(wxT("endr_numero"));
      params.push_back(teditAddressNumber->GetValue());
    }

    if (teditAddressComplement->GetValue().Length() > 0) {
      fields.push_back(wxT("endr_complemento"));
      params.push_back(teditAddressComplement->GetValue());
    }

    if (teditAddressDistrict->GetValue().Length() > 0) {
      fields.push_back(wxT("bairro"));
      params.push_back(teditAddressDistrict->GetValue());
    }

    if (teditAddressCountry->GetValue().Length() > 0) {
      fields.push_back(wxT("country_id"));
      params.push_back(teditAddressCountry->GetValue());
    }

    if (teditAddressState->GetValue().Length() > 0) {
      fields.push_back(wxT("state"));
      params.push_back(teditAddressState->GetValue());
    }

    if (teditAddressCity->GetValue().Length() > 0) {
      fields.push_back(wxT("city"));
      params.push_back(teditAddressCity->GetValue());
    }

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.endereco"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Erro ao tentar cadastrar o endereço da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }
  else {
    fields.clear();
    params.clear();

    fields.push_back(wxT("cep"));
    params.push_back(teditAddressCEP->GetValue());

    fields.push_back(wxT("rua"));
    params.push_back(teditAddressStreet->GetValue());

    fields.push_back(wxT("endr_numero"));
    params.push_back(teditAddressNumber->GetValue());

    fields.push_back(wxT("endr_complemento"));
    params.push_back(teditAddressComplement->GetValue());

    fields.push_back(wxT("bairro"));
    params.push_back(teditAddressDistrict->GetValue());

    fields.push_back(wxT("country_id"));
    params.push_back(teditAddressCountry->GetValue());

    fields.push_back(wxT("state"));
    params.push_back(teditAddressState->GetValue());

    fields.push_back(wxT("city"));
    params.push_back(teditAddressCity->GetValue());

    wxString sql = DBUtils::MountSQLUpdate(wxT("pessoa.endereco"),
                                           fields,
                                           wxString::Format(wxT("person_id = %d"), this->personId));

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Erro ao tentar modificar o endereço da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }


  /*
  // Person contact information

  // Business phone

  fields.clear();
  params.clear();

  fields.push_back(wxT("person_id"));
  params.push_back(wxString::Format(wxT("%d"), this->personId));

  sql = wxT("SELECT COUNT(*) FROM pessoa.telefone WHERE person_id = $1 AND tipo = 'C'");

  rc = dbquery.Execute(sql, params);

  fields.clear();
  params.clear();

  if (teditPhoneBusiness->GetValue().Length() > 0) {
    fields.push_back(wxT("numero"));
    params.push_back(teditPhoneBusiness->GetValue());
  }

  if (teditPhoneBusinessRamal->GetValue().Length() > 0) {
    fields.push_back(wxT("ramal"));
    params.push_back(teditPhoneBusinessRamal->GetValue());
  }

  if (teditPhoneBusinessContactName->GetValue().Length() > 0) {
    fields.push_back(wxT("contact_name"));
    params.push_back(teditPhoneBusinessContactName->GetValue());
  }

  if (dbquery.GetFieldInt(0, 0) < 1) {
    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%d"), this->personId));

    fields.push_back(wxT("tipo"));
    params.push_back(wxT("C"));;

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.telefone"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Ocorreu erro no cadastro do telefone comercial da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }
  else {
    wxString sql = DBUtils::MountSQLUpdate(wxT("pessoa.telefone"),
                                           fields,
                                           wxString::Format(wxT("person_id = %d AND tipo = 'C'"), this->personId));

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Ocorreu erro na modificação das informações sobre o telefone comercial da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }


  // Home phone

  fields.clear();
  params.clear();

  fields.push_back(wxT("person_id"));
  params.push_back(wxString::Format(wxT("%d"), this->personId));

  sql = wxT("SELECT COUNT(*) FROM pessoa.telefone WHERE person_id = $1 AND tipo = 'P'");

  rc = dbquery.Execute(sql, params);

  fields.clear();
  params.clear();

  fields.push_back(wxT("numero"));
  params.push_back(teditPhoneHome->GetValue());

  fields.push_back(wxT("contact_name"));
  params.push_back(teditPhoneHomeContactName->GetValue());

  if (dbquery.GetFieldInt(0, 0) < 1) {
    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%d"), this->personId));

    fields.push_back(wxT("tipo"));
    params.push_back(wxT("P"));;

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.telefone"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Ocorreu erro no cadastro do telefone pessoal da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }
  else {
    wxString sql = DBUtils::MountSQLUpdate(wxT("pessoa.telefone"),
                                           fields,
                                           wxString::Format(wxT("person_id = %d AND tipo = 'P'"), this->personId));

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Ocorreu erro na modificação das informações sobre o telefone pessoal da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }


  // Fax

  fields.clear();
  params.clear();

  fields.push_back(wxT("person_id"));
  params.push_back(wxString::Format(wxT("%d"), this->personId));

  sql = wxT("SELECT COUNT(*) FROM pessoa.telefone WHERE person_id = $1 AND tipo = 'F'");

  rc = dbquery.Execute(sql, params);

  fields.clear();
  params.clear();

  fields.push_back(wxT("numero"));
  params.push_back(teditPhoneFax->GetValue());

  fields.push_back(wxT("ramal"));
  params.push_back(teditPhoneFaxRamal->GetValue());

  if (dbquery.GetFieldInt(0, 0) < 1) {
    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%d"), this->personId));

    fields.push_back(wxT("tipo"));
    params.push_back(wxT("F"));

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.telefone"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Ocorreu erro no cadastro do fax da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }
  else {
    wxString sql = DBUtils::MountSQLUpdate(wxT("pessoa.telefone"),
                                           fields,
                                           wxString::Format(wxT("person_id = %d AND tipo = 'F'"), this->personId));

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr += wxT("Ocorreu erro na modificação das informações sobre o fax da pessoa.\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr() + wxT("\n\n");
    }
  }
  */

  #if 0
  else if (this->enterpriseId > 0) { // Client is a enterprise
    if (teditCPF_CNPJ->GetValue().Length() > 0) {
      fields.push_back(wxT("cnpj"));
      params.push_back(teditCPF_CNPJ->GetValue());
    }

    if (teditFantasyName->GetValue().Length() > 0) {
      fields.push_back(wxT("trading_name"));
      params.push_back(teditFantasyName->GetValue());
    }

    if (teditEmail->GetValue().Length() > 0) {
      fields.push_back(wxT("email"));
      params.push_back(teditEmail->GetValue());
    }

    if (teditHomePage->GetValue().Length() > 0) {
      fields.push_back(wxT("homepage"));
      params.push_back(teditHomePage->GetValue());
    }

    sql = DBUtils::MountSQLUpdate(wxT("empresa.empresas"),
                                  fields,
                                  wxT("enterprise_id = ") + wxString::Format(wxT("%d"), this->enterpriseId));

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      wxMessageBox(wxT("Ocorreu erro na atualização das informações principais sobre a empresa.\n\n")
                   wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }


    // Inserting enterprise address

    fields.clear();
    params.clear();

    fields.push_back(wxT("enterprise_id"));
    params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

    sql = wxT("SELECT COUNT(*) FROM empresa.empresa_endereco WHERE enterprise_id = $1");

    rc = dbquery.Execute(sql, params);

    if (dbquery.GetFieldInt(0, 0) < 1) {
      fields.clear();
      params.clear();

      fields.push_back(wxT("enterprise_id"));
      params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

      if (teditAddressCEP->GetValue().Length() > 0) {
        fields.push_back(wxT("cep"));
        params.push_back(teditAddressCEP->GetValue());
      }

      if (teditAddressStreet->GetValue().Length() > 0) {
        fields.push_back(wxT("rua"));
        params.push_back(teditAddressStreet->GetValue());
      }

      if (teditAddressNumber->GetValue().Length() > 0) {
        fields.push_back(wxT("endr_numero"));
        params.push_back(teditAddressNumber->GetValue());
      }

      if (teditAddressComplement->GetValue().Length() > 0) {
        fields.push_back(wxT("endr_complemento"));
        params.push_back(teditAddressComplement->GetValue());
      }

      if (teditAddressDistrict->GetValue().Length() > 0) {
        fields.push_back(wxT("bairro"));
        params.push_back(teditAddressDistrict->GetValue());
      }

      if (teditAddressCountry->GetValue().Length() > 0) {
        fields.push_back(wxT("country_id"));
        params.push_back(teditAddressCountry->GetValue());
      }

      if (teditAddressState->GetValue().Length() > 0) {
        fields.push_back(wxT("state"));
        params.push_back(teditAddressState->GetValue());
      }

      if (teditAddressCity->GetValue().Length() > 0) {
        fields.push_back(wxT("city"));
        params.push_back(teditAddressCity->GetValue());
      }

      wxString sql = DBUtils::MountSQLInsert(wxT("empresa.empresa_endereco"), fields);

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Erro ao tentar cadastrar o endereço da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }
    else {
      fields.clear();
      params.clear();

      fields.push_back(wxT("cep"));
      params.push_back(teditAddressCEP->GetValue());

      fields.push_back(wxT("rua"));
      params.push_back(teditAddressStreet->GetValue());

      fields.push_back(wxT("endr_numero"));
      params.push_back(teditAddressNumber->GetValue());

      fields.push_back(wxT("endr_complemento"));
      params.push_back(teditAddressComplement->GetValue());

      fields.push_back(wxT("bairro"));
      params.push_back(teditAddressDistrict->GetValue());

      fields.push_back(wxT("country_id"));
      params.push_back(teditAddressCountry->GetValue());

      fields.push_back(wxT("state"));
      params.push_back(teditAddressState->GetValue());

      fields.push_back(wxT("city"));
      params.push_back(teditAddressCity->GetValue());

      wxString sql = DBUtils::MountSQLUpdate(wxT("empresa.empresa_endereco"),
                                             fields,
                                             wxString::Format(wxT("enterprise_id = %d"), this->enterpriseId));

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Erro ao tentar modificar o endereço da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }


    // Person contact information

    // Business phone

    fields.clear();
    params.clear();

    fields.push_back(wxT("enterprise_id"));
    params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

    sql = wxT("SELECT COUNT(*) FROM empresa.empresa_telefone WHERE emrpresa_id = $1 AND tipo = 'C'");

    rc = dbquery.Execute(sql, params);

    fields.clear();
    params.clear();

    if (teditPhoneBusiness->GetValue().Length() > 0) {
      fields.push_back(wxT("numero"));
      params.push_back(teditPhoneBusiness->GetValue());
    }

    if (teditPhoneBusinessRamal->GetValue().Length() > 0) {
      fields.push_back(wxT("ramal"));
      params.push_back(teditPhoneBusinessRamal->GetValue());
    }

    if (teditPhoneBusinessContactName->GetValue().Length() > 0) {
      fields.push_back(wxT("contact_name"));
      params.push_back(teditPhoneBusinessContactName->GetValue());
    }

    if (dbquery.GetFieldInt(0, 0) < 1) {
      fields.push_back(wxT("enterprise_id"));
      params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

      fields.push_back(wxT("tipo"));
      params.push_back(wxT("C"));;

      wxString sql = DBUtils::MountSQLInsert(wxT("empresa.empresa_telefone"), fields);

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Ocorreu erro no cadastro do telefone comercial da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }
    else {
      wxString sql = DBUtils::MountSQLUpdate(wxT("empresa.empresa_telefone"),
                                             fields,
                                             wxString::Format(wxT("enterprise_id = %d AND tipo = 'C'"), this->enterpriseId));

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Ocorreu erro na modificação das informações sobre o telefone comercial da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }


    // Home phone

    fields.clear();
    params.clear();

    fields.push_back(wxT("enterprise_id"));
    params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

    sql = wxT("SELECT COUNT(*) FROM empresa.empresa_telefone WHERE enterprise_id = $1 AND tipo = 'P'");

    rc = dbquery.Execute(sql, params);

    fields.clear();
    params.clear();

    fields.push_back(wxT("numero"));
    params.push_back(teditPhoneHome->GetValue());

    fields.push_back(wxT("contact_name"));
    params.push_back(teditPhoneHomeContactName->GetValue());

    if (dbquery.GetFieldInt(0, 0) < 1) {
      fields.push_back(wxT("enterprise_id"));
      params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

      fields.push_back(wxT("tipo"));
      params.push_back(wxT("P"));;

      wxString sql = DBUtils::MountSQLInsert(wxT("empresa.empresa_telefone"), fields);

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Ocorreu erro no cadastro do telefone pessoal da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }
    else {
      wxString sql = DBUtils::MountSQLUpdate(wxT("empresa.empresa_telefone"),
                                             fields,
                                             wxString::Format(wxT("enterprise_id = %d AND tipo = 'P'"), this->enterpriseId));

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Ocorreu erro na modificação das informações sobre o telefone pessoal da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }


    // Fax

    fields.clear();
    params.clear();

    fields.push_back(wxT("enterprise_id"));
    params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

    sql = wxT("SELECT COUNT(*) FROM empresa.empresa_telefone WHERE enterprise_id = $1 AND tipo = 'F'");

    rc = dbquery.Execute(sql, params);

    fields.clear();
    params.clear();

    fields.push_back(wxT("numero"));
    params.push_back(teditPhoneFax->GetValue());

    fields.push_back(wxT("ramal"));
    params.push_back(teditPhoneFaxRamal->GetValue());

    if (dbquery.GetFieldInt(0, 0) < 1) {
      fields.push_back(wxT("enterprise_id"));
      params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

      fields.push_back(wxT("tipo"));
      params.push_back(wxT("F"));

      wxString sql = DBUtils::MountSQLInsert(wxT("empresa.empresa_telefone"), fields);

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Ocorreu erro no cadastro do fax da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }
    else {
      wxString sql = DBUtils::MountSQLUpdate(wxT("empresa.empresa_telefone"),
                                             fields,
                                             wxString::Format(wxT("enterprise_id = %d AND tipo = 'F'"), this->enterpriseId));

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        registeringError = true;

        wxMessageBox(wxT("Ocorreu erro na modificação das informações sobre o fax da empresa.\n\n")
                     wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
      }
    }


    // Client picture

    if (photoFilePath.Length() > 0) {
      params.clear();

      params.push_back(wxString::Format(wxT("%d"), this->enterpriseId));

      sql = wxT("SELECT COUNT(*) FROM empresa.empresa_imagem WHERE enterprise_id = $1");

      rc = dbquery.Execute(sql, params);

      if (rc != 0) {
        wxMessageBox(wxT("Não foi possível verificar a imagem da empresa."));
      }
      if (dbquery.GetFieldInt(0, 0) < 1) {
        char fileName[strlen((const char *) photoFilePath.mb_str(wxConvUTF8)) + 1];

        strcpy(fileName, (const char *) photoFilePath.mb_str(wxConvUTF8));

        wxFFile file(photoFilePath, wxT("rb"));

        long fileLength = file.Length();

        file.Close();

        // Opening picture file

        FILE *picturefd = fopen(fileName, "rb");

        if (!picturefd) {
          fprintf(stderr, "Não foi possível abrir o arquivo com a fotografia do cliente.\n");

          exit(1);
        }

        // Getting the file content

        char *fileContent;

        fileContent = (char *) malloc((fileLength * 2 + 1) * sizeof(char));

        int readCount = fread((void *) fileContent, 1, fileLength * 2, picturefd);

        // Check picture type by extension

        char imageMimeType[50];

        wxRegEx regex;

        if (regex.Compile(wxT("\\.png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
            regex.Matches(photoFilePath)) {
          strcpy(imageMimeType, "image/png");
        }
        else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
                 regex.Matches(photoFilePath)) {
          strcpy(imageMimeType, "image/jpeg");
        }
        else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
                 regex.Matches(photoFilePath)) {
          strcpy(imageMimeType, "image/gif");
        }
        else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
                 regex.Matches(photoFilePath)) {
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

        sprintf(sqlCommandAnsi, "INSERT INTO empresa.empresa_imagem (enterprise_id, file_mime, file_content) VALUES (%d, $1, $2)",
                                enterpriseId);

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
            wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                         wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
          }
        }
        else {
          ExecStatusType status = PQresultStatus((PGresult *) res); 

          if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
            if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
              wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                           wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
            }
          }
        }

        free(fileContent);
      }
      else {
        char fileName[strlen((const char *) photoFilePath.mb_str(wxConvUTF8)) + 1];

        strcpy(fileName, (const char *) photoFilePath.mb_str(wxConvUTF8));

        wxFFile file(photoFilePath, wxT("rb"));

        long fileLength = file.Length();

        file.Close();

        // Opening picture file

        FILE *picturefd = fopen(fileName, "rb");

        if (!picturefd) {
          fprintf(stderr, "Não foi possível abrir o arquivo com a fotografia do cliente.\n");

          exit(1);
        }

        // Getting the file content

        char *fileContent;

        fileContent = (char *) malloc((fileLength * 2 + 1) * sizeof(char));

        int readCount = fread((void *) fileContent, 1, fileLength * 2, picturefd);

        // Check picture type by extension

        char imageMimeType[50];

        wxRegEx regex;

        if (regex.Compile(wxT("\\.png$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
            regex.Matches(photoFilePath)) {
          strcpy(imageMimeType, "image/png");
        }
        else if (regex.Compile(wxT("(\\.jpg|\\.jpeg)$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
                 regex.Matches(photoFilePath)) {
          strcpy(imageMimeType, "image/jpeg");
        }
        else if (regex.Compile(wxT("\\.gif$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
                 regex.Matches(photoFilePath)) {
          strcpy(imageMimeType, "image/gif");
        }
        else if (regex.Compile(wxT("\\.bmp$"), wxRE_EXTENDED|wxRE_ADVANCED|wxRE_ICASE) &&
                 regex.Matches(photoFilePath)) {
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

        sprintf(sqlCommandAnsi, "UPDATE empresa.empresa_imagem SET file_mime = $1, file_content = $2 WHERE enterprise_id = %d",
                                enterpriseId);

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
            wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                         wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
          }
        }
        else {
          ExecStatusType status = PQresultStatus((PGresult *) res); 

          if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
            if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
              wxMessageBox(wxT("Não foi possível cadastrar a imagem do cliente.\n\nErro interno:\n") +
                           wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection))));
            }
          }
        }

        free(fileContent);
      }
    }
  }
  else {
    wxMessageBox(wxT("Cadastro de empresa como cliente não implementado."));
  }
  #endif

#if 0
  // Registering client information

  if (!registeringError) {
    fields.clear();
    params.clear();

    // Client type

    if (cboxClientType->GetStringSelection().Length() > 0) {
      fields.push_back(wxT("tipo_id"));
      params.push_back(wxString::Format(wxT("%d"), (int) cboxClientType->GetClientData(cboxClientType->GetCurrentSelection())));
    }

    wxString sql = DBUtils::MountSQLInsert(wxT("cliente.cliente"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr = (wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }


  // Get the client inserted ID from the database

  long personId;

  if (!registeringError) {
    personId = DBUtils::GetSerialKey("cliente.cliente", "cliente_id");

    if (personId < 1) {
      registeringError = true;

      errorMessageStr = wxT("Não foi possível obter a identificação do cliente cadastrado.");
    }
  }

  // Registering the client / person relation

  if (!registeringError) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%d"), personId));

    fields.push_back(wxT("cliente_id"));
    params.push_back(wxString::Format(wxT("%d"), personId));

    wxString sql = DBUtils::MountSQLInsert(wxT("cliente.pessoa"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr = (wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }

  // Registering notes

  if (!registeringError && teditNotes->GetValue().Length() > 0) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("cliente_id"));
    params.push_back(wxString::Format(wxT("%d"), personId));

    fields.push_back(wxT("conteudo"));
    params.push_back(teditNotes->GetValue().mb_str(wxConvUTF8));

    wxString sql = DBUtils::MountSQLInsert(wxT("cliente.observacao"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr = (wxT("Erro no cadastro das observações sobre o cliente.\n\n")
                         wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }
#endif

#endif


  // Successfully registering

  if (registeringError) {
    wxMessageBox(errorMessageStr, wxT("Erro"));
  }
  else {
    // Reload main panel data

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    PanelPersonMain *panelPersonMain = (PanelPersonMain *) mainFrame->GetPage(Lang::Get("Contacts"));

    if (panelPersonMain) {
      panelPersonMain->LoadData();
    }
  }

  return 0;
}


void PanelPersonEdit::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F3>
  if (event.GetKeyCode() == 342) {
    SaveData();
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


void PanelPersonEdit::OnBornDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditBornDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnBornDateChoose->GetScreenPosition().x;
  calendarPosition.y = btnBornDateChoose->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditBornDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                            calendarDialog.selectedMonth + wxT("/") + 
                            calendarDialog.selectedYear);
  }
}


void PanelPersonEdit::OnRGDateChooseRequest(wxCommandEvent &event)
{
  wxPoint calendarPosition;
  wxDateTime dateTime;

  dateTime.ParseFormat(teditRGExpedicaoDate->GetValue(), wxT("%d/%m/%Y"));

  calendarPosition.x = btnRgDateChoose->GetScreenPosition().x;
  calendarPosition.y = btnRgDateChoose->GetScreenPosition().y;

  CalendarDialog calendarDialog(this, calendarPosition, dateTime);

  calendarDialog.ShowModal();

  if (calendarDialog.hasSelection) {
    teditRGExpedicaoDate->SetValue(calendarDialog.selectedDay + wxT("/") + 
                          calendarDialog.selectedMonth + wxT("/") + 
                          calendarDialog.selectedYear);
  }
}


void PanelPersonEdit::OnSetClientAsEnterprise(wxCommandEvent &event)
{
  szgMainFields->Layout();

  wxGetApp().GetTopWindow()->Layout();

  Refresh();
  Update();


  event.Skip();
}


void PanelPersonEdit::SetAsPerson(bool isPerson)
{
   if (isPerson) {
    labelFullName->SetLabel(wxT("Nome: "));
    labelFullName->GetContainingSizer()->Layout();
    szFullName->Layout();

    lblCpfCnpj->SetLabel(wxT("CPF: "));
    lblCpfCnpj->GetContainingSizer()->Layout();

    labelFantasyName->Disable();
    teditFantasyName->Disable();

    labelInscricaoEstadual->Disable();
    teditInscricaoEstadual->Disable();

    labelRgNumber->Enable();
    teditRgNumber->Enable();
    labelRgExpedicaoDate->Enable();
    teditRGExpedicaoDate->Enable();
    labelRgOrgao->Enable();
    teditRgOrgao->Enable();
    labelRgCountry->Enable();
    teditRgCountry->Enable();
    labelRgFederalState->Enable();
    teditRgUF->Enable();
    labelRgCity->Enable();
    teditRgCity->Enable();
    btnRgDateChoose->Enable();

    cboxSex->Enable();
  }
  else {
    labelFullName->SetLabel(wxT("Razão social: "));
    labelFullName->GetContainingSizer()->Layout();
    szFullName->Layout();

    lblCpfCnpj->SetLabel(wxT("CNPJ: "));
    lblCpfCnpj->GetContainingSizer()->Layout();

    labelFantasyName->Enable();
    teditFantasyName->Enable();

    labelInscricaoEstadual->Enable();
    teditInscricaoEstadual->Enable();

    labelRgNumber->Disable();
    teditRgNumber->Disable();
    labelRgExpedicaoDate->Disable();
    teditRGExpedicaoDate->Disable();
    labelRgOrgao->Disable();
    teditRgOrgao->Disable();
    labelRgCountry->Disable();
    teditRgCountry->Disable();
    labelRgFederalState->Disable();
    teditRgUF->Disable();
    labelRgCity->Disable();
    teditRgCity->Disable();
    btnRgDateChoose->Disable();

    cboxSex->Disable();
  }
}



void PanelPersonEdit::PersonTypeChange()
{
  if (choicePersonNature->GetSelection() >= 0 &&
      choicePersonNature->GetClientObject(choicePersonNature->GetSelection()) &&
      ((ContainerIntData *) choicePersonNature->GetClientObject(choicePersonNature->GetSelection()))->GetData() == (int) 'F') {
    SetAsPerson(true);
  }
  else {
    SetAsPerson(false);
  }
}


void PanelPersonEdit::OnPersonTypeChange(wxCommandEvent &event)
{
  PersonTypeChange();
}


void PanelPersonEdit::OnPhotoChooseRequest(wxCommandEvent &event)
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
    if (!imagePhoto.LoadFile(fileDialog.GetPath())) {
      wxMessageBox(wxT("O arquivo '") + fileDialog.GetPath() + wxT("' não pôde ser carregado."));

      return;
    }

    UtilsImage::Rescale(imagePhoto, 110, 500);

    // Show picture in a bitmap object, if it's valid

    if (!imagePhoto.IsOk() || imagePhoto.GetWidth() < 1 || imagePhoto.GetHeight() < 1) {
      wxMessageBox(wxT("O arquivo '") + fileDialog.GetPath() + wxT("' não pôde ser carregado."));
    }
    else {
      photoFilePath = fileDialog.GetPath();

      wxBitmap bmpPhoto(imagePhoto);

      staticBmpPicture->SetBitmap(bmpPhoto);

      //staticBmpPicture->Layout();

      //staticBmpPicture->GetContainingSizer()->Layout();

      //wxGetApp().GetTopWindow()->Layout();

      staticBmpPicture->Refresh();
      staticBmpPicture->Update();
    }
  }
}


void PanelPersonEdit::OnDeleteRequest(wxCommandEvent& event)
{
  int rc = wxMessageBox(wxT("Você confirma a exclusão das informações sobre a pessoa?"),
                        wxT("Confirmação de exclusão de informações"),
                        wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION,
                        NULL);

  if (wxYES != rc) {
    return;
  }

  DBQuery dbquery;
  std::vector<wxString> params;

  LogMessenger *messenger = LogMessenger::GetInstance();

  // Init transaction

  rc = DBTransaction::Begin();

  if (rc != 0) {
    messenger->Register(Lang::Get("Could not initiate database transaction") + wxT("."), LogMessage::MSG_ERROR);
  }

  // Delete person's address

  params.push_back(wxString::Format(wxT("%ld"), this->personId));

  rc = dbquery.Execute(wxT("DELETE FROM ") + DB::GetTableName("person.address") + wxT(" ")
                       wxT("WHERE person_id = $1"), params);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível excluir as informações sobre o endereço da pessoa."), LogMessage::MSG_ERROR);
  }

  /*
  // Delete person's picture

  rc = dbquery.Execute(wxT("DELETE FROM pessoa.imagem ")
                       wxT("WHERE person_id = $1"), params);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível excluir as informações sobre a imagem da pessoa."), LogMessage::MSG_ERROR);
  }

  // Delete customer link

  rc = dbquery.Execute(wxT("DELETE FROM cliente.pessoa ")
                       wxT("WHERE person_id = $1"), params);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível excluir as informações sobre o cliente."), LogMessage::MSG_ERROR);
  }

  // Delete supplier link

  rc = dbquery.Execute(wxT("DELETE FROM fornecedor.pessoa ")
                       wxT("WHERE person_id = $1"), params);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível excluir as informações sobre o fornecedor."), LogMessage::MSG_ERROR);
  }

  // Delete employee link

  rc = dbquery.Execute(wxT("DELETE FROM funcionario.pessoa ")
                       wxT("WHERE person_id = $1"), params);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível excluir as informações sobre o funcionário."), LogMessage::MSG_ERROR);
  }
  */

  // Delete person

  wxString sql = "";

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("DELETE FROM ") + DB::GetTableName("person") +
          wxT(" WHERE person_id = $1");
  }
  else {
    sql = wxT("DELETE FROM ") + DB::GetTableName("person") +
          wxT(" WHERE person_id = ?");
  }

  messenger->Register(sql, LogMessage::MSG_DBG);

  rc = dbquery.Execute(sql, params);

  if (rc == 0) {
    // Commit transaction

    rc = DBTransaction::Commit();

    messenger->Register(wxT("As informações sobre a pessoa foram excluídas com sucesso."), LogMessage::MSG_ERROR);

    // Close panel

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    // Reload main panel data

    PanelPersonMain *panelPersonMain = (PanelPersonMain *) mainFrame->GetPage(Lang::Get("Contacts"));

    if (panelPersonMain) {
      panelPersonMain->LoadData();
    }

    // Remove this panel

    wxString pageTitle = Lang::Get("Contact") + wxT(" #") + wxString::Format(wxT("%d"), this->personId);

    mainFrame->PageRemove(pageTitle);
  }
  else {
    // Commit transaction

    rc = DBTransaction::Rollback();

    messenger->Register(wxT("Não foi possível excluir as informações sobre a pessoa."), LogMessage::MSG_ERROR);
  }

  event.Skip();
}


BEGIN_EVENT_TABLE(PanelPersonEdit, wxScrolledWindow)
  EVT_TOOL(PANEL_PERSON_EDIT_BTN_SAVE_ID, PanelPersonEdit::OnSaveRequest)
  EVT_TOOL(PANEL_PERSON_EDIT_BTN_DELETE_ID, PanelPersonEdit::OnDeleteRequest)
  EVT_CHAR_HOOK(PanelPersonEdit::OnCharHook)
  EVT_BUTTON(PANEL_PERSON_EDIT_BUTTON_BORN_DATE_CHOOSE_ID, PanelPersonEdit::OnBornDateChooseRequest)
  EVT_BUTTON(PANEL_PERSON_EDIT_TEDIT_RG_DATE_ID, PanelPersonEdit::OnRGDateChooseRequest)
  EVT_CHOICE(PANEL_PERSON_EDIT_CHOICE_PERSON_TYPE_ID, PanelPersonEdit::OnPersonTypeChange)
  EVT_BUTTON(PANEL_PERSON_EDIT_BTN_PHOTO_CHOOSE_ID, PanelPersonEdit::OnPhotoChooseRequest)
END_EVENT_TABLE()