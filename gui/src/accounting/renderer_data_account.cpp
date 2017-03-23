#include "renderer_data_account.hpp"


// AccountDataRenderer

AccountDataRenderer::AccountDataRenderer(wxDataViewCellMode mode, int alignment) :
       wxDataViewCustomRenderer( wxString("long"), mode, alignment )
{
  m_height = 25;
}


bool AccountDataRenderer::Render( wxRect rect, wxDC *dc, int WXUNUSED(state) )
{
  dc->SetBrush(*wxRED_BRUSH);

  dc->SetPen(*wxTRANSPARENT_PEN);

  dc->DrawRectangle(rect);

  return true;
}


bool AccountDataRenderer::Activate(wxRect WXUNUSED(cell),
                      wxDataViewModel *WXUNUSED(model), const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col))
{
  wxLogMessage(wxT("AccountDataRenderer Activate()"));

  return false;
}


bool AccountDataRenderer::LeftClick(wxPoint cursor, wxRect WXUNUSED(cell),
                       wxDataViewModel *WXUNUSED(model), const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col))
{
  wxLogMessage(wxT("AccountDataRenderer LeftClick( %d, %d )"), cursor.x, cursor.y);

  return false;
}


wxSize AccountDataRenderer::GetSize() const
{
  //return wxSize(60,m_height);
  return wxSize(60, 20);
}


bool AccountDataRenderer::SetValue(const wxVariant &value)
{
  m_height = value;

  return true;
}


bool AccountDataRenderer::GetValue(wxVariant &WXUNUSED(value)) const
{
  return true;
}
