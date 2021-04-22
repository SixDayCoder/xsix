# project name
project(echo-server)

# platform differences
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        add_definitions(-D _XSIX_LINUX)
        find_package (Threads)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(WIN32)
        add_definitions(-D _XSIX_WINDOWS)
endif(WIN32)

if(UNIX)
        add_definitions(-D _XSIX_LINUX)
endif(UNIX)

# add include dirs
include_directories(${ROOT_PROJECT_DIR}/src)
include_directories(${THIRD_ASIO_DIR}/include)

# add link dirs
link_directories(/usr/local/lib)
link_directories(${ROOT_PROJECT_DIR}/lib)

# find src files
set(ECHO_SERVER_SRC_FILES ${PROJECT_SOURCE_DIR}/echo_server.cc)
set(ECHO_CLIENT_SRC_FILES ${PROJECT_SOURCE_DIR}/echo_client.cc)

# generate exeutable
add_executable(echo-server ${ECHO_SERVER_SRC_FILES})
target_link_libraries(echo-server xsix ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(echo-server xsix)

add_executable(echo-client ${ECHO_CLIENT_SRC_FILES})
target_link_libraries(echo-client xsix ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(echo-client xsix)