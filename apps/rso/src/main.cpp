#include "App.hpp"

int main(int, char**) {
    using namespace Morph;
    WindowAppConfig appConfig = {
        ivec2(600,600),
        "rso",
        ExecutionTypes::VsyncFrames()
    };
    Application app(appConfig);
    app.Run();
}