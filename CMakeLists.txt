project(TrashManager)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "trm")
SET(Boost_ADDITIONAL_VERSIONS "1.54" "1.55.0" "1.56.0" "1.57.0" "1.58.0" "1.59.0")
SET(BOOST_MIN_VERSION "1.54.0")

find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS system filesystem)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Fatal error: Boost (version >= 1.54.0) required.\n")
endif (NOT Boost_FOUND)

find_library(SQLITE NAMES sqlite3)
if (NOT SQLITE)
    message(FATAL_ERROR "Fatal error: SQLite3 required.\n")
endif (NOT SQLITE)

target_link_libraries(${PROJECT_NAME} boost_filesystem boost_system sqlite3)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/share/trashmanager)
install(CODE "EXECUTE_PROCESS(COMMAND ln -sf /usr/share/trashmanager/trm /bin/trm)")
