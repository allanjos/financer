/**
 * About dialog.
 *
 * \file  dlg_about.cpp
 * \version 1.0
 * \since 01/29/2007 10:50:12 AM AMT
 * \revision  none
 * \author: Allann Jones
 */


#include "utils_include.hpp"
#include "panel_guide.hpp"


PanelGuide::PanelGuide(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS),
  embeddedHtmlHelp(wxHF_EMBEDDED|wxHF_DEFAULT_STYLE)
{
  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);

  SetBackgroundColour(*wxWHITE);



  Layout();

  GetParent()->Layout();



  LogMessenger *messenger = LogMessenger::GetInstance();

  wxString helpDirectory = wxStandardPaths::Get().GetExecutablePath();

  wxFileName executablePathInfo(helpDirectory);


  embeddedHelpWindow = new wxHtmlHelpWindow(this, wxID_ANY, wxDefaultPosition, wxSize(800, 600));

  embeddedHelpWindow->SetAutoLayout(true);

  // m_embeddedHtmlHelp.UseConfig(config, rootPath); // Can set your own config object here
  embeddedHtmlHelp.SetHelpWindow(embeddedHelpWindow);

  //embeddedHelpWindow->Create(this, wxID_ANY, wxDefaultPosition, wxSize(800, 600), wxTAB_TRAVERSAL|wxNO_BORDER, wxHF_DEFAULT_STYLE);

  wxString helpFilePath = executablePathInfo.GetPath() + wxFileName::GetPathSeparator() +  wxT("doc.zip");

  messenger->Register(wxT("Documentation path: ") + helpFilePath, LogMessage::MSG_INFO);

  //embeddedHtmlHelp.AddBook(wxFileName(_T("doc.zip")));
  embeddedHtmlHelp.AddBook(helpFilePath);

  embeddedHtmlHelp.DisplayContents();

  embeddedHtmlHelp.Display(0);


  //wxHtmlHelpController helpController;

  //embeddedHtmlHelp.Display(0);

  //embeddedHtmlHelp.DisplayIndex();

  //helpController.DisplayContents();

  //helpController.Display(_T("Introduction"));

  /*
  if (!advancedHtmlHelp.Initialize(_T("doc"))) {
    wxMessageBox(wxT("Não foi possível iniciar a ajuda da aplicação."), wxT("Erro"), wxICON_ERROR);
  }
  */

  /*
  wxString helpFilePath = executablePathInfo.GetPath() + wxFileName::GetPathSeparator() +  wxT("doc");

  wxMessageBox(helpFilePath);
  */

  /*
  //if (!helpController.Initialize(helpFilePath))
  if (!helpController.Initialize(_T("doc"))) {
    wxMessageBox(wxT("Não foi possível iniciar a ajuda da aplicação."), wxT("Erro"), wxICON_ERROR);
  }
  //helpController->Initialize(helpFilePath);
  */


  // Top sizer ending configuration.

  //szTop->SetSizeHints(this);

  Layout();

  GetParent()->Layout();


  //embeddedHelpWindow->SetSize(GetSize());


  embeddedHelpWindow->Layout();
}


void PanelGuide::Terminate()
{
  Close();

  Destroy();
}


void PanelGuide::OnResize(wxSizeEvent &event)
{
  embeddedHelpWindow->SetSize(GetSize());

  event.Skip();
}


BEGIN_EVENT_TABLE(PanelGuide, wxScrolledWindow)
  EVT_SIZE(PanelGuide::OnResize)
END_EVENT_TABLE()
