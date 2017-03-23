/**
 * About dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include "dlg_about.hpp"
#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "app_version.hpp"
#include "pixmaps/close_16x16.xpm"
#include "pixmaps/icon.xpm"

AboutDialog::AboutDialog(wxWindow *parent):
  wxDialog(parent, wxID_ANY, "")
{
  SetTitle(Lang::Get("Application information"));

  SetSize(wxSize(440, 300));

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  // Title

  GUI::DrawTitle(this,
                 wxT(APP_NAME),
                 wxT(""),
                 wxBitmap(icon_xpm));

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, 10);


  // Application description

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, Lang::Get("App_description") + ".");
  szTop->Add(label, 0, wxBOTTOM, 15);

  wxFlexGridSizer *szFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szTop->Add(szFields, 1, wxALL, 0);

  // Application description

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Homepage") + wxT(": "));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(APP_URL));
  szFields->Add(label, 0, wxALL, 1);

  // Application version

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Version") + wxT(": "));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(APP_VERSION));
  szFields->Add(label, 0, wxALL, 1);

  // Build information

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Build") + wxT(": "));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(__DATE__ " " __TIME__));
  szFields->Add(label, 0, wxALL, 1);

  // Application description

  label = new wxStaticText(this, wxID_STATIC, Lang::Get("Author") + wxT(": "));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(APP_OWNER));
  szFields->Add(label, 0, wxALL, 1);

  szTop->AddSpacer(3);

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxALIGN_RIGHT, 3);

  wxBitmapButton *btnClose = new wxBitmapButton(this, wxID_CANCEL, wxBitmap(close_16x16_xpm),
                                                wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
  szRow->Add(btnClose, 0, wxALL | wxALIGN_BOTTOM, 1);


  //Fit();

  Centre();
}


void AboutDialog::Terminate()
{
  Close();

  //statusBar->Destroy();

  Destroy();
}


void AboutDialog::OnRightClick(wxMouseEvent& event)
{
  Terminate();
}


BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
  EVT_RIGHT_UP(AboutDialog::OnRightClick)
  EVT_LEFT_UP(AboutDialog::OnRightClick)
END_EVENT_TABLE()