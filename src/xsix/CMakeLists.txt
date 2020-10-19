# project name
project(xsix)

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
include_directories(${ROOT_PROJECT_DIR}/src)

# add link dirs
link_directories(/usr/local/lib)

# find src files
# aux_source_directory(${ROOT_PROJECT_DIR}/src LIBXSIX_SRC_FILES)
set(SRC_FILES 
	${ROOT_PROJECT_DIR}/src/xsix/bits.hpp
	${ROOT_PROJECT_DIR}/src/xsix/block_queue.hpp
	${ROOT_PROJECT_DIR}/src/xsix/buffer.cc
	${ROOT_PROJECT_DIR}/src/xsix/utils/file_utils.cc
	${ROOT_PROJECT_DIR}/src/xsix/utils/math_utils.cc
	${ROOT_PROJECT_DIR}/src/xsix/utils/string_utils.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/acceptor.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/channel.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/eventloop.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/network.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/network_socket_api.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/poller.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/poller_select.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/tcp_conn.cc
	${ROOT_PROJECT_DIR}/src/xsix/network/tcp_server.cc
)

# generate static library
add_library(xsix STATIC ${SRC_FILES})