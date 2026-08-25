// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/AssetEditors/SceneEditor.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/Textures.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <imgui.h>
#include <imgui_internal.h>

// IMPORTANT: Include ImGuizmo after imgui.h
#include <ImGuizmo.h>

#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"

namespace Cobalt
{
    constexpr auto VIEWPORT_PANEL_NAME = "Viewport";
    constexpr auto ASSET_BROWSER_PANEL_NAME = ICON_ASSETS " Asset Browser";
    constexpr auto SCENE_HIERARCHY_PANEL_NAME = ICON_HIERARCHY " Scene Hierarchy";
    constexpr auto COMPONENTS_PANEL_NAME = ICON_COMPONENTS " Components";

    constexpr auto PANEL_FLAGS = ImGuiWindowFlags_None;

    inline auto GizmoOperationToImGuizmo(const GizmoOperation operation) -> ImGuizmo::OPERATION {
        switch (operation) {
            case GizmoOperation::Universal: return ImGuizmo::UNIVERSAL;
            case GizmoOperation::Translate: return ImGuizmo::TRANSLATE;
            case GizmoOperation::Rotate: return ImGuizmo::ROTATE;
            case GizmoOperation::Scale: return ImGuizmo::SCALE;
        }

        return ImGuizmo::UNIVERSAL;
    }

    SceneEditor::SceneEditor(const UUID& asset_uuid) : AssetEditor(asset_uuid) {
        SetName("Scene Editor");
        SetIcon(ICON_SCENE);

        _assets_base_dir = Project::Get().GetProjectAssetsPath();
        _current_dir = _assets_base_dir;
    }

    void SceneEditor::OnInitLayout(const ImGuiID dockspace_id) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        ImGuiID main_area_node;
        ImGuiID right_sidebar_node;

        // 1. Split right sidebar (20%) from root -> sidebar gets full height on the right
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, &right_sidebar_node, &main_area_node);

        ImGuiID viewport_node;
        ImGuiID bottom_node;

        // 2. Split bottom panel (20%) from main_area_node -> stays strictly under viewport
        ImGui::DockBuilderSplitNode(main_area_node, ImGuiDir_Down, 0.20f, &bottom_node, &viewport_node);

        // 3. Split right sidebar vertically
        ImGuiID hierarchy_node;
        ImGuiID components_node;
        ImGui::DockBuilderSplitNode(right_sidebar_node, ImGuiDir_Up, 0.50f, &hierarchy_node, &components_node);

        if (const auto node = ImGui::DockBuilderGetNode(viewport_node)) {
            node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        }

        ImGui::DockBuilderDockWindow(VIEWPORT_PANEL_NAME, viewport_node);
        ImGui::DockBuilderDockWindow(ASSET_BROWSER_PANEL_NAME, bottom_node);
        ImGui::DockBuilderDockWindow(SCENE_HIERARCHY_PANEL_NAME, hierarchy_node);
        ImGui::DockBuilderDockWindow(COMPONENTS_PANEL_NAME, components_node);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    void SceneEditor::OnUpdate(EditorState& state) {
        AssetEditor::OnUpdate(state);
    }

    void SceneEditor::OnDrawPanels(EditorState& state) {
        DrawViewport(state);
        DrawHierarchy(state);
        DrawComponents(state);
        DrawAssetsBrowser(state);
    }

    auto SceneEditor::DrawViewport(EditorState& state) const -> void {
        const auto window_class = GetWindowClass();
        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin(VIEWPORT_PANEL_NAME, nullptr, PANEL_FLAGS);
        {
            static auto mode = ImGuizmo::LOCAL;
            static auto should_snap = false;
            f32 snap_amount = 0.0f;

            if (ImGui::IsWindowFocused()) {
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                    state.gizmo_operation = GizmoOperation::Universal;
                } else if (ImGui::IsKeyPressed(ImGuiKey_W)) {
                    state.gizmo_operation = GizmoOperation::Translate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
                    state.gizmo_operation = GizmoOperation::Rotate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_R)) {
                    state.gizmo_operation = GizmoOperation::Scale;
                }

                if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                    should_snap = true;
                } else if (ImGui::IsKeyReleased(ImGuiKey_LeftShift)) {
                    should_snap = false;
                }
            }

            static auto toolbar_pos = ImVec2();
            toolbar_pos = ImGui::GetCursorScreenPos();

            if (const auto color_id = state.framebuffer.GetColorAttachmentID(0); color_id >= 0) {
                const auto viewport_size = ImGui::GetContentRegionAvail();
                state.framebuffer.Resize(viewport_size.x, viewport_size.y);
                ImGui::Image(color_id, viewport_size, {0, 1}, {1, 0});
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

            ImGui::SetCursorScreenPos(toolbar_pos);
            if (ImGui::RadioButton("Universal", state.gizmo_operation == GizmoOperation::Universal)) {
                state.gizmo_operation = GizmoOperation::Universal;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Translate", state.gizmo_operation == GizmoOperation::Translate)) {
                state.gizmo_operation = GizmoOperation::Translate;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", state.gizmo_operation == GizmoOperation::Rotate)) {
                state.gizmo_operation = GizmoOperation::Rotate;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", state.gizmo_operation == GizmoOperation::Scale)) {
                state.gizmo_operation = GizmoOperation::Scale;
            }
            ImGui::SameLine();
            ImGui::Dummy({20, 0});
            ImGui::SameLine();
            if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL)) {
                mode = ImGuizmo::LOCAL;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD)) {
                mode = ImGuizmo::WORLD;
            }

            ImGui::PopStyleVar();

            if (state.selected_entity != entt::null && SceneManager::Get().GetState() == SceneState::None) {
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

                auto entity = Entity(state.selected_entity, &state.active_scene->GetRegistry());
                auto& transform_component = entity.GetComponent<TransformComponent>();
                auto transform_matrix = transform_component.TransformMatrix();

                if (should_snap) {
                    // TODO: Implement snapping
                } else {
                    snap_amount = 0.0f;
                }

                ImGuizmo::Manipulate(glm::value_ptr(state.editor_camera.GetView()),
                                     glm::value_ptr(state.editor_camera.GetProjection(state.framebuffer.GetSize())),
                                     GizmoOperationToImGuizmo(state.gizmo_operation), mode, glm::value_ptr(transform_matrix), nullptr,
                                     should_snap ? &snap_amount : nullptr);

                if (ImGuizmo::IsUsing()) {
                    Vec3 scale;
                    Quat rotation;
                    Vec3 position;
                    Vec3 skew;
                    Vec4 perspective;
                    glm::decompose(transform_matrix, scale, rotation, position, skew, perspective);

                    switch (state.gizmo_operation) {
                        case GizmoOperation::Universal:
                            transform_component.position = {position.x, position.y};
                            transform_component.rotation = glm::degrees(glm::eulerAngles(rotation)).z;
                            transform_component.scale = {scale.x, scale.y};
                            break;
                        case GizmoOperation::Translate: transform_component.position = {position.x, position.y}; break;
                        case GizmoOperation::Rotate: transform_component.rotation = glm::degrees(glm::eulerAngles(rotation)).z; break;
                        case GizmoOperation::Scale: transform_component.scale = {scale.x, scale.y}; break;
                        default:;
                    }
                }
            }
        }
        ImGui::End();
    }

    auto SceneEditor::DrawHierarchy(EditorState& state) const -> void {
        const auto window_class = GetWindowClass();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin(SCENE_HIERARCHY_PANEL_NAME, nullptr, PANEL_FLAGS);
        {
            // Fix annoyance of have to click into the viewport in order to change the gizmo operation
            if (ImGui::IsWindowFocused()) {
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                    state.gizmo_operation = GizmoOperation::Universal;
                } else if (ImGui::IsKeyPressed(ImGuiKey_W)) {
                    state.gizmo_operation = GizmoOperation::Translate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
                    state.gizmo_operation = GizmoOperation::Rotate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_R)) {
                    state.gizmo_operation = GizmoOperation::Scale;
                }
            }

            auto index = 0;
            const auto view = state.active_scene->GetRegistry().view<entt::entity>();
            for (auto it = view.rbegin(); it != view.rend(); ++it) {
                const auto entity = *it;
                auto [name, id] = state.active_scene->GetRegistry().get<TagComponent>(entity);

                ImGui::PushID(id.value);

                constexpr auto padding = 10.0f;
                const auto content_avail = ImGui::GetContentRegionAvail();
                const auto title_height = ImGui::GetTextLineHeightWithSpacing();
                const auto min = ImGui::GetCursorScreenPos();
                const auto max = ImVec2{min.x + content_avail.x, min.y + title_height + padding * 2.0f};
                const auto size = max - min;

                auto bg_col = index++ & 1 ? Colors::node_odd : Colors::node_even;

                ImGui::PushID(index * 8127812 % 23);
                const auto clicked = ImGui::InvisibleButton("##Node", size);
                const auto hovered = ImGui::IsItemHovered();
                ImGui::PopID();

                if (clicked) {
                    state.selected_entity = entity;
                }

                if (state.selected_entity == entity) {
                    bg_col = Colors::node_active;
                }

                bg_col = hovered ? Colors::node_hovered : bg_col;

                ImGui::RenderFrame(min, max, ImGui::GetColorU32(IMVEC4(bg_col)), false);

                ImGui::SetCursorScreenPos({min.x + padding, min.y + padding});
                ImGui::Text("%s", name.c_str());

                ImGui::SetCursorScreenPos(min);
                ImGui::Dummy(size);

                ImGui::PopID();
            }

            if (ImGui::IsWindowHovered()) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    state.selected_entity = entt::null;
                }

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    ImGui::OpenPopup("Scene Hierarchy Context Menu");
                }
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
            if (ImGui::BeginPopupContextItem("Scene Hierarchy Context Menu")) {
                ImGui::SeparatorText("Create");
                if (Widgets::Button("Empty Entity")) {
                    const auto entity = state.active_scene->CreateEntity("Entity");
                    state.selected_entity = entity.ID();

                    ImGui::CloseCurrentPopup();
                }
                if (Widgets::Button("Sprite Entity")) {
                    auto entity = state.active_scene->CreateEntity("Sprite");
                    entity.AddComponent<SpriteComponent>();
                    state.selected_entity = entity.ID();

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(2);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    auto SceneEditor::DrawComponents(EditorState& state) const -> void {
        const auto window_class = GetWindowClass();
        ImGui::SetNextWindowClass(&window_class);
        Widgets::Begin(COMPONENTS_PANEL_NAME, {8, 8}, PANEL_FLAGS);
        {
            auto& imgui_style = ImGui::GetStyle();

            if (state.selected_entity != entt::null) {
                auto entity = Entity(state.selected_entity, &state.active_scene->GetRegistry());

                // TODO: Rework this popup menu
                ImGui::PushFont(Fonts::icon);
                if (Widgets::Button("Add Component", Variant::Default)) {
                    ImGui::OpenPopup("Add Component Popup Menu");
                }
                ImGui::PopFont();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
                if (ImGui::BeginPopupContextItem("Add Component Popup Menu")) {
                    ImGui::SeparatorText("Add Component");

                    if (!entity.HasComponent<SpriteComponent>()) {
                        if (Widgets::Button("Sprite Component")) {
                            entity.AddComponent<SpriteComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (!entity.HasComponent<ScriptComponent>()) {
                        if (Widgets::Button("Script Component")) {
                            entity.AddComponent<ScriptComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar(2);

                if (entity.HasComponent<TagComponent>()) {
                    if (Widgets::CollapsingHeader(ICON_TAG " Tag", Colors::tag)) {
                        auto& [name, uuid] = entity.GetComponent<TagComponent>();
                        Widgets::TextInput("Name", &name);

                        ImGui::PushStyleColor(ImGuiCol_Text, IMVEC4(Colors::text_muted));
                        ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
                        ImGui::PopStyleColor();
                    }
                }

                if (entity.HasComponent<TransformComponent>()) {
                    if (Widgets::CollapsingHeader(ICON_TRANSFORM " Transform", Colors::transform)) {
                        auto& [position, scale, rotation] = entity.GetComponent<TransformComponent>();
                        ImGui::DragFloat2("Position", &position[0], 0.1f);
                        ImGui::DragFloat2("Scale", &scale[0], 0.1f);
                        ImGui::DragFloat("Rotation", &rotation, 0.1f);
                    }
                }

                if (entity.HasComponent<SpriteComponent>()) {
                    if (Widgets::CollapsingHeader(ICON_SPRITE " Sprite", Colors::sprite)) {
                        auto& [tint, uuid] = entity.GetComponent<SpriteComponent>();
                        ImGui::ColorEdit4("Tint", &tint.r);

                        const auto uuid_str = std::to_string(uuid.value);
                        const auto region_avail = ImGui::GetContentRegionAvail();
                        const auto cursor_pos = ImGui::GetCursorScreenPos();
                        const auto draw_list = ImGui::GetWindowDrawList();
                        constexpr auto thumbnail_size = ImVec2{60, 60};
                        constexpr auto thumbnail_padding = 10.0f;
                        constexpr auto rect_height = thumbnail_size.y + thumbnail_padding * 2.0f;

                        ImGui::InvisibleButton(uuid_str.c_str(), {region_avail.x, rect_height});

                        if (ImGui::BeginDragDropTarget()) {
                            if (const auto* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_AND_DROP")) {
                                const u64 uid = *static_cast<u64*>(payload->Data);
                                uuid.value = uid;
                            }
                            ImGui::EndDragDropTarget();
                        }

                        draw_list->AddRectFilled(cursor_pos, cursor_pos + ImVec2{region_avail.x, rect_height},
                                                 ImGui::GetColorU32(imgui_style.Colors[ImGuiCol_FrameBg]), imgui_style.FrameRounding);

                        const auto texture = AssetManager::Get().GetAsset<Texture2D>(uuid);
                        const auto text_line_height = ImGui::GetTextLineHeight();
                        const auto text_y_offset = rect_height * 0.5f - text_line_height * 0.5f;
                        if (texture) {
                            ImGui::SetCursorScreenPos(cursor_pos + ImVec2{thumbnail_padding, thumbnail_padding});
                            ImGui::Image(texture->GetRendererID(), thumbnail_size, {0, 1}, {1, 0});

                            ImGui::SetCursorScreenPos(cursor_pos + ImVec2{thumbnail_size.x + thumbnail_padding * 2, text_y_offset});
                            const auto meta = AssetManager::Get().GetRegistry().GetMetadata(uuid).value();
                            const auto texture_name_string = meta.path.filename().string();
                            ImGui::Text("%s", texture_name_string.c_str());
                        } else {
                            const auto min = cursor_pos + ImVec2{thumbnail_padding, thumbnail_padding};
                            draw_list->AddRectFilled(min, min + thumbnail_size, IM_COL32_WHITE);

                            ImGui::SetCursorScreenPos(cursor_pos + ImVec2{thumbnail_size.x + thumbnail_padding * 2, text_y_offset});
                            ImGui::Text("Default Texture");
                        }

                        ImGui::SetCursorScreenPos(cursor_pos);
                        ImGui::Dummy({0, rect_height});
                    }
                }

                if (entity.HasComponent<ScriptComponent>()) {
                    if (Widgets::CollapsingHeader(ICON_SCRIPT " Script", Colors::script)) {
                        auto& [uuid, _] = entity.GetComponent<ScriptComponent>();

                        const auto uuid_str = std::to_string(uuid.value);
                        const auto region_avail = ImGui::GetContentRegionAvail();
                        const auto cursor_pos = ImGui::GetCursorScreenPos();
                        const auto draw_list = ImGui::GetWindowDrawList();
                        constexpr auto thumbnail_padding = 10.0f;
                        const auto text_line_height = ImGui::GetTextLineHeight();
                        const auto rect_height = text_line_height + thumbnail_padding * 2.0f;

                        ImGui::InvisibleButton(uuid_str.c_str(), {region_avail.x, rect_height});

                        if (ImGui::BeginDragDropTarget()) {
                            if (const auto* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_AND_DROP")) {
                                const u64 uid = *static_cast<u64*>(payload->Data);
                                uuid.value = uid;
                            }
                            ImGui::EndDragDropTarget();
                        }

                        draw_list->AddRectFilled(cursor_pos, cursor_pos + ImVec2{region_avail.x, rect_height},
                                                 ImGui::GetColorU32(imgui_style.Colors[ImGuiCol_FrameBg]), imgui_style.FrameRounding);

                        auto class_name = String{};
                        if (auto meta_opt = AssetManager::Get().GetRegistry().GetMetadata(uuid)) {
                            auto& meta = meta_opt.value();
                            class_name = meta.path.filename().string().c_str();
                        } else {
                            class_name = "None";
                        }

                        ImGui::SetCursorScreenPos(cursor_pos + ImVec2(thumbnail_padding, thumbnail_padding));
                        ImGui::Text("Script Class: %s", class_name.c_str());

                        ImGui::SetCursorScreenPos(cursor_pos);
                        ImGui::Dummy({0, rect_height});
                    }
                }
            }
        }
        Widgets::End();
    }

    constexpr float THUMBNAIL_MIN_SIZE = 70.0f;
    constexpr float THUMBNAIL_MAX_SIZE = 150.0f;

    auto SceneEditor::DrawAssetsBrowser(EditorState& state) -> void {
        const auto window_class = GetWindowClass();
        ImGui::SetNextWindowClass(&window_class);
        Widgets::Begin(ASSET_BROWSER_PANEL_NAME, {8, 8}, PANEL_FLAGS);
        {
            ImGui::PushFont(Fonts::icon);

            if (Widgets::Button(ICON_ARROW_LEFT, Variant::Default, {0, 0}, true)) {
                if (_current_dir != std::filesystem::path(_assets_base_dir)) {
                    _current_dir = _current_dir.parent_path();
                    _directory_changed = true;
                }
            }

            ImGui::SameLine();

            if (Widgets::Button(ICON_REFRESH, Variant::Default, {0, 0}, true) || _directory_changed) {
                for (auto& entry : std::filesystem::directory_iterator(_current_dir)) {
                    if (!entry.is_directory() && entry.path().extension() != ".meta") {
                        AssetManager::Get().RegisterAsset(entry.path());
                    }
                }

                _directory_changed = false;
            }

            ImGui::PopFont();

            static auto padding = 8.0f;
            static auto thumbnail_size = 100.0f;
            const auto cell_size = thumbnail_size + padding;

            const auto panel_width = ImGui::GetContentRegionAvail().x;
            auto columnCount = static_cast<i32>(panel_width / cell_size);
            if (columnCount < 1) {
                columnCount = 1;
            }

            ImGui::Columns(columnCount, nullptr, false);

            for (auto& directory_entry : std::filesystem::directory_iterator(_current_dir)) {
                const auto& path = directory_entry.path();
                if (path.extension() == ".meta") {
                    continue;
                }

                auto filename_string = path.filename().string();

                const auto texture = GetTextureFromDirEntry(directory_entry);
                auto tint_col = ImVec4{1, 1, 1, 1};

                if (directory_entry.is_directory()) {
                    if (_directory_colors.contains(path)) {
                        tint_col = IMVEC4(_directory_colors[path]);
                    } else {
                        tint_col = IMVEC4(Colors::directory);
                    }
                }

                auto cursor_pos = ImGui::GetCursorScreenPos();
                ImGui::InvisibleButton(filename_string.c_str(), {thumbnail_size, thumbnail_size});

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    if (auto metadata_opt = AssetManager::Get().GetRegistry().GetMetadata(path)) {
                        auto metadata = metadata_opt.value();
                        ImGui::SetDragDropPayload("ASSET_DRAG_AND_DROP", &metadata.uuid.value, sizeof(u64));

                        auto asset_name = metadata.path.filename().string();
                        ImGui::Text("%s", asset_name.c_str());
                    }
                    ImGui::EndDragDropSource();
                }

                if (ImGui::IsItemHovered()) {
                    tint_col *= 0.9f;

                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        if (directory_entry.is_directory()) {
                            _current_dir /= path.filename();
                            _directory_changed = true;
                        }
                    }

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                        ImGui::OpenPopup("Asset Context Menu");
                    }
                }

                ImGui::SetCursorScreenPos(cursor_pos);
                ImGui::ImageWithBg(texture->GetRendererID(), {thumbnail_size, thumbnail_size}, {0, 1}, {1, 0}, {0, 0, 0, 0}, tint_col);
                ImGui::TextWrapped("%s", filename_string.c_str());

                ImGui::NextColumn();
            }

            ImGui::Columns(1);

            if (const auto& io = ImGui::GetIO(); io.MouseWheel != 0 && io.KeyCtrl && ImGui::IsWindowHovered()) {
                thumbnail_size += 2.0f * io.MouseWheel;

                if (thumbnail_size < THUMBNAIL_MIN_SIZE) {
                    thumbnail_size = THUMBNAIL_MIN_SIZE;
                } else if (thumbnail_size > THUMBNAIL_MAX_SIZE) {
                    thumbnail_size = THUMBNAIL_MAX_SIZE;
                }
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
            if (ImGui::BeginPopupContextItem("Asset Context Menu")) {
                if (Widgets::Button("Change Color")) {
                    CORE_INFO("Editor::AssetBrowser: Pressed Change Color");
                }

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(1);
        }
        Widgets::End();
    }

    auto SceneEditor::GetTextureFromDirEntry(const std::filesystem::directory_entry& entry) const -> Texture2D* {
        if (entry.is_directory()) {
            return &Textures::directory;
        }

        switch (AssetManager::GetAssetTypeFromExtension(entry.path())) {
            case AssetType::Texture: return &Textures::sprite;
            case AssetType::Script: return &Textures::script;
            case AssetType::Scene:
            default: return &Textures::placeholder;
        }
    }
} // namespace Cobalt
