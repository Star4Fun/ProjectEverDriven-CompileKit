cmake_minimum_required(VERSION 3.5)

project(ProjectEverDriven-CompileKit LANGUAGES C CXX)

set(PATH C:\\Users\\David\\QtProjects\\build-ProjectEverDriven-CompileKit-Desktop_32_Bit-Debug;C:\\Users\\David\\QtProjects\\build-ProjectEverDriven-CompileKit-Desktop_32_Bit-Debug\\Wars;%PATH%)

#find_library(DLL1_LIBRARY NAMES dll1 PATHS C:\\Users\\David\\QtProjects\\build-ProjectEverDriven-CompileKit-Desktop_32_Bit-Debug\\Wars)
#find_library(DLL2_LIBRARY NAMES dll2 PATHS C:\\Users\\David\\QtProjects\\build-ProjectEverDriven-CompileKit-Desktop_32_Bit-Debug)

add_executable(ProjectEverDriven-CompileKit main.c DLLTools.c DLLTools.h ${SRCS} icon.rc)
#MoldLoader.c MoldLoader.h

install(TARGETS ProjectEverDriven-CompileKit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(ProjectEverDriven-CompileKit PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

set_target_properties(ProjectEverDriven-CompileKit PROPERTIES OUTPUT_NAME "PED-CompileKit")

#target_link_libraries(ProjectEverDriven-CompileKit PRIVATE ${DLL1_LIBRARY} ${DLL2_LIBRARY})

#target_link_directories(ProjectEverDriven-CompileKit PRIVATE "C:\\Users\\David\\Desktop\\Echidna\\hsp351")s
