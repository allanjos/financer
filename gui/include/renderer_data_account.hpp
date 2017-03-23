/**
 * \brief Accounts main dialog.
 *
 * \file  dlg_account_main.hpp
 * \version 1.0
 * \since 14/11/2008
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _RENDERER_DATA_ACCOUNT_HPP_
#define _RENDERER_DATA_ACCOUNT_HPP_


#include "utils_include.hpp"
#include "model_account_data.hpp"


class AccountDataRenderer: public wxDataViewCustomRenderer
{
  public:
    AccountDataRenderer(wxDataViewCellMode mode, int alignment);

    virtual bool Render(wxRect rect, wxDC *dc, int WXUNUSED(state));

    virtual bool Activate(wxRect WXUNUSED(cell),
                          wxDataViewModel *WXUNUSED(model), const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col));

    virtual bool LeftClick(wxPoint cursor, wxRect WXUNUSED(cell),
                           wxDataViewModel *WXUNUSED(model), const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col));

    virtual wxSize GetSize() const;

    virtual bool SetValue(const wxVariant &value);

    virtual bool GetValue(wxVariant &WXUNUSED(value)) const;

  private:
    long m_height;
};


#endif  /* _RENDERER_DATA_ACCOUNT_HPP_ */
