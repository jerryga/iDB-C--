//
//  DBTool.cpp
//  MagicCardGame
//
//  Created by Static Ga on 13-9-24.
//
//

#include "DBTool.h"
#include <sstream>

using namespace std;

static DBTool *tool = NULL;

DBTool *DBTool::getInstance() {
    if (NULL == tool) {
        tool = new DBTool();
    }
    return tool;
}


bool DBTool::openDB(const char *filename) {
if (NULL == pDB) {        
    result = sqlite3_open(filename, &pDB);
    if (SQLITE_OK != result) {
        printf("打开数据库失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
        return false;
        }
    }
    return true;
}

int is_existed( void * para, int n_column, char ** column_value, char ** column_name )
{
    bool *isExisted_=(bool*)para;
    *isExisted_=(**column_value)!='0';
    return 0;
}

bool DBTool::createTable(const char *tableName) {
    
    if (!isTableExist(tableName)) {
        std::string sqlstr;
        stringstream ss_stream;

        ss_stream.clear();
        ss_stream<<"create table "<<tableName<<" (ID integer primary key autoincrement)";//设置ID为主键，且自动增加
        sqlstr.clear();
        sqlstr = ss_stream.str();
    
        return performSQL(sqlstr.c_str());
    }

    return true;
}

bool DBTool::addColumnToTable(const char *tableName, const char *column, const char *dataType) {
    if (!isTableExist(tableName)) {
        if (!createTable(tableName)) {
            return false;
        }
    }
    std::string sqlstr;
    stringstream ss_stream;
    ss_stream<<"ALTER TABLE "<<tableName<<" ADD "<<column<<" "<<dataType;
    sqlstr = ss_stream.str();
    return performSQL(sqlstr.c_str());
}

bool DBTool::isRowExistInTable(const char *tableName, const char *column,const char *rowValue) {
  
    if (!isTableExist(tableName)) {
        if (!createTable(tableName)) {
            return false;
        }
    }
    
    if (!isColumnExistInTable(tableName, column)) {
        if (!addColumnToTable(tableName, column, "TEXT")) {
            return false;
        }
    }
    
    std::string sqlstr;
    stringstream ss_stream;
    ss_stream<<"select count(*) from "<<tableName<<" where "<<column<<" = "<<"'"<<rowValue<<"'";
    sqlstr = ss_stream.str();
    result = sqlite3_exec( pDB, sqlstr.c_str() , is_existed, &isExisted_, &errMsg );
    if (SQLITE_OK != result) {
        printf( "isRowExist SQL失败，错误码:%d ，错误原因:%s\n" , result, errMsg );

        return false;
    }else {
        return isExisted_;
    }
}

bool DBTool::addRowToTable(const char *tableName, const char *column, const char *rowValue) {
    if (!isTableExist(tableName)) {
        if (!createTable(tableName)) {
            return false;
        }
    }
    
    if (!isColumnExistInTable(tableName, column)) {
        if (!addColumnToTable(tableName, column, "TEXT")) {
            return false;
        }
    }

    std::string sqlstr;
    stringstream ss_stream;
    ss_stream<<"insert into "<<tableName<< "("<<column<<")"<<" values "<<"("<<"'"<<rowValue<<"'"<<")";
    sqlstr = ss_stream.str();
    return performSQL(sqlstr.c_str());
}

bool DBTool::deleteRowInTable(const char *tableName, const char *column, const char *rowValue) {
    if (!isTableExist(tableName)) {
        if (!createTable(tableName)) {
            return false;
        }
    }
    std::string sqlstr;
    stringstream ss_stream;
    ss_stream<<"delete from "<<tableName<<" where "<<column<<" = "<<"'"<<rowValue<<"'";
    sqlstr = ss_stream.str();
    return performSQL(sqlstr.c_str());
}

bool DBTool::performSQL(const char *sqlStr) {
    printf("SQL To Perform %s\n",sqlStr);
    result=sqlite3_exec( pDB, sqlStr , NULL, NULL, &errMsg );
    if( result != SQLITE_OK ) {
        printf( "执行SQL失败，错误码:%d ，错误原因:%s\n" , result, errMsg );

        return false;
    }else {
        return true;
    }
}

#pragma mark - private

bool DBTool::isTableExist(const char *tableName) {
    std::string sqlstr;
    
    stringstream ss_stream;
    ss_stream<<"select count(type) from sqlite_master where type='table' and name="<<"'"<<tableName<<"'";
    sqlstr = ss_stream.str();
    
    sqlite3_exec(pDB, sqlstr.c_str() , is_existed, &isExisted_, &errMsg );
    
    return isExisted_;
    
}

bool DBTool::isColumnExistInTable(const char *tableName, const char *column) {
    std::string sqlstr;
    
    stringstream ss_stream;
    ss_stream<<"select "<<column<<" from "<<tableName;
    sqlstr = ss_stream.str();
    
    
    return performSQL(sqlstr.c_str());

}
