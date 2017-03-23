#include "utils_tax.hpp"


TaxIPI::TaxIPI() {
  this->code = wxT("");

  this->percentage = 0.0;
}


void TaxIPI::CopyFrom(TaxIPI *tax)
{
  this->code = tax->code;

  this->percentage = tax->percentage;
}


int TaxIPI::SetCode(wxString code)
{
  this->code = code;

  return 0;
}


int TaxIPI::SetPercentage(double percentage)
{
  this->percentage = percentage;

  return 0;
}


wxString TaxIPI::GetCode()
{
  return this->code;
}


double TaxIPI::GetPercentage()
{
  return this->percentage;
}


TaxICMS::TaxICMS()
{
  this->codeA = wxT("");

  this->codeB = wxT("");

  this->percentage = 0.0;

  this->calculusBase = 0.0;
}


int TaxICMS::SetCodeA(wxString codeA) {
  this->codeA = codeA;

  return 0;
}


int TaxICMS::SetCodeB(wxString codeB) {
  this->codeB = codeB;

  return 0;
}


int TaxICMS::SetPercentage(double percentage) {
  this->percentage = percentage;

  return 0;
}


int TaxICMS::SetCalculusBase(double calculusBase) {
  this->calculusBase = calculusBase;

  return 0;
}


wxString TaxICMS::GetCodeA() {
  return this->codeA;
}


wxString TaxICMS::GetCodeB() {
  return this->codeB;
}


double TaxICMS::GetPercentage() {
  return this->percentage;
}


double TaxICMS::GetCalculusBase() {
  return this->calculusBase;
}


void TaxICMS::CopyFrom(TaxICMS *icms)
{
  if (!icms) {
    return;
  }

  this->codeA = icms->codeA;

  this->codeB = icms->codeB;

  this->percentage = icms->percentage;

  this->calculusBase = icms->calculusBase;
}



Tax::Tax()
{
  this->name = wxT("");

  this->typeIdStr = wxT("");

  this->percentage = 0.0;
}


int Tax::GetId()
{
  return this->id;
}


wxString Tax::GetName()
{
  return this->name;
}


wxString Tax::GetTypeId()
{
  return this->typeIdStr;
}


double Tax::GetPercentage()
{
  return this->percentage;
}


void Tax::SetId(int id)
{
  this->id = id;
}


void Tax::SetName(wxString name)
{
  this->name = name;
}


void Tax::SetTypeId(wxString typeIdStr)
{
  this->typeIdStr = typeIdStr;
}


void Tax::SetPercentage(double percentage)
{
  this->percentage = percentage;
}



ProductTaxes::ProductTaxes()
{
  this->productId = -1;

  this->taxes.clear();
}


std::vector<Tax> ProductTaxes::GetTaxes(void)
{
  return this->taxes;
}


void ProductTaxes::CopyFrom(ProductTaxes *productTaxes)
{
  if (!productTaxes) {
    return;
  }

  SetProductId(productTaxes->GetProductId());

  productTaxes->GetIPI(&this->ipi);

  productTaxes->GetICMS(&this->icms);

  this->taxes = productTaxes->GetTaxes();
}


int ProductTaxes::SetProductId(long productId)
{
  this->productId = productId;

  return 0;
}


long ProductTaxes::GetProductId()
{
  return this->productId;
}


int ProductTaxes::AddTax(Tax tax)
{
  this->taxes.push_back(tax);

  return 0;
}

void ProductTaxes::ClearTaxes(void)
{
  this->taxes.clear();
}


size_t ProductTaxes::GetTaxesCount()
{
  return this->taxes.size();
}


Tax ProductTaxes::GetTax(size_t index)
{
  return this->taxes.at(index);
}


int ProductTaxes::SetICMS(TaxICMS *icms) {
  this->icms.CopyFrom(icms);

  return 0;
}


int ProductTaxes::GetICMS(TaxICMS *icms) {
  if (!icms) {
    return -1;
  }

  icms->CopyFrom(&this->icms);

  return 0;
}


int ProductTaxes::SetIPI(TaxIPI *ipi) {
  this->ipi.CopyFrom(ipi);

  return 0;
}


int ProductTaxes::GetIPI(TaxIPI *ipi) {
  if (!ipi) {
    return -1;
  }

  ipi->CopyFrom(&this->ipi);

  return 0;
}
