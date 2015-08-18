#ifndef BOOST_SYSTEM_NO_DEPRECATED
#define BOOST_SYSTEM_NO_DEPRECATED

#include "system.h"
#include "params.h"

using namespace trm;

int main(int argc, char** argv)
{
    System sys;
    Params para(sys);
    if (argc == 1)
        para.getParams(NULL, NULL, NULL);
    else if (argc == 2)
        para.getParams(argv[1], NULL, NULL);
    else if (argc == 3)
        para.getParams(argv[1], argv[2], NULL);
    else if (argc == 4)
        para.getParams(argv[1], argv[2], argv[3]);
    para.chooseFunc();
    sys.execute();

    return 0;
}

#endif
