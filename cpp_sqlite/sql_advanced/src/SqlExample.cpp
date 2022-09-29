#include <cstdio>
#include <iostream>

#include "sqlcpp.h"
#include "sqlite3.h"

constexpr const char* db_file = DB_PATH "/scratch.db";

int main(int argc, char* argv[]) {
    printf("sqlite3 version: %s\n", sqlite3_libversion());
    printf("sqlite3 source ID: %s\n", sqlite3_sourceid());
    printf("db file: %s\n", db_file);

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    int res = sqlite3_open(db_file, &db);

    if (res != SQLITE_OK) {
        puts("Database failed to open! Executed with error: ");
        puts(sqlite3_errstr(res));
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    // if I am here, it means that database was successfully opened
    puts("Successfully opened database!");

    // prepare a statement
    sqlite3_prepare_v2(db, "SELECT * FROM customer", -1, &stmt, nullptr);

    // list column names
    int col_count = sqlite3_column_count(stmt);
    printf("Number of columns in the result: %d\n", col_count);

    for (int i = 0; i < col_count; ++i) {
        printf("%s", sqlite3_column_name(stmt, i));
        if (i < col_count - 1) printf(", ");
    }
    puts("");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < col_count; ++i) {
            printf("%s", sqlite3_column_text(stmt, i));
            if (i < col_count - 1) printf(", ");
        }
        puts("");
    }

    puts("finalize statement");
    sqlite3_finalize(stmt);
    puts("close db");
    sqlite3_close(db);
    puts("done");

    return EXIT_SUCCESS;
}