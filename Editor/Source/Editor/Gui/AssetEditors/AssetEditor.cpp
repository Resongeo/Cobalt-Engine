// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/AssetEditors/AssetEditor.hpp"

namespace Cobalt
{
    AssetEditor::AssetEditor(const UUID asset_uuid) : _asset_uuid(asset_uuid) {}

    auto AssetEditor::Draw(EditorState& state) -> void {
        const auto dockspace_id = ImGui::GetID((std::to_string(_asset_uuid.value) + "_DockSpace").c_str());

        _window_class.ClassId = ImGui::GetID(_asset_uuid.value);
        _window_class.DockingAllowUnclassed = false;

        if (!GetIsLayoutInitialized()) {
            OnInitLayout(dockspace_id);
            FinishLayoutInitialization();
        }

        ImGui::DockSpace(dockspace_id, {0, 0}, ImGuiDockNodeFlags_None, &_window_class);

        OnDrawPanels(state);
    }

    auto AssetEditor::Focus() -> void {
        _wants_focus = true;
    }

    auto AssetEditor::Unfocus() -> void {
        _wants_focus = false;
    }

    auto AssetEditor::GetName() const -> const String& {
        return _name;
    }

    auto AssetEditor::GetIcon() const -> const String {
        return String(_icon);
    }

    auto AssetEditor::GetIsDirty() const -> bool {
        return _is_dirty;
    }

    auto AssetEditor::GetIsLayoutInitialized() const -> bool {
        return _is_layout_initialized;
    }

    auto AssetEditor::GetAssetUUID() const -> UUID {
        return _asset_uuid;
    }

    auto AssetEditor::GetWantsFocus() const -> bool {
        return _wants_focus;
    }

    auto AssetEditor::GetWindowClass() const -> ImGuiWindowClass {
        return _window_class;
    }

    auto AssetEditor::FinishLayoutInitialization() -> void {
        _is_layout_initialized = true;
    }

    auto AssetEditor::SetName(const String& name) -> void {
        _name = name;
    }

    auto AssetEditor::SetIcon(const char* icon) -> void {
        _icon = icon;
    }

    auto AssetEditor::SetIsDirty(const bool is_dirty) -> void {
        _is_dirty = is_dirty;
    }
} // namespace Cobalt
