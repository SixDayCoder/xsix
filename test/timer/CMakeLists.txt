# project name
project(test-timer)

# platform differences
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        add_definitions(-D _XSIX_LINUX)
        find_package (Threads)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(WIN32)
        add_definitions(-D _XSIX_WINDOWS)
endif(WIN32)

# add include dirs
include_directories(${ROOT_PROJECT_DIR}/src)
include_directories(${THIRD_ASIO_DIR}/include)

# add link dirs
link_directories(/usr/local/lib)
link_directories(${ROOT_PROJECT_DIR}/lib)

# find src files
set(TIMER_TEST_SRC_FILTES ${PROJECT_SOURCE_DIR}/timer_test.cc)

# generate exeutable
add_executable(test-timer ${TIMER_TEST_SRC_FILTES})
target_link_libraries(test-timer xsix ${CMAKE_THREAD_LIBS_INIT})