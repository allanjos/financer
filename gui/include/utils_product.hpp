#ifndef _UTILS_PRODUCT_HPP_
#define _UTILS_PRODUCT_HPP_

#include "utils_monetary.hpp"
#include "utils_tax.hpp"
#include "gui_lib.hpp"

class Product {
    long id;

    wxString name;

  public:
    Product();

    Product(long id);

    int GetInfo();

    void SetId(long id);

    long GetId(bool) {
      return this->id;
    }
};


class ProductUtils {
  public:
    static bool GetPrice(long productId, Price &price);
};


class ProductTax {
  private:
    int id;

    wxString name;

    char typeId;

    double percentage;

  public:

    ProductTax();

    unsigned int GetId();

    wxString GetName();

    char GetTypeId();

    double GetPercentage();

    void SetId(unsigned int id);

    void SetName(wxString name);

    void SetTypeId(char typeId);

    void SetPercentage(double percentage);
};


class ProductPurchase {
  private:
    long productId;

    long supplierId;

    double quantity;

    double purchasePriceUnity;

    double purchasePriceTotal;

  public:
    ProductPurchase();

    ProductTaxes productTaxes;

    void SetProductId(long productId);

    void SetSupplierId(long supplierId);

    void SetQuantity(double quantity);

    void SetPriceUnity(double purchasePriceUnity);

    void SetPriceTotal(double purchasePriceTotal);

    long GetProductId();

    long GetSupplierId();

    double GetQuantity();

    double GetPriceUnity();

    double GetPriceTotal();
};

#endif /* _UTILS_PRODUCT_HPP_ */
