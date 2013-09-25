//
//  DBTool.h
//  MagicCardGame
//
//  Created by Static Ga on 13-9-24.
//
//

#ifndef __MagicCardGame__DBTool__
#define __MagicCardGame__DBTool__

#include <iostream>
#include <sqlite3.h>

static sqlite3 *pDB;

class DBTool {
    
private:
    bool isExisted_;
    char * errMsg = NULL;
    int result;
    
    const char *dbfile;
    DBTool (const char *filename = ""):dbfile(filename){
    }
    ~DBTool() {
        if (pDB) {
            sqlite3_close(pDB);
        }
    }
public:

    static DBTool *getInstance();
    bool openDB(const char *filename);
    bool createTable(const char *tableName);
    bool addColumnToTable(const char *tableName, const char *column, const char *dataType);
    
    bool isRowExistInTable(const char *tableName , const char *column,const char *rowValue);
    bool addRowToTable(const char *tableName, const char *column, const char *rowValue);
    bool deleteRowInTable(const char *tableName, const char *column, const char *rowValue);
    
    bool performSQL(const char *sqlStr);
    
private:
    bool isTableExist(const char *tableName);
    bool isColumnExistInTable(const char *tableName,const char *column);
};

#endif /* defined(__MagicCardGame__DBTool__) */
