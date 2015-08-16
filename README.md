# TrashManager
A Commandline Trash Programm like the Windows Trash http://www.ghuul.de

Installation requires following Packages sqlite3, sqlite3-dev, libboost-filesystem, libboost-filesystem-dev, libboost-system, libboost-system-dev

I used cmake for the Programm. For install the Programm do...

cmake [TRASHMANAGERDIRECTORY]

make

make install //you need root permission for this command

You can use the Programm like this...

Parameters are:

-d --delete [PATH]                             deletes a Object defined in [PATH]

-r --restore [OBJECTNAME][ID] [RESTOREPATH]    restore a Object with [OBJECTNAME] or id:[ID]. The Restore Path is the old Path where the Object are from. Additional Parameter[RESTOREPATH] lets you restore to another Path as the old.

-l --list                                      list all deleted Object in Trash

-s --search [OBJECTNAME]                       searches for an Object [OBJECTNAME]

-S --size                                      shows the Size of the Trash for the actually User

-e --empty                                     empty the Trash for the User

-q -quiet                                      this is an additioanl Parameter. with this parameter the output will
                                               be not shown. Example: trm -dq [PATH] or trm --delete-quiet [PATH]
                                               
-h --help                                      shows the help
