# Install script for directory: C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/pkgs/recastnavigation_x64-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/recastnavigation" TYPE FILE FILES "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/version.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/recastnavigation.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation/recastnavigation-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation/recastnavigation-targets.cmake"
         "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/CMakeFiles/Export/4924bdfd16b98d877c245a9bd0ffe4a4/recastnavigation-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation/recastnavigation-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation/recastnavigation-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation" TYPE FILE FILES "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/CMakeFiles/Export/4924bdfd16b98d877c245a9bd0ffe4a4/recastnavigation-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation" TYPE FILE FILES "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/CMakeFiles/Export/4924bdfd16b98d877c245a9bd0ffe4a4/recastnavigation-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/recastnavigation" TYPE FILE FILES
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/recastnavigation-config.cmake"
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/recastnavigation-config-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/DebugUtils/cmake_install.cmake")
  include("C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/Detour/cmake_install.cmake")
  include("C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/DetourCrowd/cmake_install.cmake")
  include("C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/DetourTileCache/cmake_install.cmake")
  include("C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/Recast/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-rel/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
