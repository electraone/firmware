#pragma once

#include "sqlite3.h"

class Model
{
public:
    Model() : dbHandle(nullptr)
    {
    }

    virtual ~Model()
    {
        close();
    }

    void attach(const char *filename)
    {
        int rc = sqlite3_open(filename, &dbHandle);

        if (rc != SQLITE_OK) {
            System::logger.write(
                LOG_ERROR, "Cannot open database: %s", sqlite3_errmsg(dbHandle));
            sqlite3_close(dbHandle);
        }
    }

    void create(void)
    {
        char *errorMessage = 0;
        const char *sql = "CREATE TABLE IF NOT EXISTS Synths "
                          "(Id INT, Manufacturer TEXT, Model TEXT);";

        int rc = sqlite3_exec(dbHandle, sql, 0, 0, &errorMessage);

        if (rc != SQLITE_OK) {
            System::logger.write(LOG_ERROR, "SQL error: %s", errorMessage);
            sqlite3_free(errorMessage);
        }
    }

    void insertRows(void)
    {
        char *errorMessage = 0;
        const char *sql = "INSERT INTO Synths VALUES(1, 'Yamaha', 'TX7');"
                          "INSERT INTO Synths VALUES(2, 'Yamaha', 'DX7');"
                          "INSERT INTO Synths VALUES(3, 'Yamaha', 'TX816');";

        int rc = sqlite3_exec(dbHandle, sql, 0, 0, &errorMessage);

        if (rc != SQLITE_OK) {
            System::logger.write(LOG_ERROR, "SQL error: %s", errorMessage);
            sqlite3_free(errorMessage);
        }
    }

    void query(void)
    {
        sqlite3_stmt *stmt;

        int rc =
            sqlite3_prepare_v2(dbHandle,
                               "SELECT id, manufacturer, model from synths",
                               -1,
                               &stmt,
                               0);

        if (rc != SQLITE_OK) {
            System::logger.write(
                LOG_ERROR, "Failed to fetch data: %s", sqlite3_errmsg(dbHandle));
            return;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *manufacturer = sqlite3_column_text(stmt, 1);
            const unsigned char *model = sqlite3_column_text(stmt, 2);

            System::logger.write(LOG_ERROR,
                                 "id: %d, manufacturer: %s model: %s",
                                 id,
                                 manufacturer,
                                 model);
        }

        sqlite3_finalize(stmt);
    }

    int getCount(void)
    {
        sqlite3_stmt *stmt;
        int count = 0;

        int rc = sqlite3_prepare_v2(
            dbHandle, "SELECT count(1) from synths", -1, &stmt, 0);

        if (rc != SQLITE_OK) {
            System::logger.write(
                LOG_ERROR, "Failed to fetch count: %s", sqlite3_errmsg(dbHandle));
            return (-1);
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        return (count);
    }

    void update(void)
    {
        char *errorMessage = nullptr;
        const char *sql = "UPDATE cars set name = 'buchticka' where ID=7;";

        int rc = sqlite3_exec(dbHandle, sql, 0, 0, &errorMessage);

        if (rc != SQLITE_OK) {
            System::logger.write(LOG_ERROR, "SQL error: %s", errorMessage);
            sqlite3_free(errorMessage);
        }
    }

    void remove(void)
    {
        char *errorMessage = nullptr;
        const char *sql = "DELETE from cars where ID=1;";

        int rc = sqlite3_exec(dbHandle, sql, 0, 0, &errorMessage);

        if (rc != SQLITE_OK) {
            System::logger.write(LOG_ERROR, "SQL error: %s", errorMessage);
            sqlite3_free(errorMessage);
        }
    }

    void close(void)
    {
        sqlite3_close(dbHandle);
    }

private:
    sqlite3 *dbHandle;
};
