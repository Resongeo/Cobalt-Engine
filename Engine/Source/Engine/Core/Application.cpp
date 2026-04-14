// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Events/ApplicationEvents.hpp"
#include "Engine/Events/DropEvents.hpp"
#include "Engine/Events/GamepadEvents.hpp"
#include "Engine/Events/KeyboardEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Platform/Platform.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    Application::~Application() {
        _cleanup();
    }

    auto Application::run() -> void {
        if (!_initialize()) {
            Logger::fatal("Engine::Core::Application", "Initialization failed. Exiting program.");
            return;
        }

        on_begin();
        _main_loop();
        on_end();
    }

    auto Application::_initialize() const -> bool {
        if (!Platform::sdl3_init()) return false;
        if (!Window::initialize()) return false;
        if (!Platform::opengl_init()) return false;

        return true;
    }

    auto Application::_main_loop() -> void {
        while (!m_close_requested) {
            _poll_events();

            on_update();

            Window::instance().swap_buffers();
        }
    }

    auto Application::_cleanup() const -> void {
        Window::destroy();
    }

    auto Application::_poll_events() -> void {
        static SDL_Event e;

        while (SDL_PollEvent(&e)) {
            on_sdl_event(&e);

            switch (e.type) {
                // Application events
                case SDL_EVENT_QUIT: {
                    auto event = ApplicationQuitEvent();
                    m_close_requested = true;
                    on_event(event);
                    break;
                }

                // Window events
                case SDL_EVENT_WINDOW_MOVED: {
                    auto event = WindowMovedEvent(e.window.data1, e.window.data2);
                    on_event(event);
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    auto event = WindowResizedEvent(e.window.data1, e.window.data2);
                    on_event(event);
                    break;
                }
                case SDL_EVENT_WINDOW_MINIMIZED: {
                    auto event = WindowMinimizedEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_WINDOW_MAXIMIZED: {
                    auto event = WindowMaximizedEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_WINDOW_RESTORED: {
                    auto event = WindowRestoredEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: {
                    auto event = WindowEnterFullscreenEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: {
                    auto event = WindowLeaveFullscreenEvent();
                    on_event(event);
                    break;
                }

                // Keyboard events
                case SDL_EVENT_KEY_DOWN: {
                    auto event = KeyboardKeyDownEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_KEY_UP: {
                    auto event = KeyboardKeyUpEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_TEXT_INPUT: {
                    auto event = KeyboardTextInputEvent();
                    on_event(event);
                    break;
                }

                // Gamepad events
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    auto event = GamepadAxisMotionEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    auto event = GamepadButtonDownEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    auto event = GamepadButtonUpEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_GAMEPAD_ADDED: {
                    auto event = GamepadAddedEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    auto event = GamepadRemovedEvent();
                    on_event(event);
                    break;
                }

                // Drop events
                case SDL_EVENT_DROP_FILE: {
                    auto event = DropFileEvent(e.drop.data);
                    on_event(event);
                    break;
                }
                case SDL_EVENT_DROP_TEXT: {
                    auto event = DropTextEvent(e.drop.data);
                    on_event(event);
                    break;
                }
                case SDL_EVENT_DROP_BEGIN: {
                    auto event = DropBeginEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_DROP_COMPLETE: {
                    auto event = DropCompleteEvent();
                    on_event(event);
                    break;
                }
                case SDL_EVENT_DROP_POSITION: {
                    auto event = DropPositionEvent(e.drop.x, e.drop.y);
                    on_event(event);
                    break;
                }
                default: return;
            }
        }
    }
} // namespace Cobalt
