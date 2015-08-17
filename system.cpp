#include "system.h"

/*
 * ----------------------------------------------------------------------------------
 * PRIVATE *
 * ----------------------------------------------------------------------------------
 */

void trm::System::deleteFile()
{
    fs::path path = destPath;
    path.remove_filename();
    if (!fs::exists(path))
        fs::create_directory(path);
    size = fs::file_size(sourcePath);
    trm::System::renameObject();
    if (!quiet)
    {
        std::cout << "Copy to trash " << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << destPath.string() << std::endl;
    }
    fs::copy_file(sourcePath, destPath);
    if (!quiet)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Finally delete " << sourcePath.string() << std::endl;
    }
    fs::remove(sourcePath);
}

void trm::System::deleteDir()
{
    fs::path path = destPath;
    path.remove_filename();
    if (!fs::exists(path))
        fs::create_directory(path);
    trm::System::getDirSize();
    trm::System::renameObject();
    if (!quiet)
    {
        std::cout << "Copy to trash " << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << destPath.string() << std::endl;
    }
    fs::copy_directory(sourcePath, destPath);

    for (fs::recursive_directory_iterator it(sourcePath);
         it != fs::recursive_directory_iterator(); ++it)
    {
        std::string substrPath = sourcePath.string();
        std::string path = it->path().string();
        path.erase(0, substrPath.length());
        std::string strDestPath = destPath.string() + path;
        fs::path itDestPath(strDestPath);
        if (fs::is_directory(*it))
        {
            if (!quiet)
                std::cout << itDestPath.string() << std::endl;
            fs::copy_directory(it->path(), itDestPath);
        }
        else
        {
            if (!quiet)
                std::cout << itDestPath.string() << std::endl;
            fs::copy_file(it->path(), itDestPath);
        }
    }
    if (!quiet)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Finally delete " << sourcePath.string() << std::endl;
    }
    fs::remove_all(sourcePath);

}

void trm::System::restoreFile()
{
    if (fs::exists(sourcePath))
    {
        if (!quiet)
            std::cout << destPath.string() << std::endl;
        destPath /= sourcePath.filename();
        if (!fs::exists(destPath))
            fs::copy_file(sourcePath, destPath);
        else
            std::cout << "There is already a File with this Name... Canceled" << std::endl;
    }
    else
    {
        std::cout << "The File to restore doesnt exists." << std::endl;
        exit(0);
    }

    if (!quiet)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Finally delete " << sourcePath.string() << std::endl;
    }
    fs::remove(sourcePath);
}

void trm::System::restoreDir()
{
    if (fs::exists(sourcePath))
    {
        if (!quiet)
            std::cout << destPath.string() << std::endl;
        destPath /= sourcePath.filename();
        if (!fs::exists(destPath))
            fs::copy_directory(sourcePath, destPath);
        else
            std::cout << "There is already a Directory with this Name... Canceled" << std::endl;

        for (fs::recursive_directory_iterator it(sourcePath);
             it != fs::recursive_directory_iterator(); ++it)
        {
            std::string substrPath = sourcePath.string();
            std::string path = it->path().string();
            path.erase(0, substrPath.length());
            std::string strDestPath = destPath.string() + path;
            fs::path itDestPath(strDestPath);
            if (fs::is_directory(*it))
            {
                if (!quiet)
                    std::cout << itDestPath.string() << std::endl;
                fs::copy_directory(it->path(), itDestPath);
            }
            else
            {
                if (!quiet)
                    std::cout << itDestPath.string() << std::endl;
                fs::copy_file(it->path(), itDestPath);
            }
        }
    }
    else
    {
        std::cout << "The Directory to restore doesnt exists." << std::endl;
        exit(0);
    }

    if (!quiet)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Finally delete " << sourcePath.string() << std::endl;
    }
    fs::remove_all(sourcePath);
}

void trm::System::getDirSize()
{
    for (fs::recursive_directory_iterator it(sourcePath);
        it != fs::recursive_directory_iterator(); ++it)
    {
        if (fs::is_regular_file(*it))
        {
            size += fs::file_size(*it);
        }
    }
}

void trm::System::delLastSlash()
{
    std::string path = sourcePath.string();
    if (path.at(path.length() - 1) == '/')
        sourcePath.remove_filename();
}

void trm::System::getUser()
{
    struct passwd *pw;
    uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    user = std::string(pw->pw_name);
    homedir = std::string(pw->pw_dir);
}

void trm::System::renameObject()
{
    if (fs::exists(destPath))
    {
        std::cout << "A File or Directory with this Name already exists in the Trash... make Copy" << std::endl;
        fs::path path(destPath);

        int i = 1;
        while (fs::exists(path))
        {
            path.remove_filename();
            std::string filename = destPath.filename().string();
            std::string number;
            number = boost::lexical_cast<std::string>(i);
            if (fs::is_directory(destPath))
                filename += "_" + number;
            else
            {
                std::size_t found = filename.find_last_of(".");
                std::string prefix = filename.substr(0, found);
                std::string suffix = filename.substr(found + 1);
                filename = prefix + "_" + number + "." + suffix;
            }
            std::string strPath = path.string() + "/" + filename;
            path = strPath;
            i++;
        }
        destPath = path;
    }
}

/*
 * ----------------------------------------------------------------------------------
 * PUBLIC
 * ----------------------------------------------------------------------------------
 */

trm::System::System()
{
    quiet = false;
    //get Current Path in Shell
    currentDir = fs::current_path();
    //make Trash Directory
    trashDir = currentDir.string() + TRASH;
    //get the actually User that is logged in
    trm::System::getUser();
    db.init(user, homedir);
}

trm::System::~System()
{

}

void trm::System::deleteObj()
{
    if (fs::is_regular_file(sourcePath))
        trm::System::deleteFile();
    else if (fs::is_directory(sourcePath))
        trm::System::deleteDir();

    db.createEntry(sourcePath, destPath, size);
    if (!quiet)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Done" << std::endl;
    }
}

void trm::System::restoreObj()
{
    if (!quiet)
    {
        std::cout << "Restore File" << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }
    sqlite3_stmt *stmt;

    if (objectName.compare(0, 3, "id:") == 0)
    {
        int found = objectName.find_last_of(":");
        std::string strId = objectName.substr(found+1);
        int id = atoi(strId.c_str());
        stmt = db.getEntry(id);
    }
    else
        stmt = db.getEntry(objectName);

    int res = sqlite3_step(stmt);
    if (res == SQLITE_ROW)
    {
        objectName = (char*)sqlite3_column_text(stmt, 0);
        sourcePath = (char*)sqlite3_column_text(stmt, 1);
        if (optDestPath == "")
            destPath = (char*)sqlite3_column_text(stmt, 2);
        else
            destPath = optDestPath;
    }
    else if (res == SQLITE_DONE)
    {
        std::cout << "No Database Entry found" << std::endl;
        exit(0);
    }
    sqlite3_finalize(stmt);

    fs::path path(destPath);
    if (fs::exists(path.remove_filename()))
    {
        if (fs::is_directory(sourcePath))
            trm::System::restoreDir();
        else
            trm::System::restoreFile();

        db.deleteEntry(objectName);
    }
    else
    {
        std::cout << "Destination Path doesnt exists." << std::endl;
        exit(0);
    }
    if (!quiet)
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Done" << std::endl;
    }
}

void trm::System::listTrash()
{
    sqlite3_stmt *stmt;
    stmt = db.listEntrys();

    std::cout << "List Trash" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    int res = 0;
    while (true)
    {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
        {
            std::cout << "ID: " << (char*)sqlite3_column_text(stmt, 0)
                    << std::endl;
            std::cout << "ObjectName: " << (char*)sqlite3_column_text(stmt, 1)
                    << std::endl;
            std::cout << "Size in Bytes: " << (char*)sqlite3_column_text(stmt,2)
                    << std::endl;
            std::cout << "Trash Path: " << (char*)sqlite3_column_text(stmt, 3)
                    << std::endl;
            std::cout << "Old Path: " << (char*)sqlite3_column_text(stmt, 4)
                    << std::endl;
            std::cout << "deleted at: " << (char*)sqlite3_column_text(stmt, 5)
                    << std::endl;
            std::cout << "---------------------------------------------------"
                    << std::endl;
            std::cout << "\n";
        }
        if (res == SQLITE_DONE)
        {
            break;
        }
    }
    sqlite3_finalize(stmt);
}

void trm::System::searchTrash()
{

    sqlite3_stmt *stmt;
    stmt = db.searchEntry(objectName);

    std::cout << "Search Trash" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    int res = 0;
    while (true)
    {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
        {
            std::cout << "ID: " << (char*)sqlite3_column_text(stmt, 0)
                    << std::endl;
            std::cout << "ObjectName: " << (char*)sqlite3_column_text(stmt, 1)
                    << std::endl;
            std::cout << "Size in Bytes: " << (char*)sqlite3_column_text(stmt, 2)
                    << std::endl;
            std::cout << "Trash Path: " << (char*)sqlite3_column_text(stmt, 3)
                    << std::endl;
            std::cout << "Old Path: " << (char*)sqlite3_column_text(stmt, 4)
                    << std::endl;
            std::cout << "deleted at: " << (char*)sqlite3_column_text(stmt, 5)
                    << std::endl;
            std::cout << "\n";
        }
        if (res == SQLITE_DONE)
            break;
    }
    sqlite3_finalize(stmt);
}

void trm::System::emptyTrash()
{
    sqlite3_stmt *stmt;
    stmt = db.pathEntry();

    std::cout << "You really want to empty the Trash? [y/n] ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "y")
    {
        if (!quiet)
        {
            std::cout << "\nEmpty Trash" << std::endl;
            std::cout << "-----------------------------" << std::endl;
        }
        int res = 0;
        while (true)
        {
            res = sqlite3_step(stmt);
            if (res == SQLITE_ROW)
            {
                char *path = (char*)sqlite3_column_text(stmt, 0);
                fs::path trashPath(path);
                if (fs::exists(trashPath))
                {
                    if (!quiet)
                        std::cout << path << std::endl;
                    if (fs::is_directory(trashPath))
                        fs::remove_all(trashPath);
                    else
                        fs::remove(trashPath);
                }
            }
            if (res == SQLITE_DONE)
                break;
        }
        fs::remove(db.getDbFile());
        if (!quiet)
        {
            std::cout << "-----------------------------" << std::endl;
            std::cout << "Done" << std::endl;
        }
    }
    else
        std::cout << "Canceled..." << std::endl;
}

void trm::System::showTrashSize()
{
    sqlite3_stmt *stmt;
    stmt = db.sizeEntry();

    std::cout << "Trash Size" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    int res = 0;
    while (true)
    {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
        {
            char *cFileSize = (char*)sqlite3_column_text(stmt, 0);
            boost::uintmax_t fileSize;
            fileSize = boost::lexical_cast<boost::uintmax_t>(cFileSize);
            size += fileSize;
        }
        if (res == SQLITE_DONE)
            break;
    }
    std::string bytes = " Bytes";
    std::string strSize;
    if (size > 1000000)
    {
        float x = size / 10000;
        size = boost::lexical_cast<boost::uintmax_t>(x);
        bytes = " MByte";
        strSize = boost::lexical_cast<std::string>(size);
        strSize = strSize.insert(strSize.length() - 2, ",");
    }
    else
        strSize = boost::lexical_cast<std::string>(size);

    std::cout << "Trashsize of User " + user + " has " + strSize + bytes << std::endl;
}

void trm::System::setPath(std::string path, std::string optPath)
{
    if (fs::exists(path))
    {
        sourcePath = fs::complete(path);
        trm::System::delLastSlash();
        destPath = sourcePath;
        destPath.remove_filename();
        destPath /= ".trash";
        destPath /= sourcePath.filename();

    }
    else
    {
        std::cout << "Entered Path didnt exists" << std::endl;
        exit(0);
    }
    if (optPath != "")
    {
        if (fs::exists(optPath))
        {
            optDestPath = fs::complete(optPath);
        }
        else
        {
            std::cout << "Retore Path didnt exists" << std::endl;
            exit(0);
        }
    }
}

void trm::System::setName(std::string name, std::string destPath)
{
    objectName = name;
    optDestPath = destPath;
}

void trm::System::setFunction(const boost::function<void()> &f)
{
    func = f;
}

void trm::System::execute()
{
    func();
}

void trm::System::help()
{
    std::cout << "Trash Manager Help" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Usage: trm [PARAMETER] [PATH][OBJECTNAME][ID] [RESTOREPATH]" << std::endl;
    std::cout << "\n-d --delete\t\t" << "deletes the File or Directory [PATH]" << std::endl;
    std::cout << "-r --restore\t\t" << "restores the Object [OBJECTNAME]/id:[ID] optional [RESTOREPATH]" << std::endl;
    std::cout << "-l --list\t\t" << "list all deleted Objects in Database" << std::endl;
    std::cout << "-s --search\t\t" << "search Objects in Database [OBJECTNAME]" << std::endl;
    std::cout << "-S --size\t\t" << "shows the Size of the deleted Data for the User" << std::endl;
    std::cout << "-e --empty\t\t" << "empty the Trash of the User" << std::endl;
    std::cout << "-h --help\t\t" << "shows the help" << std::endl;
    std::cout << "q -quiet\t\t" << "with the additioanl Parameter the output will be disabled" << std::endl;
    std::cout << "Example: trm -dq [PATH] or trm --delete-quiet [PATH]" << std::endl;
}
