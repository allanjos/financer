/**
 * Registering dialog - Header file.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_ESTATE_NEW_HPP_
#define _PANEL_ESTATE_NEW_HPP_


#include "utils_include.hpp"


class PanelEstateNew: public wxScrolledWindow
{
  private:
    wxNotebook *notebook;

    wxTextCtrl *teditName;

    wxTextCtrl *teditCode;

    wxTextCtrl *teditDescription;

    wxTextCtrl *teditAcquisitionDate;

    wxBitmapButton *btnCalendarAcquisitionDate;

    wxComboBox *cboxType;

    wxComboBox *cboxManufacturer;

    wxComboBox *cboxSupplier;

    wxComboBox *cboxTrademark;

    wxComboBox *cboxMeasureUnit;

    wxTextCtrl *teditWeigthLiquid;

    wxTextCtrl *teditTempoGarantia;

    wxChoice *cboxTempoGarantiaType;

    wxTextCtrl *teditWeigthBrute;

    wxTextCtrl *teditValue;

    wxTextCtrl *teditLiquidValue;

    wxTextCtrl *teditValueMarket;

    wxTextCtrl *teditDepreciacaoTaxa;

    wxTextCtrl *teditDepreciacaoDate;

    wxBitmapButton *btnCalendarDepreciacaoDate;

    wxChoice *choiceDepreciacaoAcelerada;

    wxTextCtrl *teditDepreciacaoAcumulada;

    wxTextCtrl *teditSeguroCompanhia;

    wxTextCtrl *teditSeguroApolice;

    wxTextCtrl *teditSeguroApoliceVencimentoDate;

    wxBitmapButton *btnCalendarApoliceVencimentoDate;

    wxTextCtrl *teditCategoryId;

    wxTextCtrl *teditCategoryName;

    wxBitmapButton *btnImage;

    wxCheckBox *chkActive;

    // Physical identification

    wxTextCtrl *teditSerieNumber;

    // Dimensions

    wxTextCtrl *teditLength;

    wxTextCtrl *teditWidth;

    wxTextCtrl *teditHeight;

    wxTextCtrl *teditTechnicalInfo;

    wxChoice *cboxPurchase;

    wxChoice *cboxBuyPriceCurrency;

    wxTextCtrl *teditBuyPrice;

    wxChoice *cboxCommonBuyPriceCurrency;

    wxTextCtrl *teditCommonBuyPrice;

    wxTextCtrl *teditWantedBuyPrice;

    wxTextCtrl *teditBuyQuantity;

    wxChoice *cboxEnterprise;

    // Stock

    wxChoice *cboxLocation;

    wxTextCtrl *teditQuantity;

    wxTextCtrl *teditStoreCode;

    wxTextCtrl *teditBarCode;

    wxTextCtrl *teditQttyMin;

    wxTextCtrl *teditQttyRenew;

    // Sale information

    wxTextCtrl *teditSaleGain;

    // Fiscal Information

    wxTextCtrl *teditFiscalCodigoIPI;

    wxStaticBox* staticBoxFiscalInfoMiscTaxes;

    wxListCtrl *listCtrlMiscTaxes;

    // Picture

    wxTextCtrl *teditPictureFilePath;

    wxStaticBitmap *staticBmpPicture;

    wxBitmap bmpPictureEmpty;

    wxImage imagePicture;

    int ncmId;

    wxTextCtrl *teditFiscalCodigoNCM;

    wxTextCtrl *teditFiscalNCMDescription;

    wxChoice *cboxIcmsCstA;

    wxChoice *cboxIcmsCstB;

    wxAuiToolBar *toolBarMiscTaxes;


    void OnSaveRequest(wxCommandEvent &event);

    void OnAcquisitionDateChooseRequest(wxCommandEvent &event);

    void OnExpiryDateChooseRequest(wxCommandEvent &event);

    void OnDepreciationBeginDateChooseRequest(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnChoosePictureFileRequest(wxCommandEvent &event);

    int LoadIcmsCstTables();

    void OnDeleteRequest(wxCommandEvent &event);

    void OnNcmChooseDialogRequest(wxCommandEvent &event);

    void OnCloseRequest(wxCloseEvent& event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelEstateNew(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Loads groups and subgroups.
     */
    int LoadGroupsListing();

    /**
     * Loads measuments listing.
     */
    int LoadMeasureListing();
};


#endif  /* _PANEL_ESTATE_NEW_HPP_ */