// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Attributes.hpp"

namespace Cobalt
{
    Attribute::Attribute(const AttributeDataType data_type) : type(data_type) {
        switch (type) {
            case AttributeDataType::Bool: size = 1; break;
            case AttributeDataType::Int: size = 4; break;
            case AttributeDataType::Int2: size = 4 * 2; break;
            case AttributeDataType::Int3: size = 4 * 3; break;
            case AttributeDataType::Int4: size = 4 * 4; break;
            case AttributeDataType::Float: size = 4; break;
            case AttributeDataType::Float2: size = 4 * 2; break;
            case AttributeDataType::Float3: size = 4 * 3; break;
            case AttributeDataType::Float4: size = 4 * 4; break;
            case AttributeDataType::Mat4: size = 4 * 4 * 4; break;
            default: break;
        }
    }

    auto Attribute::GetElementCount() const -> u32 {
        switch (type) {
            case AttributeDataType::Bool: return 1;
            case AttributeDataType::Int: return 1;
            case AttributeDataType::Int2: return 2;
            case AttributeDataType::Int3: return 3;
            case AttributeDataType::Int4: return 4;
            case AttributeDataType::Float: return 1;
            case AttributeDataType::Float2: return 2;
            case AttributeDataType::Float3: return 3;
            case AttributeDataType::Float4: return 4;
            case AttributeDataType::Mat4: return 4 * 4;
            default: return 0;
        }
    }

    auto AttributeLayout::Create(const InitializerList<Attribute>& attributes) -> void {
        _attributes = attributes;
        _stride = 0;
        u32 offset = 0;

        for (auto& attribute : _attributes) {
            attribute.offset = offset;
            offset += attribute.size;
            _stride += attribute.size;
        }
    }

    auto AttributeLayout::GetAttributes() -> Vector<Attribute>& {
        return _attributes;
    }

    auto AttributeLayout::GetStride() -> u32 {
        return _stride;
    }

    auto AttributeLayout::begin() -> Vector<Attribute>::iterator {
        return _attributes.begin();
    }
    auto AttributeLayout::end() -> Vector<Attribute>::iterator {
        return _attributes.end();
    }
    auto AttributeLayout::begin() const -> Vector<Attribute>::const_iterator {
        return _attributes.begin();
    }
    auto AttributeLayout::end() const -> Vector<Attribute>::const_iterator {
        return _attributes.end();
    }
} // namespace Cobalt
