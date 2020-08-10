#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    
    for(i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    
    return 0;
}
*/

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    
    fprintf(stderr, "%s: ", (const char*)data);
    
    for(i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    
    return 0;
}

int main()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "Callback function called";
    
    rc = sqlite3_open("test.db", &db);
    
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    
    //sql = "CREATE TABLE COMPANY("  \
            "ID INT PRIMARY KEY     NOT NULL," \
            "NAME                   TEXT   NOT NULL," \
            "AGE                       INT     NOT NULL," \
            "ADDRESS    CHAR(50)," \
            "SALARY       REAL );";
    
    //sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
    
    //rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    //sql = "SELECT * FROM COMPANY";
    
    //sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
            "SELECT * from COMPANY";
    
    sql = "DELETE from COMPANY where ID=2; " \
            "SELECT * from COMPANY";
    
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }
    else
    {
        fprintf(stderr, "Table created successfully\n");
    }
    
    sqlite3_close(db);
    
    return 0;
}
