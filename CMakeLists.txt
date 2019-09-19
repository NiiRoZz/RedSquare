cmake_minimum_required(VERSION 3.0)
project(RedSquare
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Boost REQUIRED COMPONENTS locale)

add_executable(RedSquare
  code/client/main.cc
  code/client/Square.cc
  code/client/Game.cc
  code/common/World.cc
  code/common/Sockets.cc
  code/common/ThreadCom.cc
)

target_link_libraries(RedSquare
  gf::gf0
)

add_executable(RedSquare-Server
  code/server/main.cc
  code/server/Square.cc
  code/server/Game.cc
  code/common/World.cc
  code/common/Sockets.cc
  code/common/ThreadCom.cc
)

target_link_libraries(RedSquare-Server
  gf::gf0
)
