/**
 * Supplier main dialog.
 *
 * \version 1.0
 * \since 24/09/2009 01:06
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#include "utils_include.hpp"
#include "utils_lang.hpp"
#include "utils_monetary.hpp"
#include "utils_config.hpp"
#include "gui_controls.hpp"

#include "panel_income_new.hpp"
#include "panel_income_edit.hpp"
#include "panel_expense_new.hpp"
#include "panel_expense_edit.hpp"
#include "panel_income_expense.hpp"
#include "dlg_main.hpp"
#include "app.hpp"

#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/income_16x16.xpm"
#include "pixmaps/expense_16x16.xpm"
#include "pixmaps/add_16x16.xpm"

#include "wx/colordlg.h"
#include "wx/image.h"
#include "wx/artprov.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/filename.h"

PanelIncomeExpense::PanelIncomeExpense(wxWindow *parent):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS)
{
  this->isLoaded = false;

  SetAutoLayout(true);

  SetScrollbars(1, 1, 20, 20);


  // Main sizer

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  // Toolbar

  wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                           wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND | wxTB_RIGHT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID,
                   Lang::Get("New income"),
                   wxBitmap(income_16x16_xpm));

  /*
  toolBar->SetToolDropDown(DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID, true);  

  toolBar->Connect((int) DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID,
                   wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN,
                   wxAuiToolBarEventHandler(PanelIncomeExpense::OnDropDownToolbarIncome),
                   NULL,
                   this);
  */

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID,
                   Lang::Get("New expense"),
                   wxBitmap(expense_16x16_xpm));

  /*
  toolBar->SetToolDropDown(DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID, true);  

  toolBar->Connect((int) DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID,
                   wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN,
                   wxAuiToolBarEventHandler(PanelIncomeExpense::OnDropDownToolbarExpense),
                   NULL,
                   this);
  */

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_INCOME_EXPENSE_MAIN_BTN_REFRESH_ID, Lang::Get("Refresh"), wxBitmap(refresh_16x16_xpm));

  toolBar->Realize();

  szMargin->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);


  // Top sizer

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, GUI::WINDOW_MARGIN_SIZE);


  // Sizer to split listings and textual information regions

  wxBoxSizer *szCols = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szCols, 1, (wxALL & ~wxTOP)|wxEXPAND|wxGROW, 0);


  // Listings

  wxBoxSizer *szListings = new wxBoxSizer(wxVERTICAL);
  szCols->Add(szListings, 1, wxALL| wxEXPAND | wxGROW, 0);


  // Graph options

  wxBoxSizer *szGraphOptions = new wxBoxSizer(wxHORIZONTAL);

  szListings->Add(szGraphOptions, 0, wxBOTTOM | wxEXPAND, 5);

  // Spinner to filter graph by year

  wxStaticText *label = new wxStaticText(this, wxID_ANY, Lang::Get("Year") + wxT(": "));
  szGraphOptions->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

  spinnerYear = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS, 2000, 2014, 2014);

  szGraphOptions->Add(spinnerYear, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

  // Graph

  panelGraph = new GraphIncomeExpenseCanvas(this);

  ((GraphIncomeExpenseCanvas *) panelGraph)->SetYear(wxDateTime::GetCurrentYear());

  szListings->Add(panelGraph, 0, wxBOTTOM | wxEXPAND | wxGROW, 5);

  panelGraph->SetScrollbars(1, 1, 20, 20);

  panelGraph->SetBackgroundColour(*wxWHITE);


  // Incomes listing

  wxBoxSizer *szIncomesListing = new wxBoxSizer(wxVERTICAL);
  szListings->Add(szIncomesListing, 1, (wxALL & ~wxTOP)|wxEXPAND|wxGROW, 5);

  label = new wxStaticText(this, wxID_ANY, Lang::Get("Incomes"));
  szIncomesListing->Add(label, 0, wxTOP|wxBOTTOM, 10);

  wxFont font = label->GetFont();
  font.MakeBold();
  font.SetPointSize(font.GetPointSize() + 1);
  label->SetFont(font);

  // List control

  listCtrlIncomes = new wxListCtrl(this,
                                   DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID,
                                   wxDefaultPosition,
                                   wxSize(5, 5),
                                   wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);

  szIncomesListing->Add(listCtrlIncomes, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Assign an image list to the control

  wxImageList* imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(income_16x16_xpm));
  listCtrlIncomes->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  int colTypeWidth = 90;
  int colAmountValueWidth = 90;
  int colAmountPaidValueWidth = 120;
  int colPaymentDateWidth = 120;
  int colNameWidth = 250;

  wxListItem itemColName;
  itemColName.SetText(Lang::Get("Name"));
  listCtrlIncomes->InsertColumn(0, itemColName);
  listCtrlIncomes->SetColumnWidth(0, colNameWidth);

  wxListItem itemColType;
  itemColType.SetText(Lang::Get("Type"));
  listCtrlIncomes->InsertColumn(1, itemColType);
  listCtrlIncomes->SetColumnWidth(1, colTypeWidth);

  wxListItem itemColAmountValue;
  itemColAmountValue.SetText(Lang::Get("Amount"));
  itemColAmountValue.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlIncomes->InsertColumn(2, itemColAmountValue);
  listCtrlIncomes->SetColumnWidth(2, colAmountValueWidth);

  wxListItem itemColPaymentDate;
  itemColPaymentDate.SetText(Lang::Get("Payment date"));
  listCtrlIncomes->InsertColumn(3, itemColPaymentDate);
  listCtrlIncomes->SetColumnWidth(3, colPaymentDateWidth);

  wxListItem itemColAmountPaidValue;
  itemColAmountPaidValue.SetText(Lang::Get("Received amount"));
  itemColAmountPaidValue.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlIncomes->InsertColumn(4, itemColAmountPaidValue);
  listCtrlIncomes->SetColumnWidth(4, colAmountPaidValueWidth);


  // Expenses

  wxBoxSizer *szExpensesListing = new wxBoxSizer(wxVERTICAL);
  szListings->Add(szExpensesListing, 1, wxALL | wxEXPAND | wxGROW, 0);

  label = new wxStaticText(this, wxID_ANY, Lang::Get("Expenses"));
  szExpensesListing->Add(label, 0, wxTOP | wxBOTTOM, 10);

  font = label->GetFont();
  font.MakeBold();
  font.SetPointSize(font.GetPointSize() + 1);
  label->SetFont(font);

  // Expenses listing

  listCtrlExpenses = new wxListCtrl(this,
                                  DLG_INCOME_EXPENSE_MAIN_LISTCTRL_EXPENSES_ID,
                                  wxDefaultPosition,
                                  wxSize(5, 5),
                                  wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);

  szExpensesListing->Add(listCtrlExpenses, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Assign an image list to the control

  wxImageList* imageListExpense = new wxImageList(16, 16);
  imageListExpense->Add(wxIcon(expense_16x16_xpm));
  listCtrlExpenses->AssignImageList(imageListExpense, wxIMAGE_LIST_SMALL);

  int colExpenseTypeWidth = 90;
  int colExpenseAmountValueWidth = 90;
  int colExpenseAmountPaidValueWidth = 120;
  int colExpensePaymentDateWidth = 120;
  int colExpenseNameWidth = 250;

  wxListItem itemColExpenseName;
  itemColExpenseName.SetText(Lang::Get("Name"));
  listCtrlExpenses->InsertColumn(0, itemColExpenseName);
  listCtrlExpenses->SetColumnWidth(0, colExpenseNameWidth);

  wxListItem itemColExpenseType;
  itemColExpenseType.SetText(Lang::Get("Type"));
  listCtrlExpenses->InsertColumn(1, itemColExpenseType);
  listCtrlExpenses->SetColumnWidth(1, colExpenseTypeWidth);

  wxListItem itemColExpenseAmountValue;
  itemColExpenseAmountValue.SetText(Lang::Get("Amount"));
  itemColExpenseAmountValue.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlExpenses->InsertColumn(2, itemColExpenseAmountValue);
  listCtrlExpenses->SetColumnWidth(2, colExpenseAmountValueWidth);

  wxListItem itemColExpensePaymentDate;
  itemColExpensePaymentDate.SetText(Lang::Get("Payment date"));
  listCtrlExpenses->InsertColumn(3, itemColExpensePaymentDate);
  listCtrlExpenses->SetColumnWidth(3, colExpensePaymentDateWidth);

  wxListItem itemColExpenseAmountPaidValue;
  itemColExpenseAmountPaidValue.SetText(Lang::Get("Paid amount"));
  itemColExpenseAmountPaidValue.SetAlign(wxLIST_FORMAT_RIGHT);
  listCtrlExpenses->InsertColumn(4, itemColExpenseAmountPaidValue);
  listCtrlExpenses->SetColumnWidth(4, colExpenseAmountPaidValueWidth);

  listCtrlExpenses->Connect(wxEVT_COMMAND_LIST_KEY_DOWN,
                            wxListEventHandler(PanelIncomeExpense::OnExpenseKeyDown),
                            NULL,
                            this);


  // 2nd column

  // Textual informations

  wxScrolledWindow *panelValues = new wxScrolledWindow(this,
                                                       wxID_ANY,
                                                       wxDefaultPosition,
                                                       wxSize(270, -1),
                                                       wxTAB_TRAVERSAL | wxBORDER_THEME);
  szCols->Add(panelValues, 0, wxLEFT | wxEXPAND | wxGROW, 5);

  panelValues->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szValuesMargin = new wxBoxSizer(wxVERTICAL);
  panelValues->SetSizer(szValuesMargin);

  wxBoxSizer *szValues = new wxBoxSizer(wxVERTICAL);
  szValuesMargin->Add(szValues, 1, wxALL|wxEXPAND|wxGROW, 5);

  panelValues->SetBackgroundColour(*wxWHITE);

  // Values - General - Label

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("General"));
  szValues->Add(label, 0, wxBOTTOM, 10);

  label->SetForegroundColour(GUI_COLOR_LABEL);

  font = label->GetFont();
  font.MakeBold();
  font.SetPointSize(font.GetPointSize());
  label->SetFont(font);


  // Informations about general expense and incomes

  wxBoxSizer *szGeneralInfo = new wxBoxSizer(wxHORIZONTAL);
  szValues->Add(szGeneralInfo, 1, (wxALL & ~wxTOP)|wxEXPAND|wxGROW, 0);

  // Informations about payments

  // Total paid

  wxFlexGridSizer *szgGeneralInfo = new wxFlexGridSizer(0, 4,
                                                        GUI::FIELDS_SPACING_VERTICAL,
                                                        GUI::FIELDS_SPACING_HORIZONTAL);
  szGeneralInfo->Add(szgGeneralInfo, 0, wxALL&(~wxBOTTOM), 0);

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Received amount") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountPaid = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelAmountPaid->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgGeneralInfo->Add(labelAmountPaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgGeneralInfo->AddSpacer(1);

  // Installments not paid

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Amount to receive") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountInstallmentsNotPaid = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelAmountInstallmentsNotPaid->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgGeneralInfo->Add(labelAmountInstallmentsNotPaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgGeneralInfo->AddSpacer(1);

  // Total amount of expenses

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Paid expenses") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelExpensePaid = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelExpensePaid->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgGeneralInfo->Add(labelExpensePaid, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgGeneralInfo->AddSpacer(1);

  // Total amount of expenses

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Result") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  font = label->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  label->SetFont(font);
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgGeneralInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelResult = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  font = labelResult->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  labelResult->SetFont(font);
  szgGeneralInfo->Add(labelResult, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  labelResultName = new wxStaticText(panelValues, wxID_STATIC, wxT(""));
  labelResultName->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgGeneralInfo->Add(labelResultName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Values - Current month - Label

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Current month"));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szValues->Add(label, 0, wxTOP | wxEXPAND | wxBOTTOM, 10);

  font = label->GetFont();
  font.MakeBold();
  font.SetPointSize(font.GetPointSize());
  label->SetFont(font);

  // Values - Current month - Fields

  wxBoxSizer *szCurrentMonthInfoInfo = new wxBoxSizer(wxHORIZONTAL);
  szValues->Add(szCurrentMonthInfoInfo, 1, (wxALL & ~wxTOP)|wxEXPAND|wxGROW, 0);

  // Informations about payments

  // Total paid

  wxFlexGridSizer *szgCurrentMonthInfo = new wxFlexGridSizer(0, 4,
                                                             GUI::FIELDS_SPACING_VERTICAL,
                                                             GUI::FIELDS_SPACING_HORIZONTAL);
  szCurrentMonthInfoInfo->Add(szgCurrentMonthInfo, 0, wxALL&(~wxBOTTOM), 0);

  // Installments not paid

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Received amount") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountPaidCurrentMonth = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelAmountPaidCurrentMonth->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentMonthInfo->Add(labelAmountPaidCurrentMonth, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgCurrentMonthInfo->AddGrowableCol(2);

  szgCurrentMonthInfo->AddSpacer(1);

  // Installments not paid

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Amount to receive") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountNotPaidCurrentMonth = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelAmountNotPaidCurrentMonth->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentMonthInfo->Add(labelAmountNotPaidCurrentMonth, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgCurrentMonthInfo->AddSpacer(1);

  // Total amount of expenses

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Paid expenses") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelExpensePaidCurrentMonth = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelExpensePaidCurrentMonth->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentMonthInfo->Add(labelExpensePaidCurrentMonth, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgCurrentMonthInfo->AddSpacer(1);

  // Total amount of expenses

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Result") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  font = label->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  label->SetFont(font);
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentMonthInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelResultCurrentMonth = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  font = labelResultCurrentMonth->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  labelResultCurrentMonth->SetFont(font);
  szgCurrentMonthInfo->Add(labelResultCurrentMonth, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  labelResultNameCurrentMonth = new wxStaticText(panelValues, wxID_STATIC, wxT(""));
  labelResultNameCurrentMonth->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentMonthInfo->Add(labelResultNameCurrentMonth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Informations current month

  // Values - Today - Label

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Today"));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szValues->Add(label, 0, wxTOP | wxBOTTOM, 10);

  font = label->GetFont();
  font.MakeBold();
  font.SetPointSize(font.GetPointSize());
  label->SetFont(font);

  // Values - Today - Fields

  wxBoxSizer *szCurrentDayInfoInfo = new wxBoxSizer(wxHORIZONTAL);
  szValues->Add(szCurrentDayInfoInfo, 1, (wxALL & ~wxTOP)|wxEXPAND|wxGROW, 0);

  // Total paid

  wxFlexGridSizer *szgCurrentDayInfo = new wxFlexGridSizer(0, 4, GUI::FIELDS_SPACING_VERTICAL, GUI::FIELDS_SPACING_HORIZONTAL);
  szCurrentDayInfoInfo->Add(szgCurrentDayInfo, 0, wxALL&(~wxBOTTOM), 0);

  // Installments not paid

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Received amount") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountPaidCurrentDay = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelAmountPaidCurrentDay->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentDayInfo->Add(labelAmountPaidCurrentDay, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgCurrentDayInfo->AddGrowableCol(2);

  szgCurrentDayInfo->AddSpacer(1);

  // Installments not paid

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Amount to receive") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelAmountNotPaidCurrentDay = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelAmountNotPaidCurrentDay->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentDayInfo->Add(labelAmountNotPaidCurrentDay, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgCurrentDayInfo->AddSpacer(1);

  // Total amount of expenses

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Paid expenses") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelExpensePaidCurrentDay = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  labelExpensePaidCurrentDay->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentDayInfo->Add(labelExpensePaidCurrentDay, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  szgCurrentDayInfo->AddSpacer(1);

  // Total amount of expenses

  label = new wxStaticText(panelValues, wxID_STATIC, Lang::Get("Result") + wxT(": "));
  label->SetForegroundColour(GUI_COLOR_LABEL);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  label = new wxStaticText(panelValues, wxID_STATIC, Config::Get("currency.abbr") + " ");
  font = label->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  label->SetFont(font);
  label->SetForegroundColour(GUI_COLOR_LABEL_MONETARY);
  szgCurrentDayInfo->Add(label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);

  labelResultCurrentDay = new wxStaticText(panelValues, wxID_STATIC, wxT("0,00"));
  font = labelResultCurrentDay->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  labelResultCurrentDay->SetFont(font);
  szgCurrentDayInfo->Add(labelResultCurrentDay, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 1);

  labelResultNameCurrentDay = new wxStaticText(panelValues, wxID_STATIC, wxT(""));
  labelResultNameCurrentDay->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  szgCurrentDayInfo->Add(labelResultNameCurrentDay, 0, wxALL|wxALIGN_CENTER_VERTICAL, 1);


  // Load information
  if (DB::IsConnected()) {
    //LoadGeneralInfo();

    //LoadCurrentMonthInfo();

    //LoadCurrentDayInfo();


    LoadData();
  }


  // Update UI

  this->isLoaded = true;


  // Events

  Connect(wxEVT_SPINCTRL,
          wxSpinEventHandler(PanelIncomeExpense::OnSpinnerGraphYearEvent),
          NULL,
          this);

  Connect(wxEVT_SIZE,
          wxSizeEventHandler(PanelIncomeExpense::OnSizeEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_PAGEUP,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_PAGEDOWN,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_THUMBTRACK,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_THUMBRELEASE,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_CHANGED,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_TOP,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_BOTTOM,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_LINEUP,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);

  Connect(wxEVT_SCROLL_LINEDOWN,
          wxScrollEventHandler(PanelIncomeExpense::OnScrollEvent),
          NULL,
          this);


  // Update screen

  //panelGraph->Refresh();
  //panelGraph->Update();


  // Focus on list

  listCtrlIncomes->SetFocus();
}


int PanelIncomeExpense::LoadGeneralInfo(void)
{
  std::vector<wxString> params;

  DBQuery dbquery;

  double amountPaid = 0.0;
  double amountInstallmentsNotPaid = 0.0;

  wxString sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, ")
                 wxT("p.pagamento_forma, p.valor, p.quitada ")
                 wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ")
                 wxT("ORDER BY p.pagamento_date");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de parcelas de pagamento. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("t")) == 0) {
        amountPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        amountInstallmentsNotPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
    }
  }

  labelAmountPaid->SetLabel(Monetary::Number2Monetary(amountPaid));
  labelAmountPaid->Layout();
  labelAmountPaid->GetContainingSizer()->Layout();

  labelAmountInstallmentsNotPaid->SetLabel(Monetary::Number2Monetary(amountInstallmentsNotPaid));
  labelAmountInstallmentsNotPaid->Layout();
  labelAmountInstallmentsNotPaid->GetContainingSizer()->Layout();

  // Expenses on current month

  double expenseTotalPaid = 0.0;
  double expenseTotalNotPaid = 0.0;

  sql = wxT("SELECT d.installment_id, d.expense_id, d.account_id, ")
        wxT("d.pagamento_forma, d.valor, d.quitada ")
        wxT("FROM ") + DB::GetTableName("financial.expense.payment.installment") + wxT(" d ")
        //wxT("WHERE (SELECT date_part('month', d.pagamento_date)) = (SELECT date_part('month', now())) ")
        //wxT("AND (SELECT date_part('year', d.pagamento_date)) = (SELECT date_part('year', now())) ")
        wxT("ORDER BY d.pagamento_date");

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de parcelas de pagamento. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("t")) == 0) {
        expenseTotalPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        expenseTotalNotPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
    }
  }

  labelExpensePaid->SetLabel(Monetary::Number2Monetary(expenseTotalPaid));
  labelExpensePaid->Layout();
  labelExpensePaid->GetContainingSizer()->Layout();

  double result = amountPaid - expenseTotalPaid;

  //wxMessageBox(wxString::Format(wxT("%f"), result));

  labelResult->SetLabel(Monetary::Number2Monetary(result));
  //labelResult->SetLabel(wxString::Format(wxT("%f"), result));
  labelResult->SetForegroundColour(*wxBLUE);
  labelResult->Layout();
  labelResult->GetContainingSizer()->Layout();

  if (result < 0) {
    labelResult->SetForegroundColour(GUI_COLOR_LABEL_EXPENSE);

    labelResultName->SetLabel(wxT(" (-)"));
    labelResultName->SetForegroundColour(GUI_COLOR_LABEL_EXPENSE);
  }
  else if (result > 0) {
    labelResult->SetForegroundColour(GUI_COLOR_LABEL_INCOME);

    labelResultName->SetLabel(wxT(" (+)"));
    labelResultName->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  }
  else {
    labelResultName->SetLabel(wxT(" (=)"));
  }
  labelResultName->Layout();
  labelResultName->GetContainingSizer()->Layout();


  return 0;
}


int PanelIncomeExpense::LoadCurrentMonthInfo(void)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  std::vector<wxString> params;

  DBQuery dbquery;

  double amountPaid = 0.0;
  double amountInstallmentsNotPaid = 0.0;

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, ")
          wxT("p.pagamento_forma, p.valor, p.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ")
          wxT("WHERE (SELECT date_part('month', p.pagamento_date)) = (SELECT date_part('month', now())) ")
          wxT("AND (SELECT date_part('year', p.pagamento_date)) = (SELECT date_part('year', now())) ")
          wxT("ORDER BY p.pagamento_date");
  }
  else {
    sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, ")
          wxT("p.pagamento_forma, p.valor, p.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ")
          wxT("WHERE (SELECT strftime('%%m', p.pagamento_date)) = (SELECT strftime('%%m', CURRENT_TIMESTAMP)) ")
          wxT("AND (SELECT strftime('%%Y', p.pagamento_date)) = (SELECT strftime('%%Y', CURRENT_TIMESTAMP)) ")
          wxT("ORDER BY p.pagamento_date");

    // SELECT p.installment_id, p.income_id, p.account_id, p.pagamento_forma, p.valor, p.quitada FROM finance_income_installment p WHERE (SELECT strftime('%m', p.pagamento_date)) = (strftime('%m', CURRENT_TIMESTAMP)) AND (strftime('%Y', CURRENT_TIMESTAMP)) = (SELECT strftime('%Y', CURRENT_TIMESTAMP)) ORDER BY p.pagamento_date
  }

  messenger->Register(sql, LogMessage::MSG_DBG);

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de parcelas de pagamento.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("t")) == 0) {
        amountPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        amountInstallmentsNotPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
    }
  }

  labelAmountPaidCurrentMonth->SetLabel(Monetary::Number2Monetary(amountPaid));
  labelAmountPaidCurrentMonth->Layout();
  labelAmountPaidCurrentMonth->GetContainingSizer()->Layout();

  labelAmountNotPaidCurrentMonth->SetLabel(Monetary::Number2Monetary(amountInstallmentsNotPaid));
  labelAmountNotPaidCurrentMonth->Layout();
  labelAmountNotPaidCurrentMonth->GetContainingSizer()->Layout();


  // Expenses on current month

  double expenseTotalPaidCurrentMonth = 0.0;
  double expenseTotalNotPaidCurrentMonth = 0.0;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT d.installment_id, d.expense_id, d.account_id, ")
          wxT("d.pagamento_forma, d.valor, d.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.expense.payment.installment") + wxT(" d ")
          wxT("WHERE (SELECT strftime('%%m', d.pagamento_date)) = (SELECT date_part('month', now())) ")
          wxT("AND (SELECT date_part('year', d.pagamento_date)) = (SELECT date_part('year', now())) ")
          wxT("ORDER BY d.pagamento_date");
  }
  else {
    sql = wxT("SELECT d.installment_id, d.expense_id, d.account_id, ")
          wxT("d.pagamento_forma, d.valor, d.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.expense.payment.installment") + wxT(" d ")
          wxT("WHERE (SELECT strftime('%%m', d.pagamento_date)) = (SELECT strftime('%%m', CURRENT_TIMESTAMP)) ")
          wxT("AND (SELECT strftime('%%Y', d.pagamento_date)) = (SELECT strftime('%%Y', CURRENT_TIMESTAMP)) ")
          wxT("ORDER BY d.pagamento_date");
  }

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de parcelas de pagamento. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("t")) == 0) {
        expenseTotalPaidCurrentMonth += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        expenseTotalNotPaidCurrentMonth += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
    }
  }

  labelExpensePaidCurrentMonth->SetLabel(Monetary::Number2Monetary(expenseTotalPaidCurrentMonth));
  labelExpensePaidCurrentMonth->Layout();
  labelExpensePaidCurrentMonth->GetContainingSizer()->Layout();

  double resultCurrentMonth = amountPaid - expenseTotalPaidCurrentMonth;

  labelResultCurrentMonth->SetLabel(Monetary::Number2Monetary(resultCurrentMonth));
  labelResultCurrentMonth->SetForegroundColour(*wxBLUE);
  labelResultCurrentMonth->Layout();
  labelResultCurrentMonth->GetContainingSizer()->Layout();

  if (resultCurrentMonth < 0) {
    labelResultNameCurrentMonth->SetLabel(wxT("  (-)"));
    labelResultNameCurrentMonth->SetForegroundColour(GUI_COLOR_LABEL_EXPENSE);
  }
  else if (resultCurrentMonth > 0) {
    labelResultNameCurrentMonth->SetLabel(wxT("  (+)"));
    labelResultNameCurrentMonth->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  }
  labelResultNameCurrentMonth->Layout();
  labelResultNameCurrentMonth->GetContainingSizer()->Layout();

  return 0;
}


int PanelIncomeExpense::LoadCurrentDayInfo(void)
{
  std::vector<wxString> params;

  DBQuery dbquery;

  double amountPaid = 0.0;
  double amountInstallmentsNotPaid = 0.0;

  wxString sql;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, ")
          wxT("p.pagamento_forma, p.valor, p.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ")
          wxT("WHERE (SELECT date_part('month', p.pagamento_date)) = (SELECT date_part('month', now())) ")
          wxT("AND (SELECT date_part('year', p.pagamento_date)) = (SELECT date_part('year', now())) ")
          wxT("AND (SELECT date_part('day', p.pagamento_date)) = (SELECT date_part('day', now())) ")
          wxT("ORDER BY p.pagamento_date");
  }
  else {
    sql = wxT("SELECT p.installment_id, p.income_id, p.account_id, ")
          wxT("p.pagamento_forma, p.valor, p.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ")
          wxT("WHERE (SELECT strftime('%%m', p.pagamento_date)) = (SELECT strftime('%%m', CURRENT_TIMESTAMP)) ")
          wxT("AND (SELECT strftime('%%Y', p.pagamento_date)) = (SELECT strftime('%%Y', CURRENT_TIMESTAMP)) ")
          wxT("AND (SELECT strftime('%%d', p.pagamento_date)) = (SELECT strftime('%%d', CURRENT_TIMESTAMP)) ")
          wxT("ORDER BY p.pagamento_date");
  }

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de parcelas de pagamento. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("t")) == 0) {
        amountPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        amountInstallmentsNotPaid += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
    }
  }

  labelAmountPaidCurrentDay->SetLabel(Monetary::Number2Monetary(amountPaid));
  labelAmountPaidCurrentDay->Layout();
  labelAmountPaidCurrentDay->GetContainingSizer()->Layout();

  labelAmountNotPaidCurrentDay->SetLabel(Monetary::Number2Monetary(amountInstallmentsNotPaid));
  labelAmountNotPaidCurrentDay->Layout();
  labelAmountNotPaidCurrentDay->GetContainingSizer()->Layout();


  // Expenses on current month

  double expenseTotalPaidCurrentDay = 0.0;
  double expenseTotalNotPaidCurrentDay = 0.0;

  if (DB::GetType() == DB_TYPE_POSTGRESQL) {
    sql = wxT("SELECT d.installment_id, d.expense_id, d.account_id, ")
          wxT("d.pagamento_forma, d.valor, d.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.expense.payment.installment") + wxT(" d ")
          wxT("WHERE (SELECT date_part('month', d.pagamento_date)) = (SELECT date_part('month', now())) ")
          wxT("AND (SELECT date_part('year', d.pagamento_date)) = (SELECT date_part('year', now())) ")
          wxT("AND (SELECT date_part('day', d.pagamento_date)) = (SELECT date_part('day', now())) ")
          wxT("ORDER BY d.pagamento_date");
  }
  else {
    sql = wxT("SELECT d.installment_id, d.expense_id, d.account_id, ")
          wxT("d.pagamento_forma, d.valor, d.quitada ")
          wxT("FROM ") + DB::GetTableName("financial.expense.payment.installment") + wxT(" d ")
          wxT("WHERE (SELECT strftime('%%m', d.pagamento_date)) = (SELECT strftime('%%m', CURRENT_TIMESTAMP)) ")
          wxT("AND (SELECT strftime('%%Y', d.pagamento_date)) = (SELECT strftime('%%Y', CURRENT_TIMESTAMP)) ")
          wxT("AND (SELECT strftime('%%d', d.pagamento_date)) = (SELECT strftime('%%d', CURRENT_TIMESTAMP)) ")
          wxT("ORDER BY d.pagamento_date");
  }

  rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de parcelas de pagamento. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      if (dbquery.GetFieldStrByName(resultIndex, wxT("quitada")).Cmp(wxT("t")) == 0) {
        expenseTotalPaidCurrentDay += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
      else {
        expenseTotalNotPaidCurrentDay += dbquery.GetFieldDoubleByName(resultIndex, wxT("valor"));
      }
    }
  }

  labelExpensePaidCurrentDay->SetLabel(Monetary::Number2Monetary(expenseTotalPaidCurrentDay));
  labelExpensePaidCurrentDay->Layout();
  labelExpensePaidCurrentDay->GetContainingSizer()->Layout();

  double resultCurrentDay = amountPaid - expenseTotalPaidCurrentDay;

  labelResultCurrentDay->SetLabel(Monetary::Number2Monetary(resultCurrentDay));
  labelResultCurrentDay->SetForegroundColour(*wxBLUE);
  labelResultCurrentDay->Layout();
  labelResultCurrentDay->GetContainingSizer()->Layout();

  if (resultCurrentDay < 0) {
    labelResultNameCurrentDay->SetLabel(wxT("  (-)"));
    labelResultNameCurrentDay->SetForegroundColour(GUI_COLOR_LABEL_EXPENSE);
  }
  else if (resultCurrentDay > 0) {
    labelResultNameCurrentDay->SetLabel(wxT("  (+)"));
    labelResultNameCurrentDay->SetForegroundColour(GUI_COLOR_LABEL_INCOME);
  }
  labelResultNameCurrentDay->Layout();
  labelResultNameCurrentDay->GetContainingSizer()->Layout();

  return 0;
}


int PanelIncomeExpense::LoadData(void)
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register(wxT("PanelIncomeExpense::LoadData()"), LogMessage::MSG_DBG);

  LoadIncomesListing();

  LoadExpensesListing();

  ((GraphIncomeExpenseCanvas *) panelGraph)->GetGraphInfo();
  
  panelGraph->Refresh();

  panelGraph->Update();

  return 0;
}


int PanelIncomeExpense::LoadIncomesListing(void)
{
  std::vector<wxString> params;

  listCtrlIncomes->Freeze();

  listCtrlIncomes->DeleteAllItems();

  DBQuery dbquery;

  wxString sql = wxT("SELECT d.*, ")
                       wxT("(SELECT SUM(p.valor) ")
                        wxT("FROM ") + DB::GetTableName("financial.income.payment.installment") + wxT(" p ")
                        wxT("WHERE p.income_id = d.income_id ")
                        wxT("AND p.quitada = 1) AS montante_pago ")
                 wxT("FROM ") + DB::GetTableName("financial.income") + wxT(" d ")
                 wxT("ORDER BY d.nome");

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de receitas. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      long itemIndex = listCtrlIncomes->GetItemCount();

      // Descriptive name

      listCtrlIncomes->InsertItem(itemIndex, dbquery.GetFieldStrByName(resultIndex, wxT("nome")), 0);

      // Item data - Expense ID

      listCtrlIncomes->SetItemData(itemIndex, dbquery.GetFieldLongByName(resultIndex, wxT("income_id")));
      
      // Expense type

      if (dbquery.GetFieldStrByName(resultIndex, wxT("tipo_id")).Cmp(wxT("F")) == 0) {
        listCtrlIncomes->SetItem(resultIndex, 1, Lang::Get("Fixed"));
      }
      else {
        listCtrlIncomes->SetItem(resultIndex, 1, Lang::Get("Variable"));
      }

      // Amount

      if (dbquery.GetFieldStrByName(resultIndex, wxT("montante_valor")).Length() > 0) {
        listCtrlIncomes->SetItem(resultIndex,
                                 2,
                                 Config::Get("currency.abbr") + " " + Monetary::Number2Monetary(dbquery.GetFieldStrByName(resultIndex, wxT("montante_valor"))));
      }

      // Payment date

      wxString paymentDate = "";
      
      if (dbquery.GetFieldStrByName(resultIndex, "pagamento_dia").Length() == 1)
        paymentDate += "0";

      paymentDate += dbquery.GetFieldStrByName(resultIndex, "pagamento_dia") + "/";

      if (dbquery.GetFieldStrByName(resultIndex, "pagamento_mes").Length() == 1)
        paymentDate += "0";

      paymentDate += dbquery.GetFieldStrByName(resultIndex, "pagamento_mes") + "/";

      paymentDate += dbquery.GetFieldStrByName(resultIndex, "pagamento_ano");

      listCtrlIncomes->SetItem(resultIndex, 3, paymentDate);

      // Paid amount

      listCtrlIncomes->SetItem(resultIndex,
                               4,
                               Config::Get("currency.abbr") + " " + Monetary::Number2Monetary(dbquery.GetFieldStrByName(resultIndex, wxT("montante_pago"))));

      // Expense status

      /*
      if (dbquery.GetFieldStrByName(resultIndex, wxT("status")).Cmp(wxT("A")) == 0) {
        GUI::ListSetRowBgColor(listCtrlIncomes, itemIndex);
      }
      else {
        GUI::ListSetRowBgColor(listCtrlIncomes, itemIndex, GUI_COLOR_LIST_ITEM_DISABLED);
      }
      */
    }
  }

  listCtrlIncomes->Thaw();


  LoadGeneralInfo();

  LoadCurrentMonthInfo();

  LoadCurrentDayInfo();


  return 0;
}


int PanelIncomeExpense::LoadExpensesListing(void)
{
  std::vector<wxString> params;

  listCtrlExpenses->Freeze();

  listCtrlExpenses->DeleteAllItems();

  DBQuery dbquery;

  wxString sql = "SELECT d.*, "
                       "(SELECT SUM(p.valor) "
                       "FROM " + DB::GetTableName("financial.expense.payment.installment") + " p "
                       "WHERE p.expense_id = d.expense_id "
                       "AND p.quitada = 1) AS montante_pago "
                 "FROM " + DB::GetTableName("financial.expense") + " d "
                 "ORDER BY d.nome";

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível obter a listagem de despesas. ")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }
  else {
    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      long itemIndex = listCtrlExpenses->GetItemCount();

      listCtrlExpenses->InsertItem(itemIndex, dbquery.GetFieldStrByName(resultIndex, "nome"), 0);

      listCtrlExpenses->SetItemData(itemIndex, dbquery.GetFieldLongByName(resultIndex, "expense_id"));
      
      // Type

      if (dbquery.GetFieldStrByName(resultIndex, "tipo_id").Cmp("F") == 0) {
        listCtrlExpenses->SetItem(resultIndex, 1, Lang::Get("Fixed"));
      }
      else {
        listCtrlExpenses->SetItem(resultIndex, 1, Lang::Get("Variable"));
      }
      
      // Amount

      if (dbquery.GetFieldStrByName(resultIndex, "montante_valor").Length() > 0) {
        listCtrlExpenses->SetItem(resultIndex,
                                  2,
                                  Config::Get("currency.abbr") + " " + Monetary::Number2Monetary(dbquery.GetFieldStrByName(resultIndex, "montante_valor")));
      }

      // Payment date

      wxString paymentDate = "";
      
      if (dbquery.GetFieldStrByName(resultIndex, "pagamento_dia").Length() == 1)
        paymentDate += "0";

      paymentDate += dbquery.GetFieldStrByName(resultIndex, "pagamento_dia") + "/";

      if (dbquery.GetFieldStrByName(resultIndex, "pagamento_mes").Length() == 1)
        paymentDate += "0";

      paymentDate += dbquery.GetFieldStrByName(resultIndex, "pagamento_mes") + "/";

      paymentDate += dbquery.GetFieldStrByName(resultIndex, "pagamento_ano");

      listCtrlExpenses->SetItem(resultIndex, 3, paymentDate);

      // Paid amount

      listCtrlExpenses->SetItem(resultIndex,
                                4,
                                Config::Get("currency.abbr") + " " + Monetary::Number2Monetary(dbquery.GetFieldStrByName(resultIndex, "montante_pago")));

      /*
      if (dbquery.GetFieldStrByName(resultIndex, "status").Cmp("A") == 0) {
        GUI::ListSetRowBgColor(listCtrlExpenses, itemIndex);
      }
      else {
        GUI::ListSetRowBgColor(listCtrlExpenses, itemIndex, GUI_COLOR_LIST_ITEM_DISABLED);
      }
      */
    }
  }

  listCtrlExpenses->Thaw();

  // Load data

  LoadGeneralInfo();

  LoadCurrentMonthInfo();

  LoadCurrentDayInfo();

  return 0;
}


void PanelIncomeExpense::OnSpinnerGraphYearEvent(wxSpinEvent& event)
{
  ((GraphIncomeExpenseCanvas *) panelGraph)->SetYear(spinnerYear->GetValue());

  ((GraphIncomeExpenseCanvas *) panelGraph)->GetGraphInfo();

  panelGraph->Layout();
  panelGraph->Refresh();
  panelGraph->Update();
}


void PanelIncomeExpense::OnSizeEvent(wxSizeEvent& event)
{
  ((GraphIncomeExpenseCanvas *) panelGraph)->GetGraphInfo();

  panelGraph->Refresh();
  panelGraph->Update();

  event.Skip();
}


void PanelIncomeExpense::OnScrollEvent(wxScrollEvent& event)
{
  ((GraphIncomeExpenseCanvas *) panelGraph)->GetGraphInfo();

  panelGraph->Refresh();
  panelGraph->Update();

  event.Skip();
}


void PanelIncomeExpense::Terminate()
{
  Close();

  Destroy();
}


/*
void PanelIncomeExpense::OnIncomesMainDialogRequest(wxCommandEvent& event)
{
  IncomeMainDialog incomeMainDialog(this);

  incomeMainDialog.ShowModal();
}
*/


void PanelIncomeExpense::OnIncomeNewDialogRequest(wxCommandEvent& event)
{
  /*
  IncomeNewDialog incomeNewDialog(this);

  incomeNewDialog.ShowModal();

  LoadData();

  if (incomeNewDialog.IsCanceled()) {
    return;
  }
  */

  /*
  IncomeEditDialog incomeEditDialog(wxGetApp().GetTopWindow());

  incomeEditDialog.LoadData(incomeNewDialog.GetExpenseId());

  incomeEditDialog.SetAutoLayout(true);

  incomeEditDialog.Layout();

  incomeEditDialog.ShowModal();
  */

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageCreate(&panelIncomeNew, Lang::Get("New income"), true);
}


void PanelIncomeExpense::OnExpensesMainDialogRequest(wxCommandEvent& event)
{
  /*
  ExpenseMainDialog expenseMainDialog(this);

  expenseMainDialog.ShowModal();
  */
}


void PanelIncomeExpense::OnExpenseNewDialogRequest(wxCommandEvent& event)
{
  /*
  wxString pageTitle = wxT("Nova Despesa");

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  if (!mainFrame->PageIsOpened(pageTitle)) {
    panelExpenseNew = new PanelExpenseNew(this);

    mainFrame->AddPage(panelExpenseNew, pageTitle);
  }
  else {
    mainFrame->PageSelect(pageTitle);
  }
  */


  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  mainFrame->PageCreate(&panelExpenseNew, Lang::Get("New expense"), true);


  /*
  ExpenseNewDialog expenseNewDialog(this);

  expenseNewDialog.ShowModal();

  LoadData();

  if (expenseNewDialog.IsCanceled()) {
    return;
  }

  ExpenseEditDialog expenseEditDialog(wxGetApp().GetTopWindow());

  expenseEditDialog.LoadData(expenseNewDialog.GetExpenseId());

  expenseEditDialog.SetAutoLayout(true);

  expenseEditDialog.Layout();

  expenseEditDialog.ShowModal();
  */
}

/*
void PanelIncomeExpense::OnKeyChar(wxKeyEvent& event)
{
  // CTRL-N - New supplier
  if (event.ControlDown() && event.GetKeyCode() == 14) {
    SupplierNewDialog supplierNewDialog(this);

    supplierNewDialog.ShowModal();
  }
  // CTRL-P - Search
  else if (event.ControlDown() && event.GetKeyCode() == 16) {
    teditSearch->SetFocus();
  }

  event.Skip();
}
*/


void PanelIncomeExpense::OnExpenseKeyDown(wxListEvent& event)
{
  //wxMessageBox(wxT("OnExpenseKeyDown"));

  event.Skip();
}


void PanelIncomeExpense::OnRefreshRequest(wxCommandEvent &event)
{
  LoadData();
}


void PanelIncomeExpense::OnCharHook(wxKeyEvent& event)
{
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // <F2>
  if (event.GetKeyCode() == WXK_F2) {
    MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

    mainFrame->PageCreate(&panelIncomeNew, Lang::Get("New income"), true);
  }
  // Refresh
  else if (event.GetKeyCode() == WXK_F5) {
    LoadData();
  }
  // Exit <ESC>
  else if (event.GetKeyCode() == WXK_ESCAPE) {
    Close();
  }

  event.Skip();
}


void PanelIncomeExpense::OnIncomesListItemEditDialogRequest(wxListEvent &event)
{
  wxListItem listItem = event.GetItem();

  int data = listItem.GetData();

  //wxMessageBox(wxString::Format(wxT("data = %d"), data));

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  wxString title = wxString::Format(Lang::Get("Income") + wxT(" #%d"), data);

  if (mainFrame->PageIsOpened(title)) {
    return;
  }

  mainFrame->PageCreate(&panelIncomeEdit, title, true);

  panelIncomeEdit->LoadData(listItem.GetData());
}


void PanelIncomeExpense::OnExpensesListItemEditDialogRequest(wxListEvent &event)
{
  wxListItem listItem = event.GetItem();

  MainFrame *mainFrame = (MainFrame *) wxGetApp().GetTopWindow();

  long data = listItem.GetData();

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("Expense ID: %d", (int) data);

  wxString title = wxString::Format(Lang::Get("Expense") + wxT(" #%d"), (int) data);

  if (mainFrame->PageIsOpened(title)) {
    return;
  }

  mainFrame->PageCreate(&panelExpenseEdit, title, true);

  panelExpenseEdit->LoadData(data);
}


void PanelIncomeExpense::OnDropDownToolbarIncome(wxAuiToolBarEvent& event)
{
  if (event.IsDropDownClicked()) {
    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());
    
    tb->SetToolSticky(event.GetId(), true);

    // Popup menu

    wxMenu menuPopup;

    /*
    wxMenuItem* menuItemIncomeMain =  new wxMenuItem(&menuPopup,
                                                     DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID,
                                                     wxT("Receitas"),
                                                     wxT("Informações sobre receitas."));
    menuItemIncomeMain->SetBitmap(wxBitmap(list_16x16_xpm));
    menuPopup.Append(menuItemIncomeMain);
    */

    wxMenuItem* menuItemIncomeNew =  new wxMenuItem(&menuPopup,
                                                    DLG_INCOME_EXPENSE_MAIN_TOOL_INCOME_NEW_ID,
                                                    Lang::Get("New income"));
    menuItemIncomeNew->SetBitmap(wxBitmap(add_16x16_xpm));
    menuPopup.Append(menuItemIncomeNew);

    // line up our menu with the button

    wxRect rect = tb->GetToolRect(event.GetId());
    wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());

    pt = ScreenToClient(pt);

    PopupMenu(&menuPopup, pt);

    // make sure the button is "un-stuck"
    tb->SetToolSticky(event.GetId(), false);
  }
}


void PanelIncomeExpense::OnDropDownToolbarExpense(wxAuiToolBarEvent& event)
{
  if (event.IsDropDownClicked()) {
    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());
    
    tb->SetToolSticky(event.GetId(), true);

    // Popup menu

    wxMenu menuPopup;

    /*
    wxMenuItem* menuItemExpenseMain =  new wxMenuItem(&menuPopup,
                                                     DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID,
                                                     wxT("Depesas"),
                                                     wxT("Informações sobre despesas."));
    menuItemExpenseMain->SetBitmap(wxBitmap(list_16x16_xpm));
    menuPopup.Append(menuItemExpenseMain);
    */

    wxMenuItem* menuItemExpenseNew =  new wxMenuItem(&menuPopup,
                                                     DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSE_NEW_ID,
                                                     Lang::Get("New expense"));
    menuItemExpenseNew->SetBitmap(wxBitmap(add_16x16_xpm));
    menuPopup.Append(menuItemExpenseNew);

    // line up our menu with the button

    wxRect rect = tb->GetToolRect(event.GetId());
    wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());

    pt = ScreenToClient(pt);

    PopupMenu(&menuPopup, pt);

    // make sure the button is "un-stuck"
    tb->SetToolSticky(event.GetId(), false);
  }
}


BEGIN_EVENT_TABLE(PanelIncomeExpense, wxScrolledWindow)
  EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID, PanelIncomeExpense::OnIncomeNewDialogRequest)
  EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID, PanelIncomeExpense::OnExpenseNewDialogRequest)
  //EVT_CHAR(PanelIncomeExpense::OnKeyChar)
  //EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_TOOL_INCOMES_MAIN_ID, PanelIncomeExpense::OnIncomesMainDialogRequest)
  EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_TOOL_INCOME_NEW_ID, PanelIncomeExpense::OnIncomeNewDialogRequest)
  //EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSES_MAIN_ID, PanelIncomeExpense::OnExpensesMainDialogRequest)
  EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_TOOL_EXPENSE_NEW_ID, PanelIncomeExpense::OnExpenseNewDialogRequest)
  EVT_TOOL(DLG_INCOME_EXPENSE_MAIN_BTN_REFRESH_ID, PanelIncomeExpense::OnRefreshRequest)
  EVT_CHAR_HOOK(PanelIncomeExpense::OnCharHook)
  EVT_LIST_ITEM_ACTIVATED(DLG_INCOME_EXPENSE_MAIN_LISTCTRL_INCOMES_ID, PanelIncomeExpense::OnIncomesListItemEditDialogRequest)
  EVT_LIST_ITEM_ACTIVATED(DLG_INCOME_EXPENSE_MAIN_LISTCTRL_EXPENSES_ID, PanelIncomeExpense::OnExpensesListItemEditDialogRequest)
END_EVENT_TABLE()