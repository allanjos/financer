// AccountDataModel

#include "model_account_data.hpp"
#include "utils_lang.hpp"


AccountDataModelNode::AccountDataModelNode(AccountDataModelNode* parent,
                                           int accountId,
                                           const wxString &account_name,
                                           const wxString &type,
                                           const wxString &balance)
{
  m_parent = parent;

  this->accountId = accountId;

  accountName = account_name;

  accountType = type;

  accountBalance = balance;

  m_isContainer = false;
}


AccountDataModelNode::AccountDataModelNode(AccountDataModelNode* parent,
                                           const wxString &account_name)
{
  this->m_parent = parent;
  this->accountId = 0;
  this->accountName = account_name;
  this->accountType = wxT("");
  this->accountBalance = wxT("");
  this->m_isContainer = true;

  /*
  PGresult *res;

  res = PQexec(g_dbConnection, "SELECT contabil.conta_get_balance() AS balance");

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    wxMessageBox(wxString(wxT("Não foi possível obter o balanço geral das contas.\n\n")) +
                 wxString(wxT("Erro interno do banco: ")) + wxString(PQerrorMessage(g_dbConnection),
                                                                     wxConvUTF8,
                                                                     strlen(PQerrorMessage(g_dbConnection))));
  }
  else {
    char *fieldValue;

    fieldValue = PQgetvalue(res, 0, PQfnumber(res, "balance"));

    this->accountBalance = wxString(fieldValue, wxConvUTF8, strlen(fieldValue));
  }

  PQclear(res);
  */
}


AccountDataModelNode::~AccountDataModelNode()
{
  size_t count = m_children.GetCount();
  size_t i;

  for (i = 0; i < count; i++) {
    AccountDataModelNode *child = m_children[i];

    delete child;
  }
}


void AccountDataModelNode::SetAsContainer(bool isContainer)
{
  m_isContainer = isContainer;
}


bool AccountDataModelNode::IsContainer()
{
  return m_isContainer;
}


AccountDataModelNode* AccountDataModelNode::GetParent()
{
  return m_parent;
}


AccountDataModelNodes &AccountDataModelNode::GetChildren()
{
  return m_children;
}


AccountDataModelNode* AccountDataModelNode::GetNthChild(unsigned int n)
{
  return m_children.Item(n);
}


void AccountDataModelNode::Insert(AccountDataModelNode* child, unsigned int n)
{
  m_children.Insert(child, n);
}


void AccountDataModelNode::Append(AccountDataModelNode* child)
{
  m_children.Add(child);
}

unsigned int AccountDataModelNode::GetChildCount() {
  return m_children.GetCount();
}



// constructor

AccountDataModel::AccountDataModel()
{
  m_root = NULL;

  LoadData();
}


int AccountDataModel::DeleteNodeChildren(AccountDataModelNode* node)
{
#if 0
  if (node == NULL) {
    return -1;
  }

  size_t count = node->GetChildren().GetCount();

  if (count == 0) {
    //AccountDataModelNode* parent = node->GetParent();

    //node->GetChildren().Clear();

    AccountDataModelNode* parent = node->GetParent();

    if (parent) {
      //parent->GetChildren().RemoveAt(parent->GetChildren().GetCount() - 1);
      parent->GetChildren().Remove(node);
    }

    delete node;

    //if (index == 0) {
    //  DeleteNodeChildren(parent, -1);
    //}

    return 0;
  }
  else {
    AccountDataModelNode *child = node->GetChildren().Item(count - 1);

    DeleteNodeChildren(child);

    /*
    AccountDataModelNode* parent = node->GetParent();

    for (size_t itemIndex = count - 1; itemIndex >= 0; itemIndex--) {
      AccountDataModelNode *child = node->GetChildren().Item(itemIndex);

      DeleteNodeChildren(child, itemIndex);

      break;
    }
    */
  }
#endif

  return 0;
}


int AccountDataModel::LoadData() {
  if (m_root != NULL) {
    DeleteNodeChildren(m_root);

    m_root = NULL;

    // Clear current data to reload it after

    //m_root->m_children.Clear();

    //delete m_root;
  }


  m_root = new AccountDataModelNode(NULL, Lang::Get("Accounts"));

  //LoadAccountChildList(m_root);

  dataModelNodeAccountActiveRoot = new AccountDataModelNode(m_root,
                                                            0,
                                                            wxT("1. ") + Lang::Get("Assets"),
                                                            wxT(""),
                                                            wxT(""));

  //dataModelNodeAccountActiveRoot->SetAsContainer(true);

  m_root->Append(dataModelNodeAccountActiveRoot);

  LoadAccountChildList(dataModelNodeAccountActiveRoot, "A");


  dataModelNodeAccountPassiveRoot = new AccountDataModelNode(m_root,
                                                             0,
                                                             wxT("2. ") + Lang::Get("Liabilities"),
                                                             wxT(""),
                                                             wxT(""));

  //dataModelNodeAccountPassiveRoot->SetAsContainer(true);

  m_root->Append(dataModelNodeAccountPassiveRoot);

  LoadAccountChildList(dataModelNodeAccountPassiveRoot, "P");


  dataModelNodeAccountOfResultRoot = new AccountDataModelNode(m_root,
                                                              0,
                                                              wxT("3. ") + Lang::Get("Profit and loss"),
                                                              wxT(""),
                                                              wxT(""));

  //dataModelNodeAccountOfResultRoot->SetAsContainer(true);

  m_root->Append(dataModelNodeAccountOfResultRoot);

  LoadAccountChildList(dataModelNodeAccountOfResultRoot, "R");

  return 0;
}


AccountDataModel::~AccountDataModel()
{
  delete m_root;
}


AccountDataModelNode *AccountDataModel::GetRoot(void)
{
  return m_root;
}


int AccountDataModel::GetAccountId(const wxDataViewItem &item) const
{
  AccountDataModelNode *node = (AccountDataModelNode *) item.GetID();

  if (!node) {
    return -1;
  }

  return node->accountId;
}


wxString AccountDataModel::GetTitle(const wxDataViewItem &item) const
{
  AccountDataModelNode *node = (AccountDataModelNode *) item.GetID();

  if (!node) {
    return wxEmptyString;
  }

  return node->accountName;
}


int AccountDataModel::LoadAccountChildList(AccountDataModelNode* dataModelNode, const char *accountType)
{
  //AccountDataModelNode *childDataModelNode = new AccountDataModelNode(dataModelNode,
  //                                                                    ++itemId,
  //                                                                    wxT("conta"),
  //                                                                    wxT("tipo"),
  //                                                                    wxT("balanço"));

  //dataModelNode->Append(childDataModelNode);

  LogMessenger *messenger = LogMessenger::GetInstance();


  DBQuery dbquery;

  wxString sql;

  if (accountType != NULL) {
    sql = wxString::Format(wxT("SELECT a.*, ")
                                  wxT("st.nome AS tipo_nome, ")
                                  wxT("c.sigla AS moeda_sigla ")
                                  //wxT("contabil.conta_get_balance(a.account_id) AS balance ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" a ")
                           wxT("LEFT JOIN ") + DB::GetTableName("accounting.account.subtype") + wxT(" st ON (a.subtipo_id = st.subtipo_id) ")
                           wxT("LEFT JOIN ") + DB::GetTableName("monetary.currency") + wxT(" c ON (c.currency_id = a.currency_id) ")
                           wxT("WHERE conta_parent_id IS NULL ")
                           wxT("AND tipo_id = '%s' ")
                           wxT("ORDER BY a.codigo, lower(a.nome)"), accountType);
  }
  else {
    sql = wxString::Format(wxT("SELECT a.*, ")
                                  wxT("st.nome AS tipo_nome, ")
                                  wxT("c.sigla AS moeda_sigla ")
                                  //wxT("contabil.conta_get_balance(a.account_id) AS balance ")
                           wxT("FROM ") + DB::GetTableName("accounting.account") + wxT(" a ")
                           wxT("LEFT JOIN ") + DB::GetTableName("accounting.account.subtype") + wxT(" st ON (a.subtipo_id = st.subtipo_id) ")
                           wxT("LEFT JOIN ") + DB::GetTableName("monetary.currency") + wxT(" c ON (c.currency_id = a.currency_id) ")
                           wxT("WHERE a.conta_parent_id = %d ")
                           wxT("ORDER BY a.codigo, lower(a.nome)"), dataModelNode->accountId);
  }

  messenger->Register(sql, LogMessage::MSG_DBG);


  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível obter a listagem de contas cadastradas."), LogMessage::MSG_ERROR);
  }
  else if (dbquery.GetRowsCount() > 0) {
    dataModelNode->SetAsContainer(true);

    for (int resultIndex = 0; resultIndex < dbquery.GetRowsCount(); resultIndex++) {
      // Account code

      wxString accountCode = dbquery.GetFieldStrByName(resultIndex, wxT("codigo"));

      // Account name

      wxString accountName = dbquery.GetFieldStrByName(resultIndex, wxT("nome"));

      // Type name

      wxString accountTypeName = dbquery.GetFieldStrByName(resultIndex, wxT("tipo_nome"));

      // Currency

      wxString currencyAcronym = dbquery.GetFieldStrByName(resultIndex, wxT("moeda_sigla"));


      // Account balance

      wxString balanceStr = ""; // = dbquery.GetFieldStrByName(resultIndex, wxT("balance"));

      DBQuery dbqueryBalance;

      /*
      sql = wxString::Format("SELECT SUM(VALOR) "
                             "FROM contabil.bank_account_event "
                             "WHERE account_id = %d", dbquery.GetFieldIntByName(resultIndex, "account_id"));

      //wxMessageBox(sql);

      int rc = dbqueryBalance.Execute(sql);

      if (rc != 0) {
        wxMessageBox(wxT("Não foi possível obter o balanço da conta ") + accountName);
      }
      else {
        balanceStr = dbqueryBalance.GetFieldStr(resultIndex, 0);

        if (balanceStr.Length() == 0) {
          balanceStr = "0,00";
        }
      }
      */

      balanceStr = "0,00";

      //wxMessageBox(dbqueryBalance.GetFieldStr(resultIndex, 0));

      // Data view node

      wxString caption = "";

      if (accountCode.Length() > 0) {
        caption = accountCode + " - ";
      }

      caption += accountName;

      AccountDataModelNode *childDataModelNode = new AccountDataModelNode(dataModelNode,
                                                                          dbquery.GetFieldIntByName(resultIndex, "account_id"),
                                                                          caption,
                                                                          accountTypeName,
                                                                          currencyAcronym + " " + balanceStr);
      childDataModelNode->SetAsContainer(true);

      dataModelNode->Append(childDataModelNode);

      // Load children if there are

      LoadAccountChildList(childDataModelNode);
    }
  }


  return 0;
}

// helper methods to change the model

void AccountDataModel::Delete(const wxDataViewItem &item)
{
    AccountDataModelNode *node = (AccountDataModelNode*) item.GetID();

    wxDataViewItem parent(node->GetParent());

    node->GetParent()->GetChildren().Remove(node);

    delete node;

    // notify control
    ItemDeleted(parent, item);
}


// override sorting to always sort branches ascendingly

int AccountDataModel::Compare(const wxDataViewItem &item1,
                          const wxDataViewItem &item2,
                          unsigned int column,
                          bool ascending)
{
  if (IsContainer(item1) && IsContainer(item2)) {
    wxVariant value1,value2;

    GetValue(value1, item1, 0);
    GetValue(value2, item2, 0);

    wxString str1 = value1.GetString();
    wxString str2 = value2.GetString();

    int res = str1.Cmp(str2);

    if (res) return res;

    // items must be different
    wxUIntPtr litem1 = (wxUIntPtr) item1.GetID();
    wxUIntPtr litem2 = (wxUIntPtr) item2.GetID();

    return litem1-litem2;
  }

  return wxDataViewModel::Compare( item1, item2, column, ascending );
}


// implementation of base class virtuals to define model

unsigned int AccountDataModel::GetColumnCount() const
{
  return 3;
}


wxString AccountDataModel::GetColumnType(unsigned int col) const
{
  return wxT("string");
}


bool AccountDataModel::HasContainerColumns(const wxDataViewItem &item) const
{
  return true;
}


void AccountDataModel::GetValue(wxVariant &variant,
                            const wxDataViewItem &item,
                            unsigned int col) const
{
  AccountDataModelNode *node = (AccountDataModelNode*) item.GetID();

  switch (col) {
    case 0:
      variant = node->accountName;
      break;

    case 1:
      variant = node->accountType;
      break;

    case 2:
      variant = node->accountBalance;
      break;
  }
}


// Colors

bool AccountDataModel::GetAttr(unsigned int row, unsigned int col, wxDataViewItemAttr &attr)
{
  wxMessageBox(wxT("GetAttr()"));

  attr.SetColour(*wxBLUE);

  attr.SetItalic(true);

  return true;
}


bool AccountDataModel::SetValue(const wxVariant &variant,
                            const wxDataViewItem &item,
                            unsigned int col)
{
    AccountDataModelNode *node = (AccountDataModelNode*) item.GetID();

    switch (col) {
      case 0:
        node->accountName = variant.GetString();
        return true;

      case 1:
        node->accountType = variant.GetString();
        return true;

      case 2:
        node->accountBalance = variant.GetString();
        return true;

      default:
        wxLogError(wxT("AccountDataModel::SetValue: wrong column"));
    }

    return false;
}


wxDataViewItem AccountDataModel::GetParent(const wxDataViewItem &item) const
{
  // the invisble root node has no parent
  if (!item.IsOk())
    return wxDataViewItem(0);

  AccountDataModelNode *node = (AccountDataModelNode *) item.GetID();

  // "MyMusic" also has no parent
  if (node == m_root)
    return wxDataViewItem(0);

  return wxDataViewItem((void*) node->GetParent());
}


bool AccountDataModel::IsContainer(const wxDataViewItem &item) const
{
    // the invisble root node can have children (in
    // our model always "MyMusic")
    if (!item.IsOk())
      return true;

    AccountDataModelNode *node = (AccountDataModelNode*) item.GetID();

    return node->IsContainer();
}


unsigned int AccountDataModel::GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const
{
  AccountDataModelNode *node = (AccountDataModelNode*) parent.GetID();

  if (!node) {
    array.Add(wxDataViewItem((void*) m_root));

    return 1;
  }

  /*
  if (node == m_classical) {
    AccountDataModel *model = (AccountDataModel*)(const AccountDataModel*) this;

    model->m_classicalMusicIsKnownToControl = true;
  }
  */

  if (node->GetChildCount() == 0) {
    return 0;
  }

  unsigned int count = node->GetChildren().GetCount();
  unsigned int pos;

  for (pos = 0; pos < count; pos++) {
    AccountDataModelNode *child = node->GetChildren().Item(pos);

    array.Add(wxDataViewItem((void*) child));
  }

  return count;
}

// DnD

bool AccountDataModel::IsDraggable(const wxDataViewItem &item)
{
  // only drag items
  return (!IsContainer(item));
}


size_t AccountDataModel::GetDragDataSize( const wxDataViewItem &item, const wxDataFormat &WXUNUSED(format) )
{
  wxPrintf("GetDragDataSize\n");

  AccountDataModelNode *node = (AccountDataModelNode*) item.GetID();
  wxString data;

  data += node->accountName; data += wxT(" ");
  data += node->accountType;

  return strlen(data.utf8_str()) + 1;
}


bool AccountDataModel::GetDragData(const wxDataViewItem &item, const wxDataFormat &WXUNUSED(format),
                         void* dest, size_t WXUNUSED(size) )
{
    wxPrintf( "GetDragData\n" );

    AccountDataModelNode *node = (AccountDataModelNode*) item.GetID();
    wxString data;

    data += node->accountName; data += wxT(" ");
    data += node->accountType;

    wxCharBuffer buffer(data.utf8_str());

    memcpy(dest, buffer, strlen(buffer) + 1);

    return true;
}

/*
static int my_sort_reverse( int *v1, int *v2 )
{
   return *v2-*v1;
}


static int my_sort( int *v1, int *v2 )
{
   return *v1-*v2;
}
*/