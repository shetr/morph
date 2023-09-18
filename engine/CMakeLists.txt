
file(GLOB_RECURSE ENGINE_SRC
    "src/**.hpp"
    "src/**.cpp"
)

set(ENGINE_INCLUDE 
    "src" 
    "../extern/glm" 
    "../extern/glfw/include"
    "../extern/glad/include"
    #"../extern/assimp/include"
    "../extern/stb"
    "../extern/fmt/include"
    "../extern/spdlog/include"
)

add_library(morph_engine STATIC ${ENGINE_SRC})

target_include_directories(morph_engine PUBLIC ${ENGINE_INCLUDE})

#target_precompile_headers(morph_engine PUBLIC src/Types.hpp)

target_link_libraries(morph_engine PUBLIC glm)
target_link_libraries(morph_engine PUBLIC glfw)
target_link_libraries(morph_engine PUBLIC glad)
#target_link_libraries(morph_engine PUBLIC assimp)
target_link_libraries(morph_engine PUBLIC fmt)
target_link_libraries(morph_engine PUBLIC spdlog)

