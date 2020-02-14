cmake_minimum_required(VERSION 3.0)
project(RedSquare
  VERSION 0.9.0
)

find_package(gf REQUIRED)
find_package(gfnet REQUIRED)

set(REDSQUARE_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/redsquare")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT MSVC)
  add_definitions(-Wall -Wextra -g -pedantic)
endif()

add_executable(RedSquare
  code/client/main.cc
  code/client/ClientNetwork.cc
  code/client/Scenes.cc
  code/client/MainMenuScene.cc
  code/client/ConnectionScene.cc
  code/client/LobbyScene.cc
  code/client/RoomScene.cc
  code/client/GameScene.cc
  code/client/World.cc
  code/client/Entities.cc
  code/client/ClientEntity.cc
  code/client/Player.cc
  code/client/Monster.cc
  code/client/Prop.cc
  code/client/ItemHolder.cc
  code/client/Inventory.cc
  code/client/InventoryUI.cc
  code/client/InventoryWidget.cc
  code/client/InventorySlot.cc
  code/client/ClientItem.cc
  code/client/Hud.cc
  code/client/SpellWidget.cc

  code/common/Entity.cc
  code/common/Item.cc
  code/common/Singletons.cc
  
  vendor/gf-imgui/imgui/imgui.cpp
  vendor/gf-imgui/imgui/imgui_demo.cpp
  vendor/gf-imgui/imgui/imgui_draw.cpp
  vendor/gf-imgui/imgui/imgui_widgets.cpp
  vendor/gf-imgui/imgui_impl_gf.cc
)

target_link_libraries(RedSquare
  gf::gf0
  gf::gfnet0
  Threads::Threads
)

target_include_directories(RedSquare
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui/imgui
)

install(
  TARGETS RedSquare
  RUNTIME DESTINATION games
)

add_executable(RedSquare-Server
  code/server/main.cc
  code/server/GameFactory.cc
  code/server/GameInstance.cc
  code/server/RedsquareFactory.cc
  code/server/ServerGroup.cc
  code/server/ServerLobby.cc
  code/server/ServerNetwork.cc
  code/server/ServerPlayer.cc
  code/server/ServerRoom.cc
  code/server/RedsquareInstance.cc
  code/server/World.cc
  code/server/ServerEntity.cc
  code/server/Player.cc
  code/server/Monster.cc
  code/server/Prop.cc
  code/server/ItemHolder.cc
  code/server/ServerItem.cc
  code/server/Inventory.cc

  code/common/Entity.cc
  code/common/Item.cc
  code/common/Singletons.cc
)

target_link_libraries(RedSquare-Server
  gf::gf0
  gf::gfnet0
  Threads::Threads
)

target_include_directories(RedSquare-Server
  PRIVATE
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
