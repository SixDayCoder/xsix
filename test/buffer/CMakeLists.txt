# project name
project(test-buffer)

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
set(BUFFER_TEST_SRC_FILTES ${PROJECT_SOURCE_DIR}/buffer_test.cc)

# generate exeutable
add_executable(test-buffer ${BUFFER_TEST_SRC_FILTES})
target_link_libraries(test-buffer xsix ${CMAKE_THREAD_LIBS_INIT})