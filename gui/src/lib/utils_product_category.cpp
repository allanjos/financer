#include "utils_product_category.hpp"
#include "utils_database.hpp"


wxString ProductCategoryUtils::GetCompoundName(long categoryId)
{
  DBQuery dbquery;
  wxString compoundName = wxT("");

  while (1) {
    int rc = dbquery.Execute(wxString::Format(wxT("SELECT categoria_id, categoria_parent_id, nome ")
                                              wxT("FROM produto.categoria ")
                                              wxT("WHERE categoria_id = %ld"), categoryId));

    if (rc != 0) {
      wxMessageBox(wxString(wxT("Não foi possível obter a listagem de categorias de produtos cadastradas.\n\n")) +
                   wxString(wxT("Erro interno do banco: ")) + dbquery.GetMessageStr());

      return wxEmptyString;
    }
    else if (dbquery.GetRowsCount() > 0) {
      if (compoundName.Length() > 0) {
        compoundName = dbquery.GetFieldStrByName(0, wxT("nome")) + wxT(" / ") + compoundName;
      }
      else {
        compoundName = dbquery.GetFieldStrByName(0, wxT("nome"));
      }

      categoryId = dbquery.GetFieldLongByName(0, wxT("categoria_parent_id"));

      if (dbquery.GetFieldLongByName(0, wxT("categoria_parent_id")) < 1) {
        break;
      }
    }
  }

  return compoundName;
}
