cmake_minimum_required(VERSION 3.0)
project(RedSquare
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

set(REDSQUARE_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/redsquare")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(RedSquare
  code/client/main.cc
  code/client/Player.cc
  code/client/Game.cc
  code/client/ThreadCom.cc
  code/client/World.cc
  code/client/Monster.cc
  code/client/Chat.cc
  code/client/Prop.cc
  code/client/Hud.cc
  code/client/Inventory.cc
  code/common/Sockets.cc
  code/common/Entity.cc
  code/common/Singletons.cc

)

target_link_libraries(RedSquare
  gf::gf0
  Threads::Threads
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
  code/server/Chat.cc
  code/server/ServerEntity.cc
  code/server/Prop.cc
  code/common/Sockets.cc
  code/common/Singletons.cc
  code/common/Entity.cc
)

target_link_libraries(RedSquare-Server
  gf::gf0
  ${Boost_LIBRARIES}
  Threads::Threads
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

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/redsquare"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/games"
)
