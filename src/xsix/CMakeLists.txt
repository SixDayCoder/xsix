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
include_directories(${THIRD_ASIO_DIR}/include)

# add link dirs
link_directories(/usr/local/lib)

# find src files
# aux_source_directory(${ROOT_PROJECT_DIR}/src LIBXSIX_SRC_FILES)
set(SRC_FILES 
	${ROOT_PROJECT_DIR}/src/xsix/bits.hpp
	${ROOT_PROJECT_DIR}/src/xsix/block_queue.hpp
	${ROOT_PROJECT_DIR}/src/xsix/buffer.hpp
	${ROOT_PROJECT_DIR}/src/xsix/fmt.hpp

	${ROOT_PROJECT_DIR}/src/xsix/log/logger.cc
	${ROOT_PROJECT_DIR}/src/xsix/log/log_define.cc

	${ROOT_PROJECT_DIR}/src/xsix/network/tcp_client.hpp
	${ROOT_PROJECT_DIR}/src/xsix/network/tcp_connection.hpp
	${ROOT_PROJECT_DIR}/src/xsix/network/tcp_server.hpp

	${ROOT_PROJECT_DIR}/src/xsix/utils/math_utils.hpp
	${ROOT_PROJECT_DIR}/src/xsix/utils/string_utils.hpp

	${ROOT_PROJECT_DIR}/src/xsix/actor/actor_base.cc
	${ROOT_PROJECT_DIR}/src/xsix/actor/actor_core.cc
)

# generate static library
add_library(xsix STATIC ${SRC_FILES})