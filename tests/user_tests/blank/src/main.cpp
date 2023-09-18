#include <iostream>

#include <spdlog/spdlog.h>

int main(int, char**) {
    spdlog::info("Hello, {}!", "World");
}
