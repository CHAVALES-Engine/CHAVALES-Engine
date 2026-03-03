if (NOT EXISTS "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/install_manifest.txt\"")
endif()

file(READ "C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/dependencies/SDLproj/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND C:/Users/andres/Documents/CLASE/cmake-4.2.3-windows-x86_64/bin/cmake.exe -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach()
