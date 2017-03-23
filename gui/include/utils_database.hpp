#ifndef UTILS_DATABASE_HPP
#define UTILS_DATABASE_HPP

#include <vector>
#include "gui_lib.hpp"
#include "utils_log.hpp"
#include "wx/hashmap.h"


/**
 * Database type.
 */

typedef enum DBType_e {
  DB_TYPE_SQLITE,
  DB_TYPE_POSTGRESQL
} DBType_t;

WX_DECLARE_HASH_MAP(wxString,      // type of the keys
                    wxString,  // any type you like
                    wxStringHash,  // hasher
                    wxStringEqual, // key equality predicate
                    DBSchemaHash); // name of the class

class DB {
  private:

    // Constructor. Private. Use GetInstance() to get a database instance.

    DB();

  public:

    static bool initialized; /** Flag to query if database is initialized or not. */

    static bool connected; /** Flag to query if database is connected or not. */

    static int status; /** Current database status code. */

    static void *connection; /**< Connection handler. Platform dependent. */

    static DBType_t dbType;

    static wxString host;

    static wxString user;

    static wxString password;

    static wxString dbName;

    static wxString port;

    static wxString sslMode;

    static wxString filePath;

    ~DB();

    static int Init();

    static int Init(DBType_t dbType);

    static DBType_t GetType();

    static DB *GetInstance();

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
 * Database query.
 */

class DBQuery {

  public:

    void *query;

    DBQuery();

    ~DBQuery();

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

    int GetFieldLength(unsigned int rowIndex, int colIndex);

    wxString GetFieldStr(unsigned int rowIndex, int colIndex, wxString defaultValue = wxT(""));

    int GetFieldInt(unsigned int rowIndex, int colIndex);

    long GetFieldLong(unsigned int rowIndex, int colIndex);

    float GetFieldFloat(unsigned int rowIndex, int colIndex);

    double GetFieldDouble(unsigned int rowIndex, int colIndex);

    const char *GetFieldAnsiStr(unsigned int rowIndex, int colIndex);
};


/**
 * Utilitaries functions.
 */

class DBUtils {
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


class DBTransaction {
  public:
    static int Begin();

    static int Commit();

    static int Rollback();
};


#endif /* _UTILS_DATABASE_HPP_ */