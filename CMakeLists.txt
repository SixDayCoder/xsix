# project name
project(xsix)

cmake_minimum_required(VERSION 3.12)

# set c/c++ version
add_definitions(-std=c++11)

set(CMAKE_C_STANDARD 99)

# output
# message("current_project_dir" = ${PROJECT_SOURCE_DIR})

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

# ${PROJECT_SOURCE_DIR} the close dir of CMakeLists.txt

# set root dir
set(ROOT_PROJECT_DIR ${PROJECT_SOURCE_DIR})

# set binary dir
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set lib dir
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# add include dirs
include_directories(${PROJECT_SOURCE_DIR})

# add link dirs
# link_directories(/usr/local/protobuf/lib)
link_directories(/usr/local/lib)
link_directories(${PROJECT_BINARY_DIR}/lib)

# find all files in dir to your name aux_source_directory(dir, my_name)
#aux_source_directory(${PROJECT_SOURCE_DIR}/xsix/impl LIBXSIX_SRC_FILES)

# link all
#add_executable(testlibsix ${TEST_LIBXSIX_SRC_FILES})
#target_link_libraries(xredis hiredis)
add_subdirectory(${PROJECT_SOURCE_DIR}/xsix)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/echo-client)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/echo-server)
