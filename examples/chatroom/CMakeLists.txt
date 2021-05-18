# project name
project(chatroom)

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
link_directories(${ROOT_PROJECT_DIR}/bin)

# find src files
set(CHAT_ROOM_CLIENT_SRC_FILTES
    ${PROJECT_SOURCE_DIR}/chat_room_msg.hpp
    ${PROJECT_SOURCE_DIR}/chat_room_client.cc
)

set(CHAT_ROOM_SERVER_SRC_FILTES 
    ${PROJECT_SOURCE_DIR}/chat_room_msg.hpp
    ${PROJECT_SOURCE_DIR}/chat_room_server.cc
)

# generate exeutable
add_executable(chatroom-client ${CHAT_ROOM_CLIENT_SRC_FILTES})
target_link_libraries(chatroom-client xsix ${CMAKE_THREAD_LIBS_INIT})

add_executable(chatroom-server ${CHAT_ROOM_SERVER_SRC_FILTES})
target_link_libraries(chatroom-server xsix ${CMAKE_THREAD_LIBS_INIT})