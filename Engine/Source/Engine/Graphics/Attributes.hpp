// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Base.hpp"

namespace Cobalt::Engine
{
    enum class AttributeDataType
    {
        None = 0,
        Bool,
        Int,
        Int2,
        Int3,
        Int4,
        Float,
        Float2,
        Float3,
        Float4,
        Mat4,
    };

    struct Attribute
    {
        AttributeDataType type = AttributeDataType::None;
        u32 offset = 0;
        u32 size = 0;

        Attribute(const AttributeDataType data_type);

        auto element_count() const -> u32;
    };

    class AttributeLayout final
    {
    public:
        AttributeLayout() = default;

        auto create(const InitializerList<Attribute>& attributes) -> void;
        auto attributes() -> Vector<Attribute>&;
        auto stride() -> u32;

        auto begin() -> Vector<Attribute>::iterator;
        auto end() -> Vector<Attribute>::iterator;
        auto begin() const -> Vector<Attribute>::const_iterator;
        auto end() const -> Vector<Attribute>::const_iterator;

    private:
        Vector<Attribute> m_attributes = {};
        u32 m_stride = 0;
    };
}