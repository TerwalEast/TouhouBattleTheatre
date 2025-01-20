# CMakeList.txt: TouhouBattleTheatre 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable (TouhouBattleTheatre "TouhouBattleTheatre.cpp" "TouhouBattleTheatre.h" "src/Terrain.h" "src/Terrain.cpp" "src/TestApplication.cpp" "src/TestApplication.h" "src/Touhou.h" "src/ShaderManager.h" "src/ShaderManager.cpp" "src/Camera.h" "src/RTSCameraController.h" "src/MapTool.h" "src/DebugCameraController.h" "src/MapTool.cpp" "src/TileMap.h" "src/TileMap.cpp" "src/Stage.h" "src/WorldMap.h" "src/WorldMap.cpp" "src/Event.h" "src/OrthoCameraController.h" "src/Scenario.h")

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
find_package(recastnavigation CONFIG REQUIRED)
target_link_libraries(TouhouBattleTheatre PRIVATE RecastNavigation::Detour RecastNavigation::Recast RecastNavigation::DebugUtils RecastNavigation::DetourCrowd)
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


