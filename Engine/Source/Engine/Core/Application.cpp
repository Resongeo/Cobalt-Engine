// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Platform/Window.hpp"

namespace Cobalt
{
    auto Application::run(const CommandLineArgs& args) -> void {
        if (!initialize(args)) {
            CORE_CRITICAL("Application: Initialization failed! Exiting program...");
            return;
        }

        begin(m_ctx);
        main_loop();
        end(m_ctx);
    }

    auto Application::initialize(const CommandLineArgs& args) -> bool {
        Log::init(m_ctx);

        m_ctx.project.init(args);
        m_ctx.asset_manager.init(m_ctx.project);
        m_ctx.scene_manager.init(m_ctx);

        if (!m_ctx.window.init(m_ctx)) return false;
        if (!m_ctx.script_manager.init(m_ctx)) return false;

        m_ctx.dialog_manager.init(m_ctx.window);

        return true;
    }

    auto Application::main_loop() -> void {
        while (!m_ctx.close_requested) {
            Log::flush_events(m_ctx);

            m_ctx.window.poll_events(m_ctx);
            update(m_ctx);
            m_ctx.window.swap_buffers();
        }
    }

    Application::~Application() {
        m_ctx.asset_manager.save_registry();
        m_ctx.script_manager.destroy();
        m_ctx.window.destroy();
    }
} // namespace Cobalt
