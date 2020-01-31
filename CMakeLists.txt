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
  code/client/World.cc
  code/client/Monster.cc
  code/client/Chat.cc
  code/client/Prop.cc
  code/client/Hud.cc
  code/client/InventoryUI.cc
  code/client/InventoryWidget.cc
  code/client/InventorySlot.cc
  code/client/MainMenu.cc
  code/client/ClientItem.cc
  code/client/SpellWidget.cc
  code/client/ClientEntity.cc
  code/client/Inventory.cc
  code/client/ItemHolder.cc
  code/common/Sockets.cc
  code/common/Entity.cc
  code/common/Singletons.cc
  code/common/Item.cc

  vendor/gf-imgui/imgui/imgui.cpp
  vendor/gf-imgui/imgui/imgui_demo.cpp
  vendor/gf-imgui/imgui/imgui_draw.cpp
  vendor/gf-imgui/imgui/imgui_widgets.cpp
  vendor/gf-imgui/imgui_impl_gf.cc
)

target_link_libraries(RedSquare
  gf::gf0
  Threads::Threads
)

target_include_directories(RedSquare
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui/imgui
)

target_compile_features(RedSquare
  PUBLIC
    cxx_std_17
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
  code/server/Inventory.cc
  code/server/ServerItem.cc
  code/server/ItemHolder.cc
  code/common/Sockets.cc
  code/common/Singletons.cc
  code/common/Entity.cc
  code/common/Item.cc
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

target_compile_features(RedSquare-Server
  PUBLIC
    cxx_std_17
)

install(
  TARGETS RedSquare-Server
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/redsquare"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/games"
)
