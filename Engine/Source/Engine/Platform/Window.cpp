// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Platform/Window.hpp"
#include "Engine/Core/Defines.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Events/ApplicationEvents.hpp"
#include "Engine/Events/DropEvents.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/GamepadEvents.hpp"
#include "Engine/Events/KeyboardEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"

#include <SDL3/SDL.h>
#include <optick.h>

namespace Cobalt
{
    struct WindowState
    {
        SDL_Window* window;
        NativeEventCallback event_callback;
    };

    static WindowState state;

    auto Window::Initialize() -> Result<void, WindowInitError> {
#if defined(CONFIGURATION_DEBUG)
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif

        SDL_SetLogOutputFunction(
                [](void*, int, const SDL_LogPriority priority, const char* message) {
                    switch (priority) {
                        case SDL_LOG_PRIORITY_CRITICAL: {
                            CORE_CRITICAL("SDL3: {}", message);
                            break;
                        }
                        case SDL_LOG_PRIORITY_ERROR: {
                            CORE_ERROR("SDL3: {}", message);
                            break;
                        }
                        case SDL_LOG_PRIORITY_WARN: {
                            CORE_WARN("SDL3: {}", message);
                            break;
                        }
                        case SDL_LOG_PRIORITY_INFO: {
                            CORE_INFO("SDL3: {}", message);
                            break;
                        }
                        default: {
                            CORE_TRACE("SDL3: {}", message);
                            break;
                        }
                    }
                },
                nullptr);

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            SDL_Log("%s", SDL_GetError());
            return Err(WindowInitError::SDLInit);
        }
        CORE_INFO("Platform::Window: SDL3 initialized.");

        constexpr auto window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        const auto primary_display = SDL_GetPrimaryDisplay();
        auto* display_mode = SDL_GetCurrentDisplayMode(primary_display);

        const auto title = Project::GetName();

        state.window = SDL_CreateWindow(title.c_str(), static_cast<int>(display_mode->w * 0.8), static_cast<int>(display_mode->h * 0.8),
                                        window_flags);
        if (state.window == nullptr) {
            SDL_Log("%s", SDL_GetError());
            return Err(WindowInitError::WindowCreate);
        }

        SDL_SetWindowPosition(state.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(state.window);

        return {};
    }

    auto Window::Shutdown() -> void {
        SDL_DestroyWindow(state.window);
        SDL_Quit();
    }

    auto Window::PollEvents() -> void {
        OPTICK_EVENT();

        static SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event)) {
            if (state.event_callback) {
                state.event_callback(&sdl_event);
            }

            switch (sdl_event.type) {
                // Application events
                case SDL_EVENT_QUIT: {
                    EventBus::Trigger<ApplicationQuitEvent>();
                    break;
                }

                // Window events
                case SDL_EVENT_WINDOW_MOVED: {
                    EventBus::Trigger<WindowMovedEvent>(
                            {static_cast<u32>(sdl_event.window.data1), static_cast<u32>(sdl_event.window.data2)});
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    EventBus::Trigger<WindowResizedEvent>(
                            {static_cast<u32>(sdl_event.window.data1), static_cast<u32>(sdl_event.window.data2)});
                    break;
                }
                case SDL_EVENT_WINDOW_MINIMIZED: {
                    EventBus::Trigger<WindowMinimizedEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_MAXIMIZED: {
                    EventBus::Trigger<WindowMaximizedEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_RESTORED: {
                    EventBus::Trigger<WindowRestoredEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: {
                    EventBus::Trigger<WindowEnterFullscreenEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: {
                    EventBus::Trigger<WindowLeaveFullscreenEvent>();
                    break;
                }

                // Keyboard events
                case SDL_EVENT_KEY_DOWN: {
                    EventBus::Trigger<KeyboardKeyDownEvent>();
                    break;
                }
                case SDL_EVENT_KEY_UP: {
                    EventBus::Trigger<KeyboardKeyUpEvent>();
                    break;
                }
                case SDL_EVENT_TEXT_INPUT: {
                    EventBus::Trigger<KeyboardTextInputEvent>();
                    break;
                }

                // Gamepad events
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    EventBus::Trigger<GamepadAxisMotionEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    EventBus::Trigger<GamepadButtonDownEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    EventBus::Trigger<GamepadButtonUpEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_ADDED: {
                    EventBus::Trigger<GamepadAddedEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    EventBus::Trigger<GamepadRemovedEvent>();
                    break;
                }

                // Drop events
                case SDL_EVENT_DROP_FILE: {
                    EventBus::Trigger<DropFileEvent>({sdl_event.drop.data});
                    break;
                }
                case SDL_EVENT_DROP_TEXT: {
                    EventBus::Trigger<DropTextEvent>({sdl_event.drop.data});
                    break;
                }
                case SDL_EVENT_DROP_BEGIN: {
                    EventBus::Trigger<DropBeginEvent>();
                    break;
                }
                case SDL_EVENT_DROP_COMPLETE: {
                    EventBus::Trigger<DropCompleteEvent>();
                    break;
                }
                case SDL_EVENT_DROP_POSITION: {
                    EventBus::Trigger<DropPositionEvent>({static_cast<u32>(sdl_event.drop.x), static_cast<u32>(sdl_event.drop.y)});
                    break;
                }

                default: return;
            }
        }
    }

    auto Window::GetHandle() -> SDL_Window* {
        return state.window;
    }

    auto Window::GetSize() -> Vec<2, i32> {
        i32 width, height = 0;
        SDL_GetWindowSize(state.window, &width, &height);
        return {width, height};
    }

    auto Window::SetTitle(const String& title) -> void {
        SDL_SetWindowTitle(state.window, title.data());
    }

    auto Window::SetNativeEventCallback(NativeEventCallback callback) -> void {
        state.event_callback = std::move(callback);
    }
} // namespace Cobalt
