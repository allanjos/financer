/**
 * Class utilities to handle taxes.
 *
 * \author Allann Jones <alljols@yaho.com.br>
 * \since 08/03/2009
 */

#ifndef _UTILS_TAX_HPP_
#define _UTILS_TAX_HPP_


#include <vector>
#include "gui_lib.hpp"


class TaxIPI {
  private:
    wxString code;

    double percentage;

  public:
    TaxIPI();

    void CopyFrom(TaxIPI *tax);

    int SetCode(wxString code);

    int SetPercentage(double percentage);

    wxString GetCode();

    double GetPercentage();
};


class TaxICMS {
  private:
    wxString codeA;

    wxString codeB;

    double percentage;

    double calculusBase;

  public:
    TaxICMS();

    int SetCodeA(wxString codeA);

    int SetCodeB(wxString codeB);

    int SetPercentage(double percentage);

    int SetCalculusBase(double calculusBase);

    wxString GetCodeA();

    wxString GetCodeB();

    double GetPercentage();

    double GetCalculusBase();

    void CopyFrom(TaxICMS *icms);
};


class Tax {
  public:
    int id;

    wxString name;

    double percentage;

    wxString typeIdStr;

    Tax();

    int GetId();

    wxString GetName();

    wxString GetTypeId();

    double GetPercentage();

    void SetId(int id);

    void SetName(wxString name);

    void SetTypeId(wxString typeId);

    void SetPercentage(double percentage);
};


class ProductTaxes {
  public:
    long productId;

    TaxIPI ipi;

    TaxICMS icms;

    std::vector<Tax> taxes;

    ProductTaxes();

    void CopyFrom(ProductTaxes *productTaxes);

    int SetProductId(long productId);

    long GetProductId();

    int AddTax(Tax tax);

    void ClearTaxes();

    size_t GetTaxesCount();

    Tax GetTax(size_t index);

    int SetICMS(TaxICMS *icms);

    int GetICMS(TaxICMS *icms);

    int SetIPI(TaxIPI *ipi);

    int GetIPI(TaxIPI *ipi);

    std::vector<Tax> GetTaxes();
};


#endif /* _UTILS_TAX_HPP_ */
