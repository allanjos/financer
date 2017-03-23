#ifndef UTILS_DATABASE_POSTGRESQL_HPP
#define UTILS_DATABASE_POSTGRESQL_HPP

#include "utils_database.hpp"


class DBPostgreSQL
{
  private:

    // Constructor. Private. Use GetInstance() to get a database instance.

    DBPostgreSQL();

  public:
    static DBType_t dbType;

    static bool connected; /** Flag to query if database is connected or not. */

    static int status; /** Current database status code. */

    static void *connection; /**< Connection handler. Platform dependent. */

    static wxString type;

    static wxString host;

    static wxString user;

    static wxString password;

    static wxString dbName;

    static wxString port;

    static wxString sslMode;

    static DBSchemaHash dbSchemaHash;

    ~DBPostgreSQL();

    static int Init(DBType_t dbType);

    static DBPostgreSQL *GetInstance();

    static int Connect();

    static int Connect(wxString host,
                       wxString user,
                       wxString password,
                       wxString dbName = "",
                       wxString port = "",
                       wxString sslMode = "");

    static void Disconnect();

    static bool IsConnected();

    static wxString GetTableName(wxString name);

    static wxString GetTableName(const char *name);

    static void InitializeSchema();

    static int Setup();
};


/**
 * Database result / result set.
 */

class DBPostgreSQLResult {
  public:
    enum {
      WX_DATABASE_RS_NO_ERROR = 0,
      WX_DATABASE_RS_QUERY_EMPTY,
      WX_DATABASE_RS_COPY_OUT,
      WX_DATABASE_RS_COPY_IN,
      WX_DATABASE_RS_BAD_RESPONSE,
      WX_DATABASE_RS_NONFATAL_ERROR,
      WX_DATABASE_RS_ERROR
    };


    void *res;

    bool freed;

    DBPostgreSQLResult();

    ~DBPostgreSQLResult();

    bool IsOk();

    void Free();

    int GetFieldsCount();

    int GetRowsCount();

    int GetAffectedRowsCount();

    wxString GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue);

    int GetFieldIntByName(unsigned int rowIndex, wxString name);

    long GetFieldLongByName(unsigned int rowIndex, wxString name);

    float GetFieldFloatByName(unsigned int rowIndex, wxString name);

    double GetFieldDoubleByName(unsigned int rowIndex, wxString name);

    const char *GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue = NULL);

    int GetFieldLengthByName(unsigned int rowIndex, wxString name);

    int GetFieldLength(unsigned int rowIndex, unsigned int colIndex);

    wxString GetFieldStr(unsigned int rowIndex, unsigned int colIndex, wxString defaultValue = wxT(""));

    int GetFieldInt(unsigned int rowIndex, unsigned int colIndex);

    long GetFieldLong(unsigned int rowIndex, unsigned int colIndex);

    float GetFieldFloat(unsigned int rowIndex, unsigned int colIndex);

    double GetFieldDouble(unsigned int rowIndex, unsigned int colIndex);

    const char *GetFieldAnsiStr(unsigned int rowIndex, unsigned int colIndex);
};


/**
 * Database query.
 */

class DBPostgreSQLQuery {
  public:
    DBPostgreSQLResult result;

    bool dbError;

    wxString messageStr;

    std::vector<wxString> params;

    DBPostgreSQLQuery();

    ~DBPostgreSQLQuery();

    void Reset();

    int Execute(wxString command);

    int Execute(wxString command,
                std::vector<wxString> params,
                std::vector<wxString> paramTypes = std::vector<wxString>());

    void FreeResult();

    int GetResultStatus();

    wxString GetMessageStr();

    int GetFieldsCount();

    int GetRowsCount();

    int GetAffectedRowsCount();

    wxString GetFieldStrByName(unsigned int rowIndex, wxString name, wxString defaultValue = wxT(""));

    int GetFieldIntByName(unsigned int rowIndex, wxString name);

    long GetFieldLongByName(unsigned int rowIndex, wxString name);

    float GetFieldFloatByName(unsigned int rowIndex, wxString name);

    double GetFieldDoubleByName(unsigned int rowIndex, wxString name);

    const char *GetFieldAnsiStrByName(unsigned int rowIndex, wxString name, const char *defaultValue = NULL);

    int GetFieldLengthByName(unsigned int rowIndex, wxString name);

    int GetFieldLength(unsigned int rowIndex, unsigned int colIndex);

    wxString GetFieldStr(unsigned int rowIndex, unsigned int colIndex, wxString defaultValue = wxT(""));

    int GetFieldInt(unsigned int rowIndex, unsigned int colIndex);

    long GetFieldLong(unsigned int rowIndex, unsigned int colIndex);

    float GetFieldFloat(unsigned int rowIndex, unsigned int colIndex);

    double GetFieldDouble(unsigned int rowIndex, unsigned int colIndex);

    const char *GetFieldAnsiStr(unsigned int rowIndex, unsigned int colIndex);
};


/**
 * Utilitaries functions.
 */

class DBPostgreSQLUtils {
  public:
    static wxString MountSQLInsert(wxString tableName,
                                   std::vector<wxString> fields);

    static wxString MountSQLInsert(wxString tableName,
                                   std::vector<wxString> fields,
                                   std::vector<wxString> valueModifiers);

    static wxString MountSQLUpdate(wxString tableName,
                                   std::vector<wxString> fields,
                                   wxString whereClause = "");

    static wxString MountSQLUpdate(wxString tableName,
                                   std::vector<wxString> fields,
                                   wxString whereClause,
                                   std::vector<wxString> valueModifiers);

    static long GetSerialKey(wxString tableName, wxString columnName);
};


class DBPostgreSQLTransaction {
  public:
    static int Begin();

    static int Commit();

    static int Rollback();
};


#endif /* UTILS_DATABASE_POSTGRESQL_HPP */