// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/AssetEditors/AssetEditor.hpp"
#include "Engine/Assets/AssetType.hpp"
#include "Engine/Core/Types/Memory.hpp"

namespace Cobalt
{
    class AssetEditorManager final
    {
    public:
        auto Init() -> void;
        auto Update(EditorState& state) -> void;
        auto Draw(EditorState& state) -> void;

        auto OpenEditor(UUID asset_uuid) -> void;

        static auto Get() -> AssetEditorManager&;

    private:
        Vector<Box<AssetEditor>> _editors = {};
    };
}