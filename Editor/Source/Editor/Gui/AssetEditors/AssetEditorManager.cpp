// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/AssetEditors/AssetEditorManager.hpp"
#include "Editor/Gui/AssetEditors/SceneEditor.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/Assets/AssetManager.hpp"

#include <imgui_internal.h>

namespace Cobalt
{
    auto AssetEditorManager::Init() -> void {}

    auto AssetEditorManager::Update(EditorState& state) -> void {
        for (const auto& asset_editor : _editors) {
            asset_editor->OnUpdate(state);
        }
    }

    auto AssetEditorManager::Draw(EditorState& state) -> void {
        for (const auto& asset_editor : _editors) {
            auto flags = asset_editor->GetIsDirty() ? ImGuiWindowFlags_UnsavedDocument : 0;
            flags |= ImGuiWindowFlags_NoSavedSettings;

            if (asset_editor->GetWantsFocus()) {
                ImGui::SetNextWindowFocus();
                asset_editor->Unfocus();
            }

            const auto main_dockspace_id = ImGui::DockBuilderGetCentralNode(state.main_dockspace_id)->ID;

            ImGui::SetNextWindowDockID(main_dockspace_id, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowClass(&state.main_dockspace_window_class);
            ImGui::Begin((asset_editor->GetIcon() + " " + asset_editor->GetName()).c_str(), nullptr, flags);
            asset_editor->Draw(state);
            ImGui::End();
        }
    }

    auto AssetEditorManager::OpenEditor(const UUID asset_uuid) -> void {
        auto asset_meta = AssetManager::Get().GetRegistry().GetMetadata(asset_uuid);

        bool found_opened = false;
        for (const auto& asset_editor : _editors) {
            if (asset_editor->GetAssetUUID() == asset_uuid) {
                asset_editor->Focus();
                found_opened = true;
                break;
            }
        }

        if (!found_opened) {
            switch (asset_meta->type) {
                case AssetType::Scene: {
                    _editors.push_back(Memory::MakeBox<SceneEditor>(asset_uuid));
                    break;
                }
                default: break;
            }
        }
    }

    auto AssetEditorManager::Get() -> AssetEditorManager& {
        static AssetEditorManager instance;
        return instance;
    }
} // namespace Cobalt
