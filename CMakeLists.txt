cmake_minimum_required(VERSION 3.5)

project(ProjectEverDriven-CompileKit LANGUAGES C CXX)

add_executable(ProjectEverDriven-CompileKit main.c DLLTools.c DLLTools.h ${SRCS} icon.rc)
#MoldLoader.c MoldLoader.h

install(TARGETS ProjectEverDriven-CompileKit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(ProjectEverDriven-CompileKit PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

set_target_properties(ProjectEverDriven-CompileKit PROPERTIES OUTPUT_NAME "PED-CompileKit")
