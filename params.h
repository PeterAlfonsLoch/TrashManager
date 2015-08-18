#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <boost/bind.hpp>
#include "system.h"

namespace trm
{
class Params
{
private:
    /*
     * System Object *
     * -------------
     * System Object from main.cpp
     */
    System *sys;
    /*
     * Function Parameter *
     * ------------------
     * Here we will choose which function will be called. This Variable can be "-d" as
     * Example.
     */
    char *param;
    /*
     * Path Parameter *
     * --------------
     * If you want to delete a file or restore it a Path is been needed. It can be the
     * Full Path with the Filename or only the Filename.
     */
    char *path;
    /*
     * Restore Path Parameter *
     * ----------------------
     * This Parameter is only needed if you want to restore a File to a different Path
     * as the Oldpath saved in the Database
     */
    char *optPath;
    /* Choose Function without Path *
     * ----------------------------
     * If the Path Parameter is empty we choose only Functions without the Path Parameter
     */
    void withoutPath();
    /* Choose Function with Path *
     * -------------------------
     * If the Path Parameter is NOT empty we choose Functions with the Path Parameter
     */
    void withPath();
    /* Choose Function with optional Path
     * ----------------------------------
     * If all Paremeters are not empty we choose the Function with all ones. In this Case
     * it is only the Restore Method
     */
    void withOptPath();
public:
    /*
     * Constructor with System Object
     */
    Params(System &system);
    /*
     * get Parameters *
     * --------------
     * the given Arguments from the Commandline.
     * @p1: Parameter 1 (Example "-d")
     * @p2: Path
     * @p3: optional Path (only for restore)
     */
    void getParams(char *p1, char *p2, char *p3);
    /*
     * Choose Function *
     * ---------------
     * With the given Parameters the right Function will be choosen and given to the
     * System Object for an execute later
     */
    void chooseFunc();
};
}

#endif // PARAMS_H
