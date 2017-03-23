/**
 * Enterprise main dialog. Header.
 *
 * \file panel_owner_edit.hpp
 * \version 1.0
 * \since 22/10/2008
 * \revision  none
 * \author: Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_OWNER_EDIT_HPP_
#define _PANEL_OWNER_EDIT_HPP_


#include "utils_include.hpp"


class PanelOwnerEdit: public wxScrolledWindow
{
  private:
    wxStaticBitmap *staticBmpPicture;

    wxImage imagePhoto;

    wxString photoFilePath;

    wxFlexGridSizer *szgMainFields;

    wxChoice *cboxClientType; /**< Client type list. */

    wxBoxSizer *szFullName;

    wxStaticText *labelFullName;

    wxTextCtrl *teditFullName;

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

    wxTextCtrl *teditEmail02;

    wxTextCtrl *teditHomePage;

    wxTextCtrl *teditNotes;

    wxBitmapButton *btnBornDateChoose;

    /**
     * Char hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    void OnSaveRequest(wxCommandEvent &event);

    void SetAsPerson(bool isPerson);

    void PersonTypeChange();

    void OnPersonTypeChange(wxCommandEvent &event);

    void OnPhotoChooseRequest(wxCommandEvent &event);

    /**
     * Request for choose a born date.
     */
    void OnBornDateChooseRequest(wxCommandEvent &event);

    void OnRGDateChooseRequest(wxCommandEvent &event);

    int SaveData();

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelOwnerEdit(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    int LoadData();
};


#endif  /* _PANEL_OWNER_EDIT_HPP_ */