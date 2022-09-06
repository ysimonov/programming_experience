#ifndef SQL_HANDLER
#define SQL_HANDLER

#include <memory>
#include <sqlite3.h>
#include <string>

class HandlerSQLite
{
    std::string _db_name = "mock.db";
    sqlite3 *_db;

  public:
    HandlerSQLite(std::string db_name = "mock.db") : _db_name(db_name){};
    ~HandlerSQLite();

    // return exit success status
    // 0 - success, 1 - failure
    // function that creates tables
    int create_tables();

    std::string &get_db_name()
    {
        return _db_name;
    }

    sqlite3 *get_db() const
    {
        return _db;
    }
};

#endif