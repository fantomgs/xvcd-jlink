cmake_minimum_required(VERSION 3.6)

project(test_jlink)

set(SOURCE_FILES
    JlinkUtil.c
    test.c)

add_executable(test_jlink ${SOURCE_FILES})

#target_include_directories(xvcd_jlink PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/
#        )

#link_directories(${CMAKE_CURRENT_SOURCE_DIR})

#find_library(JLINKARM_LIBRARY JLinkARM.dll)

#if (JLINKARM_LIBRARY)
#    target_link_libraries(xvcd_jlink ${JLINKARM_LIBRARY})
#endif ()
#target_link_libraries(xvcd_jlink JLinkARM.dll)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_jlink ${CMAKE_CURRENT_SOURCE_DIR}/JLinkARM.dll)


project(xvcd_jlink)

set(SOURCE_FILES
        JlinkUtil.c
        io_jlink.c
        xvcd.c)

add_executable(xvcd_jlink ${SOURCE_FILES})

target_link_libraries(xvcd_jlink ${CMAKE_CURRENT_SOURCE_DIR}/JLinkARM.dll libws2_32.dll)