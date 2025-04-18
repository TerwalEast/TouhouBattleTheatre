# CMakeList.txt: TouhouBattleTheatre 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable (TouhouBattleTheatre "TouhouBattleTheatre.cpp" "TouhouBattleTheatre.h"
"src/lib/stb_image.cpp" "src/lib/stb_truetype.cpp" 
"src/Terrain.h" "src/Terrain.cpp" "src/TestApplication.cpp" "src/TestApplication.h" "src/Touhou.h" "src/ShaderManager.h" "src/ShaderManager.cpp" "src/MapTool.h" "src/MapTool.cpp" "src/Stage.h" "src/Event.h" "src/Scenario.h" "src/TextureManager.h" 
 "src/camera/Camera.h" "src/camera/RTSCameraController.h" "src/camera/DebugCameraController.h" "src/camera/OrthoCameraController.h"
"src/world/WorldMap.h" "src/world/WorldMap.cpp" "src/world/TileMap.h" "src/world/TileMap.cpp" "src/world/Actor.h" "src/world/Explorer.h" "src/world/Explorer.cpp" "src/world/Roamer.h" "src/world/Cursor.h" "src/world/Cursor.cpp" "src/world/WorldUI.h" "src/world/WorldUI.cpp" "src/world/WorldUIItem.h" "src/world/WorldUIItem.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TouhouBattleTheatre PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
target_link_libraries(TouhouBattleTheatre PRIVATE SDL3::SDL3)
find_path(CUTE_HEADERS_INCLUDE_DIRS "cute_alloc.h")
target_include_directories(TouhouBattleTheatre PRIVATE ${CUTE_HEADERS_INCLUDE_DIRS})
find_package(glad CONFIG REQUIRED)
target_link_libraries(TouhouBattleTheatre PRIVATE glad::glad)
find_package(glm CONFIG REQUIRED)
target_link_libraries(TouhouBattleTheatre PRIVATE glm::glm)
find_package(OpenGL REQUIRED)
target_link_libraries(TouhouBattleTheatre PRIVATE OpenGL::GL)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(TouhouBattleTheatre PRIVATE spdlog::spdlog)
find_package(Stb REQUIRED)
target_include_directories(TouhouBattleTheatre PRIVATE ${Stb_INCLUDE_DIR})

if (WIN32)
    add_custom_command(
        TARGET TouhouBattleTheatre POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:TouhouBattleTheatre>"
        VERBATIM
    )
endif()



add_custom_command(
        TARGET TouhouBattleTheatre POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/TouhouBattleTheatre/res
                ${CMAKE_CURRENT_BINARY_DIR}/res)




