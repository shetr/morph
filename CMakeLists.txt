cmake_minimum_required(VERSION 3.11...3.16)

project(Morph
    VERSION 0.1
    DESCRIPTION "Morph game engine"
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)

    find_package(Doxygen)
    if(Doxygen_FOUND)
	add_subdirectory(docs)
    else()
	message(STATUS "Doxygen not found, not building docs")
    endif()

    add_compile_definitions(
        $<$<CONFIG:Debug>:MORPH_DEBUG>
        $<$<CONFIG:Release>:MORPH_RELEASE>
    )
    
    if(ENABLE_DEPLOY)
        add_compile_definitions(MORPH_DEPLOY)
        add_compile_definitions(MORPH_ENGINE_RES="res/engine/")
        add_compile_definitions(MORPH_APP_RES="res/app/")
    else()
        add_compile_definitions(MORPH_ENGINE_RES="${CMAKE_CURRENT_SOURCE_DIR}/engine/res/")
    endif()
    
    if(ENABLE_PROFILING)
        add_compile_definitions(MORPH_PROFILE)
    endif()
    
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        add_compile_definitions(MORPH_WINDOWS)
    endif()
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        add_compile_definitions(MORPH_LINUX)
    endif()
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
        add_compile_definitions(MORPH_APPLE)
    endif()

endif()

add_subdirectory(engine)

add_subdirectory(apps)

add_subdirectory(tests/unit_tests)
add_subdirectory(tests/user_tests)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
add_subdirectory(extern/googletest)
#add_subdirectory(extern/assimp)
add_subdirectory(extern/fmt)
add_subdirectory(extern/spdlog)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)