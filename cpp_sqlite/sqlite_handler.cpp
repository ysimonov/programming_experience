#include "sqlite_handler.hpp"
#include <iostream>
#include <sqlite3.h>

HandlerSQLite::~HandlerSQLite()
{
    _db_name = "mock.db";
    sqlite3_close(_db);
}

int HandlerSQLite::create_tables()
{
    std::string table1 = "CREATE TABLE ";

    int exit_status = 0;

    // open db file for writing
    auto *db = this->get_db();
    const auto db_name = this->get_db_name().c_str();

    exit_status = sqlite3_open(db_name, &db);

    if (exit_status != SQLITE_OK)
    {
        std::cerr << "Error while opening DB: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Opened database successfully!" << std::endl;
    }

    // create tables and populate with data
    // TODO
    char *message_error;

    if (exit_status != SQLITE_OK)
    {
        std::cerr << "Error while creating tables: " << std::endl;
        sqlite3_free(message_error);
        sqlite3_close(db);
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Table(s) created successfully!" << std::endl;
    }

    return EXIT_SUCCESS;
}