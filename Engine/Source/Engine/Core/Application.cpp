// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Events/ApplicationEvents.hpp"
#include "Engine/Events/DropEvents.hpp"
#include "Engine/Events/GamepadEvents.hpp"
#include "Engine/Events/KeyboardEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto Application::run(const CommandLineArgs& args) -> void {
        if (!initialize(args)) {
            Logger::fatal("Engine::Core::Application", "Initialization failed. Exiting program.");
            return;
        }

        begin(m_ctx);
        main_loop();
        end(m_ctx);
    }

    auto Application::initialize(const CommandLineArgs& args) -> bool {
        m_ctx.project.init(args);
        m_ctx.asset_manager.init(m_ctx.project);

        if (!m_ctx.window.init(m_ctx.project)) return false;

        return true;
    }

    auto Application::main_loop() -> void {
        while (!m_ctx.close_requested) {
            poll_events();
            update(m_ctx);
            m_ctx.window.swap_buffers();
        }
    }

    auto Application::poll_events() -> void {
        static SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event)) {
            on_sdl_event(&sdl_event);

            switch (sdl_event.type) {
                // Application events
                case SDL_EVENT_QUIT: {
                    auto e = ApplicationQuitEvent();
                    m_ctx.close_requested = true;
                    event(e);
                    break;
                }

                // Window events
                case SDL_EVENT_WINDOW_MOVED: {
                    auto e = WindowMovedEvent(sdl_event.window.data1, sdl_event.window.data2);
                    event(e);
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    auto e = WindowResizedEvent(sdl_event.window.data1, sdl_event.window.data2);
                    event(e);
                    ;
                    break;
                }
                case SDL_EVENT_WINDOW_MINIMIZED: {
                    auto e = WindowMinimizedEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_WINDOW_MAXIMIZED: {
                    auto e = WindowMaximizedEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_WINDOW_RESTORED: {
                    auto e = WindowRestoredEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: {
                    auto e = WindowEnterFullscreenEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: {
                    auto e = WindowLeaveFullscreenEvent();
                    event(e);
                    break;
                }

                // Keyboard events
                case SDL_EVENT_KEY_DOWN: {
                    auto e = KeyboardKeyDownEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_KEY_UP: {
                    auto e = KeyboardKeyUpEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_TEXT_INPUT: {
                    auto e = KeyboardTextInputEvent();
                    event(e);
                    break;
                }

                // Gamepad events
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    auto e = GamepadAxisMotionEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    auto e = GamepadButtonDownEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    auto e = GamepadButtonUpEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_ADDED: {
                    auto e = GamepadAddedEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    auto e = GamepadRemovedEvent();
                    event(e);
                    break;
                }

                // Drop events
                case SDL_EVENT_DROP_FILE: {
                    auto e = DropFileEvent(sdl_event.drop.data);
                    event(e);
                    break;
                }
                case SDL_EVENT_DROP_TEXT: {
                    auto e = DropTextEvent(sdl_event.drop.data);
                    event(e);
                    break;
                }
                case SDL_EVENT_DROP_BEGIN: {
                    auto e = DropBeginEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_DROP_COMPLETE: {
                    auto e = DropCompleteEvent();
                    event(e);
                    break;
                }
                case SDL_EVENT_DROP_POSITION: {
                    auto e = DropPositionEvent(sdl_event.drop.x, sdl_event.drop.y);
                    event(e);
                    break;
                }
                default: return;
            }
        }
    }

    Application::~Application() {
        m_ctx.window.destroy();
    }
} // namespace Cobalt
