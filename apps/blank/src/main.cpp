#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Window/WindowManager.hpp>

namespace Morph {

void custom_main() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::trace("Hello, {}! \n ha \n", "World");
    spdlog::debug("Hello, {}!", "World");
    spdlog::info("Hello, {}!", "World");
    spdlog::warn("Hello, {}!", "World");
    spdlog::error("Hello, {}!", "World");

    glm::vec3 v = {0, 1, 2};
    spdlog::info("Vector: {}", v);

    ref<vec3> vRef = v;
    spdlog::info("Vector ref: {}", vRef.get());

    {
        unique<WindowManager> winManager = unique<WindowManager>(WindowManager::Get());
    }

    panic("Aaaaah");
    spdlog::critical("Hello, {}!", "World");

}
    
}

int main(int, char**) {
    Morph::custom_main();
    return EXIT_SUCCESS;
}