#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace trm
{
class Database
{
private:
    /*
     * New Database File *
     * -----------------
     * If a new Database File is created this Value is true. Needed to create a
     * Database Table if the File is new.
     */
    bool newDbFile;
    /*
     * Username *
     * --------
     * Actually User that is logged in
     */
    std::string user;
    /*
     * Complete Path to the Database File *
     */
    std::string dbFile;
    /*
     * Database Status *
     * ---------------
     * Saves the return from the Database. Needed to check Database Status after
     * Database Actions
     */
    int dbStatus;
    /*
     * Database Object *
     * ---------------
     * Saves the Database Object from the Database Open Action
     */
    sqlite3 *db;
    /*
     * Database Error Message *
     * ----------------------
     * If execution gives an Error it will be saved here
     */
    char *errMsg;
    /*
     * Opens the Database Connection *
     */
    void openDb();
    /*
     * Set Trashdb Path *
     * ----------------
     * Sets the right Trashdb Path for the actually User that is logged in
     */
    void setDbPath();
    /*
     * Creates Database Table *
     */
    void createDbTable();
public:
    Database();
    ~Database();

    /*
     * Initialize Database *
     * -------------------
     * Sets Database Path, opens Databaseconnection, Creates Databasetable if necessary
     */
    void init(std::string username);
    /*
     * Get Database File *
     * -----------------
     * Returns the complete Path with the Database File *
     */
    fs::path getDbFile();
    /*
     * Write to Database *
     * -----------------
     * With the File delete Action we create a Database Entry.
     */
    void createEntry(fs::path path, fs::path trashPath, std::size_t size);
    /*
     * Get one Entry *
     * -------------
     * For the restore Procedure we need the Database Entry. With the Name or the ID.
     */
    sqlite3_stmt* getEntry(std::string name);
    sqlite3_stmt* getEntry(int id);
    /*
     * Delete Database Entry *
     * ---------------------
     * Deletes a Entry by ID.
     */
    void deleteEntry(std::string name);
    /*
     * List all Entrys *
     * ---------------
     * Here we return a List of all Database Entrys.
     */
    sqlite3_stmt* listEntrys();
    /*
     * Search Entrys *
     * -------------
     * This Function allows to search the Database for One or more Entrys.
     */
    sqlite3_stmt* searchEntry(std::string objectName);
    /*
     * Return Trash Path *
     * -----------------
     * Returns all Trash Pathes to delete all Files listed in the Database.
     */
    sqlite3_stmt* pathEntry();
    /*
     * Return Trash Size *
     * -----------------
     * Returns the Trash Size for all Entrys. Complete Trash Size will be shown in System Class.
     */
    sqlite3_stmt* sizeEntry();

};
}

#endif // DATABASE_H
