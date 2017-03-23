/**
 * New product dialog.
 *
 * \file  panel_person_new.cpp
 * \version 1.0
 * \since 08/19/2007 10:50:12 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_image.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "panel_person_main.hpp"
#include "panel_person_new.hpp"
#include "dlg_main.hpp"
#include "app.hpp"
#include "dlg_calendar.hpp"
#include "pixmaps/search_16x13.xpm"
#include "pixmaps/image_16x16.xpm"
#include "pixmaps/calendar_16x16.xpm"
#include "pixmaps/photo.xpm"
#include "pixmaps/save_16x16.xpm"


PanelPersonNew::PanelPersonNew(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

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

  toolBar->AddTool(PANEL_PERSON_NEW_BTN_SAVE_ID, Lang::Get("Save"), wxBitmap(save_16x16_xpm));

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
                                                       PANEL_PERSON_NEW_BTN_PHOTO_CHOOSE_ID,
                                                       wxBitmap(image_16x16_xpm));
  szRow->Add(btnPictureChose, 0, wxALL | wxALIGN_CENTER_VERTICAL, 1);

  btnPictureChose->SetToolTip(Lang::Get("Select a picture"));

  szPicture->Add(new wxStaticLine(panelPicture), 0, wxALL|wxEXPAND|wxGROW, 0);

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

  labelFullName = new wxStaticText(this, wxID_STATIC, Lang::Get("Name") + wxT(": "));
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

  choicePersonNature = new wxChoice(panelMain, PANEL_PERSON_NEW_CHOICE_PERSON_TYPE_ID);
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
                                       PANEL_PERSON_NEW_TEDIT_RG_DATE_ID,
                                       wxBitmap(calendar_16x16_xpm));
  szRow->Add(btnRgDateChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnRgDateChoose->SetMinSize(wxSize(-1, teditRGExpedicaoDate->GetSize().GetHeight()));

  // Country

  labelRgCountry = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Country") + wxT(": "));
  szGridRG->Add(labelRgCountry, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditRgCountry = new wxTextCtrl(panelMain, PANEL_PERSON_NEW_CHOICE_RG_COUNTRY_ID);
  szGridRG->Add(teditRgCountry, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 1);

  // Registered state

  labelRgFederalState = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("State") + wxT(": "));
  szGridRG->Add(labelRgFederalState, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditRGState = new wxTextCtrl(panelMain, wxID_ANY, wxT(""));
  szGridRG->Add(teditRGState, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 1);

  // Registered city

  labelRgCity = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("City") + wxT(": "));
  szGridRG->Add(labelRgCity, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditRgCity = new wxTextCtrl(panelMain, wxID_ANY, wxT(""));
  szGridRG->Add(teditRgCity, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 1);


  szGridRG->AddGrowableCol(1);


  // Born date

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Date of birth") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szgMainFields->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 1);

  teditBornDate = new wxTextCtrl(panelMain, PANEL_PERSON_NEW_TEDIT_BORN_DATE_ID, wxT(""), wxDefaultPosition, wxSize(80, -1));
  szRow->Add(teditBornDate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnBornDateChoose = new wxBitmapButton(panelMain,
                                         PANEL_PERSON_NEW_BUTTON_BORN_DATE_CHOOSE_ID,
                                         wxBitmap(calendar_16x16_xpm));
  szRow->Add(btnBornDateChoose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  btnBornDateChoose->SetMinSize(wxSize(-1, teditBornDate->GetSize().GetHeight()));

  szRow->AddSpacer(50);

  // Tipo

  label = new wxStaticText(panelMain, wxID_STATIC, Lang::Get("Gender") + wxT(": "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxSex = new wxChoice(panelMain, wxID_ANY);
  szRow->Add(cboxSex, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  cboxSex->Append(Lang::Get("Not specified"), (wxClientData *) new ContainerIntData((int) 0));
  cboxSex->Append(Lang::Get("Male"), (wxClientData *) new ContainerIntData((int) 'M'));
  cboxSex->Append(Lang::Get("Female"), (wxClientData *) new ContainerIntData((int) 'F'));

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

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("E-mail") + wxT(": "));
  szgWebFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditEmail = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300, -1));
  szgWebFields->Add(teditEmail, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Home page

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
  szRow->Add(teditAddressStreet, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Number

  label = new wxStaticText(panelContact, wxID_STATIC, wxT("     ") + Lang::Get("Number") + wxT(": "));
  szRow->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  teditAddressNumber = new wxTextCtrl(panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50, -1));
  szRow->Add(teditAddressNumber, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  // Complement

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Complement") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  teditAddressComplement = new wxTextCtrl(panelContact, wxID_ANY, wxT(""));
  szgLocationFields->Add(teditAddressComplement, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

  // Country

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("Country") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  teditAddressCountry = new wxTextCtrl(panelContact, PANEL_PERSON_NEW_CHOICE_ADDRESS_COUNTRY_ID);
  szgLocationFields->Add(teditAddressCountry, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

  // Registered state

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("State") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  teditAddressState = new wxTextCtrl(panelContact, PANEL_PERSON_NEW_CHOICE_ADDRESS_UF_ID);
  szgLocationFields->Add(teditAddressState, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

  // Registered city

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("City") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  teditAddressCity = new wxTextCtrl(panelContact, wxID_ANY, wxT(""));
  szgLocationFields->Add(teditAddressCity, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

  // Quarter

  label = new wxStaticText(panelContact, wxID_STATIC, Lang::Get("District") + wxT(": "));
  szgLocationFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 1);

  teditAddressDistrict = new wxTextCtrl(panelContact, wxID_ANY, wxT(""));
  szgLocationFields->Add(teditAddressDistrict, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);


  szgLocationFields->AddGrowableCol(1);


  // Notes about person

  wxScrolledWindow *panelNotes = new wxScrolledWindow(notebook, wxID_ANY);

  notebook->AddPage(panelNotes, Lang::Get("Notes"), false);

  panelNotes->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szMarginNotes = new wxBoxSizer(wxVERTICAL);
  panelNotes->SetSizer(szMarginNotes);

  wxBoxSizer *szNotes = new wxBoxSizer(wxVERTICAL);
  szMarginNotes->Add(szNotes, 1, wxALL|wxEXPAND|wxGROW, GUI::WINDOW_MARGIN_SIZE);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szNotes->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Notes field

  teditNotes = new wxTextCtrl(panelNotes, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  szRow->Add(teditNotes, 1, wxALL|wxEXPAND|wxGROW, 1);

  this->CenterOnParent();


  SetClientAsPerson(true);

  teditFullName->SetFocus();
}


void PanelPersonNew::Terminate()
{
  Close();

  Destroy();
}


void PanelPersonNew::OnSaveRequest(wxCommandEvent &event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  if (teditFullName->GetValue().Length() < 1) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(Lang::Get("The highlighted fields are required") + wxT("."));

    return;
  }


  DBQuery dbquery;
  bool registeringError = false;
  wxString errorMessageStr = wxT("");


  // Init transaction

  int rc = DBTransaction::Begin();

  if (rc != 0) {
    registeringError = true;

    errorMessageStr = wxT("Erro interno do banco: ") + dbquery.GetMessageStr();
  }

  if (registeringError) {
    wxMessageBox(wxT("Não foi possível iniciar a transação."));

    return;
  }

  std::vector<wxString> fields, params, valueModifiers;
  long personId = -1;

  // Person name

  fields.push_back(wxT("nome"));

  params.push_back(teditFullName->GetValue());

  valueModifiers.push_back("");

  // Person nature: (P) person, (J) enterprise / juridical

  if (choicePersonNature->GetClientObject(choicePersonNature->GetSelection()) != NULL) {
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

  if (teditRGState->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_state"));

    params.push_back(teditRGState->GetValue());

    valueModifiers.push_back("");
  }

  if (teditRgCountry->GetValue().Length() > 0) {
    fields.push_back(wxT("identity_country"));

    params.push_back(teditRgCountry->GetValue());

    valueModifiers.push_back("");
  }

  if (cboxSex->IsEnabled() &&
      cboxSex->GetCount() > 0 &&
      cboxSex->GetClientObject(cboxSex->GetSelection())) {
    fields.push_back(wxT("gender"));

    ContainerIntData *container = (ContainerIntData *) cboxSex->GetClientObject(cboxSex->GetSelection());

    params.push_back(wxString::Format(wxT("%c"), container->GetData()));

    valueModifiers.push_back("");
  }

  if (teditFantasyName->IsEnabled() && teditFantasyName->GetValue().Length() > 0) {
    fields.push_back(wxT("trading_name"));

    params.push_back(teditFantasyName->GetValue());

    valueModifiers.push_back("");
  }

  if (teditInscricaoEstadual->IsEnabled() && teditInscricaoEstadual->GetValue().Length() > 0) {
    fields.push_back(wxT("state_registration"));

    params.push_back(teditInscricaoEstadual->GetValue());

    valueModifiers.push_back("");
  }

  if (teditCPF_CNPJ->GetValue().Length() > 0) {
    fields.push_back(wxT("security_social_number"));

    params.push_back(teditCPF_CNPJ->GetValue());

    valueModifiers.push_back("");
  }

  if (teditBornDate->GetValue().Length() > 0) {
    fields.push_back(wxT("birth_date"));

    params.push_back(teditBornDate->GetValue());

    valueModifiers.push_back("to_date($@, 'DD/MM/YYYY')");
  }

  if (teditEmail->GetValue().Length() > 0) {
    fields.push_back(wxT("email"));

    params.push_back(teditEmail->GetValue());

    valueModifiers.push_back("");
  }
 
  if (teditHomePage->GetValue().Length() > 0) {
    fields.push_back(wxT("homepage"));

    params.push_back(teditHomePage->GetValue());

    valueModifiers.push_back("");
  }


  wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("person"), fields, valueModifiers);

  rc = dbquery.Execute(sql, params);

  if (rc != 0) {
    registeringError = true;

    messenger->Register(wxT("DB: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }


  // Get the person inserted ID from the database

  if (!registeringError) {
    personId = DBUtils::GetSerialKey(DB::GetTableName("person"), "person_id");

    if (personId < 1) {
      registeringError = true;

      messenger->Register(wxT("Não foi possível obter a identificação da pessoa cadastrada."), LogMessage::MSG_ERROR);

      messenger->Register(wxT("DB: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
    }
  }

  // Inserting person address

  if (!registeringError) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%ld"), personId));

    // Address information

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


    wxString sql = DBUtils::MountSQLInsert(DB::GetTableName("person.address"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      messenger->Register(wxT("Erro ao tentar cadastrar o endereço da pessoa."), LogMessage::MSG_ERROR);

      messenger->Register(wxT("DB: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
    }
  }


  // Registering contact information

  /*
  // Business phone

  if (!registeringError &&
      (teditPhoneBusiness->GetValue().Length() > 0 || teditPhoneBusinessRamal->GetValue().Length() > 0 ||
       teditPhoneBusinessContactName->GetValue().Length() > 0)) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%ld"), personId));

    fields.push_back(wxT("tipo"));
    params.push_back(wxT("C"));;

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

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.telefone"), fields);

    // Executing database command

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr = (wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }

  // Home phone

  if (!registeringError &&
      (teditPhoneHome->GetValue().Length() > 0 || teditPhoneHomeContactName->GetValue().Length() > 0)) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%ld"), personId));

    fields.push_back(wxT("tipo"));
    params.push_back(wxT("P"));

    if (teditPhoneHome->GetValue().Length() > 0) {
      fields.push_back(wxT("numero"));
      params.push_back(teditPhoneHome->GetValue());
    }

    if (teditPhoneHomeContactName->GetValue().Length() > 0) {
      fields.push_back(wxT("contact_name"));
      params.push_back(teditPhoneHomeContactName->GetValue());
    }

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.telefone"), fields);

    // Executing database command

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr = (wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }

  // Fax

  if (!registeringError &&
      (teditPhoneFax->GetValue().Length() > 0 || teditPhoneFaxRamal->GetValue().Length() > 0)) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("person_id"));
    params.push_back(wxString::Format(wxT("%ld"), personId));

    fields.push_back(wxT("tipo"));
    params.push_back(wxT("F"));


    if (teditPhoneFax->GetValue().Length() > 0) {
      fields.push_back(wxT("numero"));
      params.push_back(teditPhoneFax->GetValue().mb_str(wxConvUTF8));
    }

    if (teditPhoneFaxRamal->GetValue().Length() > 0) {
      fields.push_back(wxT("ramal"));
      params.push_back(teditPhoneFaxRamal->GetValue());
    }

    wxString sql = DBUtils::MountSQLInsert(wxT("pessoa.telefone"), fields);

    rc = dbquery.Execute(sql, params);

    if (rc != 0) {
      registeringError = true;

      errorMessageStr = (wxT("Erro interno do banco: ") + dbquery.GetMessageStr());
    }
  }
  */

  // Person picture
#if 0
  if (!registeringError) {
    if (photoFilePath.Length() > 0) {
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

      sprintf(sqlCommandAnsi, "INSERT INTO pessoa.imagem (person_id, file_mime, file_content) VALUES (%ld, $1, $2)",
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
  

  // Registering client information

  /*
  // Registering notes

  if (!registeringError && teditNotes->GetValue().Length() > 0) {
    fields.clear();
    params.clear();

    fields.push_back(wxT("cliente_id"));
    params.push_back(wxString::Format(wxT("%ld"), clientId));

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
  */

  // Successfully registering

  if (!registeringError) {
    // Commit transaction

    rc = DBTransaction::Commit();

    //wxMessageBox(wxT("Pessoa cadastrada com sucesso."));

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    // Reload main panel data

    PanelPersonMain *panelPersonMain = (PanelPersonMain *) mainFrame->GetPage(Lang::Get("Contacts"));

    if (panelPersonMain) {
      panelPersonMain->LoadData();
    }

    // Close panel

    wxString pageTitle = Lang::Get("New contact");

    mainFrame->PageRemove(pageTitle);
  }
  else {
    messenger->Register(wxT("DB: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);

    // Rollback transaction

    rc = DBTransaction::Rollback();

    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->SetStatusText(Lang::Get("Cannot register contact") + wxT("."));
  }
}


void PanelPersonNew::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == 341) {
    /*
    ProductTypeNewDialog productTypeNewDialog(this);

    productTypeNewDialog.ShowModal();

    LoadList();
    */
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


void PanelPersonNew::OnBornDateChooseRequest(wxCommandEvent &event)
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


void PanelPersonNew::OnRGDateChooseRequest(wxCommandEvent &event)
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


void PanelPersonNew::SetClientAsPerson(bool isPerson)
{
  if (isPerson) {
    labelFullName->SetLabel(Lang::Get("Name") + wxT(": "));
    labelFullName->GetContainingSizer()->Layout();
    szFullName->Layout();

    lblCpfCnpj->SetLabel(Lang::Get("Individual Taxpayer Registration") + wxT(": "));
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
    teditRgCountry->Enable();
    labelRgFederalState->Enable();
    teditRGState->Enable();
    labelRgCity->Enable();
    teditRgCity->Enable();
    btnRgDateChoose->Enable();

    cboxSex->Enable();
  }
  else {
    labelFullName->SetLabel(Lang::Get("Corporate name") + wxT(": "));
    labelFullName->GetContainingSizer()->Layout();
    szFullName->Layout();

    lblCpfCnpj->SetLabel(Lang::Get("Tax Corporation Registration") + wxT(": "));
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
    teditRGState->Disable();
    labelRgCity->Disable();
    teditRgCity->Disable();
    btnRgDateChoose->Disable();

    cboxSex->Disable();
  }
}


void PanelPersonNew::OnPersonTypeChange(wxCommandEvent &event)
{
  if (choicePersonNature->GetSelection() >= 0 &&
      choicePersonNature->GetClientObject(choicePersonNature->GetSelection()) &&
      ((ContainerIntData *) choicePersonNature->GetClientObject(choicePersonNature->GetSelection()))->GetData() == (int) 'F') {
    SetClientAsPerson(true);
  }
  else {
    SetClientAsPerson(false);
  }
}


void PanelPersonNew::OnPhotoChooseRequest(wxCommandEvent &event)
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

    UtilsImage::Rescale(imagePhoto, 120, 500);

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

      Refresh();
      Update();
    }
  }
}


BEGIN_EVENT_TABLE(PanelPersonNew, wxScrolledWindow)
  EVT_TOOL(PANEL_PERSON_NEW_BTN_SAVE_ID, PanelPersonNew::OnSaveRequest)
  EVT_CHAR_HOOK(PanelPersonNew::OnCharHook)
  EVT_BUTTON(PANEL_PERSON_NEW_BUTTON_BORN_DATE_CHOOSE_ID, PanelPersonNew::OnBornDateChooseRequest)
  EVT_BUTTON(PANEL_PERSON_NEW_TEDIT_RG_DATE_ID, PanelPersonNew::OnRGDateChooseRequest)
  EVT_CHOICE(PANEL_PERSON_NEW_CHOICE_PERSON_TYPE_ID, PanelPersonNew::OnPersonTypeChange)
  EVT_BUTTON(PANEL_PERSON_NEW_BTN_PHOTO_CHOOSE_ID, PanelPersonNew::OnPhotoChooseRequest)
END_EVENT_TABLE()