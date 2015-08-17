#ifndef SYSTEM_H
#define SYSTEM_H

#define TRASH "/.trash/"

#include <string>
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <sqlite3.h>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include "database.h"
namespace fs = boost::filesystem;

namespace trm
{
class System
{
private:
    /*
     * Username *
     * --------
     * Contains the Username that is actually logged in. Depending on the logged in User
     * in the Terminal in Linux.
     */
    std::string user;
    /*
     * Homedir *
     * -------
     * Saves the Homedir of the actually User.
     */
    std::string homedir;
    /*
     * Trash Directory *
     * -----------------
     * Depends on which Filesystem the File got deleted a ".trash/" Directory will be
     * created on it to store the deleted File.
     */
    fs::path trashDir;
    /*
     * Size of Files/Directorys *
     * ------------------------
     * Gets the Size of a deleted File or a Directory(iterates recursivly)
     */
    boost::uintmax_t size;
    /*
     * Current User Path in Shell *
     */
    fs::path currentDir;
    /*
     * Source Path *
     * -----------
     * Contains the Source Path of the File that we want to delete
     */
    fs::path sourcePath;
    /*
     * Object Name *
     * -----------
     * If we only need an Objectname insteed a Path.
     */
    std::string objectName;
    /*
     * Destination Path *
     * ----------------
     * Contains the Destination Path where the file will be moved
     */
    fs::path destPath;
    /*
     * Optional Destination Path *
     * -------------------------
     * Contains an optional Destination Path. Only needed in the Restore Method.
     */
    fs::path optDestPath;
    /*
     * Database Object *
     * ---------------
     * saves the current Database Object
     */
    trm::Database db;
    /*
     * Function object *
     * ---------------
     * Contains the Function that will be executed at the End of the Programm
     */
    boost::function<void()> func;
    /*
     * Delete File *
     * ---------
     * Copys a File and removes the old one
     */
    void deleteFile();
    /*
     * Delete Directory *
     * --------------
     * Copys the Directory recursivly
     */
    void deleteDir();
    /*
     * Restore File *
     * ------------
     * Restore the File from trash by copying it and delete the Trash File.
     */
    void restoreFile();
    /*
     * Restore Directory *
     * -----------------
     * Restore the Directory from trash by copying it and delete the Trash Directory-
     */
    void restoreDir();
    /*
     * Get Size of Directory *
     * ---------------------
     * If the Directory is not empty. Here we iterates throw it and get the complete size.
     */
    void getDirSize();
    /*
     * Delete last Slash *
     * ---------------------
     * Sometimes Pathes have a Slash at the End. In this case the boost::filesystem:filename()
     * Function doesnt work correctly with Directorys. Here we delete the Slash.
     */
    void delLastSlash();
    /*
     * Get Username *
     * ------------
     * Gets the Linux User that is logged in.
     */
    void getUser();
    /*
     * Rename Object *
     * -------------
     * Along the delete Process its possible that there are already an Object with this Objectname in the Trash. In
     * this Case we make a Copy by adding numbers at the End of the Object.
     */
    void renameObject();
public:
    System();
    ~System();

    /*
     * Quiet delete/restore *
     * --------------------
     * If the User dont want to show the details this bool must be true.
     */
    bool quiet;
    /*
     * Delete the Object *
     * -----------------
     * Deletes the File or Directory the User want to delete. For this Action the
     * Parameter "-d" is needed and as additional Argument the File/Directory Name.
     * The Name can be the complete Path with Name or only the Name.
     */
    void deleteObj();
    /*
     * Restore the Object *
     * ------------------
     * Restores the File or Directory with the -r Parameter. As Argument the ID from
     * the Database or the Filename is required.
     */
    void restoreObj();
    /*
     * List Trash *
     * ----------
     * List all Trash Files from the actually User
     */
    void listTrash();
    /*
     * Search in Trash *
     * ---------------
     * Search in the Trash vor One or more Entrys-
     */
    void searchTrash();
    /*
     * Empty Trash *
     * -----------
     * Deletes all Files and Directorys in Trash and delete the Database File.
     */
    void emptyTrash();
    /*
     * Show Trash Size *
     * ---------------
     * Shows the Trash Size for the actually User.
     */
    void showTrashSize();
    /*
     * Set Path *
     * --------
     * Sets the Source Path of the File we want to delete. Optional we can
     * define a Path where we want to restore the File to (only needed in Restore
     * Method).
     */
    void setPath(std::string path, std::string optPath);
    /*
     * Set Name *
     * --------
     * As Example: For the Search we dont need a Path but a Objectname to search for.
     */
    void setName(std::string name, std::string destPath);
    /*
     * get Functionname *
     * ----------------
     * Delivers the Function from the Parameter Class
     */
    void setFunction(const boost::function<void()> &f);
    /*
     * Execute Action *
     * --------------
     * After delivering the Function here it will be called
     */
    void execute();
    /*
     * Shows Help *
     */
    void help();
};
}
#endif // SYSTEM_H
