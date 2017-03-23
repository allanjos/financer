#ifndef _CANVAS_GRAPH_INCOME_EXPENSE_HPP_
#define _CANVAS_GRAPH_INCOME_EXPENSE_HPP_


#include "wx/hashmap.h"
#include "wx/dcgraph.h"
#include "wx/dynarray.h"
#include "utils_include.hpp"

#define GRAPH_INCOME_EXPENSE_CANVAS_HEIGHT 140

class IncomeExpenseGraphValue {
  public:
    int month;

    int year;

    double value;
};


WX_DECLARE_EXPORTED_OBJARRAY(IncomeExpenseGraphValue, IncomeExpenseGraphValueArray);


//WX_DECLARE_HASH_MAP(wxString, double, wxDoubleHash, wxStringEqual, GraphIncomeExpenseHash);

WX_DECLARE_HASH_MAP(wxString,      // type of the keys
                    double,  // any type you like
                    wxStringHash,  // hasher
                    wxStringEqual, // key equality predicate
                    GraphIncomeExpenseHash); // name of the class


class GraphIncomeExpenseCanvas: public wxScrolledWindow
{
public:
    GraphIncomeExpenseCanvas(wxWindow *parent);

    GraphIncomeExpenseCanvas(wxWindow *parent, GraphIncomeExpenseHash &graphIncomeExpenseHash);

    void SetYear(int year);

    int GetGraphInfo();

private:
    int year;

    GraphIncomeExpenseHash graphIncomeExpenseHash;

    GraphIncomeExpenseHash expenseMonthlyHash;

    IncomeExpenseGraphValueArray expenseMonthlyArray;

    IncomeExpenseGraphValueArray incomeMonthlyArray;

    double incomeExpenseMonthlyMax;

    int borderWidth;

    int monthRepresentationWidth;
    
    int marginTop;
    int marginBottom;
    int marginLeft;
    int marginRight;

    int x0;
    int xMax;

    int y0;
    int yMax;

    int          m_show;
    wxBitmap     m_smile_bmp;
    wxIcon       m_std_icon;

    int GraphGetY(int y);

    int GraphGetX(int x);

    int GraphMonthGraphExpense(wxDC *dc, int expense, int index);

    void OnPaint(wxPaintEvent &event);


    DECLARE_EVENT_TABLE()
};


#endif /* _CANVAS_GRAPH_INCOME_EXPENSE_HPP_ */