#include "WindowApp.hpp"

#include <thread>

namespace Morph {

WindowApp::WindowApp(const WindowAppConfig& config)
    :
    m_executionType(config.executionType),
    m_windowManager(WindowManager::Get()),
    m_windowManagerErrorAttacher{m_windowManager.get(), this, &WindowApp::OnWindowManagerError},
    m_windowID(value_or_panic(
        m_windowManager->CreateWindow(config.size, config.title, config.hints), 
        "failed to create window"
    )),
    m_window(value_or_panic(
        m_windowManager->GetWindow(m_windowID),
        "window unexpectedly destroyed"
    )),
    m_context(value_or_panic(GraphicsContext::MakeCurrent(window()))),
    d_updateExecutionType(MORPH_VOID(UpdateExecutionType()))
{
}

void WindowApp::Run()
{
    DefaultFramebuffer& defaultFramebuffer = context().GetDefaultFramebuffer();

    while(!window().ShouldClose() && !m_shouldClose)
    {
        Timer lastIterTimer(m_lastIterTime);
        if(m_numberOfFramesExecuted == 1) {
            m_firstFrameRunTime = m_lastIterTime.GetSeconds();
        }
        m_runTime += m_lastIterTime.GetSeconds();

        {
            Timer lastFrameTimer(m_lastFrameTime);
            RunIter(m_lastIterTime.GetSeconds(), m_lastFrameTime.GetSeconds());
        }

        defaultFramebuffer.SwapBuffers();

        m_numberOfFramesExecuted++;

        match(m_executionType,
            [&](const ExecutionTypes::RunOnce& et) {
                m_shouldClose = true;
                windowManager().PollEvents();
            },
            [&](const ExecutionTypes::RunNumberOfFrames& runNumberOfFrames) {
                if(m_numberOfFramesExecuted >= runNumberOfFrames.numberOfFrames.get()) {
                    m_shouldClose = true;
                }
                windowManager().PollEvents();
            },
            [&](const ExecutionTypes::UnlimitedFrames& et) {
                windowManager().PollEvents();
            },
            [&](const ExecutionTypes::VsyncFrames& vsyncFrames) {
                windowManager().PollEvents();
            },
            [&](const ExecutionTypes::AdaptiveVsyncFrames& adaptiveVsyncFrames) {
                if(m_runTime - m_firstFrameRunTime > 1) {
                    Monitor& monitor = value_or_panic(
                        windowManager().GetPrimaryMonitor(),
                        "failed to get primary monitor information"
                    );
                    f64 expectedFPS = monitor.GetVideoMode().refreshRate
                        / adaptiveVsyncFrames.screenUpdatesCount;
                    f64 toleranceFPS = expectedFPS / 4.0;
                    f64 actualFPS = (m_numberOfFramesExecuted - 1) / (m_runTime - m_firstFrameRunTime);
                    if(actualFPS > expectedFPS + toleranceFPS) {
                        m_executionType = ExecutionTypes::LimitedFrames(expectedFPS);
                    } else {
                        m_executionType = ExecutionTypes::VsyncFrames({
                            adaptiveVsyncFrames.screenUpdatesCount
                        });
                    }
                }
                windowManager().PollEvents();
            },
            [&](const ExecutionTypes::EventBasedFrames& et) {
                windowManager().WaitEvents();
            },
            [&](const ExecutionTypes::EventBasedFramesTimeout& eventBasedFramesTimeout) {
                windowManager().WaitEventsWithTimeout(eventBasedFramesTimeout.timeoutSeconds);
            },
            [&](const ExecutionTypes::LimitedFrames& limitedFrames) {
                windowManager().PollEvents();
                if(m_lastFrameTime.GetSeconds() < limitedFrames.minFrameTime()) {
                    std::chrono::duration<f64> sleepTime(
                        limitedFrames.minFrameTime() - m_lastFrameTime.GetSeconds()
                    );
                    std::this_thread::sleep_for(sleepTime);
                }
            }
        );
    }
}

void WindowApp::UpdateExecutionType()
{
    DefaultFramebuffer& defaultFramebuffer = context().GetDefaultFramebuffer();
    match(m_executionType,
        [&](const ExecutionTypes::VsyncFrames& vsyncFrames) {
            defaultFramebuffer.SetSwapWaitInterval(vsyncFrames.screenUpdatesCount);
        },
        [&](const ExecutionTypes::AdaptiveVsyncFrames& adaptiveVsyncFrames) {
            defaultFramebuffer.SetSwapWaitInterval(adaptiveVsyncFrames.screenUpdatesCount);
        },
        [&](const auto& other) {
            defaultFramebuffer.SetSwapWaitInterval(0);
        }
    );
}

void WindowApp::OnWindowManagerError(const WindowManagerError& error)
{
    spdlog::error("{}: {}", enum_name(error.type), error.message);
}

}