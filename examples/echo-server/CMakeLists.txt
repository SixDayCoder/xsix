# project name
project(echo-server)

# platform differences
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        add_definitions(-D _XSIX_LINUX)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(WIN32)
        add_definitions(-D _XSIX_WINDOWS)
endif(WIN32)

if(UNIX)
        add_definitions(-D _XSIX_LINUX)
endif(UNIX)

# add include dirs
include_directories(${ROOT_PROJECT_DIR}/xsix)

# add link dirs
link_directories(/usr/local/lib)
link_directories(${ROOT_PROJECT_DIR}/lib)

# find src files
set(SRC_FILES ${PROJECT_SOURCE_DIR}/echo_server.cpp)

# generate exeutable
add_executable(echo-server ${SRC_FILES})
target_link_libraries(echo-server libxsix)