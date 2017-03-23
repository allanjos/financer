#include "canvas_graph_income_expense.hpp"


#include "wx/arrimpl.cpp"
WX_DEFINE_EXPORTED_OBJARRAY(IncomeExpenseGraphValueArray);


GraphIncomeExpenseCanvas::GraphIncomeExpenseCanvas(wxWindow *parent)
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(0, 180),
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE) {
  this->GetGraphInfo();

  GraphIncomeExpenseCanvas(parent, graphIncomeExpenseHash);
}


GraphIncomeExpenseCanvas::GraphIncomeExpenseCanvas(wxWindow *parent, GraphIncomeExpenseHash &graphIncomeExpenseHash)
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(0, 180),
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
  this->year = wxDateTime::GetCurrentYear();

  this->graphIncomeExpenseHash = graphIncomeExpenseHash;

  this->GetGraphInfo();

  Layout();

  Refresh();

  Update();
}


void GraphIncomeExpenseCanvas::SetYear(int year)
{
  this->year = year;
}


int GraphIncomeExpenseCanvas::GraphGetY(int y)
{
  printf("y: %d\n", GetSize().GetHeight() - borderWidth - y);

  return (GetSize().GetHeight() - borderWidth - y);
}


int GraphIncomeExpenseCanvas::GraphGetX(int x)
{
  printf("x: %d\n", x + borderWidth);

  return (x + borderWidth);
}


int GraphIncomeExpenseCanvas::GraphMonthGraphExpense(wxDC *dc, int expense, int index)
{
  // Drawing properties

  wxColour clr(197, 229, 241);

  wxBrush brush(clr, wxBRUSHSTYLE_SOLID);

  dc->SetBrush(brush);

  dc->SetPen(wxPen(wxColour(75, 135, 166), 1, wxPENSTYLE_SOLID));

  // Drawing

  int monthY0;
  int monthHeight;

  if (expense > 0) {
    monthY0 = GraphGetY(expense);

    monthHeight = expense;
  }
  else {
    monthY0 = GraphGetY(0);

    monthHeight = (expense * -1);
  }

  dc->DrawRectangle(GraphGetX(0) + index * (monthRepresentationWidth - 1), monthY0, monthRepresentationWidth, monthHeight);

  return 0;
}


void GraphIncomeExpenseCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC pdc(this);

  //LogMessenger *messenger = LogMessenger::GetInstance();

  // Get drawing context

#if wxUSE_GRAPHICS_CONTEXT

#if TEST_CAIRO_EVERYWHERE
  wxGCDC gdc;

  gdc.SetGraphicsContext( gCairoRenderer->CreateContext( pdc ) );
#else
   wxGCDC gdc( pdc ) ;
#endif

  //wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#if wxUSE_GRAPHICS_CONTEXT
  wxDC &dc = (wxDC&) gdc;
#else
  wxDC &dc = (wxDC&) pdc;
#endif

#else

  wxDC &dc = pdc ;

#endif

  // Prepare for drawing

  PrepareDC(dc);

  GetParent()->PrepareDC(dc);

  // Dimensions

  int canvasHeight = GetSize().GetHeight();
  int canvasWidth = GetSize().GetWidth();

  // Month area width

  monthRepresentationWidth = 20;

  // Margins

  this->marginTop = 15;
  this->marginBottom = 30;
  this->marginLeft = 50;
  this->marginRight = 5;

  borderWidth = 10;

  // 

  x0 = borderWidth;
  xMax = canvasWidth - 2 * borderWidth;
  y0 = borderWidth;
  yMax = canvasHeight - 2 * borderWidth;

  printf("width: %d\n", canvasWidth);
  printf("height: %d\n", canvasHeight);

  // Background

  wxColour clr(255, 255, 255);

  wxBrush brush(clr, wxBRUSHSTYLE_SOLID);

  dc.SetBackground(brush);

  // Draw borders

  dc.SetPen(wxPen(wxColour(0xcc, 0xcc, 0xcc), 1, wxPENSTYLE_SOLID));

  dc.DrawLine(0, 0, canvasWidth - 1, 0);
  dc.DrawLine(0, 0, 0, canvasHeight - 1);
  dc.DrawLine(canvasWidth - 1, 0, canvasWidth - 1, canvasHeight - 1);
  dc.DrawLine(0, canvasHeight - 1, canvasWidth - 1, canvasHeight - 1);


  // Draw a rectangle on graph main area

  wxColour colorGraphContainer(255, 255, 255);

  wxBrush brushGraphContainer(clr, wxBRUSHSTYLE_SOLID);

  dc.SetBrush(brushGraphContainer);

  dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

  dc.DrawRectangle(this->marginLeft,
                   this->marginTop,
                   canvasWidth - this->marginLeft - this->marginRight,
                   canvasHeight - this->marginTop - this->marginBottom);

  // @remove Debug purpose

  GraphIncomeExpenseHash::iterator it;
  
  int i = 0;

  for (it = graphIncomeExpenseHash.begin(); it != graphIncomeExpenseHash.end(); ++it) {
    wxString key = it->first;
    double value = it->second;

    if (i == 0 || incomeExpenseMonthlyMax < value) {
      incomeExpenseMonthlyMax = value;
    }
    
    ++i;
  }

  // Font

  wxFont font = dc.GetFont();

  font.SetPointSize(7);

  dc.SetFont(font);

  // Variables

  int xPrevious = 0;       // Previous X
  int yIncomePrevious = 0.0;  // Previous Y
  int yExpensePrevious = 0.0;
  int x = 0;               // Current X
  int yIncome = 0.0;          // Current Y for income
  int yExpense = 0.0;          // Current Y for expense
  int yearCurrent = year;
  // Month area width
  int monthWidth = (canvasWidth - this->marginLeft - this->marginRight) / 12;
  bool incomeFound = false;
  bool expenseFound = false;

  //messenger->Register(wxString::Format("Ano corrente: %d", yearCurrent));

  // Draw information for each month

  for (int monthIndex = 0; monthIndex < 12; monthIndex++) {
    //messenger->Register(wxString::Format("Month: %d", monthIndex + 1));

    xPrevious = x;

    x = (monthWidth * monthIndex) + marginLeft; // monthIndex * 50;

    // Month vertical line

    dc.SetPen(wxPen(wxColour(0xcc, 0xcc, 0xcc), 1, wxPENSTYLE_SOLID));
    
    int monthVerticalLineHeight = canvasHeight - marginBottom;

    dc.DrawLine(x, marginTop, x, monthVerticalLineHeight);

    dc.SetTextForeground(wxColour(0x33, 0x33, 0x33));

    // Month label

    dc.DrawText(wxString::Format("%d/%d", monthIndex + 1, yearCurrent), x + 5, 2);

    // Monthly income

    // Format

    dc.SetBrush(wxBrush(wxColour(197, 229, 241), wxBRUSHSTYLE_SOLID));

    dc.SetPen(wxPen(wxColour(75, 135, 166), 1, wxPENSTYLE_SOLID));

    // Searching information for current year / month
    
    //messenger->Register("Incomes monthly count: " + wxString::Format("%d", incomeMonthlyArray.GetCount()));

    size_t incomeIndex = 0;

    incomeFound = false;

    for (incomeIndex = 0; incomeIndex < incomeMonthlyArray.GetCount(); incomeIndex++) {
      if (incomeMonthlyArray.Item(incomeIndex).year == yearCurrent &&
          incomeMonthlyArray.Item(incomeIndex).month == (monthIndex + 1)) {
        incomeFound = true;

        yIncomePrevious = yIncome;

        //messenger->Register(wxString::Format("Montante de receita de %d/%d: %f", yearCurrent, monthIndex + 1, incomeMonthlyArray.Item(incomeIndex).value));

        yIncome = round(100 * incomeMonthlyArray.Item(incomeIndex).value / incomeExpenseMonthlyMax);

        break;
      }
    }

    dc.SetTextForeground(wxColour(0x00, 0x0e, 0x59));

    if (incomeFound) {
      dc.DrawText(wxString::Format("%.2f", incomeMonthlyArray.Item(incomeIndex).value), x + 3, canvasHeight - 25);
    }
    else {
      yIncomePrevious = yIncome;

      yIncome = 0;

      dc.DrawText(wxString::Format("%.2f", 0.0), x + 3, canvasHeight - 25);
    }

    dc.SetBrush(wxColour(75, 135, 166));

    // Draw a point on the first value
    if (monthIndex == 0) {
      dc.DrawCircle(x, canvasHeight - yIncome - this->marginBottom, 3);
    }
    // Draw a line to show the changing of value
    else {
      dc.DrawLine(xPrevious, canvasHeight - this->marginBottom - yIncomePrevious, x, canvasHeight - this->marginBottom - yIncome);

      dc.DrawCircle(x, canvasHeight - yIncome - this->marginBottom, 3);
    }

    //dc.FloodFill(x + 1, canvasHeight - yIncome - this->marginBottom + 1, wxFLOOD_SURFACE);

    //messenger->Register(wxString::Format("Y da receita de %d/%d: %d", yearCurrent, monthIndex + 1, yIncome));

    // Monthly expense

    // Brush and pen

    dc.SetBrush(wxBrush(wxColour(197, 229, 241), wxBRUSHSTYLE_SOLID));

    dc.SetPen(wxPen(wxColour(0xc7, 0x3e, 0x3e), 1, wxPENSTYLE_SOLID));

    // Searching information for current year / month

    //messenger->Register("Expenses monthly count: " + wxString::Format("%d", expenseMonthlyArray.GetCount()));

    size_t expenseIndex = 0;

    expenseFound = false;

    for (expenseIndex = 0; expenseIndex < expenseMonthlyArray.GetCount(); expenseIndex++) {
      //messenger->Register(wxString::Format("OnPaint(): Expense=%f, Month=%d, Year=%d", expenseMonthlyArray.Item(expenseIndex).value,
      //                                                                                 expenseMonthlyArray.Item(expenseIndex).month,
      //                                                                                 expenseMonthlyArray.Item(expenseIndex).year));

      if (expenseMonthlyArray.Item(expenseIndex).year == yearCurrent &&
          expenseMonthlyArray.Item(expenseIndex).month == (monthIndex + 1)) {
        expenseFound = true;

        yExpensePrevious = yExpense;
        
        //messenger->Register(wxString::Format("Montante de despesa de %d/%d: %f", yearCurrent, monthIndex + 1, expenseMonthlyArray.Item(expenseIndex).value));

        yExpense = round(100 * expenseMonthlyArray.Item(expenseIndex).value / incomeExpenseMonthlyMax);

        break;
      }
    }

    dc.SetTextForeground(wxColour(0x89, 0x00, 0x00));

    if (expenseFound) {
      dc.DrawText(wxString::Format("%.2f", expenseMonthlyArray.Item(expenseIndex).value), x + 3, canvasHeight - 15);
    }
    else {
      yExpensePrevious = yExpense;

      yExpense = 0;

      dc.DrawText(wxString::Format("%.2f", 0.0), x + 3, canvasHeight - 15);
    }

    dc.SetBrush(wxColour(0xc7, 0x3e, 0x3e));

    // Draw a point on the first value
    if (monthIndex == 0) {
      dc.DrawCircle(x, canvasHeight - yExpense - this->marginBottom, 3);
    }
    // Draw a line to show the changing of value
    else {
      dc.DrawLine(xPrevious, canvasHeight - this->marginBottom - yExpensePrevious, x, canvasHeight - this->marginBottom - yExpense);

      dc.DrawCircle(x, canvasHeight - yExpense - this->marginBottom, 3);
    }

    //messenger->Register(wxString::Format("Y da despesa de %d/%d: %d", yearCurrent, monthIndex + 1, yExpense));
  }
}


int GraphIncomeExpenseCanvas::GetGraphInfo()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("GraphIncomeExpenseCanvas::GetGraphInfo()"), LogMessage::MSG_DBG);

  DBQuery dbquery;
  int resultIndex = 0;

  if (!DB::IsConnected()) {
    return -1;
  }

  graphIncomeExpenseHash.clear();

  incomeMonthlyArray.Empty();

  expenseMonthlyArray.Empty();

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT to_char(dpp.pagamento_date, 'mm') AS mes, to_char(dpp.pagamento_date, 'yyyy') AS ano, SUM(valor) AS valor ")
          wxT("FROM " + DB::GetTableName("financial.expense.payment.installment") + " dpp ")
          wxT("WHERE quitada = 1 ")
          wxT("GROUP BY ano, mes ")
          wxT("ORDER BY ano, mes");
  }
  else {
    sql = wxT("SELECT strftime('%m', dpp.pagamento_date) AS mes, strftime('%Y', dpp.pagamento_date) AS ano, SUM(valor) AS valor ")
          wxT("FROM " + DB::GetTableName("financial.expense.payment.installment") + " dpp ")
          wxT("WHERE quitada = 1 ")
          wxT("GROUP BY ano, mes ")
          wxT("ORDER BY ano, mes");
  }

  messenger->Register(sql, LogMessage::MSG_DBG);

  // @todo Select incomes / expenses by date. Show a point for each monthly value aligned by date.

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de meses e anos das despesas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    if (dbquery.GetRowsCount() > 0) {
      incomeExpenseMonthlyMax = dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
    }

    for (resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      IncomeExpenseGraphValue graphValue;

      graphValue.month = dbquery.GetFieldIntByName(resultIndex, wxT("mes"));
      graphValue.year = dbquery.GetFieldIntByName(resultIndex, wxT("ano"));
      graphValue.value = dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      
      messenger->Register("month = " + wxString::Format("%d", dbquery.GetFieldIntByName(resultIndex, wxT("mes"))) +
                          ", year = " + wxString::Format("%d", dbquery.GetFieldIntByName(resultIndex, wxT("ano"))), LogMessage::MSG_DBG);

      if (graphValue.value > incomeExpenseMonthlyMax) {
        incomeExpenseMonthlyMax = graphValue.value;
      }

      expenseMonthlyArray.Add(graphValue);
    }
  }

  // Monthly incomes

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT to_char(dpp.pagamento_date, 'mm') AS mes, to_char(dpp.pagamento_date, 'yyyy') AS ano, SUM(valor) AS valor ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" dpp ")
          wxT("WHERE quitada = 1 ")
          wxT("GROUP BY ano, mes ")
          wxT("ORDER BY ano, mes");
  }
  else {
    sql = wxT("SELECT strftime('%m', dpp.pagamento_date) AS mes, strftime('%Y', dpp.pagamento_date) AS ano, SUM(valor) AS valor ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" dpp ")
          wxT("WHERE quitada = 1 ")
          wxT("GROUP BY ano, mes ")
          wxT("ORDER BY ano, mes");
  }

  messenger->Register(wxT("SQL for monthly incomes: ") + sql, LogMessage::MSG_DBG);

  // @todo Select incomes / incomes by date. Show a point for each monthly value aligned by date.

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de meses e anos das receitas.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    //if (dbquery.GetRowsCount() > 0) {
    //  incomeExpenseMonthlyMax = dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
    //}

    for (resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      IncomeExpenseGraphValue graphValue;

      graphValue.month = dbquery.GetFieldIntByName(resultIndex, wxT("mes"));
      graphValue.year = dbquery.GetFieldIntByName(resultIndex, wxT("ano"));
      graphValue.value = dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));

      if (graphValue.value > incomeExpenseMonthlyMax) {
        incomeExpenseMonthlyMax = graphValue.value;
      }

      incomeMonthlyArray.Add(graphValue);
    }
  }

  // Incomes listing

  sql = wxT("SELECT pagamento_date, SUM(valor) AS valor ")
        wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" ")
        wxT("WHERE quitada = 1 ")
        wxT("GROUP BY pagamento_date");

  rc = dbquery.Execute(sql);

  messenger->Register(wxT("SQL for incomes listing: ") + sql, LogMessage::MSG_DBG);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter as informações para o gráfico.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      graphIncomeExpenseHash[dbquery.GetFieldStrByName(resultIndex, wxT("pagamento_date"))] = dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
    }
  }

  return 0;
}


BEGIN_EVENT_TABLE(GraphIncomeExpenseCanvas, wxScrolledWindow)
  EVT_PAINT(GraphIncomeExpenseCanvas::OnPaint)
END_EVENT_TABLE()