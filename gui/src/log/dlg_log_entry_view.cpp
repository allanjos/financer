/**
 * Log entry view dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_log_entry_view.hpp"
#include "pixmaps/delete_16x16.xpm"


LogEntryViewDialog::LogEntryViewDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           Lang::Get("Message"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxWANTS_CHARS)
{
  SetSize(600, 400);

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);


  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(wxID_ANY,
                   Lang::Get("Delete"),
                   wxBitmap(delete_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);

  wxScrolledWindow *scrolledPanel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS);

  scrolledPanel->SetScrollbars(1, 1, 20, 20);

  szMargin->Add(scrolledPanel, 1, wxALL | wxEXPAND | wxGROW, 0);

  wxBoxSizer *szPanel = new wxBoxSizer(wxVERTICAL);
  scrolledPanel->SetSizer(szPanel);


  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szPanel->Add(szTop, 1, wxALL | wxEXPAND, GUI::WINDOW_MARGIN_SIZE);


  wxFlexGridSizer *szgMainFields = new wxFlexGridSizer(0, 2, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szTop->Add(szgMainFields, 1, wxALL|wxEXPAND|wxGROW, 0);

  wxStaticText *label = new wxStaticText(scrolledPanel, wxID_STATIC, Lang::Get("Type") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 0);

  labelTypeName = new wxStaticText(scrolledPanel, wxID_STATIC, wxT(""));
  szgMainFields->Add(labelTypeName, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

  label = new wxStaticText(scrolledPanel, wxID_STATIC, Lang::Get("Message") + wxT(": "));
  szgMainFields->Add(label, 0, wxALL | wxALIGN_TOP | wxALIGN_RIGHT, 0);

  teditMessage = new wxTextCtrl(scrolledPanel, wxID_STATIC, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_WORDWRAP | wxTE_MULTILINE | wxTE_READONLY);
  szgMainFields->Add(teditMessage, 1, wxALL | wxEXPAND | wxGROW, 0);

  szgMainFields->AddGrowableCol(1);
  szgMainFields->AddGrowableRow(1);


  // Close button

  wxButton *buttonClose = new wxButton(this, wxID_CANCEL, Lang::Get("Close"));

  szMargin->Add(buttonClose, 0, wxALIGN_RIGHT | wxALL, GUI::WINDOW_MARGIN_SIZE);

  buttonClose->Connect(wxID_CANCEL,
                       wxCommandEventHandler(LogEntryViewDialog::OnCloseRequest),
                       NULL,
                       this);

  // Center

  this->Centre();
}


void LogEntryViewDialog::Terminate()
{
  Close();

  statusBar->Destroy();

  Destroy();
}


int LogEntryViewDialog::LoadData(int id)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  //int count = messenger->messages.GetCount();

  wxString messageType = wxT("");
  wxColor textColor = wxT("");
    
  switch (messenger->messages.Item(id).GetType()) {
    case LogMessage::MSG_ERROR:

      messageType = Lang::Get("Error");
      textColor = wxColor(0xff, 0x00, 0x00);

      break;

    case LogMessage::MSG_WARN:

      messageType = Lang::Get("Warning");
      textColor = wxColor(0x00, 0x00, 0x70);

      break;

    case LogMessage::MSG_DBG:

      messageType = Lang::Get("Debug");
      textColor = wxColor(0x00, 0x4b, 0x00);

      break;

    default:

      messageType = Lang::Get("Information");
      textColor = wxColor(0x00, 0x00, 0x00);

      break;
  }

  labelTypeName->SetLabel(messageType);
  labelTypeName->SetForegroundColour(textColor);

  labelTypeName->Layout();
  labelTypeName->GetContainingSizer()->Layout();

  teditMessage->SetValue(messenger->messages.Item(id).GetMessage());

  teditMessage->Layout();
  teditMessage->GetContainingSizer()->Layout();

  return 0;
}


void LogEntryViewDialog::OnCloseRequest(wxCommandEvent &event)
{
  Close();
}


BEGIN_EVENT_TABLE(LogEntryViewDialog, wxDialog)
END_EVENT_TABLE()
