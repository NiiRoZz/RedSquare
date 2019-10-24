cmake_minimum_required(VERSION 3.0)
project(RedSquare
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

add_executable(RedSquare
  code/client/main.cc
  code/client/Player.cc
  code/client/Game.cc
  code/client/ThreadCom.cc
  code/client/World.cc
  code/client/Monster.cc
  code/client/Chat.cc
  code/common/Sockets.cc
  code/common/Entity.cc
)

target_link_libraries(RedSquare
  gf::gf0
)

target_include_directories(RedSquare
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

install(
  TARGETS RedSquare
  RUNTIME DESTINATION games
)

add_executable(RedSquare-Server
  code/server/main.cc
  code/server/Player.cc
  code/server/Game.cc
  code/server/World.cc
  code/server/Monster.cc
  code/common/Sockets.cc
  code/common/Singletons.cc
  code/common/Entity.cc
)

target_link_libraries(RedSquare-Server
  gf::gf0
  ${Boost_LIBRARIES}
)

target_include_directories(RedSquare-Server
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

install(
  TARGETS RedSquare-Server
  RUNTIME DESTINATION games
)
