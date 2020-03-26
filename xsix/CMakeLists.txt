# project name
project(libxsix)

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
include_directories(${ROOT_PROJECT_DIR})

# add link dirs
link_directories(/usr/local/lib)

# find src files
aux_source_directory(${ROOT_PROJECT_DIR}/xsix/impl LIBXSIX_SRC_FILES)

# generate static library
add_library(libxsix STATIC ${LIBXSIX_SRC_FILES})