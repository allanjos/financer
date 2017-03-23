/**
 * Client editing dialog.
 *
 * \file  dlg_client_new.hpp
 * \version 1.0
 * \since 01/29/2007 10:55:20 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_PERSON_EDIT_HPP_
#define _PANEL_PERSON_EDIT_HPP_


#include "utils_include.hpp"


class PanelPersonEdit: public wxScrolledWindow
{
  private:
    int customerId;

    int personId;

    int enterpriseId;

    wxStaticBitmap *staticBmpPicture;

    wxImage imagePhoto;

    wxString photoFilePath;

    wxRadioButton *radioBtnClientIsAEnterprise;

    wxFlexGridSizer *szgMainFields;

    wxBoxSizer *szFullName;

    wxStaticText *labelFullName;

    wxTextCtrl *teditFullName;

    wxChoice *cboxClientType; /**< Client type list. */

    wxChoice *choicePersonNature;

    wxStaticText *lblCpfCnpj;

    wxTextCtrl *teditCPF_CNPJ;

    wxStaticText *labelFantasyName;
    wxTextCtrl *teditFantasyName;
    wxStaticText *labelInscricaoEstadual;
    wxTextCtrl *teditInscricaoEstadual;

    wxStaticText *labelRgNumber;
    wxTextCtrl *teditRgNumber;
    wxStaticText *labelRgOrgao;
    wxTextCtrl *teditRgOrgao;
    wxStaticText *labelRgExpedicaoDate;
    wxTextCtrl *teditRGExpedicaoDate;
    wxBitmapButton *btnRgDateChoose;
    wxStaticText *labelRgCountry;
    wxTextCtrl *teditRgCountry;
    wxStaticText *labelRgFederalState;
    wxTextCtrl *teditRgUF;
    wxStaticText *labelRgCity;
    wxTextCtrl *teditRgCity;

    wxChoice *cboxSex;

    wxTextCtrl *teditBornDate;

    wxTextCtrl *teditAddressCEP;
    wxTextCtrl *teditAddressStreet;
    wxTextCtrl *teditAddressComplement;
    wxTextCtrl *teditAddressNumber;
    wxTextCtrl *teditAddressCountry;
    wxTextCtrl *teditAddressState;
    wxTextCtrl *teditAddressCity;
    wxTextCtrl *teditAddressDistrict;

    wxTextCtrl *teditPhone01;
    wxTextCtrl *teditPhoneContactName01;
    wxChoice *choicePhoneType01;

    wxTextCtrl *teditPhone02;
    wxTextCtrl *teditPhoneContactName02;
    wxChoice *choicePhoneType02;

    wxTextCtrl *teditEmail;

    wxTextCtrl *teditHomePage;


    wxTextCtrl *teditNotes;


    wxBitmapButton *btnBornDateChoose;

    wxListCtrl *listCtrlSales;

    wxListCtrl *listCtrlBudgets;

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Delete information request.
     */
    void OnDeleteRequest(wxCommandEvent& event);

    /**
     * Close interface request.
     */
    void OnCloseRequest(wxCommandEvent &event);

    /**
     * Key hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void SetAsPerson(bool isPerson);

    void PersonTypeChange();

    void OnPersonTypeChange(wxCommandEvent &event);

    void OnPhotoChooseRequest(wxCommandEvent &event);

    /**
     * Request for choose a born date.
     */
    void OnBornDateChooseRequest(wxCommandEvent &event);

    void OnRGDateChooseRequest(wxCommandEvent &event);

    void OnSetClientAsEnterprise(wxCommandEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelPersonEdit(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Load all data on window.
     */
    int LoadData(int personId);

    int SaveData();
};


#endif  /* _PANEL_PERSON_EDIT_HPP_ */