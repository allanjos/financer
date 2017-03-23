/**
 * Registering dialog - Header file.
 *
 * \version 1.0
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_ESTATE_EDIT_HPP_
#define _PANEL_ESTATE_EDIT_HPP_


#include "utils_include.hpp"


class PanelEstateEdit: public wxScrolledWindow
{
  private:
    int goodId;

    wxAuiNotebook *nbookProductInfo;

    wxTextCtrl *teditName;

    wxTextCtrl *teditCode;

    wxTextCtrl *teditProductDescription;

    wxTextCtrl *teditAcquisitionDate;

    wxBitmapButton *btnCalendarAcquisitionDate;

    wxComboBox *cboxProductType;

    wxComboBox *cboxProductManufacturer;

    wxComboBox *cboxProductSupplier;

    wxComboBox *cboxProductTrademark;

    wxComboBox *cboxProductMeasureUnit;

    wxTextCtrl *teditProductWeigthLiquid;

    wxTextCtrl *teditProductTempoGarantia;

    wxChoice *cboxProductTempoGarantiaType;

    wxTextCtrl *teditProductWeigthBrute;

    wxTextCtrl *teditProductValue;

    wxTextCtrl *teditProductLiquidValue;

    wxTextCtrl *teditProductValueMarket;

    wxTextCtrl *teditDepreciacaoTaxa;

    wxTextCtrl *teditDepreciacaoDate;

    wxBitmapButton *btnCalendarDepreciacaoDate;

    wxChoice *choiceDepreciacaoAcelerada;

    wxTextCtrl *teditDepreciacaoAcumulada;

    wxTextCtrl *teditSeguroCompanhia;

    wxTextCtrl *teditSeguroApolice;

    wxTextCtrl *teditSeguroApoliceVencimentoDate;

    wxBitmapButton *btnCalendarApoliceVencimentoDate;


    wxTextCtrl *teditCategoryName;

    wxBitmapButton *btnProductImage;

    wxCheckBox *chkProductActive;

    wxTextCtrl *teditCategoryId;


    // Physical identification

    wxTextCtrl *teditSerieNumber;

    wxTextCtrl *teditBarCode;


    // Dimensions

    wxTextCtrl *teditProductLength;

    wxTextCtrl *teditProductWidth;

    wxTextCtrl *teditProductHeight;


    wxTextCtrl *teditTechnicalInfo;

    wxChoice *cboxProductPurchase;

    wxChoice *cboxProductBuyPriceCurrency;

    wxTextCtrl *teditProductBuyPrice;

    wxChoice *cboxProductCommonBuyPriceCurrency;

    wxTextCtrl *teditProductCommonBuyPrice;

    wxTextCtrl *teditWantedBuyPrice;

    wxTextCtrl *teditProductBuyQuantity;

    wxChoice *cboxProductEnterprise;

    // Stock

    wxChoice *cboxProductLocation;

    wxTextCtrl *teditProductQuantity;

    wxTextCtrl *teditProductStoreCode;

    wxTextCtrl *teditProductBarCode;

    wxTextCtrl *teditProductQttyMin;

    wxTextCtrl *teditProductQttyRenew;

    // Sale information

    wxTextCtrl *teditProductSaleGain;

    // Fiscal Information

    wxTextCtrl *teditProductFiscalCodigoIPI;

    wxStaticBox* staticBoxFiscalInfoMiscTaxes;

    wxListCtrl *listCtrlMiscTaxes;

    // Picture

    wxTextCtrl *teditProductPictureFilePath;

    wxStaticBitmap *staticBmpProductPicture;

    wxBitmap bmpProductPictureEmpty;

    // Composition controle

    wxListCtrl *listCtrlComposition;

    wxPanel *panelComposition;

    wxTextCtrl *teditComponentAddProductCode;

    wxTextCtrl *teditComponentAddProductName;

    wxTextCtrl *teditComponentAddProductQuantity;

    wxImage imageProductPicture;


    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;


    void OnSaveRequest(wxCommandEvent &event);

    void OnAcquisitionDateChooseRequest(wxCommandEvent &event);

    void OnExpiryDateChooseRequest(wxCommandEvent &event);

    void OnDepreciationBeginDateChooseRequest(wxCommandEvent &event);

    //int LoadProductInfo(wxString goodId, wxString quantityStr);

    void OnProductTypeChoose(wxCommandEvent &event);

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnChoosePictureFileRequest(wxCommandEvent &event);

    //void OnProductComponentSearchRequest(wxCommandEvent &event);

    //void OnAddProductToCompositionRequest(wxCommandEvent &event);

    void OnCategoryChooseRequest(wxCommandEvent &event);

    void OnDeleteRequest(wxCommandEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelEstateEdit(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadData(int goodId);

    /**
     * Loads groups and subgroups.
     */
    int LoadGroupsListing();

    /**
     * Loads manufacturers listing.
     */
    int LoadManufacturersListing();

    /**
     * Loads trademark listing.
     */
    int LoadTrademarksListing();

    /**
     * Loads measuments listing.
     */
    int LoadMeasureListing();

    int LoadSuppliersListing();

    int LoadEnterprisesListing();
};


#endif  /* _PANEL_ESTATE_EDIT_HPP_ */