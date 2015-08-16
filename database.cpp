#include "database.h"

/*
 * ----------------------------------------------------------------------------------
 * PRIVATE *
 * ----------------------------------------------------------------------------------
 */

void trm::Database::openDb()
{
    fs::path newDb(dbFile);
    if (fs::exists(newDb))
        newDbFile = false;
    else
        newDbFile = true;
    //open a Database Connection and create a Database Object
    dbStatus = sqlite3_open(dbFile.c_str(), &db);
    if (dbStatus != SQLITE_OK)
        std::cout << "Cant open Database: " << sqlite3_errmsg(db) << std::endl;
    if (newDbFile)
        trm::Database::createDbTable();
}

void trm::Database::setDbPath()
{
    std::string strPath;
    if (user == "root")
        strPath = "/root/.trashdb/";
    else
        strPath = "/home/" + (std::string)user + "/.trashdb/";
    fs::path path(strPath);
    if (!fs::exists(path))
        fs::create_directory(path);
    dbFile = strPath + "trashdb.db";
}

void trm::Database::createDbTable()
{
    //SQL String to build the Datatable
    char sql[] =    "CREATE TABLE trash("
                    "ID             INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "OBJECTNAME     TEXT        NOT NULL,"
                    "FILESIZE       INTEGER     NOT NULL,"
                    "TRASHPATH      TEXT        NOT NULL,"
                    "OLDPATH        TEXT        NOT NULL,"
                    "DELETEDAT      DATETIME    NOT NULL);";
    dbStatus = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (dbStatus != SQLITE_OK)
    {
        char text[] = "Table in Database cannot be created: ";
        std::cout << strcat(text, errMsg) << std::endl;
        exit(0);
    }
}

/*
 * ----------------------------------------------------------------------------------
 * PUBLIC *
 * ----------------------------------------------------------------------------------
 */

trm::Database::Database()
{

}

trm::Database::~Database()
{
    //Close Database
    sqlite3_close(db);
}

void trm::Database::init(std::string username)
{
    user = username;
    trm::Database::setDbPath();
    trm::Database::openDb();
}

fs::path trm::Database::getDbFile()
{
    return dbFile;
}

void trm::Database::createEntry(fs::path path, fs::path trashPath, std::size_t size)
{
    sqlite3_stmt *stmt;
    std::string objectName = trashPath.filename().string();
    const char *cPath = path.remove_filename().c_str();

    char sql[] =  "INSERT INTO trash (OBJECTNAME, FILESIZE, TRASHPATH, OLDPATH, DELETEDAT) "
                        "VALUES (?, ?, ?, ?, datetime('NOW', 'localtime'));";
    dbStatus = sqlite3_prepare(db, sql, -1, &stmt, 0);
    if (dbStatus != SQLITE_OK)
    {
        std::cout << "Database Error: " << errMsg << std::endl;
        exit(0);
    }

    if (sqlite3_bind_text(stmt, 1, objectName.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
        std::cout << "Database Bind Error: " << sqlite3_errmsg(db) << std::endl;
    if (sqlite3_bind_int(stmt, 2, static_cast<int>(size)) != SQLITE_OK)
        std::cout << "Database Bind Error: " << sqlite3_errmsg(db) << std::endl;
    if (sqlite3_bind_text(stmt, 3, trashPath.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
        std::cout << "Database Bind Error: " << sqlite3_errmsg(db) << std::endl;
    if (sqlite3_bind_text(stmt, 4, cPath, -1, SQLITE_STATIC))
        std::cout << "Database Bind Error: " << sqlite3_errmsg(db) << std::endl;

    if (sqlite3_step(stmt) != SQLITE_DONE)
        std::cout << "Database Execute Error: " << sqlite3_errmsg(db) << std::endl;
}

sqlite3_stmt* trm::Database::getEntry(std::string name)
{
    sqlite3_stmt *stmt;
    std::string sql = "SELECT OBJECTNAME, TRASHPATH, OLDPATH FROM trash WHERE OBJECTNAME=?;";

    dbStatus = sqlite3_prepare(db, sql.c_str(), -1, &stmt, 0);
    if (dbStatus != SQLITE_OK)
    {
        std::cout << "Database Error: " << errMsg << std::endl;
        exit(0);
    }

    if (sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
        std::cout << "Database Bind Error: " << sqlite3_errmsg(db) << std::endl;

    return stmt;
}
sqlite3_stmt* trm::Database::getEntry(int id)
{
    sqlite3_stmt *stmt;
    std::string sql = "SELECT OBJECTNAME, TRASHPATH, OLDPATH FROM trash WHERE ID=?;";

    dbStatus =sqlite3_prepare(db, sql.c_str(), -1, &stmt, 0);
    if (dbStatus != SQLITE_OK)
    {
        std::cout << "Database Error: " << errMsg << std::endl;
        exit(0);
    }

    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK)
        std::cout << "Database Bind Error: " << sqlite3_errmsg(db) << std::endl;

    return stmt;
}

void trm::Database::deleteEntry(std::string name)
{
    sqlite3_stmt *stmt;
    std::string sql = "DELETE FROM trash WHERE OBJECTNAME='" + name + "';";

    dbStatus = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
    if (dbStatus != SQLITE_OK)
    {
        std::cout << "Database Error: " << errMsg << std::endl;
        exit(0);
    }
}

sqlite3_stmt* trm::Database::listEntrys()
{
    sqlite3_stmt *stmt;
    char sql[] = "SELECT * FROM trash;";
    dbStatus = sqlite3_prepare(db, sql, -1, &stmt, 0);
    if (dbStatus == SQLITE_OK)
    {
        return stmt;
    }
    else
    {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
}

sqlite3_stmt* trm::Database::searchEntry(std::string objectName)
{
    sqlite3_stmt *stmt;
    std::string sql = "SELECT * FROM trash WHERE OBJECTNAME LIKE '%" + objectName + "%';";
    dbStatus = sqlite3_prepare(db, sql.c_str(), -1, &stmt, 0);
    if (dbStatus == SQLITE_OK)
    {
        return stmt;
    }
    else
    {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
}

sqlite3_stmt* trm::Database::pathEntry()
{
    sqlite3_stmt *stmt;
    char sql[] = "SELECT TRASHPATH FROM trash;";
    dbStatus = sqlite3_prepare(db, sql, -1, &stmt, 0);
    if (dbStatus == SQLITE_OK)
    {
        return stmt;
    }
    else
    {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
}

sqlite3_stmt* trm::Database::sizeEntry()
{
    sqlite3_stmt *stmt;
    char sql[] = "SELECT FILESIZE FROM trash;";
    dbStatus = sqlite3_prepare(db, sql, -1, &stmt, 0);
    if (dbStatus == SQLITE_OK)
    {
        return stmt;
    }
    else
    {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
}
