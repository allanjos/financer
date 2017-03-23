/**
 * Implement Graphical User Interface high level functions.
 *
 * \version 1.0
 * \since 05/05/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include "gui.hpp"

const int GUI::WINDOW_MARGIN_SIZE = 15;

const int GUI::FORM_MARGIN = 20;
const int GUI::FORM_INDENT_WIDTH = 10;

const int GUI::FIELDS_SPACING_HORIZONTAL = 5;
const int GUI::FIELDS_SPACING_VERTICAL = 10;
const int GUI::FIELDS_SPACING_SMALLER_HORIZONTAL = 3;
const int GUI::FIELDS_SPACING_SMALLER_VERTICAL = 3;
const int GUI::LABEL_MARGIN = 10;
const wxColor GUI::COLOR_LABEL_INFORMATION = wxColor(0x55, 0x55, 0x55);

int GUI::ListSetRowBgColor(wxGrid *grid, int row) {
  if (row % 2 == 0) {
    for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
      grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_EVEN);
    }
  }
  else {
    for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
      grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_ODD);
    }
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex) {
  if (itemIndex % 2 == 0) {
    listCtrl->SetItemBackgroundColour(itemIndex, GUI_COLOR_LIST_ITEM_EVEN);
  }
  else {
    listCtrl->SetItemBackgroundColour(itemIndex, GUI_COLOR_LIST_ITEM_ODD);
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxGrid *grid, int row, wxColour color) {
  for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
    grid->SetCellBackgroundColour(row, colIndex, color);
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, wxColour color) {
  listCtrl->SetItemBackgroundColour(itemIndex, color);

  return 0;
}

int GUI::ListSetRowBgColor(wxGrid *grid, int row, int state) {
  if (state > 0) {
    if (row % 2 == 0) {
      for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
        grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_EVEN);
      }
    }
    else {
      for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
        grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_ODD);
      }
    }
  }
  else {
    for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
      grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_DISABLED);
      grid->SetCellTextColour(row, colIndex, GUI_COLOR_FONT_DISABLED);
    }
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, int state)
{
  if (state >= 0) {
    GUI::ListSetRowBgColor(listCtrl, itemIndex);
  }
  else {
    GUI::ListSetRowBgColor(listCtrl, itemIndex, GUI_COLOR_LIST_ITEM_DISABLED);
  }

  return 0;
}

wxString GUI::ListGetItemText(wxListCtrl *listCtrl, int line, int col)
{
  wxListItem listItem;
  
  listItem.SetId(line);
  listItem.SetColumn(col);
  listItem.SetMask(wxLIST_MASK_TEXT);

  listCtrl->GetItem(listItem);

  return listItem.GetText();
}

int GUI::DrawTitle(wxDialog *windowParent,
                   const wxString &title,
                   const wxString &subTitle,
                   wxBitmap bmpTitle)
{
  wxBoxSizer *szTop = (wxBoxSizer *) windowParent->GetSizer();

  //
  // Main title
  //
  wxPanel *panelTitle = new wxPanel(windowParent, wxID_ANY);

  szTop->Add(panelTitle, 0, wxALL | wxEXPAND, 0);

  panelTitle->SetBackgroundColour(wxColour(0x54, 0x83, 0x64));

  wxBoxSizer *szTitle = new wxBoxSizer(wxHORIZONTAL);

  panelTitle->SetSizer(szTitle);


  if (bmpTitle.IsOk()) {
    wxStaticBitmap *sbmpTitle = new wxStaticBitmap(panelTitle, wxID_STATIC, bmpTitle);

    szTitle->Add(sbmpTitle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
  }

  // Title line sizer

  wxBoxSizer *szTitleRows = new wxBoxSizer(wxVERTICAL);
  szTitle->Add(szTitleRows, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

  // Title

  wxStaticText* label = new wxStaticText(panelTitle, wxID_STATIC, title);

  label->SetForegroundColour(*wxWHITE);

  wxFont font = label->GetFont();

  font.SetPointSize(font.GetPointSize() + 1);
  font.Bold();

  label->SetFont(font);

  szTitleRows->Add(label, 0, wxALL | wxALIGN_TOP | wxEXPAND, 2);

  // Subtitle

  if (subTitle.Length() > 0) {
    label = new wxStaticText(panelTitle,
                             wxID_STATIC,
                             subTitle,
                             wxDefaultPosition,
                             wxDefaultSize,
                             0);
    szTitleRows->Add(label, 0, wxALL | wxALIGN_TOP | wxEXPAND, 2);

    label->SetForegroundColour(wxColour(0x33, 0x33, 0x33));
  }

  wxPanel *panel = new wxPanel(windowParent, wxID_ANY, wxDefaultPosition, wxSize(-1, 1));
  panel->SetBackgroundColour(wxColour(0x08, 0x34, 0x08));
  szTop->Add(panel, 0, wxALL | wxEXPAND, 0);

  panel = new wxPanel(windowParent, wxID_ANY, wxDefaultPosition, wxSize(-1, 1));
  panel->SetBackgroundColour(wxColour(0x13, 0x47, 0x13));
  szTop->Add(panel, 0, wxALL | wxEXPAND, 0);

  windowParent->SetTitle(title);

  return 0;
}

wxSize GUI::GetDefaultWindowSize(wxWindow *window)
{
  if (!window) {
    return wxSize(-1, -1);
  }

  if (!window->GetParent()) {
    return wxSize(790, 580);
  }

  return wxSize(window->GetParent()->GetSize().GetWidth() - 20,
         window->GetParent()->GetSize().GetHeight() - 30);
}

void GUI::FormatSectionLabel(wxStaticText *label)
{
  wxFont font = label->GetFont();

  font.MakeBold();
  font.SetPointSize(font.GetPointSize() + 1);

  label->SetFont(font);

  label->SetMinSize(wxSize(-1, label->GetSize().GetHeight() + 3));
}

void GUI::FormatSubSectionLabel(wxStaticText *label)
{
  wxFont font = label->GetFont();

  //label->SetForegroundColour(wxColour(0x59, 0x61, 0x61));

  font.MakeBold();
  //font.SetPointSize(font.GetPointSize() + 1);

  label->SetFont(font);
}

ContainerIntData::ContainerIntData(int value) {
  this->value = value;
}

int ContainerIntData::GetData() {
  return this->value;
}