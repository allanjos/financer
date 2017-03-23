#include "utils_database.hpp"
#include "utils_database_postgresql.hpp"
#include "utils_database_sqlite.hpp"
#include "utils_config.hpp"
#include "utils_log.hpp"
#include "app_version.hpp"
#include <stdlib.h>

/* Database static attributes initialization */

bool DB::initialized = false;

bool DB::connected = false;

DBType_t DB::dbType = DB_TYPE_POSTGRESQL;

void *DB::connection = NULL;

int DB::status = 0;

wxString DB::host = "localhost";

wxString DB::user = "postgres";

wxString DB::password = "postgres";

wxString DB::dbName = "comercial";

wxString DB::port = "5432";

wxString DB::sslMode = "disable";

wxString DB::filePath = APP_NAME + wxString(".db");


/* Database class functions */

DB::DB()
{
}


DB::~DB()
{
  DB::Disconnect();

  DB::initialized = false;
}


int DB::Init()
{
  if (Config::Get("db:filepath").Cmp("PostgreSQL") == 0) {
    DB::dbType = DB_TYPE_POSTGRESQL;
  }
  else {
    DB::dbType = DB_TYPE_SQLITE;
  }

  DB::InitializeSchema();

  DB::initialized = true;

  return 0;
}


int DB::Init(DBType_t dbType)
{
  DB::dbType = dbType;

  DB::Init();

  return 0;
}


DBType_t DB::GetType()
{
  return DB::dbType;
}


DB *DB::GetInstance()
{
  // This line only runs once, thus creating the only instance in existence

  static DB *instance = new DB();

  if (!instance->initialized) {
    instance->Init();
  }

  return instance; // Always returns the same instance
}


void DB::InitializeSchema()
{
  DBSqlite::InitializeSchema();
}


int DB::Setup()
{
  return DBSqlite::Setup();
}


wxString DB::GetTableName(wxString name)
{
  return DBSqlite::dbSchemaHash[name];
}


wxString DB::GetTableName(const char *name)
{
  return DBSqlite::GetTableName(wxString(name));
}


int DB::Connect(wxString host,
                wxString user,
                wxString password,
                wxString dbName,
                wxString port,
                wxString sslMode)
{
  return DBSqlite::Connect(host);
}


int DB::Connect()
{
  return DBSqlite::Connect(Config::Get("db:filepath"));
}


void DB::Disconnect()
{
  DBSqlite::Disconnect();
}


bool DB::IsConnected()
{
  return DB::connected;
}


// Database query


DBQuery::DBQuery()
{
  query = (void *) new DBSqliteQuery();
}


DBQuery::~DBQuery()
{
  delete (DBSqliteQuery *) query;
}


void DBQuery::Reset()
{
  ((DBSqliteQuery *) query)->Reset();
}


wxString DBQuery::GetMessageStr(void)
{
  return ((DBSqliteQuery *) query)->GetMessageStr();
}


int DBQuery::Execute(wxString command)
{
  return ((DBSqliteQuery *) query)->Execute(command);
}


void DBQuery::FreeResult()
{
  ((DBSqliteQuery *) query)->FreeResult();
}


int DBQuery::Execute(wxString command, std::vector<wxString> params, std::vector<wxString> paramTypes)
{
  return ((DBSqliteQuery *) query)->Execute(command, params, paramTypes);
}


int DBQuery::GetResultStatus()
{
  return ((DBSqliteQuery *) query)->GetResultStatus();
}


int DBQuery::GetFieldsCount()
{
  return ((DBSqliteQuery *) query)->GetFieldsCount();
}


int DBQuery::GetRowsCount()
{
  return ((DBSqliteQuery *) query)->GetRowsCount();
}


int DBQuery::GetAffectedRowsCount()
{
  return ((DBSqliteQuery *) query)->GetAffectedRowsCount();
}


int DBQuery::GetFieldLengthByName(unsigned int rowIndex, wxString name)
{
  return ((DBSqliteQuery *) query)->GetFieldLengthByName(rowIndex, name);
}


int DBQuery::GetFieldLength(unsigned int rowIndex, int colIndex)
{
  return ((DBSqliteQuery *) query)->GetFieldLength(rowIndex, colIndex);
}


const char *DBQuery::GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue)
{
  return ((DBSqliteQuery *) query)->GetFieldAnsiStrByName(rowIndex, name, defaultValue);
}


wxString DBQuery::GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue)
{
  return ((DBSqliteQuery *) query)->GetFieldStrByName(rowIndex, name, defaultValue);
}


int DBQuery::GetFieldIntByName(unsigned int rowIndex, wxString name)
{
  return ((DBSqliteQuery *) query)->GetFieldIntByName(rowIndex, name);
}


long DBQuery::GetFieldLongByName(unsigned int rowIndex, wxString name)
{
  return ((DBSqliteQuery *) query)->GetFieldLongByName(rowIndex, name);
}


float DBQuery::GetFieldFloatByName(unsigned int rowIndex, wxString name)
{
  return ((DBSqliteQuery *) query)->GetFieldFloatByName(rowIndex, name);
}


double DBQuery::GetFieldDoubleByName(unsigned int rowIndex, wxString name)
{
  return ((DBSqliteQuery *) query)->GetFieldDoubleByName(rowIndex, name);
}


wxString DBQuery::GetFieldStr(unsigned int rowIndex, int colIndex, wxString defaultValue)
{
  return ((DBSqliteQuery *) query)->GetFieldStr(rowIndex, colIndex, defaultValue);
}


int DBQuery::GetFieldInt(unsigned int rowIndex, int colIndex)
{
  return ((DBSqliteQuery *) query)->GetFieldInt(rowIndex, colIndex);
}


long DBQuery::GetFieldLong(unsigned int rowIndex, int colIndex)
{
  return ((DBSqliteQuery *) query)->GetFieldLong(rowIndex, colIndex);
}


float DBQuery::GetFieldFloat(unsigned int rowIndex, int colIndex)
{
  return ((DBSqliteQuery *) query)->GetFieldFloat(rowIndex, colIndex);
}


double DBQuery::GetFieldDouble(unsigned int rowIndex, int colIndex)
{
  return ((DBSqliteQuery *) query)->GetFieldDouble(rowIndex, colIndex);
}


wxString DBUtils::MountSQLInsert(wxString tableName,
                                 std::vector<wxString> fields)
{
  return DBSqliteUtils::MountSQLInsert(tableName, fields);
}


wxString DBUtils::MountSQLInsert(wxString tableName,
                                 std::vector<wxString> fields,
                                 std::vector<wxString> valueModifiers)
{
  return DBSqliteUtils::MountSQLInsert(tableName, fields, valueModifiers);
}


wxString DBUtils::MountSQLUpdate(wxString tableName,
                                 std::vector<wxString> fields,
                                 wxString whereClause)
{
  return DBSqliteUtils::MountSQLUpdate(tableName, fields, whereClause);
}


wxString DBUtils::MountSQLUpdate(wxString tableName,
                                 std::vector<wxString> fields,
                                 wxString whereClause,
                                 std::vector<wxString> valueModifiers)
{
  return DBSqliteUtils::MountSQLUpdate(tableName, fields, whereClause, valueModifiers);
}


long DBUtils::GetSerialKey(wxString tableName, wxString columnName) {
  return DBSqliteUtils::GetSerialKey(tableName, columnName);
}


int DBTransaction::Begin()
{
  return DBSqliteTransaction::Begin();
}


int DBTransaction::Commit()
{
  return DBSqliteTransaction::Commit();
}


int DBTransaction::Rollback()
{
  return DBSqliteTransaction::Rollback();
}
