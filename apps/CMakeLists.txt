
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
        "../engine/src"
        "../extern/glm"
        "../extern/fmt/include"
        "../extern/spdlog/include"
    )
    add_executable(${subdir} ${${subdir}_SRC})
    target_include_directories(${subdir} PUBLIC ${${subdir}_INCLUDE})
    target_link_libraries(${subdir} PRIVATE glm)
    target_link_libraries(${subdir} PRIVATE morph_engine)
    target_link_libraries(${subdir} PRIVATE fmt)
    target_link_libraries(${subdir} PRIVATE spdlog)

    if(ENABLE_DEPLOY)
      add_custom_command(TARGET ${subdir} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${subdir}>/${subdir}_deploy)

      add_custom_command(TARGET ${subdir} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${subdir}> $<TARGET_FILE_DIR:${subdir}>/${subdir}_deploy/)

      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/res/)
        add_custom_command(TARGET ${subdir} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/res/ $<TARGET_FILE_DIR:${subdir}>/${subdir}_deploy/res/app/)
      endif()

      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../engine/res/)
        add_custom_command(TARGET ${subdir} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/../engine/res/ $<TARGET_FILE_DIR:${subdir}>/${subdir}_deploy/res/engine/)
      endif()
    else()
      target_compile_definitions(${subdir} PUBLIC MORPH_APP_RES="${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/res/")
    endif()
ENDFOREACH()

