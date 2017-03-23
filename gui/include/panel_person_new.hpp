/**
 * Client registering dialog.
 *
 * \file  panel_person_new.hpp
 * \version 1.0
 * \since 01/29/2007 10:55:20 AM AMT
 * \revision  none
 * \author Allann Jones <alljols[at]yahoo.com.br>
 */

#ifndef _PANEL_PERSON_NEW_HPP_
#define _PANEL_PERSON_NEW_HPP_


#include "utils_include.hpp"


class PanelPersonNew: public wxScrolledWindow
{
  private:
    wxStaticBitmap *staticBmpPicture;

    wxImage imagePhoto;

    wxString photoFilePath;

    wxFlexGridSizer *szgMainFields;

    wxBoxSizer *szFullName;
    wxStaticText *labelFullName;
    wxTextCtrl *teditFullName;

    wxStaticText *labelFantasyName;
    wxTextCtrl *teditFantasyName;

    wxChoice *choicePersonNature;

    wxStaticText *lblCpfCnpj;
    wxTextCtrl *teditCPF_CNPJ;

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
    wxTextCtrl *teditRGState;
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

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent &event);

    /**
     * Key hook handler.
     */
    void OnCharHook(wxKeyEvent& event);

    /**
     * Request for choose a born date.
     */
    void OnBornDateChooseRequest(wxCommandEvent &event);

    void OnRGDateChooseRequest(wxCommandEvent &event);

    void SetClientAsPerson(bool isPerson);

    void OnPersonTypeChange(wxCommandEvent &event);

    void OnPhotoChooseRequest(wxCommandEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    PanelPersonNew(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();
};


#endif  /* _PANEL_PERSON_NEW_HPP_ */