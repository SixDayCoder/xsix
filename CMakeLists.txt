# project name
project(root_xsix)

cmake_minimum_required(VERSION 3.12)

# set c/c++ version
add_definitions(-std=c++11)

set(CMAKE_C_STANDARD 99)

# output
message("======system name====== : " = ${CMAKE_SYSTEM_NAME})
message("======root project dir====== : " = ${PROJECT_SOURCE_DIR})

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

# ${PROJECT_SOURCE_DIR} dir of CMakeLists.txt

# set root dir
set(ROOT_PROJECT_DIR ${PROJECT_SOURCE_DIR})

# set 3rd asio dir
set(THIRD_ASIO_DIR ${PROJECT_SOURCE_DIR}/3rd/asio)
message("======asio dir====== : " = ${THIRD_ASIO_DIR})

# set binary dir
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
message("======executable output path====== : " = ${EXECUTABLE_OUTPUT_PATH})

# set lib dir
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
message("======lib output path====== : " = ${LIBRARY_OUTPUT_PATH})
message("======project bindary dir ===== : " = ${PROJECT_BINARY_DIR})

# add include dirs
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${THIRD_DIR}/asio/include)

# add link dirs
# link_directories(/usr/local/protobuf/lib)
link_directories(/usr/local/lib)
link_directories(${PROJECT_BINARY_DIR}/lib)

# find all files in dir to your name aux_source_directory(dir, my_name)
#aux_source_directory(${PROJECT_SOURCE_DIR}/xsix/impl LIBXSIX_SRC_FILES)

# source_group/create_filters -> like vs file filters

# link all
#add_executable(testlibsix ${TEST_LIBXSIX_SRC_FILES})
#target_link_libraries(xredis hiredis)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/xsix)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/echo)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/actor)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/chatroom)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/buffer)