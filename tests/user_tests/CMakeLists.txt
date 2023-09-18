
MACRO(SUBDIRLIST result curdir)
FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
SET(dirlist "")
FOREACH(child ${children})
  IF(IS_DIRECTORY ${curdir}/${child})
    LIST(APPEND dirlist ${child})
  ENDIF()
ENDFOREACH()
SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})
    file(GLOB_RECURSE ${subdir}_SRC
        "${subdir}/src/**.cpp"
    )
    set(${subdir}_INCLUDE 
        "src" 
        "../../engine/src"
        "../../extern/glm"
        "../../extern/fmt/include"
        "../../extern/spdlog/include"
    )
    add_executable(user_${subdir} ${${subdir}_SRC})
    target_include_directories(user_${subdir} PUBLIC ${${subdir}_INCLUDE})
    target_compile_definitions(user_${subdir} PUBLIC CUSTOM_DEFINE="set from cmake: ${subdir}")
    target_compile_definitions(user_${subdir} PUBLIC MORPH_APP_RES="${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/res/")
    target_link_libraries(user_${subdir} PRIVATE glm)
    target_link_libraries(user_${subdir} PRIVATE morph_engine)
    target_link_libraries(user_${subdir} PRIVATE fmt)
    target_link_libraries(user_${subdir} PRIVATE spdlog)
ENDFOREACH()

