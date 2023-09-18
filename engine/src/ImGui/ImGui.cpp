#include "ImGui.hpp"

#include <Extern/imgui/imgui.h>
#include <Extern/imgui/imgui_impl_glfw.h>
#include <Extern/imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Morph {


ImGuiDraw::ImGuiDraw(const Window& window) : m_window(&window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
ImGuiDraw::ImGuiDraw(ImGuiDraw&& draw)
{
    std::swap(m_window, draw.m_window);
}
ImGuiDraw& ImGuiDraw::operator=(ImGuiDraw&& draw)
{
    std::swap(m_window, draw.m_window);
    return *this;
}
ImGuiDraw::~ImGuiDraw()
{
    ImGuiIO& io = ImGui::GetIO();
    uvec2 winSize = m_window->GetSize();
    io.DisplaySize = ImVec2(winSize.x, winSize.y);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

ImGuiContext::ImGuiContext(const Window& window) : m_window(&window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    //style.Alpha = 0.9f;

    ImGui_ImplGlfw_InitForOpenGL(window.id(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

ImGuiContext::ImGuiContext(ImGuiContext&& context)
{
    std::swap(m_window, context.m_window);
}
ImGuiContext& ImGuiContext::operator=(ImGuiContext&& context)
{
    std::swap(m_window, context.m_window);
    return *this;
}

ImGuiContext::~ImGuiContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiDraw ImGuiContext::Draw()
{
    return ImGuiDraw(*m_window);
}

}