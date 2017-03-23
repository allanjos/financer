/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * database.h
 * Copyright (C) 2013 Allann Jones <alljols[at]yahoo.com.br>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Sindhu S'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * financer IS PROVIDED BY Sindhu S ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Sindhu S OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UTILS_DATABASE_SQLITE_H
#define UTILS_DATABASE_SQLITE_H

#include "utils_database.hpp"
#include <vector>


class DBSqlite
{
  private:

    // Constructor. Private. Use GetInstance() to get a database instance.

    DBSqlite();

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

    ~DBSqlite();

    static int Init(DBType_t dbType);

    static DBSqlite *GetInstance();

    static int Connect();

    static int Connect(wxString filePath,
                       wxString arg2 = "",
                       wxString arg3 = "",
                       wxString arg4 = "",
                       wxString arg5 = "",
                       wxString arg6 = "");

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

class DBSqliteResult {
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

    std::vector<wxString> resColumnNames;

    std::vector<std::vector<wxString> > matrixData;

    void *res;

    bool freed;

    DBSqliteResult();

    ~DBSqliteResult();

    bool IsOk();

    void Free();

    int GetFieldsCount();

    int GetRowsCount();

    int GetAffectedRowsCount();

    int GetColumnIndexByName(wxString name);

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

class DBSqliteQuery {
  public:
    DBSqliteResult result;

    bool dbError;

    wxString messageStr;

    std::vector<wxString> params;

    DBSqliteQuery();

    ~DBSqliteQuery();

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

class DBSqliteUtils {
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


class DBSqliteTransaction {
  public:
    static int Begin();

    static int Commit();

    static int Rollback();
};


#endif /* UTILS_DATABASE_SQLITE_H */