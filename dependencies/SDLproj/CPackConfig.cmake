# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "C:/Users/andres/Documents/CLASE/P3/sdl/SDL;C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj")
set(CPACK_CMAKE_GENERATOR "Visual Studio 17 2022")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Users/andres/Documents/CLASE/cmake-4.2.3-windows-x86_64/share/cmake-4.2/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "SDL3 built using CMake")
set(CPACK_GENERATOR "ZIP")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALL_CMAKE_PROJECTS "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj;SDL3;ALL;/")
set(CPACK_INSTALL_PREFIX "C:/Program Files (x86)/SDL3")
set(CPACK_MODULE_PATH "C:/Users/andres/Documents/CLASE/P3/sdl/SDL/cmake")
set(CPACK_NSIS_DISPLAY_NAME "SDL3 3.5.0")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "SDL3 3.5.0")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/Users/andres/Documents/CLASE/cmake-4.2.3-windows-x86_64/share/cmake-4.2/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SDL3 built using CMake")
set(CPACK_PACKAGE_DIRECTORY "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/dist")
set(CPACK_PACKAGE_FILE_NAME "SDL3-3.5.0-win64")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SDL3 3.5.0")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "SDL3 3.5.0")
set(CPACK_PACKAGE_NAME "SDL3")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "3.5.0")
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "5")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PROJECT_CONFIG_FILE "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CPackProjectConfig.cmake")
set(CPACK_RESOURCE_FILE_LICENSE "C:/Users/andres/Documents/CLASE/cmake-4.2.3-windows-x86_64/share/cmake-4.2/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "C:/Users/andres/Documents/CLASE/cmake-4.2.3-windows-x86_64/share/cmake-4.2/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Users/andres/Documents/CLASE/cmake-4.2.3-windows-x86_64/share/cmake-4.2/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "OFF")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "SDL3-3.5.0-src")
set(CPACK_SOURCE_ZIP "OFF")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
