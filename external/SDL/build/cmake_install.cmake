# Install script for directory: F:/GameProjs/Snapshot/External/SDL

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/sdl3.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/RelWithDebInfo/SDL3.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/Debug/SDL3.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/Release/SDL3.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/MinSizeRel/SDL3.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/RelWithDebInfo/SDL3.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/Debug/SDL3.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/Release/SDL3.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/MinSizeRel/SDL3.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/RelWithDebInfo/SDL3.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/Debug/SDL3.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/Release/SDL3.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/MinSizeRel/SDL3.pdb")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/RelWithDebInfo/SDL3_test.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/Debug/SDL3_test.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/Release/SDL3_test.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GameProjs/Snapshot/External/SDL/build/MinSizeRel/SDL3_test.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "F:/GameProjs/Snapshot/External/SDL/build/${CMAKE_INSTALL_CONFIG_NAME}/SDL3_test.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3headersTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3headersTargets.cmake"
         "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3headersTargets.cmake")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3headersTargets.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3sharedTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3sharedTargets.cmake"
         "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3sharedTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3sharedTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3sharedTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3sharedTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3sharedTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3sharedTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3sharedTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3sharedTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3testTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL3testTargets.cmake"
         "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets.cmake")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/build/CMakeFiles/Export/272ceadb8458515b2ae4b5630a6029cc/SDL3testTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "F:/GameProjs/Snapshot/External/SDL/build/SDL3Config.cmake"
    "F:/GameProjs/Snapshot/External/SDL/build/SDL3ConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL3" TYPE FILE FILES
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_assert.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_asyncio.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_atomic.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_audio.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_begin_code.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_bits.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_blendmode.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_camera.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_clipboard.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_close_code.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_copying.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_cpuinfo.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_dialog.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_egl.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_endian.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_error.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_events.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_filesystem.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_gamepad.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_gpu.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_guid.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_haptic.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_hidapi.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_hints.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_init.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_intrin.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_iostream.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_joystick.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_keyboard.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_keycode.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_loadso.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_locale.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_log.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_main.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_main_impl.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_messagebox.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_metal.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_misc.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_mouse.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_mutex.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_oldnames.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengl.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengl_glext.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengles.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengles2.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengles2_gl2.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengles2_gl2ext.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengles2_gl2platform.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_opengles2_khrplatform.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_pen.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_pixels.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_platform.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_platform_defines.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_power.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_process.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_properties.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_rect.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_render.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_scancode.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_sensor.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_stdinc.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_storage.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_surface.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_system.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_thread.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_time.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_timer.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_touch.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_tray.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_version.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_video.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_vulkan.h"
    "F:/GameProjs/Snapshot/External/SDL/build/include-revision/SDL3/SDL_revision.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL3" TYPE FILE FILES
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_assert.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_common.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_compare.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_crc32.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_font.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_fuzzer.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_harness.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_log.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_md5.h"
    "F:/GameProjs/Snapshot/External/SDL/include/SDL3/SDL_test_memory.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/licenses/SDL3" TYPE FILE FILES "F:/GameProjs/Snapshot/External/SDL/LICENSE.txt")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "F:/GameProjs/Snapshot/External/SDL/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
