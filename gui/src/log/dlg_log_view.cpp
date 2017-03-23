/**
 * Log view dialog.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "gui_controls.hpp"
#include "dlg_log_view.hpp"
#include "dlg_log_entry_view.hpp"
#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/delete_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


LogViewDialog::LogViewDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           Lang::Get("Application execution messages"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxWANTS_CHARS)
{
  SetSize(800, 600);

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);


  // Toolbar

  toolBar = new wxToolBar(this,
                          wxID_ANY,
                          wxDefaultPosition,
                          wxDefaultSize,
                          wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_LOG_VIEW_CLEAR_ID,
                   Lang::Get("Clear log"),
                   wxBitmap(delete_16x16_xpm));

  toolBar->AddSeparator();

  // Choice - Event type

  wxStaticText* label = new wxStaticText(toolBar, wxID_STATIC, Lang::Get("Show") + ": ");

  toolBar->AddControl(label);

  choiceEventType = new wxChoice(toolBar, DLG_LOG_VIEW_CHOICE_EVENT_TYPE_ID);

  choiceEventType->Append(Lang::Get("All"), (wxClientData *) new ContainerIntData((int) 'T'));
  choiceEventType->Append(Lang::Get("Error"), (wxClientData *) new ContainerIntData((int) 'E'));
  choiceEventType->Append(Lang::Get("Warning"), (wxClientData *) new ContainerIntData((int) 'A'));
  choiceEventType->Append(Lang::Get("Information"), (wxClientData *) new ContainerIntData((int) 'I'));
  choiceEventType->Append(Lang::Get("Debug"), (wxClientData *) new ContainerIntData((int) 'D'));

  choiceEventType->SetSelection(0);

  toolBar->AddControl(choiceEventType);

  // Toolbar - Search controls

  toolBar->AddSeparator();

  label = new wxStaticText(toolBar, wxID_STATIC, Lang::Get("Filter") + ": ");

  toolBar->AddControl(label);

  teditSearch = new wxTextCtrl(toolBar, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

  teditSearch->SetMinSize(wxSize(150, 23));

  toolBar->AddControl(teditSearch);

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_LOG_VIEW_REFRESH_ID,
                   Lang::Get("Refresh"),
                   wxBitmap(refresh_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(wxID_CANCEL,
                   Lang::Get("Close"),
                   wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  // Margin sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);
  szTop->Add(szMargin, 1, wxALL | wxEXPAND, 10);

  listCtrlLog = new wxListCtrl(this,
                               DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
                               wxDefaultPosition,
                               wxSize(5, 5),
                               wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);

  szMargin->Add(listCtrlLog, 1, wxALL | wxEXPAND | wxGROW, 0);

  //wxImageList* imageList = new wxImageList(16, 16);
  //imageList->Add(wxIcon(income_16x16_xpm));
  //listCtrlLog->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  wxListItem itemColTime;
  itemColTime.SetText(Lang::Get("Time"));
  listCtrlLog->InsertColumn(0, itemColTime);
  listCtrlLog->SetColumnWidth(0, 120);

  wxListItem itemColName;
  itemColName.SetText(Lang::Get("Type"));
  listCtrlLog->InsertColumn(1, itemColName);
  listCtrlLog->SetColumnWidth(1, 90);

  wxListItem itemColType;
  itemColType.SetText(Lang::Get("Message"));
  listCtrlLog->InsertColumn(2, itemColType);
  listCtrlLog->SetColumnWidth(2, 650);


  // Center dialog on screen

  this->Centre();


  // Set focus initially on list control

  listCtrlLog->SetFocus();


  // Event handlers

  listCtrlLog->Connect(wxEVT_LIST_ITEM_ACTIVATED,
                       wxListEventHandler(LogViewDialog::OnListViewItemActivated),
                       NULL,
                       this);

  choiceEventType->Connect(wxEVT_CHOICE,
                           wxCommandEventHandler(LogViewDialog::OnEventTypeChoice),
                           NULL,
                           this);

  teditSearch->Connect(wxEVT_TEXT_ENTER,
                       wxCommandEventHandler(LogViewDialog::OnSearchEnter),
                       NULL,
                       this);

  Connect(wxEVT_SHOW,
          wxShowEventHandler(LogViewDialog::OnShow),
          NULL,
          this);

  LoadData();
}


void LogViewDialog::Terminate()
{
  Close();

  statusBar->Destroy();

  Destroy();
}


int LogViewDialog::LoadData(void)
{
  listCtrlLog->Freeze();

  listCtrlLog->DeleteAllItems();

  LogMessenger *messenger = LogMessenger::GetInstance();

  int count = messenger->messages.GetCount();

  for (int index = count - 1; index >= 0; index--) {
    //printf("index = %d\n", index);

    long itemIndex = listCtrlLog->GetItemCount();

    int typeId = messenger->messages.Item(index).GetType();

    time_t messageTime = messenger->messages.Item(index).GetTime();

    wxString message = messenger->messages.Item(index).GetMessage();

    wxString messageType = wxT("");
    wxColor textColor = wxT("");

    if (choiceEventType->GetStringSelection().Cmp(Lang::Get("All")) != 0) {
      if (choiceEventType->GetStringSelection().Cmp(Lang::Get("Error")) == 0 && typeId != LogMessage::MSG_ERROR)  {
        continue;
      }
      else if (choiceEventType->GetStringSelection().Cmp(Lang::Get("Warning")) == 0 && typeId != LogMessage::MSG_WARN)  {
        continue;
      }
      else if (choiceEventType->GetStringSelection().Cmp(Lang::Get("Information")) == 0 && typeId != LogMessage::MSG_INFO)  {
        continue;
      }
      else if (choiceEventType->GetStringSelection().Cmp(Lang::Get("Debug")) == 0 && typeId != LogMessage::MSG_DBG)  {
        continue;
      }
    }

    if (teditSearch->GetValue().Length() > 0) {
      wxRegEx regex;

      wxString regexStr = teditSearch->GetValue();

      if (!regex.Compile(regexStr, wxRE_EXTENDED | wxRE_ADVANCED | wxRE_ICASE) || !regex.Matches(message)) {
        continue;
      }
    }


    switch (typeId) {
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

    wxDateTime dateTime;

    dateTime.Set(messageTime);

    listCtrlLog->InsertItem(itemIndex, dateTime.Format("%d/%m/%Y %H:%M:%S"));

    listCtrlLog->SetItemData(itemIndex, index);

    listCtrlLog->SetItem(itemIndex, 1, messageType);

    listCtrlLog->SetItem(itemIndex, 2, message);

    // Text color

    listCtrlLog->SetItemTextColour(itemIndex, textColor);
  }

  listCtrlLog->Thaw();


  return 0;
}


void LogViewDialog::OnCloseRequest(wxCommandEvent& event)
{
  Close();
}


void LogViewDialog::OnListViewItemActivated(wxListEvent& event)
{
  wxListItem listItem = event.GetItem();

  int entryIndex = listItem.GetData();


  LogEntryViewDialog dialog(this);

  dialog.LoadData(entryIndex);

  dialog.ShowModal();
}


void LogViewDialog::OnSearchEnter(wxCommandEvent& event)
{
  LoadData();
}


void LogViewDialog::OnEventTypeChoice(wxCommandEvent& event)
{
  LoadData();
}


void LogViewDialog::OnClearLogRequest(wxCommandEvent& event)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Clear();

  LoadData();
}


void LogViewDialog::OnRefreshRequest(wxCommandEvent& event)
{
  LoadData();
}


void LogViewDialog::OnCharHook(wxKeyEvent& event)
{
  // Exit <ESC>
  switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
      Close();
      break;
  }

  event.Skip();
}


void LogViewDialog::OnShow(wxShowEvent& event)
{
  if (IsShown()) {
    LoadData();
  }
}


BEGIN_EVENT_TABLE(LogViewDialog, wxDialog)
  EVT_CHAR_HOOK(LogViewDialog::OnCharHook)
  EVT_TOOL(DLG_LOG_VIEW_CLEAR_ID, LogViewDialog::OnClearLogRequest)
  EVT_TOOL(DLG_LOG_VIEW_REFRESH_ID, LogViewDialog::OnRefreshRequest)
  EVT_TOOL(wxID_CANCEL, LogViewDialog::OnCloseRequest)
END_EVENT_TABLE()