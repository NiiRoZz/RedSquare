cmake_minimum_required(VERSION 3.0)
project(Projet_L3
  VERSION 0.1.0
)
find_package(gf REQUIRED)
add_executable(Projet_L3
  code/client/main.cc
  # and the rest of your sources
)
target_link_libraries(Projet_L3
  gf::gf0
)
