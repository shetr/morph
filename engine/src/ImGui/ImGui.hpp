
#ifndef MORPH_IMGUI_HPP
#define MORPH_IMGUI_HPP

#include <Core/Core.hpp>
#include <Window/Window.hpp>
#include <Extern/imgui/imgui.h>

namespace Morph {

class ImGuiContext;

class ImGuiDraw
{
    friend class ImGuiContext;
protected:
    const Window* m_window = nullptr;
protected:
    ImGuiDraw(const Window& window);
public:
    ImGuiDraw() {}
    ImGuiDraw(const ImGuiDraw& draw) = delete;
    ImGuiDraw(ImGuiDraw&& draw);
    ImGuiDraw& operator=(ImGuiDraw&& draw);
    virtual ~ImGuiDraw();
};

class ImGuiContext
{
protected:
    const Window* m_window = nullptr;
public:
    ImGuiContext(const Window& window);
    ImGuiContext() {}
    ImGuiContext(const ImGuiContext& context) = delete;
    ImGuiContext(ImGuiContext&& context);
    ImGuiContext& operator=(ImGuiContext&& context);
    virtual ~ImGuiContext();
    ImGuiDraw Draw();
};

}

#endif // MORPH_IMGUI_HPP