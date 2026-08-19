// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Core/EditorState.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace Cobalt
{
    class AssetEditor
    {
    public:
        explicit AssetEditor(UUID asset_uuid);
        virtual ~AssetEditor() = default;

        virtual auto OnInitLayout(ImGuiID dockspace_id) -> void {}
        virtual auto OnUpdate(EditorState& state) -> void {}
        virtual auto OnDrawPanels(EditorState& state) -> void {}

        auto Draw(EditorState& state) -> void;
        auto Focus() -> void;
        auto Unfocus() -> void;
        auto FinishLayoutInitialization() -> void;

        auto GetName() const -> const String&;
        auto GetIsDirty() const -> bool;
        auto GetIsLayoutInitialized() const -> bool;
        auto GetAssetUUID() const -> UUID;
        auto GetWantsFocus() const -> bool;
        auto GetWindowClass() const -> ImGuiWindowClass;

    protected:
        auto SetName(const String& name) -> void;
        auto SetIsDirty(bool is_dirty) -> void;

    private:
        bool _is_layout_initialized = false;
        bool _is_dirty = false;
        bool _wants_focus = false;
        String _name = {};
        UUID _asset_uuid;
        ImGuiWindowClass _window_class = {};
    };
}