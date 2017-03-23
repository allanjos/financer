/**
 * Main dialog - Header file.
 *
 * \file dlg_main.hpp
 * \version 1.0
 * \since 06/13/2007
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */


/** \defgroup DlgMain Graphical User Interface - Dialogs - Main dialog */
/** @{ */


#ifndef _DLG_MAIN_HPP_
#define _DLG_MAIN_HPP_


#include "gui.hpp"

// Utility libraries
#include "utils_include.hpp"
// Log utilities library
#include "utils_log.hpp"
#include "app.hpp"
#include "dlg_log_view.hpp"

// Forward declarations

class PanelOwnerEdit;
//class PanelSummary *panelSummary;
class PanelCurrency;
class PanelPersonMain;
class PanelAccountMain;
class PanelBankMain;
class PanelIncomeExpense;
class PanelProductMain;
class PanelSupplierMain;
//class PanelInvoiceMain;
class PanelEmployeeMain;
class PanelPurchaseMain;
class PanelPurchaseRequestMain;
class PanelSaleMain;
class PanelTaxMain;
class PanelEstateMain;
class PanelLocalityMain;
//class PanelBiddingMain *panelBiddingMain;
class PanelGuide;
class MainFrame;

/**
 * Timer that is executed periodically to execute actions.
 */
class MonitorTimer: public wxTimer {
  private:
    MainFrame *frame;

  public:
    void SetFrame(MainFrame *frame);

    void Notify();
};


//extern bool splashLoaded; /**< Flag to inform if the splash screen is loaded. */

//extern wxSplashScreen *splashScreen; /** Informative splash screen. */

/*
#define TAB_TITLE_ENTERPRISE wxT("Proprietário")
#define TAB_TITLE_EXPENSES_INCOMES wxT("Receitas e Despesas")
#define TAB_TITLE_BANKS wxT("Bancos")
#define TAB_TITLE_ACCOUNTING wxT("Contabilidade")
//#define TAB_TITLE_FINANCIAL_SUMMARY wxT("Resumo financeiro")
#define TAB_TITLE_PRODUCTS wxT("Produtos")
#define TAB_TITLE_TAXES wxT("Impostos")
#define TAB_TITLE_ESTATE wxT("Patrimônio")
#define TAB_TITLE_CONTACTS wxT("Contatos")
#define TABLE_TITLE_CURRENCY wxT("Moedas Econômicas")
#define TABLE_TITLE_LOCALITIES wxT("Localidades")
#define TABLE_TITLE_ACCOUNTING_ACCOUNT_NEW wxT("Nova conta contábil")
*/

/**
 * Main frame. The main window of the application.
 */
class MainFrame: public wxFrame
{
  private:

    bool isShowed;
    bool isCreated;

    LogViewDialog *logViewDialog;

    wxStatusBar *statusBar;

    PanelOwnerEdit *panelOwnerEdit;
    //PanelSummary *panelSummary;
    PanelCurrency *panelCurrency;
    PanelPersonMain *panelPersonMain;
    PanelAccountMain *panelAccounting;
    PanelBankMain *panelBanks;
    PanelIncomeExpense *panelIncomeExpense;
    PanelTaxMain *panelTaxMain;
    PanelEstateMain *panelEstateMain;
    PanelLocalityMain *panelLocalityMain;
    PanelGuide *panelGuide;

    wxString currentDirectory;

    MonitorTimer monitorTimer;

    /**
     * Handles exit request in the main window.
     */
    void OnQuit(wxCommandEvent &event);

    /**
     * Key hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * On tab close.
     */
    void OnTabClosed(wxAuiNotebookEvent& event);

    /**
     * Log viewer dialog request.
     */
    void OnLogViewDialogRequest(wxCommandEvent& WXUNUSED(event));

    void SwitchFullscreen();

    void OnFullscreenDialogRequest(wxCommandEvent& WXUNUSED(event));

    void OnCommandEventEnterpriseRequest(wxCommandEvent& WXUNUSED(event));

    void OnCommandEventEstateRequest(wxCommandEvent& WXUNUSED(event));

    void OnCommandEventAccountsChartRequest(wxCommandEvent& WXUNUSED(event));

    void OnCommandEventBanksRequest(wxCommandEvent& WXUNUSED(event));

    void OnCommandEventIncomesExpensesRequest(wxCommandEvent& WXUNUSED(event));

    //void OnCommandEventTaxesRequest(wxCommandEvent& WXUNUSED(event));

    void OnCommandEventPeopleMainRequest(wxCommandEvent& WXUNUSED(event));

    /**
     * Local configuration dialog request.
     */
    void OnConfigLocalDialogRequest(wxCommandEvent& WXUNUSED(event));

    /**
     * General configuration dialog request.
     */
    void OnConfigGeneralDialogRequest(wxCommandEvent& WXUNUSED(event));

    /**
     * Callback to open documentation.
     */
    void OnHelpRequest(wxCommandEvent& WXUNUSED(event));

    /**
     * On about application request.
     */
    void OnAbout(wxCommandEvent &event);

    /**
     * Close event handler.
     */
    void CloseRequest(wxCloseEvent &event);

    /**
     * Presents the dialog to close application.
     */
    void CloseApplication();


    void OnReSize(wxSizeEvent &event);


    void OnMaximize(wxMaximizeEvent &event);


    void OnIconize(wxIconizeEvent &event);


    void OnShow(wxShowEvent &event);

    /**
     * Configuration dialog request.
     */
    void OnLoadFileRequest(wxCommandEvent& event);

    /**
     * Handler of file selection from the tree.
     */
    void OnSelectFileFromTree(wxTreeEvent &event);

    /**
     * Clear the current control file interface.
     */
    int ClearCurrentInterface();

    /**
     * Save modifications request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Undo modification
     */
    void OnModifyUndo(wxCommandEvent& event);

    /**
     * On key pressed.
     */
    void OnChar(wxKeyEvent& event);

    /**
     * On close notebook panel
     */
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);

    /**
     * Mouse enters within the products listing area.
     */
    void OnProductsListingMouseEnter(wxMouseEvent &event);

    void OnAccountMainDialogRequest(wxCommandEvent& event);

    //void OnAccountSummaryDialogRequest(wxCommandEvent& event);

    void OnIncomesExpensesMainDialogRequest(wxCommandEvent& event);

    //void OnTaxesMainDialogequest(wxCommandEvent& event);

    void OnConfCurrencyDialogRequest(wxCommandEvent& event);

    void OnConfLocalityDialogRequest(wxCommandEvent& event);

    void OnEnterpriseMainDialogRequest(wxCommandEvent& event);

    void OnEstateMainDialogRequest(wxCommandEvent& event);

    void OnToolbarContactsRequest(wxCommandEvent& event);

    //void OnPersonMainDialogRequest(wxCommandEvent &event);

    /**
     * Event table declaration (Internal to wxWidgets).
     */
    DECLARE_EVENT_TABLE()

  public:

    /**
     * Object constructor.
     */
    MainFrame(const wxString& title);

    ~MainFrame();

    wxStatusBar *GetStatusBar();

    void SetStatusText(wxString msg);

    wxAuiNotebook* notebook;

    /**
     * Layout window. Redraw, resize controls.
     */
    bool LayoutWindow();

    bool PageIsOpened(wxString title, bool select = false);

    bool PageRemove(wxString title);

    void PageRemoveDelayed(const unsigned int &pageIndex);

    bool PageDelete(wxString title);

    template <class T>
    bool AddPage(T *window, wxString title);

    template <class T>
    T* PageCreate(T **window, wxString title, bool once = true);

    wxWindow *GetPage(wxString title);

    bool PageSelect(wxString title);

    void ShowConfigDialog();
};


template <class T>
bool MainFrame::AddPage(T *window, wxString title)
{
  return notebook->AddPage((wxWindow *) window, title, true);
}


template <class T>
T* MainFrame::PageCreate(T **window, wxString title, bool once)
{
  if (PageIsOpened(title, true) && once) {
    return NULL;
  }

  notebook->Freeze();

  *window = new T(notebook);

  AddPage(*window, title);

  notebook->Thaw();

  wxGetApp().GetTopWindow()->Layout();

  return *window;
}


#endif /* _DLG_MAIN_HPP_ */


/** @} */