cmake_minimum_required(VERSION 3.0)
project(RedSquare
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Boost REQUIRED COMPONENTS locale)

add_executable(RedSquare
  code/client/main.cc
  # and the rest of your sources
)
target_link_libraries(RedSquare
  gf::gf0
)
