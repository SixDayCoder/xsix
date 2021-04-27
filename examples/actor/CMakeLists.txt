# project name
project(actor)

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
set(ACTOR_TEST_SRC_FILTES ${PROJECT_SOURCE_DIR}/actor.cc)

# generate exeutable
add_executable(actor-test ${ACTOR_TEST_SRC_FILTES})
target_link_libraries(actor-test xsix ${CMAKE_THREAD_LIBS_INIT})