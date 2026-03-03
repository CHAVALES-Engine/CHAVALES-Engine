# Install script for directory: C:/Users/andres/Documents/CLASE/P3/sdl/SDL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SDL3")
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
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/sdl3.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/RelWithDebInfo/SDL3-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/Debug/SDL3-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/Release/SDL3-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/MinSizeRel/SDL3-static.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/${CMAKE_INSTALL_CONFIG_NAME}/SDL3-static.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/RelWithDebInfo/SDL3_test.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/Debug/SDL3_test.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/Release/SDL3_test.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/MinSizeRel/SDL3_test.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/${CMAKE_INSTALL_CONFIG_NAME}/SDL3_test.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3headersTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3headersTargets.cmake"
         "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3headersTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3headersTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3headersTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3headersTargets.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3staticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3staticTargets.cmake"
         "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3staticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3staticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3staticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3staticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3staticTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3staticTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3staticTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3staticTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3testTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3testTargets.cmake"
         "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3testTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3testTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/SDL3Config.cmake"
    "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/SDL3ConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL3" TYPE FILE FILES
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_assert.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_asyncio.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_atomic.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_audio.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_begin_code.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_bits.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_blendmode.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_camera.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_clipboard.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_close_code.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_copying.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_cpuinfo.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_dialog.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_dlopennote.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_egl.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_endian.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_error.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_events.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_filesystem.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_gamepad.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_gpu.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_guid.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_haptic.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_hidapi.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_hints.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_init.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_intrin.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_iostream.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_joystick.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_keyboard.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_keycode.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_loadso.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_locale.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_log.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_main.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_main_impl.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_messagebox.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_metal.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_misc.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_mouse.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_mutex.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_oldnames.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengl.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengl_glext.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengles.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengles2.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengles2_gl2.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengles2_gl2ext.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengles2_gl2platform.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_opengles2_khrplatform.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_openxr.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_pen.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_pixels.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_platform.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_platform_defines.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_power.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_process.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_properties.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_rect.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_render.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_scancode.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_sensor.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_stdinc.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_storage.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_surface.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_system.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_thread.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_time.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_timer.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_touch.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_tray.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_version.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_video.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_vulkan.h"
    "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/include-revision/SDL3/SDL_revision.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL3" TYPE FILE FILES
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_assert.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_common.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_compare.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_crc32.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_font.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_fuzzer.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_harness.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_log.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_md5.h"
    "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/include/SDL3/SDL_test_memory.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/licenses/SDL3" TYPE FILE FILES "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/LICENSE.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/test/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
