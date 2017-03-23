#include "utils_database_postgresql.hpp"

#include "utils_config.hpp"
#include "utils_log.hpp"
#include <stdlib.h>

DBSchemaHash DBPostgreSQL::dbSchemaHash;

/* Database class functions */

DBPostgreSQL::DBPostgreSQL()
{
  LogMessenger *messenger = LogMessenger::GetInstance();

  DBPostgreSQL::InitializeSchema();

  /*
  wxString sql = "SET datestyle = \"ISO, DMY\"";

  DBPostgreSQLQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    messenger->Register(wxT("Não foi possível modificar o estilo de data do banco de dados."), LogMessage::MSG_ERROR);
  }
  */
}


DBPostgreSQL::~DBPostgreSQL()
{
}


DBPostgreSQL *DBPostgreSQL::GetInstance()
{
  // This line only runs once, thus creating the only existent instance

  static DBPostgreSQL *instance = new DBPostgreSQL();

  return instance; // Always returns the same instance
}


void DBPostgreSQL::InitializeSchema()
{
  // Configuration
  DBPostgreSQL::dbSchemaHash["configuration"] = "configuration.configuration";

  // Monetary
  DBPostgreSQL::dbSchemaHash["monetary.currency"] = "monetario.moeda";

  // Financial
  DBPostgreSQL::dbSchemaHash["financial.income"] = "financeiro.receita";
  DBPostgreSQL::dbSchemaHash["financial.income.payment.installment"] = "financeiro.receita_pagamento_parcela";
  DBPostgreSQL::dbSchemaHash["financial.expense"] = "financeiro.despesa";
  DBPostgreSQL::dbSchemaHash["financial.expense.payment.installment"] = "financeiro.despesa_pagamento_parcela";

  // Owner
  DBPostgreSQL::dbSchemaHash["owner"] = "owner.owner";
  DBPostgreSQL::dbSchemaHash["owner.picture"] = "owner.picture";

  // Bank
  DBPostgreSQL::dbSchemaHash["bank.account.event"] = "banco.bank_account_event";

  DBPostgreSQL::dbSchemaHash["estate.item"] = "patrimonio.bem";
}


int DBPostgreSQL::Setup()
{
  return 0;
}


wxString DBPostgreSQL::GetTableName(wxString name)
{
  return DBPostgreSQL::dbSchemaHash[name];
}


wxString DBPostgreSQL::GetTableName(const char *name)
{
  return DBPostgreSQL::GetTableName(wxString(name));
}


int DBPostgreSQL::Connect(wxString host,
                        wxString user,
                        wxString password,
                        wxString dbName,
                        wxString port,
                        wxString sslMode)
{
  DB::host = host;
  DB::user = user;
  DB::password = password;
  DB::dbName = dbName;
  DB::port = port;
  DB::sslMode = sslMode;

  LogMessenger *messenger = LogMessenger::GetInstance();

  // Connecting with the database

  wxString connectionStr = wxT("");

  if (DB::host.Length() > 0) {
    connectionStr += wxT("host=") + DB::host + wxT(" ");
  }

  if (DB::port.Length() > 0) {
    connectionStr += wxT("port=") + DB::port + wxT(" ");
  }

  if (DB::dbName.Length() > 0) {
    connectionStr += wxT("dbname=") + DB::dbName + wxT(" ");
  }

  if (DB::user.Length() > 0) {
    connectionStr += wxT("user=") + DB::user + wxT(" ");
  }

  if (DB::password.Length() > 0) {
    connectionStr += wxT("password=") + DB::password + wxT(" ");
  }

  if (DB::sslMode.Length() > 0) {
    connectionStr += wxT("sslmode=") + DB::sslMode;
  }

  messenger->Register(wxT("String de conexão: ") + connectionStr, LogMessage::MSG_DBG);

  // Parameter string to database (platform specific)

  char conninfo[strlen((const char *) connectionStr.mb_str(wxCSConv(wxT("ISO-8859-1")))) + 1];

  strcpy(conninfo, (const char *) connectionStr.mb_str(wxCSConv(wxT("ISO-8859-1"))));

  //conninfo[connectionStrSize] = NULL;

  //conninfo = "host=127.0.0.1 dbname=comercial password=calixto user=postgres sslmode=prefer";

  DB::connection = (void *) PQconnectdb(conninfo);

  DB::status = PQstatus((PGconn *) DB::connection);

  if (DB::status != CONNECTION_OK) {
    messenger->Register(wxT("Não foi possível conectar ao banco de dados.\n\n")
                        wxT("Erro interno do banco: ") + wxString(PQerrorMessage((PGconn *) DB::connection),
                                                                  wxConvUTF8,
                                                                  strlen(PQerrorMessage((PGconn *) DB::connection))), LogMessage::MSG_ERROR);

    PQfinish((PGconn *) DB::connection);

    DB::connected = false;

    DB::connection = NULL;

    return -1;
  }
  else {
    DB::connected = true;
  }

  if (DB::connected) {
    return 0;
  //messenger->Register(wxT("Database: ") + wxString(PQdb(DB::connection), wxConvUTF8, strlen(PQdb(DB::connection))), LogMessage::MSG_DBG);
  //messenger->Register(wxT("User: ") + wxString(PQuser(DB::connection), wxConvUTF8, strlen(PQuser(DB::connection))), LogMessage::MSG_DBG);
  //messenger->Register(wxT("Host: ") + wxString(PQhost(DB::connection), wxConvUTF8, strlen(PQhost(DB::connection))), LogMessage::MSG_DBG);
  //messenger->Register(wxT("Port: ") + wxString(PQport(DB::connection), wxConvUTF8, strlen(PQport(DB::connection))), LogMessage::MSG_DBG);
  //messenger->Register(wxT("Options: ") + wxString(PQoptions(DB::connection), wxConvUTF8, strlen(PQoptions(DB::connection))), LogMessage::MSG_DBG);
  }
  else {
    return -1;
  }
}


int DBPostgreSQL::Connect()
{
  return DBPostgreSQL::Connect(Config::Get("db:host"),
                               Config::Get("db:user"),
                               Config::Get("db:password"),
                               Config::Get("db:dbname"),
                               Config::Get("db:port"),
                               Config::Get("db:sslmode"));
}


void DBPostgreSQL::Disconnect()
{
  if (DB::connection) {
    PQfinish((PGconn *) DB::connection);
  }
}


bool DBPostgreSQL::IsConnected()
{
  return DB::connected;
}


/* Database [result / result set] class functions*/

DBPostgreSQLResult::DBPostgreSQLResult()
{
  this->res = NULL;

  this->freed = false;
}


DBPostgreSQLResult::~DBPostgreSQLResult()
{
  if (this->res != NULL && !this->freed) {
    PQclear((PGresult *) this->res);
  }
}


void DBPostgreSQLResult::Free(void) {
  if (this->res == NULL) {
    return;
  }

  if (this->freed) {
    return;
  }

  PQclear((PGresult *) this->res);

  this->res = NULL;

  this->freed = true;
}


bool DBPostgreSQLResult::IsOk()
{
  if (this->res == NULL) {
    return false;
  }

  ExecStatusType status = PQresultStatus((PGresult *) this->res);

  if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
    return false;
  }

  return true;
}


int DBPostgreSQLResult::GetFieldsCount()
{
  if (this->res == NULL) {
    return 0;
  }

  return PQnfields((PGresult *) this->res);
}


int DBPostgreSQLResult::GetRowsCount()
{
  if (this->res == NULL) {
    return 0;
  }

  return PQntuples((PGresult *) this->res);
}


int DBPostgreSQLResult::GetAffectedRowsCount()
{
  if (this->res == NULL) {
    return 0;
  }

  return atoi(PQcmdTuples((PGresult *) this->res));
}


int DBPostgreSQLResult::GetFieldLengthByName(unsigned int rowIndex, wxString name)
{
  return PQgetlength((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, name));
}


int DBPostgreSQLResult::GetFieldLength(unsigned int rowIndex, unsigned int colIndex)
{
  return PQgetlength((PGresult *) this->res, rowIndex, colIndex);
}


wxString DBPostgreSQLResult::GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue)
{
  char *fieldValue;
  char fieldName[strlen((const char *) name.mb_str(wxConvUTF8)) + 1];

  strcpy(fieldName, (const char *) name.mb_str(wxConvUTF8));

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, fieldName));

  if (!fieldValue) {
    return defaultValue;
  }

  return wxString(fieldValue, wxConvUTF8, strlen(fieldValue));
}


const char *DBPostgreSQLResult::GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue)
{
  char *fieldValue;
  char fieldName[strlen((const char *) name.mb_str(wxConvUTF8)) + 1];

  strcpy(fieldName, (const char *) name.mb_str(wxConvUTF8));

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, fieldName));

  if (!fieldValue) {
    return defaultValue;
  }

  return fieldValue;
}


int DBPostgreSQLResult::GetFieldIntByName(unsigned int rowIndex, wxString name)
{
  char *fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, (const char *) name.mb_str(wxConvUTF8)));

  if (!fieldValue) {
    return 0;
  }

  return atoi(fieldValue);
}


long DBPostgreSQLResult::GetFieldLongByName(unsigned int rowIndex, wxString name)
{
  char *fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, (const char *) name.mb_str(wxConvUTF8)));

  if (!fieldValue) {
    return 0;
  }

  return atol(fieldValue);
}


float DBPostgreSQLResult::GetFieldFloatByName(unsigned int rowIndex, wxString name)
{
  char *fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, (const char *) name.mb_str(wxConvUTF8)));

  if (!fieldValue) {
    return 0;
  }

  return atof(fieldValue);
}


double DBPostgreSQLResult::GetFieldDoubleByName(unsigned int rowIndex, wxString name)
{
  char *fieldValue;

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, PQfnumber((PGresult *) this->res, (const char *) name.mb_str(wxConvUTF8)));

  if (!fieldValue) {
    return 0.0;
  }

  //double rv;
  double rv = atof(fieldValue);

  //wxString(fieldValue, wxConvUTF8, strlen(fieldValue)).ToDouble(&rv);

  return rv;
}


wxString DBPostgreSQLResult::GetFieldStr(unsigned int rowIndex, unsigned int colIndex, wxString defaultValue)
{
  char *fieldValue;

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, colIndex);

  if (!fieldValue) {
    return defaultValue;
  }

  return wxString(fieldValue, wxConvUTF8, strlen(fieldValue));
}


int DBPostgreSQLResult::GetFieldInt(unsigned int rowIndex, unsigned int colIndex)
{
  char *fieldValue;

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, colIndex);

  if (!fieldValue) {
    return 0;
  }

  return atoi(fieldValue);
}


long DBPostgreSQLResult::GetFieldLong(unsigned int rowIndex, unsigned int colIndex)
{
  char *fieldValue;

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, colIndex);

  if (!fieldValue) {
    return 0;
  }

  return atol(fieldValue);
}


float DBPostgreSQLResult::GetFieldFloat(unsigned int rowIndex, unsigned int colIndex)
{
  char *fieldValue;

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, colIndex);

  if (!fieldValue) {
    return 0;
  }

  return atof(fieldValue);
}


double DBPostgreSQLResult::GetFieldDouble(unsigned int rowIndex, unsigned int colIndex)
{
  char *fieldValue;

  fieldValue = PQgetvalue((PGresult *) this->res, rowIndex, colIndex);

  if (!fieldValue) {
    return 0.0;
  }

  double rv = atof(fieldValue);

  //wxString(fieldValue, wxConvUTF8, strlen(fieldValue)).ToDouble(&rv);

  return rv;
}



DBPostgreSQLQuery::DBPostgreSQLQuery()
{
  if (this->params.size() > 0) {
    this->params.clear();
  }

  this->dbError = false;
}


DBPostgreSQLQuery::~DBPostgreSQLQuery()
{
  this->result.Free();
}


void DBPostgreSQLQuery::Reset()
{
  this->result.Free();

  if (this->params.size() > 0) {
    this->params.clear();
  }

  this->messageStr = wxT("");

  this->dbError = false;
}


wxString DBPostgreSQLQuery::GetMessageStr(void)
{
  return this->messageStr;
}


int DBPostgreSQLQuery::Execute(wxString command)
{
  PGresult *res;
  int rv = 0;

  LogMessenger *messenger = LogMessenger::GetInstance();

  if (command.Length() < 1) {
    messenger->Register(wxT("Comando vazio passado como consulta ao banco de dados."), LogMessage::MSG_ERROR);

    return -1;
  }

  messenger->Register("Database command: " + command, LogMessage::MSG_DBG);

  Reset();

  char sqlCommandAnsi[strlen((const char *) command.mb_str(wxConvUTF8)) + 1];

  strcpy(sqlCommandAnsi, (const char *) command.mb_str(wxConvUTF8));

  res = PQexec((PGconn *) DB::connection, sqlCommandAnsi);

  this->result.res = (void *) res;

  this->result.freed = false;

  if (res == NULL) {
    messenger->Register(wxT("Resultado de consulta ao banco de dados não é válido."), LogMessage::MSG_ERROR);

    rv = -1;

    if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
      this->messageStr = wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection)));

      messenger->Register(wxT("Erro interno de consulta ao banco de dados: ") + this->messageStr, LogMessage::MSG_ERROR);
    }
  }
  else {
    ExecStatusType status = PQresultStatus((PGresult *) res);

    if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
      messenger->Register("Status de execução de comando no banco de dados não é válido.", LogMessage::MSG_ERROR);

      rv = -1;

      if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
        this->messageStr = wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection)));

        messenger->Register(wxT("Erro interno de status de consulta ao banco de dados: ") + this->messageStr, LogMessage::MSG_ERROR);
      }
    }
  }

  return rv;
}


void DBPostgreSQLQuery::FreeResult()
{
  this->result.Free();
}


int DBPostgreSQLQuery::Execute(wxString command,
                               std::vector<wxString> params,
                               std::vector<wxString> paramTypes)
{
  PGresult *res;
  int rv = 0;
  char *paramValues[params.size()];

  if (command.Length() < 1) {
    return -1;
  }

  LogMessenger *messenger = LogMessenger::GetInstance();

  messenger->Register("Database command: " + command, LogMessage::MSG_DBG);

  Reset();

  char sqlCommandAnsi[strlen((const char *) command.mb_str(wxConvUTF8)) + 1];

  strcpy(sqlCommandAnsi, (const char *) command.mb_str(wxConvUTF8));

  if (params.size() < 1) {
    res = PQexec((PGconn *) DB::connection, sqlCommandAnsi);
  }
  else {
    for (unsigned int paramIndex = 0; paramIndex < params.size(); paramIndex++) {
      if (params.at(paramIndex) == wxEmptyString) {
        paramValues[paramIndex] = NULL;
      }
      else {
        char *value = (char *) malloc(sizeof(char) * strlen((const char *) params.at(paramIndex).mb_str(wxConvUTF8)) + 1);

        strcpy(value, (const char *) params.at(paramIndex).mb_str(wxConvUTF8));

        paramValues[paramIndex] = value;
      }
    }

    res = PQexecParams((PGconn *) DB::connection,
                       sqlCommandAnsi,
                       params.size(), // Parameters count
                       NULL, // Parameter types
                       paramValues, // Parameter values
                       NULL, // Parameter lenghts
                       NULL,
                       0); // Binary results

    this->result.res = (void *) res;

    this->result.freed = false;

    if (res == NULL) {
      rv = -1;

      if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
        this->messageStr = wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection)));

        messenger->Register(wxT("Erro interno de consulta ao banco de dados: ") + this->messageStr, LogMessage::MSG_ERROR);
      }
    }
    else {
      ExecStatusType status = PQresultStatus((PGresult *) res);

      if (status != PGRES_COMMAND_OK &&  status != PGRES_TUPLES_OK) {
        rv = -1;

        if (PQerrorMessage((PGconn *) DB::connection) != NULL) {
          this->messageStr = wxString(PQerrorMessage((PGconn *) DB::connection), wxConvUTF8, strlen(PQerrorMessage((PGconn *) DB::connection)));

          messenger->Register(wxT("Erro interno de consulta ao banco de dados: ") + this->messageStr, LogMessage::MSG_ERROR);
        }
      }
    }

    for (unsigned int paramIndex = 0; paramIndex < params.size(); paramIndex++) {
      if (paramValues[paramIndex] != NULL) {
        free(paramValues[paramIndex]);
      }
    }
  }

  return rv;
}


int DBPostgreSQLQuery::GetResultStatus()
{
  ExecStatusType status = PQresultStatus((PGresult *) this->result.res);

  if (status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK) {
    return (int) DBPostgreSQLResult::WX_DATABASE_RS_NO_ERROR;
  }
  else if (status == PGRES_COPY_OUT) {
    return (int) DBPostgreSQLResult::WX_DATABASE_RS_COPY_OUT;
  }
  else if (status == PGRES_COPY_IN) {
    return (int) DBPostgreSQLResult::WX_DATABASE_RS_COPY_IN;
  }
  else if (status == PGRES_NONFATAL_ERROR) {
    return (int) DBPostgreSQLResult::WX_DATABASE_RS_NONFATAL_ERROR;
  }
  else {
    return (int) DBPostgreSQLResult::WX_DATABASE_RS_ERROR;
  }
}


int DBPostgreSQLQuery::GetFieldsCount()
{
  return this->result.GetFieldsCount();
}


int DBPostgreSQLQuery::GetRowsCount()
{
  return this->result.GetRowsCount();
}


int DBPostgreSQLQuery::GetAffectedRowsCount()
{
  return this->result.GetAffectedRowsCount();
}


int DBPostgreSQLQuery::GetFieldLengthByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldLengthByName(rowIndex, name);
}


int DBPostgreSQLQuery::GetFieldLength(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldLength(rowIndex, colIndex);
}


const char *DBPostgreSQLQuery::GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue)
{
  return this->result.GetFieldAnsiStrByName(rowIndex, name, defaultValue);
}


wxString DBPostgreSQLQuery::GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue)
{
  return this->result.GetFieldStrByName(rowIndex, name, defaultValue);
}


int DBPostgreSQLQuery::GetFieldIntByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldIntByName(rowIndex, name);
}


long DBPostgreSQLQuery::GetFieldLongByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldLongByName(rowIndex, name);
}


float DBPostgreSQLQuery::GetFieldFloatByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldFloatByName(rowIndex, name);
}


double DBPostgreSQLQuery::GetFieldDoubleByName(unsigned int rowIndex, wxString name)
{
  return this->result.GetFieldDoubleByName(rowIndex, name);
}


wxString DBPostgreSQLQuery::GetFieldStr(unsigned int rowIndex, unsigned int colIndex, wxString defaultValue)
{
  return this->result.GetFieldStr(rowIndex, colIndex, defaultValue);
}


int DBPostgreSQLQuery::GetFieldInt(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldInt(rowIndex, colIndex);
}


long DBPostgreSQLQuery::GetFieldLong(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldLong(rowIndex, colIndex);
}


float DBPostgreSQLQuery::GetFieldFloat(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldFloat(rowIndex, colIndex);
}


double DBPostgreSQLQuery::GetFieldDouble(unsigned int rowIndex, unsigned int colIndex)
{
  return this->result.GetFieldDouble(rowIndex, colIndex);
}


wxString DBPostgreSQLUtils::MountSQLInsert(wxString tableName,
                                 std::vector<wxString> fields)
{
  wxString insertFieldsStr;

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0)
      insertFieldsStr += wxT(",") + fields.at(fieldIndex);
    else
      insertFieldsStr = fields.at(fieldIndex);
  }

  wxString sql = wxT("INSERT INTO ") + tableName + wxT(" (") + insertFieldsStr + wxT(") VALUES (");

  for (unsigned int valueIndex = 0; valueIndex < fields.size(); valueIndex++) {
    if (valueIndex > 0)
      sql += ",";

    sql += wxString::Format("$%d", valueIndex + 1);
  }

  sql += wxT(")");

  return sql;
}


wxString DBPostgreSQLUtils::MountSQLInsert(wxString tableName,
                                 std::vector<wxString> fields,
                                 std::vector<wxString> valueModifiers)
{
  wxString insertFieldsStr = "";

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0)
      insertFieldsStr += ",";

    insertFieldsStr += fields.at(fieldIndex);
  }

  wxString sql = "INSERT INTO " + tableName + " (" + insertFieldsStr + ") VALUES (";

  for (unsigned int valueIndex = 0; valueIndex < fields.size(); valueIndex++) {
    wxString value = wxString::Format("$%d", valueIndex + 1);

    if (valueIndex > 0)
      sql << ",";

    if (valueModifiers.size() > valueIndex && valueModifiers[valueIndex].Length() > 0) {
      wxString valueModifier = valueModifiers[valueIndex];

      wxRegEx regex("\\$@", wxRE_EXTENDED|wxRE_ADVANCED);

      regex.Replace(&valueModifier, value);

      sql << valueModifier;
    }
    else {
      sql << value;
    }
  }

  sql << ")";

  //wxMessageBox(sql);

  return sql;
}


wxString DBPostgreSQLUtils::MountSQLUpdate(wxString tableName,
                                           std::vector<wxString> fields,
                                           wxString whereClause)
{
  wxString updateFieldsStr;

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0)
      updateFieldsStr += wxString::Format(wxT(",%s=$%d"), fields.at(fieldIndex).c_str(), fieldIndex + 1);
    else
      updateFieldsStr = wxString::Format(wxT("%s=$%d"), fields.at(fieldIndex).c_str(), fieldIndex + 1);
  }

  wxString sql = wxT("UPDATE ") + tableName + wxT(" SET ") + updateFieldsStr;

  if (whereClause.Length() > 0)
    sql += wxT(" WHERE ") + whereClause;

  return sql;
}


wxString DBPostgreSQLUtils::MountSQLUpdate(wxString tableName,
                                 std::vector<wxString> fields,
                                 wxString whereClause,
                                 std::vector<wxString> valueModifiers)
{
  wxString updateFieldsStr = "";

  // Fields

  for (unsigned int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
    if (fieldIndex > 0) {
      updateFieldsStr += ",";
    }

    wxString value = wxString::Format("$%d", fieldIndex + 1);;

    if (valueModifiers.size() > fieldIndex && valueModifiers[fieldIndex].Length() > 0) {
      wxString valueModifier = valueModifiers[fieldIndex];

      wxRegEx regex("\\$@", wxRE_EXTENDED|wxRE_ADVANCED);

      regex.Replace(&valueModifier, value);

      value = valueModifier;
    }

    value = wxString::Format("%s=", fields.at(fieldIndex).c_str()) + value;

    updateFieldsStr += value;
  }

  wxString sql = wxT("UPDATE ") + tableName + wxT(" SET ") + updateFieldsStr;

  if (whereClause.Length() > 0)
    sql += wxT(" WHERE ") + whereClause;

  return sql;
}


long DBPostgreSQLUtils::GetSerialKey(wxString tableName, wxString columnName) {
  wxString sql = wxString::Format(wxT("SELECT CURRVAL(pg_get_serial_sequence('%s', '%s')) AS id"),
                                  tableName.c_str(),
                                  columnName.c_str());

  DBPostgreSQLQuery dbquery;

  int rc = dbquery.Execute(sql);

  if (rc != 0) {
    return -1;
  }
  else {
    return dbquery.GetFieldLongByName(0, wxT("id"));
  }
}


int DBPostgreSQLTransaction::Begin()
{
  DBPostgreSQLQuery dbquery;

  int rc = dbquery.Execute(wxT("BEGIN"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Não foi possível inicar a transação com o banco de dados.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }

  return rc;
}


int DBPostgreSQLTransaction::Commit()
{
  DBPostgreSQLQuery dbquery;

  int rc = dbquery.Execute(wxT("COMMIT"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Ocorreu erro na tentativa de confirmar a transação com o banco de dados.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }

  return rc;
}


int DBPostgreSQLTransaction::Rollback()
{
  DBPostgreSQLQuery dbquery;

  int rc = dbquery.Execute(wxT("COMMIT"));

  if (rc != 0) {
    LogMessenger *messenger = LogMessenger::GetInstance();

    messenger->Register(wxT("Ocorreu erro na tentativa de cancelar a transação com o banco de dados.\n\n")
                        wxT("Erro interno: ") + dbquery.GetMessageStr(), LogMessage::MSG_ERROR);
  }

  return rc;
}