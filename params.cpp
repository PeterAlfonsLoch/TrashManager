#include "params.h"

void trm::Params::withoutPath()
{
    if ((std::string)param == "-l" || (std::string)param == "--list")
        sys->setFunction(boost::bind(&System::listTrash, sys));
    else if ((std::string)param == "-e" || (std::string)param == "--empty")
        sys->setFunction(boost::bind(&System::emptyTrash, sys));
    else if ((std::string)param == "-eq" || (std::string)param == "-qe"
             || (std::string)param == "--empty-quiet")
    {
        sys->quiet = true;
        sys->setFunction(boost::bind(&System::emptyTrash, sys));
    }
    else if ((std::string)param == "-S" || (std::string)param == "--size")
        sys->setFunction((boost::bind(&System::showTrashSize, sys)));
    else
        sys->setFunction(boost::bind(&System::help, sys));
}

void trm::Params::withPath()
{

    if ((std::string)param == "-d" || (std::string)param == "--delete")
    {
        sys->setPath(path, "");
        sys->setFunction(boost::bind(&System::deleteObj, sys));
    }
    else if ((std::string)param == "-dq" || (std::string)param == "-qd"
             || (std::string)param == "--delete-quiet")
    {
        sys->quiet = true;
        sys->setPath(path, "");
        sys->setFunction(boost::bind(&System::deleteObj, sys));
    }
    else if ((std::string)param == "-s" || (std::string)param == "--search")
    {
        sys->setName(path, "");
        sys->setFunction(boost::bind(&System::searchTrash, sys));
    }
    else if ((std::string)param == "-r" ||(std::string)param == "--restore")
    {
        sys->setName(path, "");
        sys->setFunction(boost::bind(&System::restoreObj, sys));
    }
    else if ((std::string)param == "-rq" || (std::string)param == "-qr"
             ||(std::string)param == "--restore-quiet")
    {
        sys->quiet = true;
        sys->setName(path, "");
        sys->setFunction(boost::bind(&System::restoreObj, sys));
    }
    else if ((std::string)param == "-h" ||(std::string)param == "--help")
        sys->setFunction(boost::bind(&System::help, sys));
    else
        sys->setFunction(boost::bind(&System::help, sys));
}

void trm::Params::withOptPath()
{
    sys->setName(path, optPath);
    if ((std::string)param == "-r" || (std::string)param == "--restore")
        sys->setFunction(boost::bind(&System::restoreObj, sys));
    else if ((std::string)param == "-rq" || (std::string)param == "-qr"
             ||(std::string)param == "--restore-quiet")
    {
        sys->quiet = true;
        sys->setFunction(boost::bind(&System::restoreObj, sys));
    }
    else
        sys->setFunction(boost::bind(&System::help, sys));
}

trm::Params::Params(System &system)
{
    sys = &system;
    param = NULL;
    path = NULL;
    optPath = NULL;
}

void trm::Params::getParams(char *p1, char *p2, char *p3)
{
    if (p1 != NULL || p1 != "")
        param = p1;
    if (p2 != NULL || p2 != "")
        path = p2;
    if (p3 != NULL || p3 != "")
        optPath = p3;
}

void trm::Params::chooseFunc()
{
    try
    {
        if (param != NULL && path == NULL && optPath == NULL)
            trm::Params::withoutPath();
        else if (param != NULL && path != NULL && optPath == NULL)
            trm::Params::withPath();
        else if (param != NULL && path != NULL && optPath != NULL)
            trm::Params::withOptPath();
        else
            sys->setFunction(boost::bind(&System::help, sys));
    }
    catch(fs::filesystem_error &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
